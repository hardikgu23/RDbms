#include "global.h"

/**
 * @brief 
 * SYNTAX: EXPORT <matrix_name> 
 */

bool syntacticParseEXPORTMATRIX()
{
    logger.log("syntacticParseEXPORTMATRIX");
    if (tokenizedQuery.size() != 3)
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = EXPORTMATRIX;
    parsedQuery.exportMatrixName = tokenizedQuery[2];
    return true;
}

bool semanticParseEXPORTMATRIX()
{
    logger.log("semanticParseEXPORTMATRIX");
    //Matrix should exist
    if (matrixCatalogue.isMatrix(parsedQuery.exportMatrixName))
        return true;
    cout << "SEMANTIC ERROR: No such matrix exists" << endl;
    return false;
}

void executeEXPORTMATRIX()
{
    logger.log("executeEXPORTMATRIX");
    // reset BLOCK stats
    BLOCKS_READ = 0;
    BLOCKS_WRITTEN = 0;

    Matrix* matrix = matrixCatalogue.getMatrix(parsedQuery.exportMatrixName);
    matrix->makePermanent();

    // block stats
    bufferManager.printBlockAccessStat();
    return;
}