#include "global.h"
/**
 * @brief 
 * SYNTAX: COMPUTE <matrix_name>
 */
bool syntacticParseCOMPUTE()
{
    logger.log("syntacticParseCOMPUTE");
    if (tokenizedQuery.size() != 2)
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = COMPUTE;
    parsedQuery.loadMatrixName = tokenizedQuery[1];
    return true;
}

bool semanticParseCOMPUTE()
{
    logger.log("semanticParseCOMPUTE");

    // Check if mateix is already loaded
    if (!matrixCatalogue.isMatrix(parsedQuery.loadMatrixName))
    {
        cout << "SEMANTIC ERROR: Matrix does not exists" << endl;
        return false;
    }

    return true;
}

void executeCOMPUTE()
{
    logger.log("executeCOMPUTE");
    // reset BLOCK stats
    BLOCKS_READ = 0;
    BLOCKS_WRITTEN = 0;

    Matrix *matrix = matrixCatalogue.getMatrix(parsedQuery.loadMatrixName);
    matrix->compute();

    // block stats
    bufferManager.printBlockAccessStat();
    return;
}