import galois
import numpy as np

GF = galois.GF(2**8, irreducible_poly=galois.Poly.Degrees([8, 6, 5, 1, 0]))

s = 'mult={'
for x in range(0, 1<<8):
    s += '{'
    for y in range(0, 1<<8):
        res = GF(x) * GF(y)
        s += f'{res}'
        if y < (1<<8) - 1:
            s += ', '
    s += '}'
    if x < (1<<8) - 1:
        s += ',\n'
s += '};'

print(s)