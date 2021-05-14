#include <stdio.h>
#include <stdlib.h>

#include <bmp_handler>

int load_image(const char * filename, image_composition * img_comp) {
    FILE *fp;

    fp = fopen(file_name, "r");

    if (fp == NULL)
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    printf("The contents of %s file are:\n", file_name);

    while((ch = fgetc(fp)) != EOF)
        printf("%c", ch);

    fclose(fp);

    return 0;
}

