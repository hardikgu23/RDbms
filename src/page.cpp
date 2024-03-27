#include "global.h"
/**
 * @brief Construct a new Page object. Never used as part of the code
 *
 */
Page::Page()
{
    this->pageName = "";
    this->tableName = "";
    this->pageIndex = -1;
    this->rowCount = 0;
    this->columnCount = 0;
    this->rows.clear();
}

/**
 * @brief Construct a new Page:: Page object given the table name and page
 * index. When tables are loaded they are broken up into blocks of BLOCK_SIZE
 * and each block is stored in a different file named
 * "<tablename>_Page<pageindex>". For example, If the Page being loaded is of
 * table "R" and the pageIndex is 2 then the file name is "R_Page2". The page
 * loads the rows (or tuples) into a vector of rows (where each row is a vector
 * of integers).
 *
 * @param tableName 
 * @param pageIndex 
 */
Page::Page(string tableName, int pageIndex)
{
    logger.log("Page::Page");
    this->tableName = tableName;
    this->pageIndex = pageIndex;
    this->pageName = "../data/temp/" + this->tableName + "_Page" + to_string(pageIndex);
    Table table = *tableCatalogue.getTable(tableName);
    this->columnCount = table.columnCount;
    uint maxRowCount = table.maxRowsPerBlock;
    vector<int> row(columnCount, 0);
    this->rows.assign(maxRowCount, row);

    ifstream fin(pageName, ios::in);
    this->rowCount = table.rowsPerBlockCount[pageIndex];
    int number;
    for (uint rowCounter = 0; rowCounter < this->rowCount; rowCounter++)
    {
        for (int columnCounter = 0; columnCounter < columnCount; columnCounter++)
        {
            fin >> number;
            this->rows[rowCounter][columnCounter] = number;
        }
    }
    fin.close();
}

Page::Page(string tableName, int pageIndex, vector<vector<int>> rows, int rowCount)
{
    logger.log("Page::Page");
    this->tableName = tableName;
    this->pageIndex = pageIndex;
    this->rows = rows;
    this->rowCount = rowCount;
    this->columnCount = rows[0].size();
    this->pageName = "../data/temp/"+this->tableName + "_Page" + to_string(pageIndex);
}

// Check if a file exists
bool fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

Page::Page(string matrixname, int xind, int yind)
{
    logger.log("Page::Page");
    this->matrixname = matrixname;
    this->xind = xind;
    this->yind = yind;
    this->pageName = "../data/temp/" + this->matrixname + "_Page" + to_string(xind) + "_" + to_string(yind);

    this->datatype = "matrix";

    // resize matrix to MatrixPage_Dimension x MatrixPage_Dimension and initilaize values with -1
    this->matrix.resize(MatrixPage_Dimension, vector<int>(MatrixPage_Dimension, -1));
    
    if (fileExists(pageName)){
        ifstream fin(pageName, ios::in);
        // read matrix from file and store in matrix
        int number;
        for (int rowCounter = 0; rowCounter < MatrixPage_Dimension; rowCounter++)
        {
            for (int columnCounter = 0; columnCounter < MatrixPage_Dimension; columnCounter++)
            {
                fin >> number;
                this->matrix[rowCounter][columnCounter] = number;
            }
        }
        fin.close();
    }
}

Page::Page(string matrixname, int xind, int yind, vector<int> arr)
{
    // cout << "Debug: Reached matrix page" << endl;
    logger.log("Page::Page");
    this->matrixname = matrixname;
    this->xind = xind;
    this->yind = yind;
    this->pageName = "../data/temp/" + this->matrixname + "_Page" + to_string(xind) + "_" + to_string(yind);
    this->datatype = "matrix";

    this->matrix.resize(MatrixPage_Dimension, vector<int>(MatrixPage_Dimension, -1));

    // copy arr to matrix 
    // TODO[Pending]: Optimize this copy operation
    for (int rowCounter = 0; rowCounter < MatrixPage_Dimension; rowCounter++)
    {
        for (int columnCounter = 0; columnCounter < MatrixPage_Dimension; columnCounter++)
        {
            this->matrix[rowCounter][columnCounter] = arr[rowCounter * MatrixPage_Dimension + columnCounter];
        }
    }
}

/**
 * @brief Get row from page indexed by rowIndex
 * 
 * @param rowIndex 
 * @return vector<int> 
 */
vector<int> Page::getRow(int rowIndex)
{
    logger.log("Page::getRow");
    vector<int> result;
    result.clear();
    if (rowIndex >= this->rowCount)
        return result;
    return this->rows[rowIndex];
}

/**
 * @brief Return all rows in this page
 * 
 * @return vector<vector<int>> 
 */
vector<vector<int>> Page::getAllRows()
{
    return this->rows;
}

void Page::setRow(vector<int> &row)
{
    this->rows[rowCount++] = row;
}

void Page::setAllRows(vector<vector<int>> &rows)
{
    this->rows = rows;
    this->rowCount = rows.size();
}

vector<vector<int>> Page::getMatrix()
{
    return this->matrix;
}

void Page::setMatrix(vector<vector<int>> &matrix)
{
    this->matrix = matrix;
}

/**
 * @brief Get row from page indexed by rowIndex
 * 
 * @param rowIndex 
 * @return vector<int> 
 */
vector<int> Page::getMatrixRow(int rowIndex)
{
    logger.log("Page::getMatrixRow");
    vector<int> result;
    result.clear();
    if (rowIndex >= MatrixPage_Dimension)
        return result;
    return this->matrix[rowIndex];
}

/**
 * @brief writes current page contents to file.
 * 
 */
void Page::writePage()
{
    logger.log("Page::writePage");
    ofstream fout(this->pageName, ios::trunc);
    for (int rowCounter = 0; rowCounter < this->rowCount; rowCounter++)
    {
        for (int columnCounter = 0; columnCounter < this->columnCount; columnCounter++)
        {
            if (columnCounter != 0)
                fout << " ";
            fout << this->rows[rowCounter][columnCounter];
        }
        fout << endl;
    }
    fout.close();
}

/**
 * @brief writes current matrix page contents to file.
 * 
 */
void Page::writeMatrixPage()
{
    logger.log("Page::writeMatrixPage");
    BLOCKS_WRITTEN++;

    ofstream fout(this->pageName, ios::trunc);
    for (int rowCounter = 0; rowCounter < MatrixPage_Dimension; rowCounter++)
    {
        for (int columnCounter = 0; columnCounter < MatrixPage_Dimension; columnCounter++)
        {
            fout << this->matrix[rowCounter][columnCounter];
            if(columnCounter != MatrixPage_Dimension - 1)
                fout << ' ';
        }
        fout << endl;
    }
    fout.close();
}

/**
 * @brief Returns matrix[i][j] value
 * 
 */
int Page::getMatrixElement(int i, int j)
{
    return this->matrix[i][j];
}

/**
 * @brief Sets matrix[i][j] value to val
 * 
 */
void Page::setMatrixElement(int i, int j, int val)
{
    this->matrix[i][j] = val;
}

void Page::swapMatrixElement(int x1, int y1, int x2, int y2)
{
    swap(this->matrix[x1][y1], this->matrix[x2][y2]);
}

string Page::getMatrixName()
{
    return this->matrixname;
}

void Page::setPageName(string &pageName)
{
    this->pageName = pageName;
}

void Page::setDimensions(int maxRowCount, int columnCount)
{
    this->rowCount = 0;
    this->columnCount = columnCount;
    vector<int> row(columnCount, 0);
    this->rows.assign(maxRowCount, row);
}

void Page::resetRowCount()
{
    this->rowCount = 0;
}

int Page::getColumnCount()
{
    return this->columnCount;
}

int Page::getRowCount()
{
    return this->rowCount;
}