#source https://en.wikipedia.org/wiki/Ancient_Egyptian_multiplication#Russian_peasant_multiplication
# Multiply two numbers in the GF(2^8) finite field defined 
# by the polynomial x^8 + x^6 + x^5 + x + 1 = 0
# using the Russian Peasant Multiplication algorithm
# (the other way being to do carry-less multiplication followed by a modular reduction)

def gmul(a, b):
    p = 0

    while (a and b):
        if (b & 1):     
            p = p ^ a    

        if (a & 0x80):  
            a = (a << 1) ^ 0x163 #x^8 + x^6 + x^5 + x + 1 in hexa 
        else:
            a = a << 1 
        b = b >> 1 


    return p

s = 'mult={'

for x in range(1, 1<<8):
    
    s += '{'
    
    for y in range(1, 1<<8):
      
        res = gmul(x, y)

        s += f'{res}'
        if y < (1<<8) - 1:
            s += ', '
    
    s += '}'
    if x < (1<<8) - 1:
        s += ',\n'

s += '};'

print(s)