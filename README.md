# Task3

**1. RUN**
Get result for (n, k, d) code:
1. run Task3/Main.cpp
2. enter n k d 0 (Ex: "7 4 3 0")
3. wait till sign "if results ready press enter"
4. hear you have CNF form for SAT-solver
5. run py/sat.py
6. when results are ready they will shown on the screen(sometimes you can wait days..)
7. after getting results come back to c++ console and press enter. It will check results and return sign that it's really work correct (or it'll find an error)
8. now you can go back to point 1 and check another code or Ctrl+z

Check (n, k, d) code:
1. run Task3/Main.cpp
2. enter n k d 1
3. enter your code
4. wait till sign "if results ready press enter"
5. hear you have CNF form for SAT-solver
6. run py/sat.py
7. when results are ready they will shown on the screen(after reading clauses it'll return answer after few seconds)
8. after getting results come back to c++ console and press enter. It will check results and return sign that it's really work correct (or it'll find an error)
9. now you can go back to point 1 and check another code or Ctrl+z

change logging if you want
ToDo

Ex of input for checking code:
7 4 3 1
1 0 0 0 1 0 1 
0 1 0 0 1 1 1 
0 0 1 0 0 1 1 
0 0 0 1 1 1 0 

**2. SAVED ANSWERS**
after running it will save results:
1. cnf.py -- clauses = *CNF* // for running SAT-Solver
2. dimacs.txt -- CNF saved in other generally accepted format
3. dimacs_core_vars.txt -- saved numbers of base variables. This variables create final code.
4. sat_result -- return number of variable if it's value need to be 1 or negative number if it's need to be 0

**3. How it works**
* Explaining Math ToDo
