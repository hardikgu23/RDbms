#ifndef CURSOR_H
#define CURSOR_H

#include"bufferManager.h"
/**
 * @brief The cursor is an important component of the system. To read from a
 * table, you need to initialize a cursor. The cursor reads rows from a page one
 * at a time.
 *
 */
class Cursor{
    public:
    Page page;
    int pageIndex;
    int maxPageIndex = -1;
    int maxNumRows = -1;
    int NumColumns = -1;

    int indx;
    int indy;
    string tableName;
    string matrixName;
    int pagePointer;

    public:
    int rowCount;
    vector<uint> rowsPerBlockCount;
    Cursor(string tableName, int pageIndex);
    Cursor(string matrixname, int indx, int indy);
    Cursor(string &pageName, int x, int y, bool retrievalFlag);
    vector<int> getNext();
    void setNext(vector<int> &row);
    void nextPage(int pageIndex);
    void nextMatrixPage(int indx, int indy);
};

#endif