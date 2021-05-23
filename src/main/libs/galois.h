#ifndef _GALOIS_H
#define _GALOIS_H

#include <stdint.h>

typedef struct mult_table {
    uint8_t mult[1<<8][1<<8]; // 256 * 256 TODO: create TABLE :/ 
} mult_table;

// Polynomial generator g(x)
static const uint8_t g = {1, 0, 0, 0, 1, 1, 0, 1}; // 1 + x + x^5 + x^6 + x^8

/**
Generates F(x) with random coefs  
@poly: Stores the polynomial
@output: returns 0 if no error
**/ 
int get_random_poly(uint8_t ** poly);

/**
F(x) mod g(x) 
@n:  numbers to multiply (1)
@m:  numbers to multiply (2)
@output: result
**/ 
uint8_t multiply(uint8_t n, uint8_t m);

/**
Evaluates F(x)  
@poly: F(x) coefs
@x: Number to evaluate 
@result: result
@output: returns 0 if no error
**/ 
int poly_eval( uint8_t * poly, uint8_t x, uint8_t * result);

/**
Frees F(x) 
@poly: F(x)  
**/ 
void free_poly(uint8_t * poly);

#endif