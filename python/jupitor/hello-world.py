#!./path/to/venv/bin/python3

import numpy as np

print("Hello, World")

arr1 = np.array([1, 2, 3, 4])
arr2 = np.array([[1, 2], [3, 4]])
arr3 = np.array([[1, 2, 4], [3, 4, 5]])

print(arr1.ndim)
print(arr2.ndim)
print(arr3.shape)
print(arr1.dtype)
print(arr1[3:0])
