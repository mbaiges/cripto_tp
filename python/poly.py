import galois
import numpy as np

GF = galois.GF(2**8, irreducible_poly=galois.Poly.Degrees([8, 6, 5, 1, 0]));

poly_coeffs = [1, 255, 10];

p = galois.Poly(poly_coeffs, order="asc", field=GF);

x = [1, 0, 2];

print(f'p={poly_coeffs}');
print(f'x={x}');

y = p(x);

y_list = y.view(np.ndarray);
print(f'y={y_list}');

def add(pol1, pol2):
    size = len(pol1) if len(pol1) > len(pol2) else len(pol2);
    ans = [0 for i in range(0, size)];
    
    for i in range(0, size):
        if (i < len(pol1) and i < len(pol2)):
            s = GF(pol1[i]) + GF(pol2[i])
            ans[i] = int(s)
        elif i < len(pol1):
            ans[i] = pol1[i];
        else:
            ans[i] = pol2[i];

    return ans;

def sub(pol1, pol2):
    return add(pol1, pol2);

def mult(pol1, pol2):
    size = (len(pol1) - 1) + (len(pol2) - 1) + 1;
    ans = [0 for i in range(0, size)];
    
    for i in range(0, len(pol1)):
        for j in range(0, len(pol2)):
            s = GF(pol1[i]) * GF(pol2[j])
            ans[i + j] = int(s)

    return ans;

def div(pol1, pol2):
    size = (len(pol1) - 1) + (len(pol2) - 1) + 1;
    ans = [0 for i in range(0, size)];
    
    for i in range(0, len(pol1)):
        for j in range(0, len(pol2)):
            s = GF(pol1[i]) / GF(pol2[j])
            ans[i + j] = int(s)

    return ans;

# def lagrange_interpolate(x, y):
#     L = [[] for i in range(0, len(x))];

#     for i in range(0, len(x)):
#         prod = None
#         for j in range(0, len(x)):
#             if i != j:
#                 upper = sub([0, 1], [x[j]]);
#                 bot = sub([x[i]], [x[j]]);
#                 aux = div(upper, bot);

#                 print(f'upper={upper}');
#                 print(f'bottom={bot}');
#                 print(f'res={aux}');

#                 if prod == None:
#                     prod = aux;
#                 else:
#                     prod = mult(prod, aux);

#         L[i] = prod;

#     res = [0 for i in range(0, len(x))];

#     acum = None;

#     for i in range(0, len(x)):
#         print(f"L[{i}]={L[i]}, y[{i}]={[y[i]]}");
#         aux = mult(L[i], [y[i]])

#         if acum is None:
#             acum = aux;
#         else:
#             acum = add(acum, aux);

#     return acum;

def interpolate(x, y):
    s = [0 for i in range(0, len(x))];

    x, y = x.copy(), y.copy()

    for i in range(0, len(x)):
        if x[i] == 0:
            aux_x, aux_y = x[0], y[0]
            x[0], y[0] = x[i], y[i]
            x[i], y[i] = aux_x, aux_y
            continue

    Y = [ [ 0 for j in range(0, len(x)) ] for i in range(0, len(x)) ];
    Y[0] = y;

    for j in range(0, len(x)-1):
        
        print(f'------ s_{j+1} ------\n')

        L0 = [[] for i in range(0, len(x))];

        for i in range(j, len(x)):
            prod = None
            st = f'L0[{i}] = '
            for k in range(j, len(x)):
                if i != k:
                    upper = sub([0], [x[k]]);
                    bot = sub([x[i]], [x[k]]);
                    aux = div(upper, bot);

                    if prod == None:
                        prod = aux;
                    else:
                        prod = mult(prod, aux);

                    st += f'({x[k]} / ({x[i]} - {x[k]}))'
                    st += ' x '
        
            L0[i] = prod;
            print(st)

        print(f'L(0) = {L0}')
        
        acum = None;

        st = ''
        for i in range(j, len(x)):
            aux = mult(L0[i], [Y[j][i]])

            if acum is None:
                acum = aux;
            else:
                acum = add(acum, aux);

            st += f'{L0[i]} x {Y[j][i]}'
            if i < len(x) - 1:
                st += ' + '

        print(st)
        
        s[j] = acum[0];

        print(f's[{j}] = {s[j]}')

        if j < len(x) - 1:
            for i in range(j+1, len(x)):
                upper = sub([Y[j][i]], [s[j]]);
                bot = [x[i]];
                aux = div(upper, bot);

                print(f'Y[{j+1}][{i}] = ({Y[j][i]} - {s[j]})/{x[i]} = {aux}')

                Y[j+1][i] = aux[0];

    s[len(x)-1] = Y[len(x)-1][len(x)-1]

    return s;

# res1 = lagrange_interpolate(x, y_list);

# print(f'res1={res1}');

res2 = interpolate(x, y_list);

print(f'res2={res2}');

# for coef in range(0, 1<<8):
#     poly_coeffs = [coef, (coef + 1)%256, (coef + 2)%256];
#     p = galois.Poly(poly_coeffs, order="asc", field=GF);
#     print(f'################### Coeffs={poly_coeffs} ###################')
#     for equis in range(1, 1<<8-2):
#         x = [equis, (equis + 1)%256, (equis + 2)%256]
#         y = p(x);
#         y_list = y.view(np.ndarray);
#         coeffs = interpolate(x, y_list)

#         if poly_coeffs != coeffs:
#             print(f'x={equis} ==> coeffs={coeffs}')
