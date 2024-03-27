#include "global.h"
/**
 * @brief 
 * SYNTAX: LOAD MATRIX matrix_name
 */
bool syntacticParseLOADMATRIX()
{
    logger.log("syntacticParseMatrixLOAD");
    if (tokenizedQuery.size() != 3)
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = LOADMATRIX;
    parsedQuery.loadMatrixName = tokenizedQuery[2];
    return true;
}

bool semanticParseLOADMATRIX()
{
    logger.log("semanticParseMatrixLOAD");

    // check if matrix exists
    if (!isFileExists(parsedQuery.loadMatrixName))
    {
        cout << "SEMANTIC ERROR: Data file doesn't exist" << endl;
        return false;
    }

    // // Check if mateix is already loaded
    if (matrixCatalogue.isMatrix(parsedQuery.loadMatrixName))
    {
        cout << "SEMANTIC ERROR: Matrix already exists" << endl;
        return false;
    }

    return true;
}

void executeLOADMATRIX()
{
    logger.log("executeLOAD");
    // reset BLOCK stats
    BLOCKS_READ = 0;
    BLOCKS_WRITTEN = 0;

    Matrix *matrix = new Matrix(parsedQuery.loadMatrixName);

    if (matrix->load())
    {
        matrixCatalogue.insertMatrix(matrix);
    }

    // print stats
    bufferManager.printBlockAccessStat();
    return;
}