import numpy as np
import os

# Define the dimension of the symmetric matrix
n = 10000  # Change this to your desired dimension
filename = 'symlarge.csv'  # Change this to your desired filename

# Create a symmetric matrix and store it in a csv comma separated file
Matrix = []
for i in range(n):
    row = []
    for j in range(n):
        row.append(i+j)
    Matrix.append(row)

# Save the matrix in a csv file in ../data/ folder as integer
np.savetxt(os.path.join('..', 'data', filename), Matrix, delimiter=',', fmt='%d')