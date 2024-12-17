import pysat

from pysat.card import *
from pysat.solvers import Minisat22
from pysat.solvers import Solver
from pysat.solvers import Glucose3
from pysat.solvers import Cadical195

print('reading clauses')
from myconfig import *

print('clauses have been read\n\n')

# g = Glucose3()
# for clause in clauses:
#     g.add_clause(clause)
# print(g.solve())
# print(g.get_model())

# import sys
# print(*sys.path, sep="\n")
# import sys
# sys.path.append('/usr/local/lib/python3.7/dist-packages')
# from pysat.solvers import Solver

print('\n\n\n')
c = Cadical195()
for clause in clauses:
    c.add_clause(clause)
print(c.solve())
file1 = open("../Task3/sat_result.txt", "w") 
xs = c.get_model()
if (xs != None):
    s = '[' + ', '.join(str(x) for x in xs) + ']'
else:
    s = '[]'
file1.write(s)
file1.close()
print(xs)
file2 = open(dir_name, "w")
file2.write(s)
file2.close()


# with Minisat22(bootstrap_with=clauses) as m:
#         print(m.solve())
#         print(m.get_core())
# with Solver(name='mpl', bootstrap_with=clauses) as s:
#     print (s.get_model())


# from sys import argv
# from pysat.formula import CNF
# from pysat.solvers import Solver



# def find_mus( assmp , oracle ) :
#     i = 0
#     while i < len( assmp ) :
#         ts = assmp [ : i ] + assmp [ ( i + 1 ): ]
#         if not oracle.solve ( assumptions = ts ) :
#             assmp = ts
#         else :
#             i += 1
#     return assmp

# def add_assumps( cnf ) :
#     rnv = topv = cnf.nv
#     assumps = [ ] 
#     for i in range ( len ( cnf.clauses ) ) :
#         topv += 1
#         assumps.append ( topv ) # r e g i s t e r l i t e r a l
#         cnf.clauses[i].append(-topv ) # e x t e n d c l a u s e w i t h l i t e r a l
#     cnf.nv = cnf.nv + len ( assumps ) # u p d a te # o f v a r s
#     return rnv , assumps

# def main ( ) :
#     #cnf = CNF( from_file=argv [1] ) # c r e a t e a CNF o b j e c t f rom f i l e
#     ( rnv , assumps ) = add_assumps ( cnf )
#     clauses = [[3, 2], [3, 1], [2, 1], 
#         [6, 5], [6, 4], [5, 4], 
#         [9, 8], [9, 7], [8, 7], 
#         [12, 11], [12, 10], [11, 10], 
#         [1, 4, -13], [1, -4, 13], [-1, 4, 13], [-1, -4, -13], 
#         [2, 5, -14], [2, -5, 14], [-2, 5, 14], [-2, -5, -14], 
#         [3, 6, -15], [3, -6, 15], [-3, 6, 15], [-3, -6, -15], 
#         [1, 7, -16], [1, -7, 16], [-1, 7, 16], [-1, -7, -16], 
#         [2, 8, -17], [2, -8, 17], [-2, 8, 17], [-2, -8, -17], 
#         [3, 9, -18], [3, -9, 18], [-3, 9, 18], [-3, -9, -18], 
#         [1, 10, -19], [1, -10, 19], [-1, 10, 19], [-1, -10, -19], 
#         [2, 11, -20], [2, -11, 20], [-2, 11, 20], [-2, -11, -20], 
#         [3, 12, -21], [3, -12, 21], [-3, 12, 21], [-3, -12, -21], 
#         [4, 7, -22], [4, -7, 22], [-4, 7, 22], [-4, -7, -22], 
#         [5, 8, -23], [5, -8, 23], [-5, 8, 23], [-5, -8, -23], 
#         [6, 9, -24], [6, -9, 24], [-6, 9, 24], [-6, -9, -24], 
#         [4, 10, -25], [4, -10, 25], [-4, 10, 25], [-4, -10, -25], 
#         [5, 11, -26], [5, -11, 26], [-5, 11, 26], [-5, -11, -26], 
#         [6, 12, -27], [6, -12, 27], [-6, 12, 27], [-6, -12, -27], 
#         [7, 10, -28], [7, -10, 28], [-7, 10, 28], [-7, -10, -28], 
#         [8, 11, -29], [8, -11, 29], [-8, 11, 29], [-8, -11, -29], 
#         [9, 12, -30], [9, -12, 30], [-9, 12, 30], [-9, -12, -30], 
#         [15, 14, 13], [18, 17, 16], [21, 20, 19], [24, 23, 22], [27, 26, 25], [30, 29, 28]]

#     oracle = Solver ( name=' g3 ' , bootstrapwith=clauses )
#     mus = find_mus ( assumps , oracle )
#     mus = [ ref - rnv for ref in mus ]
#     print( "MUS: " , mus )

# if __name__ == "__main__" :
#     main ( ) 