#include <stdlib.h>
#include <stdio.h>

#include <bmp_handler.h>

int main(int argc, char ** argv) {

    image_composition img_comp;

    int res;

    res = load_image("images/Albert.bmp", &img_comp);
    if (res != 0) {
        exit(EXIT_FAILURE);
    }
    
    size_t xwvu_size;
    xwvu * xwvu_arr;

    // res = pixels_to_xwvu(pixels, 48, &xwvu_arr, &xwvu_size, 6, 8);
    res = pixels_to_xwvu(img_comp.pixels, img_comp.pixels_size, &xwvu_arr, &xwvu_size, img_comp.header.image_width, img_comp.header.image_height);

    size_t new_pixels_size;
    uint8_t * new_pixels_arr;

    res = xwvu_to_pixels(xwvu_arr, xwvu_size, &new_pixels_arr, &new_pixels_size, img_comp.header.image_width, img_comp.header.image_height);

    free_pixels_array(img_comp.pixels);

    img_comp.pixels = new_pixels_arr;

    free_xwvu_array(xwvu_arr);

    res = save_image("images/Albertito.bmp", &img_comp);
    if (res != 0) {
        exit(EXIT_FAILURE);
    }

    free_image_composition(&img_comp);

    exit(EXIT_SUCCESS);
}