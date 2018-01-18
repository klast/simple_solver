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
import time

input_matrix = sc.mmread('C:/users/spelevova/documents/simple_solver/tests/model4_test/mat.mtx')
rhs = sc.mmread('C:/users/spelevova/documents/simple_solver/tests/model4_test/rhs.mtx')
matrix = input_matrix.tocsc()
nx = matrix.shape[0]
#d_matrix = matrix.todense()
#x_exact = np.linalg.solve(d_matrix, rhs)
L = scipy.sparse.tril(matrix).tocsr() 
#для предобуславливателя Гаусса-Зейделя
def GS_precond(x):
    return spla.spsolve_triangular(L, x, lower = True)
GaussSeidel_preconditioner = spla.LinearOperator((nx, nx), GS_precond)
x_exact = spla.bicgstab(matrix, rhs, tol = 1e-8, maxiter=10000, M = GaussSeidel_preconditioner)
#x_exact = x_exact.reshape((x_exact.shape[0], 1))
sc.mmwrite("C:/users/spelevova/documents/simple_solver/tests/model4_test/test.mtx", x_exact[0].reshape(x_exact[0].shape[0],1))
