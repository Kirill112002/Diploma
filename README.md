# Find Codes 
(that can exist but haven't been found before)

1. **RUN**

Get results for (n, k, d) code:
1. Run Task3/Main.cpp
2. Enter n k d 0 (Ex: "7 4 3 0")
3. Wait until "If results are ready, press Enter"
4. Hear you have CNF form for SAT-solver
5. Run py/sat.py
6. When results are ready, they will be shown on the screen (sometimes you may have to wait days..)
7. After getting results, go back to the C++ console and press Enter. It will check the results and return a sign that it is correct (or it will find an error)
8. Now you can go back to step 1 and check another code or use Ctrl+Z

Checking (n, k, d) code:
1. Run Task3/Main.cpp
2. Enter n k d 1
3. Enter your own code
4. Wait until "if results are ready, press Enter"
5. Hear you have CNF form for SAT-solver
6. Run py/sat.py
7. When results appear on screen (after reading clauses, it will return answer in a few seconds)
8. After receiving results, return to C++ console and press Enter. It will check the results and return a sign that it is correct (or it will find an error)
9. Now you can go back to step 1 and check another code or use Ctrl+Z

Ex of input for checking code:

7 4 3 1

1 0 0 0 1 0 1 

0 1 0 0 1 1 1 

0 0 1 0 0 1 1 

0 0 0 1 1 1 0 


change logging if desired
ToDo


**2. SAVING ANSWERS**

After running program will save the results in folder "(n, k, d)" (with curr n, k and d):
1. cnf.py -- clauses = *CNF* // For running SAT-Solver
2. dimacs.txt -- CNF saved as a generally accepted format
3. dimacs_core_vars.txt -- saved numbers of base variables. These variables create the final code.
4. sat_result -- return number of variable if their value should be 1, or negative number if their value should be 0

**3. How it works**

* Explaining Math ToDo
