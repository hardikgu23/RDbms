#ifndef SYNTACTICPARSER_H
#define SYNTACTICPARSER_H

#include "tableCatalogue.h"
#include "matrixCatalogue.h"

using namespace std;

enum QueryType
{
    CLEAR,
    CROSS,
    DISTINCT,
    EXPORT,
    EXPORTMATRIX,
    INDEX,
    JOIN,
    LIST,
    LOAD,
    LOADMATRIX,
    PRINT,
    PRINTMATRIX,
    CHECKSYMMETRY,
    COMPUTE,
    TRANSPOSE,
    PROJECTION,
    RENAME,
    RENAMEMATRIX,
    SELECTION,
    SORT,
    ORDER,
    GROUP,
    SOURCE,
    UNDETERMINED
};

enum BinaryOperator
{
    LESS_THAN,
    GREATER_THAN,
    LEQ,
    GEQ,
    EQUAL,
    NOT_EQUAL,
    NO_BINOP_CLAUSE
};

enum SortingStrategy
{
    ASC,
    DESC,
    NO_SORT_CLAUSE
};

enum SelectType
{
    COLUMN,
    INT_LITERAL,
    NO_SELECT_CLAUSE
};

class ParsedQuery
{

public:
    QueryType queryType = UNDETERMINED;

    string clearRelationName = "";

    string crossResultRelationName = "";
    string crossFirstRelationName = "";
    string crossSecondRelationName = "";

    string distinctResultRelationName = "";
    string distinctRelationName = "";

    string exportRelationName = "";
    string exportMatrixName = "";

    IndexingStrategy indexingStrategy = NOTHING;
    string indexColumnName = "";
    string indexRelationName = "";

    BinaryOperator joinBinaryOperator = NO_BINOP_CLAUSE;
    string joinResultRelationName = "";
    string joinFirstRelationName = "";
    string joinSecondRelationName = "";
    string joinFirstColumnName = "";
    string joinSecondColumnName = "";

    string loadRelationName = "";
    string loadMatrixName = "";
    string newMatrixName = "";

    string printRelationName = "";
    string printMatrixName = "";

    string projectionResultRelationName = "";
    vector<string> projectionColumnList;
    string projectionRelationName = "";

    string renameFromColumnName = "";
    string renameToColumnName = "";
    string renameRelationName = "";

    string orderRelationName = "";
    string orderResultRelationName = "";

    string groupRelationName = "";
    string groupResultRelationName = "";
    string groupFunction1 = "";
    string groupFunction2 = "";
    string groupColumn1 = "";
    string groupColumn2 = "";
    string groupingColumn = "";
    int groupCompareValue = -1;
    BinaryOperator groupBinaryOperator = NO_BINOP_CLAUSE;

    SelectType selectType = NO_SELECT_CLAUSE;
    BinaryOperator selectionBinaryOperator = NO_BINOP_CLAUSE;
    string selectionResultRelationName = "";
    string selectionRelationName = "";
    string selectionFirstColumnName = "";
    string selectionSecondColumnName = "";
    int selectionIntLiteral = 0;

    // SortingStrategy sortingStrategy = NO_SORT_CLAUSE;
    // string sortResultRelationName = "";
    // string sortColumnName = "";
    string sortRelationName = "";
    vector<string> sortColumnName;
    vector<int> sortOrder;

    string sourceFileName = "";

    ParsedQuery();
    void clear();
};

bool syntacticParse();
bool syntacticParseCLEAR();
bool syntacticParseCROSS();
bool syntacticParseDISTINCT();
bool syntacticParseEXPORT();
bool syntacticParseEXPORTMATRIX();
bool syntacticParseINDEX();
bool syntacticParseJOIN();
bool syntacticParseLIST();
bool syntacticParseLOAD();
bool syntacticParseLOADMATRIX();
bool syntacticParsePRINT();
bool syntacticParsePRINTMATRIX();
bool syntacticParseCHECKSYMMETRY();
bool syntacticParseRENAMEMATRIX();
bool syntacticParseCOMPUTE();
bool syntacticParseTRANSPOSEMATRIX();
bool syntacticParsePROJECTION();
bool syntacticParseRENAME();
bool syntacticParseSELECTION();
bool syntacticParseSORT();
bool syntacticParseORDER();
bool syntacticParseGROUP();
bool syntacticParseSOURCE();

bool isFileExists(string tableName);
bool isValidMatrix(const string& matrixName);
bool isQueryFile(string fileName);

#endif