#ifndef PAGE_H
#define PAGE_H

#include"logger.h"
/**
 * @brief The Page object is the main memory representation of a physical page
 * (equivalent to a block). The page class and the page.h header file are at the
 * bottom of the dependency tree when compiling files. 
 *<p>
 * Do NOT modify the Page class. If you find that modifications
 * are necessary, you may do so by posting the change you want to make on Moodle
 * or Teams with justification and gaining approval from the TAs. 
 *</p>
 */

class Page{

    string tableName;
    string pageIndex;
    int columnCount;
    int rowCount;
    vector<vector<int>> rows;
    vector<vector<int>> matrix;

    string matrixname;
    int xind;
    int yind;

    public:

    string datatype = "table"; // possible values = ["table", "matrix"]
    string pageName = "";
    Page();
    Page(string tableName, int pageIndex);
    Page(string tableName, int pageIndex, vector<vector<int>> rows, int rowCount);
    Page(string matrixname, int xind, int yind);
    Page(string matrixname, int xind, int yind, vector<int> arr);

    vector<int> getRow(int rowIndex);
    vector<vector<int>> getAllRows();
    void setRow(vector<int> &row);
    void setAllRows(vector<vector<int>> &rows);
    vector<int> getMatrixRow(int rowIndex);
    void writePage();
    void writeMatrixPage();
    vector<vector<int>> getMatrix();
    void setMatrix(vector<vector<int>> &matrix);
    int getMatrixElement(int x, int y);
    void setMatrixElement(int x, int y, int val);
    void swapMatrixElement(int x1, int y1, int x2, int y2);
    string getMatrixName();

    // Functions to set Page Private variables
    void setPageName(string &pageName);
    void setDimensions(int rowCount, int columnCount);
    void resetRowCount();
    int getColumnCount();
    int getRowCount();
};

#endif