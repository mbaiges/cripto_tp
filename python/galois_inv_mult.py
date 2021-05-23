def gf_degree(a) :
    res = 0
    a >>= 1
    while (a != 0) :
        a >>= 1
        res += 1
    return res

def gf_invert(a, mod=0x163) :
    v = mod
    g1 = 1
    g2 = 0
    j = gf_degree(a) - 8

    while (a != 1) :
        if (j < 0) :
            a, v = v, a
            g1, g2 = g2, g1
            j = -j

        a ^= v << j
        g1 ^= g2 << j

        a %= 256  # Emulating 8-bit overflow
        g1 %= 256 # Emulating 8-bit overflow

        j = gf_degree(a) - gf_degree(v)

    return g1

s = 'inv_mult={'

for x in range(1, 1<<8):

    inv_x = gf_invert(x)

    s += f'{inv_x}'
    if x < (1<<8) - 1:
        s += ', '

s += '};'

print(s)