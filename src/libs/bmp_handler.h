#ifndef _BMP_HANDLER_H
#define _BMP_HANDLER_H

#include <stdin.h>

#define BMP_HEADER_SIZE 54

typedef struct * image_composition {
    uint8_t * header;
    uint8_t * pixels;
    size_t pixels_size;
} image_composition;

typedef struct * xwvu {
    uint8_t x;
    uint8_t w;
    uint8_t v;
    uint8_t u;  
} xwvu;

/**
Loads an image given by the user 
@filename: path of the image
@img_comp: struct pointer where image info will be stored
@output: returns 0 if no error
**/ 
int load_image(const char * filename, image_composition * img_comp);

/**
Loads an image given by the user
@directory: path of the directory containing images
@imgs_comp: array of struct pointers where image info will be stored
@imgs_comp_size: array returned size
@output: returns 0 if no error
**/ 
int load_images(const char * directory, image_composition ** imgs_comp, size_t * imgs_comp_size);

/**
Frees image composition loaded
@img_comp: struct pointers where images info are stored
@output: returns 0 if no error
**/ 
int free_image_composition(image_composition * imgs_comp);

/**
Frees images composition loaded
@imgs_comp: array of struct pointers where images info are stored
@output: returns 0 if no error
**/ 
int free_images_composition(image_composition ** imgs_comp);

/**
Divides pixels matrix into 2x2 XWVU arrays
@pixels: array of pixels
@pixels_size: number of pixels 
@xwvu_array: xwvu array 
@wxvu_array_size: size of the xwvu array
@output: returns 0 if no error
**/   
int pixels_to_xwvu(uint8_t * pixels, size_t pixels_size, xwvu * xwvu_array, size_t * xwvu_array_size);

/**
Divides 2x2 XWVU arrays into pixels array
@pixels: array of pixels
@pixels_size: number of pixels 
@xwvu_array: xwvu array 
@wxvu_array_size: size of the xwvu array
@output: returns 0 if no error
**/   
int xwvu_to_pixels(xwvu * xwvu_array, size_t xwvu_array_size, uint8_t * pixels, size_t * pixels_size);

/**
Free xwvu array
@xwvu_array: xwvu array 
@output: returns 0 if no error
**/   
int free_xwvu_array(xwvu * xwvu_array);

/**
Free pixels array
@pixels: pixels array 
@output: returns 0 if no error
**/   
int free_pixels_array(uint8_t * pixels);

#endif