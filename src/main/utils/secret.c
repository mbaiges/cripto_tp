#include <stdio.h>
#include <string.h>
#include <bmp_handler.h>
#include <polynomial.h>

#include <secret.h> 

/* 
Set single bit at pos to '1' by generating a mask in the proper bit location and ORing (|) x with the mask. 
*/
#define SET_BIT(x, pos) (x |= (1U << pos))

/* 
Set single bit at pos to '0' by generating a mask in the proper bit location and Anding x with the mask. 
*/
#define CLEAR_BIT(x, pos) (x &= (~(1U << pos)))

/* 
Macro to check bit
*/
#define CHECK_BIT(x, pos) (x & (1U << pos))

#define GET_BIT(x, pos) ((CHECK_BIT(x, pos) > 0)?1:0)

static void change_bit(uint8_t * x, uint8_t pos, uint8_t bit) {
    if (bit == 1) {
        SET_BIT(*x, pos);
    }
    else {
        CLEAR_BIT(*x, pos); 
    }
}

static uint8_t get_x(uint8_t x, uint8_t * used) {
    uint8_t new_x = x;
    while(used[new_x]) {
        new_x++;
    }
    return new_x;
}

int distribute_secret(char * secret_filename, uint8_t k, char * shades_directory){
    
    image_composition img_comp;
    int res;
    // Get pixels (img_comp.pixels)
     
    res = load_image(secret_filename, &img_comp);
    if (res != 0) {
        return 1;
    }

    // Shades to xvwu (imgs_xwvu_arr)
    image_composition * imgs_comp; 
    size_t imgs_comp_size; 

    res = load_images(shades_directory, &imgs_comp, &imgs_comp_size); 
    if (res != 0 ){
        free_images_composition(imgs_comp, imgs_comp_size);
        return 1; 
    }

    size_t xwvu_size; //all shades are the same size 
    xwvu * xwvu_arr;
    xwvu * imgs_xwvu_arr[imgs_comp_size];  // all shades in xwvu

    for(size_t i = 0; i < imgs_comp_size; i++ ){
        res = pixels_to_xwvu(imgs_comp[i].pixels, imgs_comp[i].pixels_size, &xwvu_arr, &xwvu_size, imgs_comp[i].header.image_width, imgs_comp[i].header.image_height);
        if (res != 0 ){
            return 1; 
        }   
        imgs_xwvu_arr[i] = xwvu_arr;
    }

    uint8_t coeffs[k]; 
    uint8_t result;
    int xwvu_idx;

    uint8_t used[1<<8];

    for(size_t i = 0; i < img_comp.pixels_size; i+=k){
        xwvu_idx = (int) i/k;

        // Reset used array
        memset(used, 0, sizeof(used)/(sizeof(used[0])));

        // Build F(x)
        memcpy(coeffs, &img_comp.pixels[i], k*sizeof(img_comp.pixels[0])); //Get coeffs from i to i+k

        // Get Y=F(x from xwvu) 
        for(size_t j=0; j < imgs_comp_size; j++){
            // Si se repite el x no usarlo
            imgs_xwvu_arr[j][xwvu_idx].x = get_x(imgs_xwvu_arr[j][xwvu_idx].x, used);
            used[imgs_xwvu_arr[j][xwvu_idx].x] = 1;

            res = poly_eval(coeffs, k, imgs_xwvu_arr[j][xwvu_idx].x, &result); 
            if (res != 0 ){
                return 1; 
            }

            uint8_t bits[8] = {0};

            for (int b=0; b<8; b++) {
                bits[b] = GET_BIT(result, b);
            }
 
            // Distribute pixels --> set bits w:3 v:3 u:pad+2
            // w
            change_bit(&(imgs_xwvu_arr[j][xwvu_idx].w), 2, bits[7]);
            change_bit(&(imgs_xwvu_arr[j][xwvu_idx].w), 1, bits[6]);
            change_bit(&(imgs_xwvu_arr[j][xwvu_idx].w), 0, bits[5]);

            // v
            change_bit(&(imgs_xwvu_arr[j][xwvu_idx].v), 2, bits[4]);
            change_bit(&(imgs_xwvu_arr[j][xwvu_idx].v), 1, bits[3]);
            change_bit(&(imgs_xwvu_arr[j][xwvu_idx].v), 0, bits[2]);

            // u
            uint8_t pad = bits[0]; 
            for (int b=1; b<8; b++) {
                pad ^= bits[b]; 
            }
            change_bit(&(imgs_xwvu_arr[j][xwvu_idx].u), 2, pad);
            change_bit(&(imgs_xwvu_arr[j][xwvu_idx].u), 1, bits[1]);
            change_bit(&(imgs_xwvu_arr[j][xwvu_idx].u), 0, bits[0]);          
        }     
    }

    // xwvu to pixels
    size_t new_pixels_size;
    uint8_t * new_pixels_arr; 

    for(size_t i=0; i < imgs_comp_size; i++){
        res = xwvu_to_pixels(imgs_xwvu_arr[i], xwvu_size, &new_pixels_arr, &new_pixels_size, imgs_comp[i].header.image_width, imgs_comp[i].header.image_height);
        if (res != 0 ){
            return 1; 
        } 
        free_xwvu_array(imgs_xwvu_arr[i]);
        free_pixels_array(imgs_comp[i].pixels);
        imgs_comp[i].pixels = new_pixels_arr;
    }

    // Save images
    res = save_images(imgs_comp, imgs_comp_size);
    if (res != 0) {
        return 1;
    }

    free_image_composition(&img_comp);
    free_images_composition(imgs_comp, imgs_comp_size);

    return res;
}

int recover_secret(char * secret_filename, uint8_t k, char * shades_directory){
    image_composition * imgs_comp; 
    size_t imgs_comp_size;
    int res; 

    res = load_images(shades_directory, &imgs_comp, &imgs_comp_size);
    if (res != 0) {
        return 1;
    }

    // Images to xwvu 
    size_t xwvu_size;  
    xwvu * xwvu_arr;
    xwvu * imgs_xwvu_arr[imgs_comp_size];   

    for(size_t i = 0; i < imgs_comp_size; i++ ){
        res = pixels_to_xwvu(imgs_comp[i].pixels, imgs_comp[i].pixels_size, &xwvu_arr, &xwvu_size, imgs_comp[i].header.image_width, imgs_comp[i].header.image_height);
        if (res != 0 ){
            return 1; 
        }   
        imgs_xwvu_arr[i] = xwvu_arr;
    }

    // Interpolation 
    uint8_t coeffs[k];
    uint8_t pixels[k*xwvu_size]; 

    uint8_t x[k];
    uint8_t y[k];

    size_t l = (imgs_comp[0].header.image_width * imgs_comp[0].header.image_height)/k;

    size_t used_img;

    for(size_t i = 0; i < l; i++){

        used_img = 0;

        for(size_t img = 0; (used_img < k) && (img < imgs_comp_size); img++){  

            uint8_t bits[8] = {0};

            // w
            bits[7] = GET_BIT(imgs_xwvu_arr[img][i].w, 2);
            bits[6] = GET_BIT(imgs_xwvu_arr[img][i].w, 1);
            bits[5] = GET_BIT(imgs_xwvu_arr[img][i].w, 0);

            // v
            bits[4] = GET_BIT(imgs_xwvu_arr[img][i].v, 2);
            bits[3] = GET_BIT(imgs_xwvu_arr[img][i].v, 1);
            bits[2] = GET_BIT(imgs_xwvu_arr[img][i].v, 0); 

            // u
            bits[1] = GET_BIT(imgs_xwvu_arr[img][i].u, 1);
            bits[0] = GET_BIT(imgs_xwvu_arr[img][i].u, 0);

            uint8_t pad = 0; 
            for (int b=0; b<8; b++) {
                pad ^= bits[b]; 
            }
            pad = GET_BIT(pad, 0);

            uint8_t read_pad = 0;
            read_pad = GET_BIT(imgs_xwvu_arr[img][i].u, 2);

            if (pad == read_pad) {
                x[used_img] = imgs_xwvu_arr[img][i].x;
                y[used_img] = 0;
                for (int b=0; b<8; b++) {
                    change_bit(&(y[used_img]), b, bits[b]); 
                }
                used_img++;
            }
        }

        if (used_img == k) {
            res = poly_interpolate(x, y, k, coeffs);
            if (res != 0 ){
                return 1; 
            }
        }
        else {
            memset(coeffs, 0, k * sizeof(coeffs[0]));
        }

        memcpy(&(pixels[i*k]), coeffs, k * sizeof(coeffs[0]));
    }

    for(size_t i = 0 ; i< imgs_comp_size; i++){
        free_xwvu_array(imgs_xwvu_arr[i]);
    }

    image_composition img_comp;

    memcpy(&img_comp, &(imgs_comp[0]), sizeof(img_comp));
    img_comp.pixels = pixels;

    size_t secret_filename_len = strlen(secret_filename);
    memcpy(img_comp.filepath, secret_filename, secret_filename_len);
    img_comp.filepath[secret_filename_len] = '\0';

    res = save_image(&img_comp);
    if (res != 0) {
        return 1;
    }

    free_images_composition(imgs_comp, imgs_comp_size);

    return res; 
}