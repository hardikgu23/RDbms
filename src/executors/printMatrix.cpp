#include "global.h"
/**
 * @brief 
 * SYNTAX: PRINT MATRIX matrix_name
 */
bool syntacticParsePRINTMATRIX()
{
    logger.log("syntacticParsePRINTMATRIX");
    if (tokenizedQuery.size() != 3)
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = PRINTMATRIX;
    parsedQuery.printMatrixName = tokenizedQuery[2];
    return true;
}

bool semanticParsePRINTMATRIX()
{
    logger.log("semanticParsePRINTMATRIX");
    if (!matrixCatalogue.isMatrix(parsedQuery.printMatrixName))
    {
        cout << "SEMANTIC ERROR: Matrix doesn't exist" << endl;
        return false;
    }
    return true;
}

//TODO[pending]: check if printing max 20 line condition is working or not?
void executePRINTMATRIX()
{
    logger.log("executePRINTMATRIX");
    // reset BLOCK stats
    BLOCKS_READ = 0;
    BLOCKS_WRITTEN = 0;

    Matrix* matrix = matrixCatalogue.getMatrix(parsedQuery.printMatrixName);
    matrix->print();

    // block stats
    bufferManager.printBlockAccessStat();
    return;
}
