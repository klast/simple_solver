# -*- coding: utf-8 -*-
"""
Created on Wed Jan 17 13:30:08 2018

@author: spelevova
"""

import scipy.sparse.linalg as spla
import scipy.io as sc
import scipy
import numpy as np
import pandas as pd

input_matrix = sc.mmread('C:/users/spelevova/documents/simple_solver/tests/model4_test/mat.mtx')
rhs = sc.mmread('C:/users/spelevova/documents/simple_solver/tests/model4_test/rhs.mtx')
matrix = input_matrix.tocsc()
d_matrix = matrix.todense()
x_exact = np.linalg.solve(d_matrix, rhs)
#x_exact = x_exact.reshape((x_exact.shape[0], 1))
sc.mmwrite("C:/users/spelevova/documents/simple_solver/tests/model4_test/test.mtx", x_exact)
