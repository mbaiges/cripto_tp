#ifndef _SECRET_H
#define _SECRET_H

/**
Distributes secret in shades 
@secret_filename: path of the secret
@k: number of images in which the secret will be distributed 
@shades_directory: path of shades directory
@output: returns 0 if no error
**/ 
int distribute_secret(char * secret_filename, uint8_t k, char * shades_directory);

int recover_secret(char * secret_filename, uint8_t k, char * shades_directory);



#endif