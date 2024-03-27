#include "global.h"

Cursor::Cursor(string tableName, int pageIndex)
{
    logger.log("Cursor::Cursor");
    this->page = bufferManager.getPage(tableName, pageIndex);
    this->pagePointer = 0;
    this->tableName = tableName;
    this->pageIndex = pageIndex;
    this->maxPageIndex = -1;
}

Cursor::Cursor(string matrixName, int indx, int indy)
{
    logger.log("Cursor::Cursor");
    this->page = bufferManager.getMatrixPage(matrixName, indx, indy);
    this->pagePointer = 0;
    this->matrixName = matrixName;
    this->indx = indx;
    this->indy = indy;
}

/**
 * @brief Cursor structure made for helping in External sorting.
 * It can help in getting next row or storing rows based on flag value set
 */
Cursor::Cursor(string &tableName, int x, int y, bool retrieveFlag)
{
    logger.log("Cursor::Cursor");
    
    if (retrieveFlag) {

        int startPage = x;
        int endPage = y;
        
        this->tableName = tableName; 
        this->page = bufferManager.getPage(tableName, startPage);
        this->pageIndex = startPage;
        this->maxPageIndex = endPage;
        this->pagePointer = 0;
    }
    else
    {
        int maxNumRows;
        if(x != -1)
            maxNumRows = x;
        else
            maxNumRows = (uint)((BLOCK_SIZE * 1000) / (sizeof(int) * y));
        int numColumns = y;
        
        this->tableName = tableName;

        this->page = Page();
        string name = "../data/temp/" + tableName + "_Page0";
        this->page.setPageName(name);
        this->page.setDimensions(maxNumRows, numColumns);

        this->pageIndex = 0;
        this->pagePointer = 0;
        this->maxNumRows = maxNumRows;
        this->NumColumns = numColumns;
        this->rowCount = 0;
        this->rowsPerBlockCount.push_back(0);
    }
}

/**
 * @brief This function reads the next row from the page. The index of the
 * current row read from the page is indicated by the pagePointer(points to row
 * in page the cursor is pointing to).
 *
 * @return vector<int> 
 */
vector<int> Cursor::getNext()
{
    logger.log("Cursor::geNext");
    vector<int> result = this->page.getRow(this->pagePointer);
    this->pagePointer++;
    if(result.empty()){
        if(this->pageIndex == this->maxPageIndex)
            return result;
        tableCatalogue.getTable(this->tableName)->getNextPage(this);
        if(!this->pagePointer){
            result = this->page.getRow(this->pagePointer);
            this->pagePointer++;
        }
    }
    return result;
}

/**
 * @brief This function store the next sorted row in the resultant pages.
 * If a pages gets full, then it automatically moves to next page.
 */
void Cursor::setNext(vector<int> &row)
{
    logger.log("Cursor::setNext");
    if(this->pagePointer == this->maxNumRows)
    {
        // Write this page
        this->page.writePage();

        // Create new Page
        this->pageIndex++;
        this->rowsPerBlockCount.push_back(0);
        this->pagePointer = 0;

        this->page = Page();
        string name = "../data/temp/" + tableName + "_Page" + to_string(pageIndex);
        this->page.setPageName(name);
        this->page.setDimensions(this->maxNumRows, this->NumColumns); 
    }
    page.setRow(row);
    this->pagePointer++;
    this->rowCount++;
    this->rowsPerBlockCount.back()++;
}

/**
 * @brief Function that loads Page indicated by pageIndex. Now the cursor starts
 * reading from the new page.
 *
 * @param pageIndex 
 */
void Cursor::nextPage(int pageIndex)
{
    logger.log("Cursor::nextPage");
    this->page = bufferManager.getPage(this->tableName, pageIndex);
    this->pageIndex = pageIndex;
    this->pagePointer = 0;
}

/**
 * @brief Function that loads Page indicated by {indx, indy}. Now the cursor starts
 * reading from the new page.
 *
 * @param indx
 * @param indy
 */
void Cursor::nextMatrixPage(int indx, int indy)
{
    logger.log("Cursor::nextMatrixPage");
    this->page = bufferManager.getMatrixPage(this->matrixName, indx, indy);
    this->indx = indx;
    this->indy = indy;
    this->pagePointer = 0;
}