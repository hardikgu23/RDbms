#include "global.h"

/**
 * @brief Construct a new Matrix:: Matrix object
 *
 */
Matrix::Matrix()
{
    logger.log("Matrix::Matrix");
}

/**
 * @brief Construct a new Matrix:: Matrix object used in the case where the data
 * file is available and LOAD command has been called. This command should be
 * followed by calling the load function;
 *
 * @param MatrixName 
 */
Matrix::Matrix(string MatrixName)
{
    logger.log("Matrix::Matrix");
    this->sourceFileName = "../data/" + MatrixName + ".csv";
    this->MatrixName = MatrixName;
}

/**
 * @brief The load function is used when the LOAD command is encountered. It
 * reads data from the source file, splits it into block.
 *
 * @return true if the Matrix has been successfully loaded 
 * @return false if an error occurred 
 */
bool Matrix::load()
{
    logger.log("Matrix::load");
    fstream fin(this->sourceFileName, ios::in);
    string line;
    if (getline(fin, line))
    {
        fin.close();
        if (this->blockify())
            return true;
    }
    fin.close();
    return false;
}

/**
 * @brief This function splits the matrix into smaller matrices and stores each smaller matrix in 
 * a seperate block
 *
 * @return true if successfully blockified
 * @return false otherwise
 */
bool Matrix::blockify()
{
    logger.log("Matrix::blockify");
    ifstream fin(this->sourceFileName, ios::in);

    // find dimension of matrix
    string firstline;
    getline(fin, firstline);

    fin.close();

    int num_entries =0;
    int posn =0;

    while ((posn = firstline.find(',')) != std::string::npos) {
        num_entries++;
        firstline.erase(0, posn + 1);
    }
    num_entries++; // Count the last entry
    this->dimension = num_entries;

    // cout << "Debug: Matrix dimension: " << this->dimension << endl;

    /*
        Lets say mat1.csv contains a 9X9 matrix and each page can store a matrix of dimension 3X3
        Then the matrix will be split into 9 pages as follows:

        `page00`  `page01`  `page02`  
        | 0 1 2 | | 3 4 5 | | 6 7 8 |
        | 9 1 7 | | 3 6 5 | | 1 4 6 |
        | 2 5 6 | | 6 3 6 | | 2 4 5 |

        `page10`  `page11`  `page12`
        | 2 7 6 | | 2 3 6 | | 3 5 2 |
        | 9 6 8 | | 2 3 6 | | 6 2 7 |
        | 8 5 2 | | 2 4 5 | | 2 3 8 |
       
        `page20`  `page21`  `page22`
        | 2 4 2 | | 7 4 2 | | 2 5 2 |
        | 7 2 4 | | 2 5 1 | | 3 6 7 |
        | 1 2 7 | | 8 2 8 | | 2 5 3 |
    */

    // block[i] -> stores the ith page of matrix for one row. Like in above example, each row consists of 3 pages
    int pages_per_row = ceil( (this->dimension*1.0) / MatrixPage_Dimension); // Number of pages in each row
    this->blockCount = pages_per_row;

    cout << "Pages per row is " << pages_per_row << endl;

    // cout << "Debug: Pages per row: " << pages_per_row << endl;

    vector<vector<int>> block(pages_per_row);

    // Open the file again
    fin.open(this->sourceFileName, ios::in);

    int indx = 0;
    while(1)
    {
        bool flag = false;
        string line;
        int num_rows_stored = 0;

        while(getline(fin, line))
        {
            int cur_block =0;
            flag = true;

            stringstream ss(line);
            string token;

            int stored = 0;

            // cout << "Debug: Line: " << line << endl;
            while(getline(ss, token, ','))
            {
                if(stored == MatrixPage_Dimension)
                {
                    stored = 0;
                    cur_block++;

                    // cout << "Debug: Page was incremented" << endl;
                }

                block[cur_block].push_back(stoi(token));
                stored++;
            }
            // cout << "Debug: Line processed" << endl;

            // If the number of entries in a row is not a multiple of Page_Dimension, then fill the remaining entries with -1
            while(stored != MatrixPage_Dimension)
            {
                // cout << "Debug: Filling -1 " << stored << " " << cur_block << endl;
                block[cur_block].push_back(-1);
                stored++;
            }

            num_rows_stored++;
            if(num_rows_stored == min(MatrixPage_Dimension, this->dimension))
                break;
        }

        int total_num_per_page = MatrixPage_Dimension*MatrixPage_Dimension;

        // if the number of rows in a block is not a multiple of Page_Dimension, then fill the remaining rows with -1
        while(num_rows_stored != MatrixPage_Dimension)
        {
            for(int i=0; i<pages_per_row; i++)
                block[i].insert(block[i].end(), total_num_per_page - block[i].size(), -1);
            num_rows_stored++;
        }

        if(!flag)
            break;

        // TODO: Write these blocks to disk, breaking for now
        for(int indj=0; indj<pages_per_row; indj++)
        {
            // cout << "Debug: Writing page " << indx << " " << indj << " " << block[indj].size() << endl;
            bufferManager.writeMatrixPage(this->MatrixName, indx, indj, block[indj]);
        }

        // reset the block
        for(int indj=0; indj<pages_per_row; indj++)
            block[indj].clear();

        indx++;
    }
   return true;
}

/**
 * @brief Function prints the first few rows of the matrix. If the matrix contains
 * more rows than PRINT_COUNT, exactly PRINT_COUNT rows are printed, else all
 * the rows are printed.
 *
 */
void Matrix::print()
{
    logger.log("Matrix::print");
    uint count = min((int)PRINT_COUNT, this->dimension);

    vector<int> row;

    int indx;
    int indy;

    Page page;

    for(int i=0; i< count; i++)
    {
        int count = 0;
        for(int j=0; j< this->blockCount; j++)
        {
            indx = i/MatrixPage_Dimension;
            indy = j;
            page = bufferManager.getMatrixPage(this->MatrixName, indx, indy);
            row = page.getMatrixRow(i%MatrixPage_Dimension);

            for(int i=0; i < row.size(); ++i)
            {
                if(count >= min(20, this->dimension))
                    break;
                cout << row[i] << " ";
                count++;
            }
            if(count >= min(20, this->dimension))
                    break;
        }
        cout << endl;
    }
}

/**
 * @brief Function to check if matrix is already exported
 *
 * @return true if exported
 * @return false otherwise
 */
bool Matrix::isPermanent()
{
    logger.log("Matrix::isPermanent");
    if (this->sourceFileName == "../data/" + this->MatrixName + ".csv")
        return true;
    return false;
}

/**
 * @brief called when EXPORT command is invoked to move source file to "data"
 * folder.
 *
 */
void Matrix::makePermanent()
{
    // cout << "Coming in makePermanent" << endl;
    logger.log("Matrix::makePermanent");
    if(!this->isPermanent())
        bufferManager.deleteFile(this->sourceFileName);
    string newSourceFile = "../data/" + this->MatrixName + ".csv";
    ofstream fout(newSourceFile, ios::out);
    
    // Store the number of valid entries left in each row
    vector<int> entries_left(this->dimension, this->dimension);
    vector<int> row;

    for(int indx =0; indx < this->blockCount; ++indx)
    {
        int line_count = 0;
        while(line_count != min(MatrixPage_Dimension, this->dimension))
        {
            for(int indy =0; indy < this->blockCount; ++indy)
            {
                // Check if it is a valid line or not ?
                int line_num = MatrixPage_Dimension*indx + line_count;
                if(line_num >= this->dimension)
                    break;
                
                // extract the page[indx][indy]
                Page page = bufferManager.getMatrixPage(this->MatrixName, indx, indy);
                row = page.getMatrixRow(line_count);

                // write the row
                for (int posn=0; posn < row.size(); ++posn)
                {
                    fout << row[posn];
                    entries_left[line_num]--;

                    if(entries_left[line_num] != 0)
                    {
                        fout << ",";
                    } 
                    else
                    {
                        fout << "\n";
                        break;
                    }
                }
                
            }
            line_count++;
        }
    }
    fout.close();
}

/**
 * @brief Tells if the matrix is symmetric or not
 *
 * @return True if matrix is symmetric else false
 */
bool Matrix::isSymmetric()
{
    logger.log("Matrix::isSymmetric");

    // Diagonal pages should be symmetric
    for (int i =0; i < this->blockCount; i++)
    {
        Page page = bufferManager.getMatrixPage(this->MatrixName, i, i);
        
        // checking if matrix for this page is symmetric or not?
        for (int x=0; x < MatrixPage_Dimension; x++)
        {
            for(int y=0; y < MatrixPage_Dimension; y++)
            {
                if(page.getMatrixElement(x,y) != page.getMatrixElement(y,x))
                    return false;
            }
        }
    }

    // Pageij should be symmetric with Pageji
    // That means Pageij[i][j] = Pageji[j][i]

    for(int i=0; i < this->blockCount; i++)
    {
        for(int j=i+1; j < this->blockCount; j++)
        {
            Page pageij = bufferManager.getMatrixPage(this->MatrixName, i, j);
            Page pageji = bufferManager.getMatrixPage(this->MatrixName, j, i);

            for(int x=0; x < MatrixPage_Dimension; x++)
            {
                for(int y=0; y < MatrixPage_Dimension; y++)
                {
                    if(pageij.getMatrixElement(x,y) != pageji.getMatrixElement(y,x))
                        return false;
                }
            }
        }
    }

    return true;
}

void __Matrixtranspose__(Page &page)
{
    for(int i=0; i < MatrixPage_Dimension; ++i)
    {
        for(int j=i+1; j < MatrixPage_Dimension; ++j)
        {
            page.swapMatrixElement(i,j,j,i);
        }
    }
}

/**
 * @brief Function will transpose the matrix
 *
 */
void Matrix::transpose()
{
    logger.log("Matrix::transpose");

    for(int i=0; i < this->blockCount; ++i)
    {
        Page page = bufferManager.getMatrixPage(this->MatrixName, i, i);
        __Matrixtranspose__(page);
        page.writeMatrixPage();
    }

    // transpose the pages off diagonal
    for (int indx=0; indx < this->blockCount; ++indx)
    {
        for(int indy=indx+1; indy < this->blockCount; ++indy)
        {
            Page pageij = bufferManager.getMatrixPage(this->MatrixName, indx, indy);
            Page pageji = bufferManager.getMatrixPage(this->MatrixName, indy, indx);

            __Matrixtranspose__(pageij);
            __Matrixtranspose__(pageji);

            pageij.writeMatrixPage();
            pageji.writeMatrixPage();

        }
    }

    // delete pages from buffer for this matrix
    bufferManager.deleteFromPool(this->MatrixName);
}

/**
 * @brief Function will calculate A - A^T for a matrix A
 *
 */
void __compute__(Page &page1, Page &page2, Page &respage)
{
    for(int i=0; i<MatrixPage_Dimension; ++i)
    {
        for(int j=0; j<MatrixPage_Dimension; ++j)
        {
            int val = page1.getMatrixElement(i,j) - page2.getMatrixElement(j,i);
            respage.setMatrixElement(i,j, val);
        }
    }
}

// TOOD[pending]: Optimize this function
void Matrix::compute()
{
    logger.log("Matrix::compute");
    string resultantMatrixName = this->MatrixName + "_RESULT";

    // First create all the pages for resultant matrix
    for (int i=0; i < this->blockCount; ++i)
    {
        for (int j=i; j < this->blockCount; ++j)
        {
            Page pageij = bufferManager.getMatrixPage(this->MatrixName, i, j);
            Page pageji = bufferManager.getMatrixPage(this->MatrixName, j, i);

            Page respageij(resultantMatrixName, i, j);
            Page respageji(resultantMatrixName, j, i);

            __compute__(pageij, pageji, respageij);
            if (i != j)
            __compute__(pageji, pageij, respageji);

            respageij.writeMatrixPage();
            if(i != j)
            respageji.writeMatrixPage();
        }
    }

    // Add the resultant matrix to matrixCatalogue
    Matrix* resultantMatrix = new Matrix(resultantMatrixName);
    resultantMatrix->dimension = this->dimension;
    resultantMatrix->blockCount = this->blockCount;
    matrixCatalogue.insertMatrix(resultantMatrix);
}

void Matrix::rename(string newMatrixName)
{
    logger.log("Matrix::rename");
    // delete all current pages of this matrix from pool
    bufferManager.deleteFromPool(this->MatrixName);

    // rename all the pages of the matrix
    for (int i=0; i < this->blockCount; ++i)
    {
        for(int j=0; j < this->blockCount; ++j)
        {
           string oldname = "../data/temp/" + this->MatrixName + "_Page" + to_string(i) + "_" + to_string(j);
           string newname = "../data/temp/" + newMatrixName + "_Page" + to_string(i) + "_" + to_string(j);
           std::rename(oldname.c_str(), newname.c_str());
        }
    }
    
    matrixCatalogue.deleteMatrixFromCatalogue(this->MatrixName);
    this->MatrixName = newMatrixName;
    matrixCatalogue.insertMatrix(this);
}