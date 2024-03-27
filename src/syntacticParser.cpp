#include "global.h"

bool syntacticParse()
{
    logger.log("syntacticParse");
    string possibleQueryType = tokenizedQuery[0];

    if (tokenizedQuery.size() < 2)
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }

    if (possibleQueryType == "CLEAR")
        return syntacticParseCLEAR();
    else if (possibleQueryType == "INDEX")
        return syntacticParseINDEX();
    else if (possibleQueryType == "LIST")
        return syntacticParseLIST();
    else if(possibleQueryType == "LOAD")
    {
        if(tokenizedQuery[1] == "MATRIX")
            return syntacticParseLOADMATRIX();
        else
            return syntacticParseLOAD();
    }
    else if (possibleQueryType == "PRINT")
    {
        if(tokenizedQuery[1] == "MATRIX")
            return syntacticParsePRINTMATRIX();
        else
        return syntacticParsePRINT();
    }
    else if(possibleQueryType == "CHECKSYMMETRY")
        return syntacticParseCHECKSYMMETRY();
    else if(possibleQueryType == "COMPUTE")
        return syntacticParseCOMPUTE();
    else if(possibleQueryType == "TRANSPOSE" && tokenizedQuery[1] == "MATRIX")
        return syntacticParseTRANSPOSEMATRIX();
    else if (possibleQueryType == "RENAME")
    {
        if(tokenizedQuery[1] == "MATRIX")
            return syntacticParseRENAMEMATRIX();
        else
        return syntacticParseRENAME();
    }
    else if(possibleQueryType == "EXPORT")
    {
        if(tokenizedQuery[1] == "MATRIX")
            return syntacticParseEXPORTMATRIX();
        else
        return syntacticParseEXPORT();
    }
    else if(possibleQueryType == "SOURCE")
        return syntacticParseSOURCE();
    else if(possibleQueryType == "SORT")
        return syntacticParseSORT();
    else
    {
        string resultantRelationName = possibleQueryType;
        if (tokenizedQuery[1] != "<-" || tokenizedQuery.size() < 3)
        {
            cout << "SYNTAX ERROR" << endl;
            return false;
        }
        possibleQueryType = tokenizedQuery[2];
        if (possibleQueryType == "PROJECT")
            return syntacticParsePROJECTION();
        else if (possibleQueryType == "SELECT")
            return syntacticParseSELECTION();
        else if (possibleQueryType == "JOIN")
            return syntacticParseJOIN();
        else if (possibleQueryType == "CROSS")
            return syntacticParseCROSS();
        else if (possibleQueryType == "DISTINCT")
            return syntacticParseDISTINCT();
        else if(possibleQueryType == "ORDER")
            return syntacticParseORDER();
        else if(possibleQueryType == "GROUP")
            return syntacticParseGROUP();
        else
        {
            cout << "SYNTAX ERROR" << endl;
            return false;
        }
    }
    return false;
}

ParsedQuery::ParsedQuery()
{
}

void ParsedQuery::clear()
{
    logger.log("ParseQuery::clear");
    this->queryType = UNDETERMINED;

    this->clearRelationName = "";

    this->crossResultRelationName = "";
    this->crossFirstRelationName = "";
    this->crossSecondRelationName = "";

    this->distinctResultRelationName = "";
    this->distinctRelationName = "";

    this->exportRelationName = "";
    this->exportMatrixName = "";

    this->indexingStrategy = NOTHING;
    this->indexColumnName = "";
    this->indexRelationName = "";

    this->joinBinaryOperator = NO_BINOP_CLAUSE;
    this->joinResultRelationName = "";
    this->joinFirstRelationName = "";
    this->joinSecondRelationName = "";
    this->joinFirstColumnName = "";
    this->joinSecondColumnName = "";

    this->loadRelationName = "";

    this->printRelationName = "";

    this->projectionResultRelationName = "";
    this->projectionColumnList.clear();
    this->projectionRelationName = "";

    this->renameFromColumnName = "";
    this->renameToColumnName = "";
    this->renameRelationName = "";

    this->selectType = NO_SELECT_CLAUSE;
    this->selectionBinaryOperator = NO_BINOP_CLAUSE;
    this->selectionResultRelationName = "";
    this->selectionRelationName = "";
    this->selectionFirstColumnName = "";
    this->selectionSecondColumnName = "";
    this->selectionIntLiteral = 0;

    // this->sortingStrategy = NO_SORT_CLAUSE;
    // this->sortResultRelationName = "";
    // this->sortColumnName = "";
    this->sortRelationName = "";
    this->sortOrder.clear();
    this->sortColumnName.clear();

    this->orderRelationName = "";
    this->orderResultRelationName = "";

    this->sourceFileName = "";
}

/**
 * @brief Checks to see if source file exists. Called when LOAD command is
 * invoked.
 *
 * @param tableName 
 * @return true 
 * @return false 
 */
bool isFileExists(string tableName)
{
    string fileName = "../data/" + tableName + ".csv";
    struct stat buffer;
    return (stat(fileName.c_str(), &buffer) == 0);
}

/**
 * @brief Checks to see Matrix is valid or not. It will check for any invalid value and 
 * if it is square matrix or not.
 *
 * @param MatrixName 
 * @return true 
 * @return false 
 */
bool isValidMatrix(const string& MatrixName)
{
    // TODO[pending]: Check if it is handling spaces or not?
    // Construct the file path
    string filename = "../data/" + MatrixName + ".csv";

    ifstream file(filename);
    int numbers_in_row =0;
    string line;

    int dim = 0;
    int num_rows = 0;

    // Read the CSV file line by line
    while (getline(file, line))
    {
        stringstream ss(line);
        string token;
        int num;

        // Parse each element of the current line
         while(getline(ss, token, ','))
        {
            // cout << "hello " << token << endl;
            numbers_in_row++;
        }

        // If this is the first row, set n
        if (numbers_in_row && dim == 0)
            dim = numbers_in_row;

        // Check if the row has the same number of elements as the previous rows
        if (numbers_in_row != dim)
        {
            cout << "Rows in the CSV file have inconsistent sizes: " << filename << endl;
            return false;
        }

        // Clear the row vector for the next iteration
        numbers_in_row = 0;
        num_rows++;
    }

    // Check if the matrix is empty
    if (dim == 0) {
        cout << "Empty matrix in the CSV file: " << filename << endl;
        return false;
    }

    // Check if it is a square matrix or not 
    if(dim != num_rows) {
        cout << "Matrix in the CSV file is not square: " << filename << endl;
        return false;
    }

    // Matrix is valid
    return true;
}

/**
 * @brief Checks to see if source file exists. Called when SOURCE command is
 * invoked.
 *
 * @param tableName 
 * @return true 
 * @return false 
 */
bool isQueryFile(string fileName){
    fileName = "../data/" + fileName + ".ra";
    struct stat buffer;
    return (stat(fileName.c_str(), &buffer) == 0);
}
