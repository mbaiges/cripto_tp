#ifndef _BMP_HANDLER_H
#define _BMP_HANDLER_H

#define MAX_FILEPATH_LENGTH 512

#include <stdlib.h>
#include <stdint.h>

struct BMP_header {
    char file_signature[2];             // The characters ‘B’ and ‘M’
    uint32_t file_size;                 // Size in bytes of the file including headers and pixel data
    uint8_t reserved[4];                // Unused
    uint32_t data_offset;               // Offset in the file where the pixel data is stored
    uint32_t header_size;               // The header is fixed size: 40 bytes
    uint32_t image_width;               // Width of the image in pixels
    uint32_t image_height;              // Height of the image in pixels
    uint16_t image_color_planes;        // Number of color planes (must be 1)
    uint16_t image_bpp;                 // Number of bits per pixel
    uint32_t image_compression;         // Compression method
    uint32_t image_size;                // Can be 0 if image is not compressed, otherwise is the size of the compressed image
    uint32_t image_pixels_per_meter_X;  // Horizontal resolution in pixels per meter
    uint32_t image_pixels_per_meter_Y;  // Vertical resolution in pixels per meter
    uint32_t image_colors_used;         // Number used colors
    uint32_t image_important_colors;    // Number of important color. Can be 0 If all colors are important
};

typedef struct image_composition {
    char filepath[MAX_FILEPATH_LENGTH];
    struct BMP_header header;
    uint8_t * colors_table;  // Between offset and header 
    uint8_t * pixels;
    size_t pixels_size;
} image_composition;

typedef struct xwvu {
    uint8_t x;
    uint8_t w;
    uint8_t v;
    uint8_t u;
} xwvu;

/**
Loads an image given by the user 
@filepath: path of the image
@img_comp: struct pointer where image info will be stored
@output: returns 0 if no error
**/ 
int load_image(const char * filepath, image_composition * img_comp);

/**
Loads an image given by the user
@directory: directory of images
@imgs_comp: array of struct pointers where image info will be stored
@imgs_comp_size: array returned size
@output: returns 0 if no error
**/ 
int load_images(char * directory, image_composition ** imgs_comp, size_t * imgs_comp_size);

/**
Saves an image given by the user 
@filename: path of the image
@img_comp: struct pointer where image info is stored
@output: returns 0 if no error
**/ 
int save_image(image_composition * img_comp);

/**
Loads an image given by the user
@imgs_comp: array of struct pointers where image info will be stored
@imgs_comp_size: array returned size
@output: returns 0 if no error
**/ 
int save_images(image_composition * imgs_comp, size_t imgs_comp_size);

/**
Divides pixels matrix into 2x2 XWVU arrays
@pixels: array of pixels
@pixels_size: number of pixels 
@xwvu_array: xwvu array 
@wxvu_array_size: size of the xwvu array
@width: image width (pixels)
@height: image height (pixels)
@output: returns 0 if no error
**/   
int pixels_to_xwvu(uint8_t * pixels, size_t pixels_size, xwvu ** xwvu_array, size_t * xwvu_array_size, size_t width, size_t height);

/**
Divides 2x2 XWVU arrays into pixels array
@pixels: array of pixels
@pixels_size: number of pixels 
@xwvu_array: xwvu array 
@wxvu_array_size: size of the xwvu array
@width: image width (pixels)
@height: image height (pixels)
@output: returns 0 if no error
**/   
int xwvu_to_pixels(xwvu * xwvu_array, size_t xwvu_array_size, uint8_t ** pixels, size_t * pixels_size, size_t width, size_t height);

/**
Frees image composition loaded
@img_comp: struct pointers where images info are stored
**/ 
void free_image_composition(image_composition * img_comp);

/**
Frees images composition loaded
@imgs_comp: array of struct pointers where images info are stored
**/ 
void free_images_composition(image_composition * imgs_comp, size_t imgs_comp_size);

/**
Free xwvu array
@xwvu_array: xwvu array 
**/   
void free_xwvu_array(xwvu * xwvu_array);

/**
Free pixels array
@pixels: pixels array 
**/   
void free_pixels_array(uint8_t * pixels);

#endif