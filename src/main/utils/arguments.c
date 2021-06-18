#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>

#include <arguments.h>

static int ARGS_N = 4;

static void stderr_print_usage(void) {
    fprintf(stderr, "usage: './ss d secret.bmp k shades_dir/' to distribute secret or './ss r secret.bmp k shades_dir/' to recover secret\n");
}

int parse_arguments(int argc, char ** argv, args_st * args) {

    if (argc != ARGS_N) {
        fprintf(stderr, "error: missing arguments\n");
        stderr_print_usage();
        return 1;
    }

    char * op_str = argv[0];
    char * secret_filename = argv[1];
    char * k_str = argv[2];
    char * shades_directory = argv[3];

    // printf("Arguments:\n");
    // printf("\top_str: %s\n", op_str);
    // printf("\tsecret_filename: %s\n", secret_filename);
    // printf("\tk_str: %s\n", k_str);
    // printf("\tshades_directory: %s\n", shades_directory);

    bool errored = false;

    // d

    if (strlen(op_str) == 1 && (*op_str == 'd' || *op_str == 'r')) {
        if (*op_str == 'd') {
            args->op = DISTRIBUTE;
        }
        else { // *op_str == 'r'
            args->op = RECOVER;
        }
    }
    else {
        errored = true;
        fprintf(stderr, "error: argument 1 must be 'd' or 'r'\n");
    }

    // secret filename

    if( args->op == DISTRIBUTE && access( secret_filename, F_OK ) != 0 ) {
        errored = true;
        fprintf(stderr, "error: secret file does not exist\n");
    }
    else {
        args->secret_filename = secret_filename;
    }

    // k

    char *aux = k_str;
    bool valid_number = true;

    while (*aux) {
        if (*aux < '4' || *aux > '6') {
            valid_number = false;
            errored = true;
            fprintf(stderr, "error: argument 3 must be 4, 5, or 6\n");
            break;
        }
        aux++;
    }

    if (valid_number) {
        args->k = atoi(k_str);
    }

    // shades directory

    struct stat stats;

    stat(shades_directory, &stats);

    if (!S_ISDIR(stats.st_mode)) {
        errored = true;
        fprintf(stderr, "error: shades directory not found\n");
    }
    else {
        int file_count = 0;
        DIR * dirp;
        struct dirent * entry;

        dirp = opendir(shades_directory);
        while ((entry = readdir(dirp)) != NULL) {
            if (entry->d_type == DT_REG) { /* If the entry is a regular file */
                file_count++;
            }
        }
        closedir(dirp);

        if (file_count < args->k) {
            errored = true;
            fprintf(stderr, "error: shades directory does not have at least %d files\n", args->k);
        }
        else {
            args->shades_directory = shades_directory;
        }
    }

    if (errored) {
        return 1;
    }

    return 0;
}