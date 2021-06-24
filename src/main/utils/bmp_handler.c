#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

#include <bmp_handler.h>

void print_header(struct BMP_header * header) {

    printf("file_signature=%c%c\n", header->file_signature[0], header->file_signature[1]);
    printf("file_size=%u\n", header->file_size);
    // printf("\treserved=%x", header->reserved);
    printf("data_offset=%u\n", header->data_offset);
    printf("header_size=%u\n", header->header_size);
    printf("image_width=%u\n", header->image_width);
    printf("image_height=%u\n", header->image_height);
    printf("image_color_planes=%u\n", header->image_color_planes);
    printf("image_bpp=%u\n", header->image_bpp);
    printf("image_compression=%u\n", header->image_compression);
    printf("image_size=%u\n", header->image_size);
    printf("image_pixels_per_meter_X=%u\n", header->image_pixels_per_meter_X);
    printf("image_pixels_per_meter_Y=%u\n", header->image_pixels_per_meter_Y);
    printf("image_colors_used=%u\n", header->image_colors_used);
    printf("image_important_colors=%u\n", header->image_important_colors);

    printf("\n");

}

int load_image(const char * filepath, image_composition * img_comp) {
    
    FILE *fp;

    fp = fopen(filepath, "rb");

    if (fp == NULL){
        perror("error while opening the file");
        return -1;
    }

    strcpy(img_comp->filepath, filepath);

    fseek(fp,0,0);
    fread(&(img_comp->header.file_signature[0]),1,1,fp);
    fread(&(img_comp->header.file_signature[1]),1,1,fp);
    fread(&(img_comp->header.file_size),1,4,fp);
    fread(&(img_comp->header.reserved),1,4,fp);
    fread(&(img_comp->header.data_offset),1,4,fp);
    fread(&(img_comp->header.header_size),1,4,fp);
    fread(&(img_comp->header.image_width),1,4,fp);
    fread(&(img_comp->header.image_height),1,4,fp);
    fread(&(img_comp->header.image_color_planes),1,2,fp);
    fread(&(img_comp->header.image_bpp),1,2,fp);
    fread(&(img_comp->header.image_compression),1,4,fp);
    fread(&(img_comp->header.image_size),1,4,fp);
    fread(&(img_comp->header.image_pixels_per_meter_X),1,4,fp);
    fread(&(img_comp->header.image_pixels_per_meter_Y),1,4,fp);
    fread(&(img_comp->header.image_colors_used),1,4,fp);
    fread(&(img_comp->header.image_important_colors),1,4,fp);
    
    size_t colors_table_size = img_comp->header.data_offset-54;
    img_comp->colors_table = malloc(colors_table_size * sizeof(uint8_t));
    fread(img_comp->colors_table,1,colors_table_size,fp);
    
    // TODO: Que hacemos con lo que hay entre 54 Bytes y el Offset

    fseek(fp, img_comp->header.data_offset, 0); // Pixels begin in offset 

    //TODO: Revisar porque a veces viene 0 de image_size ...
    // img_comp->pixels_size = img_comp->header.image_size; 
    //Solucion
    img_comp->pixels_size = img_comp->header.image_width * img_comp->header.image_height;
    img_comp->header.image_size = img_comp->pixels_size;

    img_comp->pixels = malloc(img_comp->pixels_size * sizeof(uint8_t));
    fread(img_comp->pixels,1,img_comp->pixels_size,fp);

    // print_header(&(img_comp->header));
    // print_image(img_comp->pixels, img_comp->header.image_width, img_comp->header.image_height);

    fclose(fp);

    return 0;
}

int load_images(char *directory, image_composition ** imgs_comp, size_t * imgs_comp_size) {

    char filepath[MAX_FILEPATH_LENGTH];

    DIR *d;
    struct dirent *dir;
    d = opendir(directory);
    int error = 0;

    if(d) {
        image_composition * images = NULL;
        size_t dir_len = strlen(directory);
        strcpy(filepath, directory);
        filepath[dir_len] = '/';
        size_t max_filename_len = MAX_FILEPATH_LENGTH - (dir_len + 1);
        size_t cant_imgs = 0;

        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_REG) {  // If the entry is a regular file 
                if(strlen(dir->d_name) >= max_filename_len) {
                    return 1;
                }
                cant_imgs++;
                images = (image_composition *) realloc(images, cant_imgs * sizeof(image_composition));
                if (images == NULL) {
                    return 1;
                }
                strcpy(filepath + dir_len + 1, dir->d_name);
                
                int result = load_image(filepath, &(images[cant_imgs-1]));

                if(result != 0) {
                    error = 1;
                    break;
                }
            }
        }
        closedir(d);
        *imgs_comp = images;
        *imgs_comp_size = cant_imgs;
    }
    else {
        return 1;
    }

    return error;
}

int save_image(image_composition * img_comp) {
    
    FILE *fp;

    fp = fopen(img_comp->filepath, "wb");

    if (fp == NULL){
        perror("Error while opening the file.\n");
        // exit(EXIT_FAILURE);
        return 1;
    }

    fwrite(&(img_comp->header.file_signature[0]),1,1,fp);
    fwrite(&(img_comp->header.file_signature[1]),1,1,fp);
    fwrite(&(img_comp->header.file_size),1,4,fp);
    fwrite(&(img_comp->header.reserved),1,4,fp);
    fwrite(&(img_comp->header.data_offset),1,4,fp);
    fwrite(&(img_comp->header.header_size),1,4,fp);
    fwrite(&(img_comp->header.image_width),1,4,fp);
    fwrite(&(img_comp->header.image_height),1,4,fp);
    fwrite(&(img_comp->header.image_color_planes),1,2,fp);
    fwrite(&(img_comp->header.image_bpp),1,2,fp);
    fwrite(&(img_comp->header.image_compression),1,4,fp);
    fwrite(&(img_comp->header.image_size),1,4,fp);
    fwrite(&(img_comp->header.image_pixels_per_meter_X),1,4,fp);
    fwrite(&(img_comp->header.image_pixels_per_meter_Y),1,4,fp);
    fwrite(&(img_comp->header.image_colors_used),1,4,fp);
    fwrite(&(img_comp->header.image_important_colors),1,4,fp);

    size_t colors_table_size = img_comp->header.data_offset-54;
    fwrite(img_comp->colors_table,1,colors_table_size,fp);
    
    // TODO: Que hacemos con lo que hay entre 54 Bytes y el Offset --> por ahora le cambiamos el offset a 54B 

    fwrite(img_comp->pixels,1,img_comp->header.image_size,fp);

    fclose(fp);
    return 0;
}

int save_images(image_composition * imgs_comp, size_t imgs_comp_size) {
    for(size_t i = 0 ; i < imgs_comp_size ; i++) {
        if (save_image(&(imgs_comp[i])) != 0)
            return -1;
    }

    return 0;
}

int pixels_to_xwvu(uint8_t * pixels, size_t pixels_size, xwvu ** xwvu_array, size_t * xwvu_array_size, size_t width, size_t height) {

    uint8_t matrix[height][width];
    memset(matrix, 0, sizeof(uint8_t)*height*width);

    int i = 0;
    int j = 0;

    if (width*height < 1) {
        return 1;
    }

    for (int it = (width*height)-1; it >= 0 ; it--) {
        i = height - (it / width) - 1;
        j = it % width;

        matrix[i][j] = pixels[it];
    }

    *xwvu_array_size = (width*height)/4;

    *xwvu_array = malloc(*xwvu_array_size * sizeof(xwvu));

    int rows = (int) height;
    int cols = (int) width;

    uint8_t * row1, * row2;
    
    int k = 0;
    for (int i=0; i < rows; i+=2 ){
        row1 = matrix[i];   //42 43 44 45 46 47
        row2 = matrix[i+1]; //36 37 38 39 40 41
        
        for(int j = 0; j < cols; j+=2){

            (*xwvu_array)[k].x = row1[j];
            (*xwvu_array)[k].w = row1[j+1];
            (*xwvu_array)[k].v = row2[j];
            (*xwvu_array)[k].u = row2[j+1];

            // xwvu_array[k] = [42,43,36,37]
            k++;
        }
    }

    return 0;
}

int xwvu_to_pixels(xwvu * xwvu_array, size_t xwvu_array_size, uint8_t ** pixels, size_t * pixels_size, size_t width, size_t height) {

    int new_pixels_size = xwvu_array_size*4;
    int row, col;
    
    int rows = (int) height/2;
    int cols = (int) width/2;

    *pixels = malloc(new_pixels_size * sizeof(uint8_t));

    for (int i=0; i < (int) xwvu_array_size; i++) {
        row = i / cols;
        col = i % cols;

        //  col = 0            col = 1          col = 2

        //  i = 0              i = 1          i = 2      --- row = 0 (que tiene la original_row 0 y 1)
        // [[42,43,36,37], [44,45,38,39], [46,47,40,41], --> width/2 = cols
        //    x w  v  u 
        // 42,4344,45  46,47
        // 36,37  38,39  40,41 
        //  i = 3              i = 4          i = 5      --- row = 1
        // [30,31,24,25], [32,33,26,27], [34,35,28,29]

        /*
        (*pixels)[((2*rows - 2*row - 1) - 0) * 2*cols + (2*col + 0)] = xwvu_array[i].x;
        (*pixels)[((2*rows - 2*row - 1) - 0) * 2*cols + (2*col + 1)] = xwvu_array[i].w;
        (*pixels)[((2*rows - 2*row - 1) - 1) * 2*cols + (2*col + 0)] = xwvu_array[i].v;
        (*pixels)[((2*rows - 2*row - 1) - 1) * 2*cols + (2*col + 1)] = xwvu_array[i].u;
        */

        (*pixels)[(2*(rows - row) - 1) * 2*cols + 2*col] = xwvu_array[i].x;
        (*pixels)[(2*(rows - row) - 1) * 2*cols + (2*col + 1)] = xwvu_array[i].w;
        (*pixels)[((2*(rows - row) - 1) - 1) * 2*cols + 2*col] = xwvu_array[i].v;
        (*pixels)[((2*(rows - row) - 1) - 1) * 2*cols + (2*col + 1)] = xwvu_array[i].u;
    }

    *pixels_size = new_pixels_size;
    
    return 0;
}

void free_image_composition(image_composition * img_comp) {
    free_pixels_array(img_comp->pixels);
    free(img_comp->colors_table);
}

void free_images_composition(image_composition * imgs_comp, size_t imgs_comp_size) {
    for (int i=0; i < (int) imgs_comp_size; i++) {
        free_image_composition(&(imgs_comp[i]));
    }
    free(imgs_comp);
}

void free_xwvu_array(xwvu * xwvu_array) {
    free(xwvu_array);
}

void free_pixels_array(uint8_t * pixels) {
    free(pixels);
}