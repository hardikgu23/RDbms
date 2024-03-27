#include "global.h"
/**
 * @brief 
 * SYNTAX: Raname Matrix <matrix_name> <new_matrix_name>
 */
bool syntacticParseRENAMEMATRIX()
{
    logger.log("syntacticParseRENAMEMATRIX");
    if (tokenizedQuery.size() != 4)
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = RENAMEMATRIX;
    parsedQuery.loadMatrixName = tokenizedQuery[2];
    parsedQuery.newMatrixName = tokenizedQuery[3];
    return true;
}

bool semanticParseRENAMEMATRIX()
{
    logger.log("semanticParseRENAMEMATRIX");
    if (!matrixCatalogue.isMatrix(parsedQuery.loadMatrixName))
    {
        cout << "SEMANTIC ERROR:  Matrix does not exists" << endl;
        return false;
    }
    return true;
}

void executeRENAMEMATRIX()
{
    logger.log("executeRENAMEMATRIX");
    // reset BLOCK stats
    BLOCKS_READ = 0;
    BLOCKS_WRITTEN = 0;

    Matrix *matrix = matrixCatalogue.getMatrix(parsedQuery.loadMatrixName);
    matrix->rename(parsedQuery.newMatrixName);

    // block stats
    bufferManager.printBlockAccessStat();
    return;
}