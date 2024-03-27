#include "global.h"
/**
 * @brief 
 * SYNTAX: TRANSPOSE MATRIX <matrix_name>
 */
bool syntacticParseTRANSPOSEMATRIX()
{
    logger.log("syntacticParseTRANSPOSEMATRIX");
    if (tokenizedQuery.size() != 3)
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = TRANSPOSE;
    parsedQuery.loadMatrixName = tokenizedQuery[2];
    return true;
}

bool semanticParseTRANSPOSEMATRIX()
{
    logger.log("semanticParseTRANSPOSEMATRIX");
    if (!matrixCatalogue.isMatrix(parsedQuery.loadMatrixName))
    {
        cout << "SEMANTIC ERROR: Matrix does not exists" << endl;
        return false;
    }
    return true;
}

void executeTRANSPOSEMATRIX()
{
    logger.log("executeTRANSPOSEMATRIX");
    // reset BLOCK stats
    BLOCKS_READ = 0;
    BLOCKS_WRITTEN = 0;

    Matrix *matrix = matrixCatalogue.getMatrix(parsedQuery.loadMatrixName);
    matrix->transpose();

    // block stats
    bufferManager.printBlockAccessStat();
    return;
}