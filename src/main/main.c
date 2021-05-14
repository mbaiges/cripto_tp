#include <bmp_handler.h>
#include <stdio.h>

int main(int argc, char ** argv) {
    printf("argc=%d, argv[0]=%s", argc, argv[0]);

    image_composition img_comp;

    load_image("images/Albert.bmp", &img_comp);

    return 0;
}