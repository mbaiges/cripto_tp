#include <stdlib.h>
#include <stdio.h>
#include <galois.h>

#include <arguments.h>
#include <secret.h>
// #include <bmp_handler.h>

static uint16_t POLY_GEN = 0x163;

static int (* secret_funcs[])(char *, uint8_t, char *) = {distribute_secret, recover_secret};

int main(int argc, char ** argv) {

    int res;
    args_st args;

    res = parse_arguments(argc-1, &argv[1], &args);
    if (res != 0) {
        exit(EXIT_FAILURE);
    }

    galois_init(POLY_GEN);

    res = secret_funcs[args.op](args.secret_filename, args.k, args.shades_directory);
    if (res != 0) {
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}