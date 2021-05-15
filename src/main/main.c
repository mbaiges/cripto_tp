#include <stdlib.h>

#include <bmp_handler.h>

int main(int argc, char ** argv) {

    image_composition img_comp;

    int res;

    res = load_image("images/Albert.bmp", &img_comp);
    if (res != 0) {
        exit(EXIT_FAILURE);
    }

    res = save_image("images/Albertito.bmp", &img_comp);
    if (res != 0) {
        exit(EXIT_FAILURE);
    }

    free_image_composition(&img_comp);

    exit(EXIT_SUCCESS);
}