#include"global.h"
#include <functional>

/**
 * @brief File contains method to process GROUP commands.
 * 
 * syntax:
 * <new_table> <- GROUP BY <MAXgrouping_attribute> FROM <table_name> HAVING
   <aggregate(attribute)> <bin_op> <attribute_value> RETURN
   <aggregate_func(attribute)>
 */

unordered_set<string> allowedFunctions = {"MAX", "MIN", "SUM", "AVG", "COUNT"};
map<string, int>functionName = {{"MAX", 0}, {"MIN", 1}, {"SUM", 2}, {"AVG", 3}, {"COUNT", 4}};

bool storeFunctionColumnName(string &input, string &funcName, string &columnName)
{
    // Find the opening and closing parentheses
    size_t openParenPos = input.find('(');
    size_t closeParenPos = input.find(')');

    // Check if both parentheses are found and they are in the correct order
    if (openParenPos != string::npos && closeParenPos != string::npos && openParenPos < closeParenPos) {
        funcName = input.substr(0, openParenPos);
        columnName = input.substr(openParenPos + 1, closeParenPos - openParenPos - 1);

        if(!allowedFunctions.count(funcName))
        {
            return false;
        }
        return true;
    }
    else
    {
        return false;
    }
}

bool syntacticParseGROUP(){
    logger.log("syntacticParseGROUP");
    
    if(tokenizedQuery.size() != 13)
    {
        cout << "Syntax Error" << endl;
        return false;
    }
    if(tokenizedQuery[3] != "BY" || tokenizedQuery[5] != "FROM" || tokenizedQuery[7] != "HAVING" || tokenizedQuery[11] != "RETURN")
    {
        cout << "Syntax Error" << endl;
        return false;
    }

    parsedQuery.queryType = GROUP;
    parsedQuery.groupResultRelationName = tokenizedQuery[0];
    parsedQuery.groupRelationName = tokenizedQuery[6];
    parsedQuery.groupingColumn = tokenizedQuery[4];

    if(!storeFunctionColumnName(tokenizedQuery[8], parsedQuery.groupFunction1, parsedQuery.groupColumn1))
    {
        cout << "Syntax Error" << endl;
        return false;
    }

    if(!storeFunctionColumnName(tokenizedQuery[12], parsedQuery.groupFunction2, parsedQuery.groupColumn2))
    {
        cout << "Syntax Error" << endl;
        return false;
    }

    try {
        parsedQuery.groupCompareValue = stoi(tokenizedQuery[10]);
    }
    catch (const invalid_argument& e){
        cout << "SYNTAX ERROR" << endl;
        return false;
    }

    string binaryOperator = tokenizedQuery[9];
    if (binaryOperator == "<")
        parsedQuery.groupBinaryOperator = LESS_THAN;
    else if (binaryOperator == ">")
        parsedQuery.groupBinaryOperator = GREATER_THAN;
    else if (binaryOperator == ">=" || binaryOperator == "=>")
        parsedQuery.groupBinaryOperator = GEQ;
    else if (binaryOperator == "<=" || binaryOperator == "=<")
        parsedQuery.groupBinaryOperator = LEQ;
    else if (binaryOperator == "==")
        parsedQuery.groupBinaryOperator = EQUAL;
    else if (binaryOperator == "!=")
        parsedQuery.groupBinaryOperator = NOT_EQUAL;
    else
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }

    return true;
}

bool semanticParseGROUP(){
    logger.log("semanticParseGROUP");

    if(!tableCatalogue.isTable(parsedQuery.groupRelationName)){
        cout<<"SEMANTIC ERROR: Relation doesn't exist"<<endl;
        return false;
    }

    // checking if all column names are valid or not?
    if(!tableCatalogue.isColumnFromTable(parsedQuery.groupColumn1, parsedQuery.groupRelationName) || 
        !tableCatalogue.isColumnFromTable(parsedQuery.groupColumn2, parsedQuery.groupRelationName) ||
        !tableCatalogue.isColumnFromTable(parsedQuery.groupingColumn, parsedQuery.groupRelationName))
    {
        cout << "SEMANTIC ERROR: Column Doesn't exist" << endl;
        return false;
    }
    
    return true;
}

class check
{
    public:
        string function1, function2;
        int attribValue;
        int compValue;
        int ind1, ind2;

        Cursor* resultCursor;
        int sortingColumnInd;
        int op;

        int maxValue1;
        int minValue1;
        int avgValue1;
        int sumValue1;
        int count1;

        int maxValue2;
        int minValue2;
        int avgValue2;
        int sumValue2;
        int count2;
    
    void resetStats()
    {
        this->attribValue = -1;
        this->maxValue1 = INT32_MIN;
        this->minValue1 = INT32_MAX;
        this->sumValue1 = 0;
        this->avgValue1 = 0;
        this->count1 = 0;

        this->maxValue2 = INT32_MIN;
        this->minValue2 = INT32_MAX;
        this->avgValue2 = 0;
        this->sumValue2 = 0;
        this->count2 = 0;
    }

    void storeIfSatisfy()
    {
        vector<int> record;
        bool isSatisfy = false;
        int fun1_value, fun2_value;

        int fun1_values[] = {maxValue1, minValue1, sumValue1, avgValue1, count1};
        int fun2_values[] = {maxValue2, minValue2, sumValue2, avgValue2, count2};

        fun1_value = fun1_values[functionName[function1]];
        fun2_value = fun2_values[functionName[function2]];

        switch (op) {
            case LESS_THAN:
                isSatisfy = (fun1_value < compValue);
                break;
            case GREATER_THAN:
                isSatisfy = (fun1_value > compValue);
                break;
            case LEQ:
                isSatisfy = (fun1_value <= compValue);
                break;
            case GEQ:
                isSatisfy = (fun1_value >= compValue);
                break;
            case EQUAL:
                isSatisfy = (fun1_value == compValue);
                break;
        }

        if(isSatisfy)
        {
            record.push_back(attribValue);
            record.push_back(fun2_value);
            resultCursor->setNext(record);
        }
    }

    check(string &fun1, string& fun2, string &column1, string &column2, string &sortingColumn, int op, int compValue, vector<string> &columns, Cursor* resultCursor)
    {
        this->function1 = fun1;
        this->function2 = fun2;
        this->resultCursor = resultCursor;
        this->op = op;
        this->compValue = compValue;

        for(int i=0; i<columns.size(); ++i)
        {
            if(columns[i] == column1)
                this->ind1 = i;
            if(columns[i] == column2)
                this->ind2 = i;
            if(columns[i] == sortingColumn)
                this->sortingColumnInd = i;
        }
        resetStats();

    }

    void record(vector<int> &x)
    {
        // if this is a new kind of record, then check whether previous record is satisfying the value. if yes, then store it
        if( (count1 != 0) && (x[sortingColumnInd] != this->attribValue))
        {
            storeIfSatisfy();
            resetStats();
        }

        this->attribValue = x[sortingColumnInd];
        count1++;
        maxValue1 = max(maxValue1, x[ind1]);
        minValue1 = min(minValue1, x[ind1]);
        sumValue1 = sumValue1 + x[ind1];
        avgValue1 = sumValue1 / count1;

        count2++;
        maxValue2 = max(maxValue2, x[ind2]);
        minValue2 = min(minValue2, x[ind2]);
        sumValue2 = sumValue2 + x[ind2];
        avgValue2 = sumValue2 / count2;
    }
};

void executeGROUP(){
    logger.log("executeGROUP");
    Table * table = tableCatalogue.getTable(parsedQuery.groupRelationName);
    string name = "sorted" + table->tableName;
    vector<string> sortColumnName = {parsedQuery.groupingColumn};
    vector<int> sortOrder = {1};

    table->sort(sortColumnName, sortOrder, name);

    // Load the temp sorted table and make resultTable
    Table* sortedTable = new Table(name);
    sortedTable->load();
    tableCatalogue.insertTable(sortedTable);

    Table* resultTable = new Table(parsedQuery.groupResultRelationName);
    tableCatalogue.insertTable(resultTable);

    // Make cursors to iterate through these tables
    Cursor cursor(sortedTable->tableName, 0);
    Cursor resultCursor(resultTable->tableName, -1, 2, false);

    string c1 = parsedQuery.groupingColumn;
    string c2 = parsedQuery.groupFunction2 + parsedQuery.groupColumn2;
    resultTable->columnCount = 2;
    resultTable->columns = {c1, c2};

    check checker(parsedQuery.groupFunction1, parsedQuery.groupFunction2,
                parsedQuery.groupColumn1, parsedQuery.groupColumn2, parsedQuery.groupingColumn, 
                parsedQuery.groupBinaryOperator , parsedQuery.groupCompareValue ,sortedTable->columns, &resultCursor);

    vector<int> r;
    r = cursor.getNext();

    while(!r.empty())
    {
        checker.record(r);
        r = cursor.getNext();   
    }
    checker.storeIfSatisfy();
    resultCursor.page.writePage();

    bufferManager.deleteFile(sortedTable->sourceFileName);
    tableCatalogue.deleteTable(sortedTable->tableName);

    table->unload();
    table->reset();
    table->load();

    resultTable->rowCount = resultCursor.rowCount;
    resultTable->rowsPerBlockCount = resultCursor.rowsPerBlockCount;
    resultTable->blockCount = resultCursor.rowsPerBlockCount.size();
    resultTable->maxRowsPerBlock = resultCursor.maxNumRows;

    resultTable->makePermanent();
    tableCatalogue.deleteTable(resultTable->tableName);
}