#include <polynomial.h>
#include <galois.h>
#include <string.h>
#include <stdio.h>

static void poly_print(uint8_t * poly, size_t g, const char * name) {
    printf("%s = ", name);
    for(size_t n=0; n<g; n++) {
        printf("%d X^%ld", poly[n], n);
        if (n < g-1) {
            printf(" + ");
        }
    }
    printf("\n");
}

int poly_add(uint8_t * poly1, size_t poly1_g, uint8_t * poly2, size_t poly2_g, uint8_t * result) {
    size_t i;

    size_t result_g;

    if (poly1_g >= poly2_g) {
        result_g = poly1_g;
    }
    else {
        result_g = poly2_g;
    }

    memset((void *) result, 0, result_g * sizeof(uint8_t)); // inicializacion de result en 0

    for (i=0; i<poly1_g || i<poly2_g; i++) {
        if (i<poly1_g && i<poly2_g) {
            result[i] = gadd(poly1[i], poly2[i]);
        }
        else if (i<poly1_g) {
            result[i] = poly1[i];
        }
        else {
            result[i] = poly2[i];
        }
    }
    return 0;
}

int poly_mult(uint8_t * poly1, size_t poly1_g, uint8_t * poly2, size_t poly2_g, uint8_t * result) {
    size_t i, j;

    size_t result_g = (poly1_g-1) + (poly2_g-1) + 1;
    memset((void *) result, 0, result_g * sizeof(uint8_t)); // inicializacion de result en 0
    
    for (i=0; i<poly1_g; i++) {
        for (j=0; j<poly2_g; j++) {
            result[i+j] = gadd(result[i+j], gmul(poly1[i], poly2[j]));
        }
    }
    return 0;
}

int poly_mult_scalar(uint8_t * poly, size_t poly_g, uint8_t scalar, uint8_t * result) {
    size_t result_g = poly_g;
    memset((void *) result, 0, result_g * sizeof(uint8_t)); // inicializacion de result en 0

    for (size_t i=0; i<poly_g; i++) {
        result[i] = gmul(poly[i], scalar);
    }
    return 0;
}

int poly_div_scalar(uint8_t * poly, size_t poly_g, uint8_t scalar, uint8_t * result) {
    size_t result_g = poly_g;
    memset((void *) result, 0, result_g * sizeof(uint8_t)); // inicializacion de result en 0

    for (size_t i=0; i<poly_g; i++) {
        result[i] = gdiv(poly[i], scalar);
    }
    return 0;
}

int poly_eval(uint8_t * poly, size_t poly_g, uint8_t x, uint8_t * result) {
    return 0;
}

int lagrange_interpolate(uint8_t tuples[][2], size_t k, uint8_t * poly) {
    
    setvbuf(stdout, NULL, _IONBF, 0);

    uint8_t x_i, x_j, y_i;

    int res;

    uint8_t divisor;

    uint8_t poly_aux1[k];
    uint8_t poly_aux2[k];
    uint8_t poly_aux3[k];

    uint8_t L[k][k];

    memset((void *) L, 0, k*k*sizeof(uint8_t));

    uint8_t poly_mult_acum[k];
    int poly_mult_empty;

    size_t acum_g;

    printf("K = %ld\n", k);

    for(size_t i =0; i < k; i++){
        printf("i = %ld\n", i);

        x_i = tuples[i][0];

        memset((void *) poly_mult_acum, 0, k * sizeof(uint8_t));
        poly_mult_empty = 1;
        acum_g = 0;
    
        for(size_t j = 0; j < k; j++){
            printf("j = %ld\n", j);
            x_j = tuples[j][0];

            // L(t) = (t-x_j)/(x_i-x_j)
            if (j != i) {
                poly_aux1[0] = x_j; // sumar es restar (x - x_j)
                poly_aux1[1] = 1;

                divisor = gsub(x_i, x_j);
                
                res = poly_div_scalar(poly_aux1, 2, divisor, poly_aux2);
                if (res != 0) {
                    return res;
                }

                poly_print(poly_aux2, 2, "poly_aux2");

                if (poly_mult_empty) {
                    for(size_t n=0; n<k; n++) {
                        poly_mult_acum[n] = poly_aux2[n];
                    }
                    poly_mult_empty = 0;

                    poly_print(poly_mult_acum, 2, "poly_mult_acum");
                }
                else {
                    for(size_t n=0; n<k; n++) {
                        poly_aux3[n] = poly_mult_acum[n];
                        if (poly_aux3[n] > 0 && n > acum_g) {
                            acum_g = n;
                        }
                    }
                    acum_g++;

                    printf("acum_g = %ld\n", acum_g);
                    poly_print(poly_aux3, acum_g, "poly_aux3");

                    res = poly_mult(poly_aux2, 2, poly_aux3, acum_g, poly_mult_acum);
                    if (res != 0) {
                        return res;
                    }

                    poly_print(poly_mult_acum, (2-1) + (acum_g-1) + 1, "poly_mult_acum");
                }

            }
        }

        for(size_t n=0; n<k; n++) {
            L[i][n] = poly_mult_acum[n];
        }
    }

    uint8_t poly_sum_acum[k];

    memset((void *) poly_sum_acum, 0, k * sizeof(uint8_t));

    for(size_t i=0; i<k; i++) {
        y_i = tuples[i][1];

        res = poly_div_scalar(L[i], k, y_i, poly_aux2);
        if (res != 0) {
            return res;
        }

        for(size_t n=0; n<k; n++) {
            poly_aux3[n] = poly_sum_acum[n];
        }

        res = poly_add(poly_aux3, k, poly_aux2, k, poly_sum_acum);
        if (res != 0) {
            return res;
        }
    }

    poly_print(poly_sum_acum, k, "poly_sum_acum");

    for(size_t n=0; n<k; n++) {
        poly[n] = poly_sum_acum[n];
    }

    return 0;
}