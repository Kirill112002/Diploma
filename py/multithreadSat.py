#!/usr/bin/env python

import logging
import multiprocessing
from pysat.examples.genhard import PHP
from pysat.solvers import Solver
from pysat.solvers import Cadical195
import time

from myconfig import *

logger_format = '%(asctime)s:%(threadName)s:%(message)s'
logging.basicConfig(format=logger_format, level=logging.INFO, datefmt="%H:%M:%S")

def run_solver(solver, pigeons):
    """
        Run a single solver on a given formula.
    """

    logging.info(f"starting {solver} for {pigeons} pigeons")
    with Solver(name=solver, bootstrap_with=PHP(pigeons-1)) as s:
        for clause in clauses:
            s.add_clause(clause)
        res = s.solve()
    logging.info(f"finished {solver} for {pigeons} pigeons -- {res} outcome")

if __name__ == '__main__':
    logging.info("Main started")
    logging.info("Creating tasks")

    threads = [multiprocessing.Process(target=run_solver, args=(solver, pigeons)) \
            for solver, pigeons in zip(['cd'], [11])]
    for thread in threads:
        thread.start()
    for thread in threads:
        thread.join() # waits for thread to complete its task

    logging.info("Main Ended")

