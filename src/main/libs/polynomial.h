#ifndef _POLYNOMIAL_H
#define _POLYNOMIAL_H

#include <stdint.h>
#include <stdlib.h>

/**
Polynomial addition 
@poly1: first polynomial
@poly1_g: first polynomial grade
@poly2: secondary polynomial
@poly2_g: secondary polynomial grade
@result: result polynomial
@output: returns 0 if no error
**/ 
int poly_add(uint8_t * poly1, size_t poly1_g, uint8_t * poly2, size_t poly2_g, uint8_t * result);

/**
Polynomial multiplication
@poly1: first polynomial
@poly1_g: first polynomial grade
@poly2: secondary polynomial
@poly2_g: secondary polynomial grade
@result: result polynomial
@output: returns 0 if no error
**/ 
int poly_mult(uint8_t * poly1, size_t poly1_g, uint8_t * poly2, size_t poly2_g, uint8_t * result);

/**
Polynomial scalar multiplication
@poly: polynomial
@poly_g: polynomial grade 
@scalar: scalar
@result: result polynomial 
@output: returns 0 if no error
**/ 
int poly_mult_scalar(uint8_t * poly, size_t poly_g, uint8_t scalar, uint8_t * result);

/**
Polynomial scalar division
@poly: polynomial
@poly_g: polynomial grade 
@scalar: scalar
@result: result polynomial 
@output: returns 0 if no error
**/ 
int poly_div_scalar(uint8_t * poly, size_t poly_g, uint8_t scalar, uint8_t * result);

/**
Polynomial evaluation
@poly: polynomial
@poly_g: polynomial grade 
@x: x to eval
@result: result of polynomial eval 
@output: returns 0 if no error
**/ 
int poly_eval(uint8_t * poly, size_t poly_g, uint8_t x, uint8_t * result);

/**
Lagrange interpolation for secret reconstruction in GF(2^8)
@x: x from shadows
@y: f(x) from shadows  
@k: number of shadows
@poly: answer coefs
@output: returns 0 if no error
**/ 
int lagrange_interpolate(uint8_t * x, uint8_t * y, size_t k, uint8_t * poly);

#endif