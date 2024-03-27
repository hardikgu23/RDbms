import pandas as pd
import random

# Create a dictionary to store data for each column
data = {
    'A': [random.randint(1, 5) for _ in range(1020)],
    'B': [random.randint(1, 5) for _ in range(1020)],
    'C': [random.randint(1, 5) for _ in range(1020)],
    'D': [random.randint(1, 5) for _ in range(1020)],
    'E': [random.randint(1, 5) for _ in range(1020)]
}

# Create a DataFrame from the dictionary
df = pd.DataFrame(data)

# Save the DataFrame as a CSV file in the specified folder
csv_file_path = '../data/Test.csv'
df.to_csv(csv_file_path, index=False)

print(f"CSV file '{csv_file_path}' has been created with 1000 rows and columns A, B, C, D, and E.")
