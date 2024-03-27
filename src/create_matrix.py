import random
import csv

# Create a 3x3 matrix (you can populate it with your data)
dim = 1000
matrix = []
for i in range(0, dim):
    x = []
    for j in range(dim*i+1, dim*(i+1)+1):
        x.append(random.randint(-2147483648, 2147483647))
    matrix.append(x)

# Specify the CSV file name
csv_filename = "../data/largemat.csv"

# Write the matrix to the CSV file
with open(csv_filename, mode='w', newline='') as file:
    writer = csv.writer(file, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)
    for row in matrix:
        writer.writerow(row)

print(f"Matrix has been written to {csv_filename}")