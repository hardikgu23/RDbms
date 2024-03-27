## Idea
We have divided the input **NxN** matrix into smaller **MxM** matrix and store this submatrix in pages. This was done to do transpose and compute operation efficiently which otherwise would have been very inefficient if matrix was stored row wise in pages.

The value of **M** is decided in such a way that entire submatrix can be stored in a single page. This can be calculated using -> \
M = `(int)sqrt((BLOCK_SIZE * 1024) / sizeof(int))`  
M = 10 (considering BLOCK_SIZE = 1)

## Code Implementation

- Created **Matrix** class that stores info related to a particular matrix. This included matrixName, sourceFileName, NoOfPages used to store it, its dimension.

- Created **matrixCatalogue** class which resembles the tableCatalogue in terms of functionality

- Modifies **Page** and **BufferManager** classes, adding functions like GetMatrixPage and WriteMatrixPage.

- Modified **syntacticParser** and **semanticParser** to support new commands.

- Added new executors for commands -> **loadMatrix**, **PrintMatrix**, **CheckSymmetry**, **TransposeMatrix**, **Compute**. 

The changes corresponding to each command are listed below :

`LOAD MATRIX` : It works by Creating a Matrix object (matrix) for the passed matrixFile, it then calls **matrix.load()** which in turn calls **matrix.blokify()**. This blockify function is responsible for breaking of matrix into smaller matrix and calling **bufferManager.writeMatrixPage()** for writting page into disk.

Blocks Accessed : We are writting each block once in the disk, So  
`Total blocks accessed = <Total_Num_Blocks>`

Error handling -> Checking for correct syntax. Raise error if matrix file does not exists or matrix is already loaded.

`PRINT MATRIX` : This method prints the matrix. If dimension of matrix are greater than 20, then it prints top 20 rows and columns.
It works by iterating through the blocks of the same row 
i.e pagei0, pagei1, pagei2.... . Each iteration through a row of block will give us one row of matrix. Then we repeat the same process to get the next line of matrix from these blocks. Once we have exhausted these blocks, we move to next set of blocks (those in next row).

Example : Lets say each block is storing a 3x3 submatrix and matrix size is 9x9. Then we will traverse in this manner 

Page00,Page01,Page02 -> Gets 1st row of matrix
Page00,Page01,Page02 -> Gets 2nd row of matrix
Page00,Page01,Page02 -> Gets 3rd row of matrix
Page10,Page11,Page12 -> Gets 4th row of matrix
.
.
.
Page20,Page21,Page22 -> Gets 9th row of matrix.

We do this for max top 20 rows of matrix

Blocks Accessed : Since minimum size of block can store a 10x10 matrix, thus we would have to fetch at max 4 pages to print top 20x20 matrix.

`Total Blocks Accessed = max 4 blocks`

Error handling -> Checking for correct syntax. Raise error if matrix does not exists (not loaded).

`EXPORT MATRIX` : This command works very similary to the print matrix. It iterates through row of pages to get a single row of matrix and print it into the csv file.
For each row of matrix, we will have to read a row of page M(=10) times.

`Total Blocks Accessd = M * <Total_Num_Blocks>`

Error handling -> Checking for correct syntax. Raise error if matrix does not exists (not loaded).

`RENAME MATRIX` : This command changes the name of matrix for the current session. It does this by changing the name of all pages associated with currentMatrixName to newMatrixName (Page_OldName_i_j -> Page_NewName_i_j).
It replaces the old matrix from matrixCatalogue by changing the key(to NewmatrixName).
Pages corresponding to oldMatrix are deleted from pool.

No blocks are accessed for this command.

Error handling -> Checking for correct syntax. Raise error if matrix does not exists (not loaded).

`CHECKSYMMETRY` : This command access Pageij and Pageji and checks whether they are symmetric to each other or not (ie Pageij[x][y] == Pageji[y][x] ?). If we find any combination of ij to not satisfy this, then the matrix is not symmetric and we return `false` else we return `true`.
We will be reading each block once in the memory and then compare it with its counterpart.Thus

`Total Blocks Accessd = <Total_Num_Blocks>`

Error handling -> Checking for correct syntax. Raise error if matrix does not exists (not loaded).

`TRANSPOSE MATRIX` : This command access Pageij and Pageji and transposes the submatrix stored in the respective page. Then we store this transposed submatrix of Pageij into Pageji and vice versa.
We will be reading each block once and will write into each block once. Thus

`Total Blocks Accessd = 2 * <Total_Num_Blocks>`

Error handling -> Checking for correct syntax. Raise error if matrix does not exists (not loaded).

`COMPUTE` : This command is very similar to Transpose in its functioning. We access Pageij and Pageji and instead of just transposing submatrix,
we do subMatrix[x][y] - subMatrix[y][x] and then swap the page's submatrices.
The block access will be same as that of TRANSPOSE as each block is read once and written once.

`Total Blocks Accessd = 2 * <Total_Num_Blocks>`

Error handling -> Checking for correct syntax. Raise error if matrix does not exists (not loaded).

## Contributions (50-50)

Hardik - Worked on Load, PRINT, TRANSPOSE, COMPUTE and RENAME command.

Vidhi - Worked on PRINT, EXPORT and CHECKSYMMETRY. Tested the code against various kind of matrices and error Handling.