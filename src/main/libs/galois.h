#ifndef _GALOIS_H
#define _GALOIS_H

#include <stdint.h>

/* Inits matrix */
int galois_init(uint16_t g);

/* Add two numbers in the GF(2^8) finite field */
uint8_t gadd(uint8_t a, uint8_t b);

/* Multiply two numbers in the GF(2^8) finite field */
uint8_t gmul(uint8_t a, uint8_t b);

/* Substract two numbers in the GF(2^8) finite field */
uint8_t gsub(uint8_t a, uint8_t b);

/* Divide two numbers in the GF(2^8) finite field */
uint8_t gdiv(uint8_t a, uint8_t b);

/**
Evaluates F(x)  
@poly: F(x) coefs
@x: Number to evaluate 
@result: result
@output: returns 0 if no error
**/ 
int poly_eval(uint8_t * poly, uint8_t k, uint8_t x, uint8_t * result);

#endif