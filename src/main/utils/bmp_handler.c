#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <bmp_handler.h>

int load_image(const char * filename, image_composition * img_comp) {
    
    FILE *fp;

    fp = fopen(filename, "rb");

    if (fp == NULL){
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    // memcpy(&(img_comp->header), fp, sizeof(struct BMP_header));

    fread(&(img_comp->header),sizeof(unsigned char),8, fp);

    printf("Header:\n");

    printf("\tfile_signature=%c%c\n", img_comp->header.file_signature[0], img_comp->header.file_signature[1]);
    // printf("\tfile_size=%x\n", img_comp->header.file_size);
    printf("\tfile_size[0]=%x\n", img_comp->header.file_size[0]);
    printf("\tfile_size[1]=%x\n", img_comp->header.file_size[1]);
    printf("\tfile_size[2]=%x\n", img_comp->header.file_size[2]);
    printf("\tfile_size[3]=%x\n", img_comp->header.file_size[3]);
    // printf("\treserved=%u", image_comp->header.reserved);
    printf("\tdata_offset=%x\n", img_comp->header.data_offset);
    printf("\theader_size=%u\n", img_comp->header.header_size);
    printf("\timage_width=%u\n", img_comp->header.image_width);
    printf("\timage_height=%u\n", img_comp->header.image_height);
    printf("\timage_color_planes=%u\n", img_comp->header.image_color_planes);
    printf("\timage_bpp=%u\n", img_comp->header.image_bpp);
    printf("\timage_compression=%u\n", img_comp->header.image_compression);
    printf("\timage_size=%u\n", img_comp->header.image_size);
    printf("\timage_pixels_per_meter_X=%u\n", img_comp->header.image_pixels_per_meter_X);
    printf("\timage_pixels_per_meter_Y=%u\n", img_comp->header.image_pixels_per_meter_Y);
    printf("\timage_colors_used=%u\n", img_comp->header.image_colors_used);
    printf("\timage_important_colors=%u\n", img_comp->header.image_important_colors);

    printf("\n");

    fclose(fp);

    return 0;
}

