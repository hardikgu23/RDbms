#include "global.h"
/**
 * @brief 
 * SYNTAX: CHECKSYMMETRY matrix_name
 */
bool syntacticParseCHECKSYMMETRY()
{
    logger.log("syntacticParseCHECKSYMMETRY");
    if (tokenizedQuery.size() != 2)
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = CHECKSYMMETRY;
    parsedQuery.loadMatrixName = tokenizedQuery[1];
    return true;
}

bool semanticParseCHECKSYMMETRY()
{
    logger.log("semanticParseCHECKSYMMETRY");
    if (!matrixCatalogue.isMatrix(parsedQuery.loadMatrixName))
    {
        cout << "SEMANTIC ERROR:  Matrix does not exists" << endl;
        return false;
    }
    return true;
}

void executeCHECKSYMMETRY()
{
    logger.log("executeCHECKSYMMETRY");
    // reset BLOCK stats
    BLOCKS_READ = 0;
    BLOCKS_WRITTEN = 0;

    Matrix *matrix = matrixCatalogue.getMatrix(parsedQuery.loadMatrixName);
    if (matrix->isSymmetric())
    {
        cout << "TRUE" << endl;
    }
    else
    {
        cout << "FALSE" << endl;
    }

    // block stats
    bufferManager.printBlockAccessStat();
    return;
}