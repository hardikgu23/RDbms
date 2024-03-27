#include "global.h"
/**
 * @brief 
 * SYNTAX: R <- JOIN relation_name1, relation_name2 ON column_name1 bin_op column_name2
 */
bool syntacticParseJOIN()
{
    logger.log("syntacticParseJOIN");
    if (tokenizedQuery.size() != 9 || tokenizedQuery[5] != "ON")
    {
        cout << "SYNTAC ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = JOIN;
    parsedQuery.joinResultRelationName = tokenizedQuery[0];
    parsedQuery.joinFirstRelationName = tokenizedQuery[3];
    parsedQuery.joinSecondRelationName = tokenizedQuery[4];
    parsedQuery.joinFirstColumnName = tokenizedQuery[6];
    parsedQuery.joinSecondColumnName = tokenizedQuery[8];

    string binaryOperator = tokenizedQuery[7];
    if (binaryOperator == "<")
        parsedQuery.joinBinaryOperator = LESS_THAN;
    else if (binaryOperator == ">")
        parsedQuery.joinBinaryOperator = GREATER_THAN;
    else if (binaryOperator == ">=" || binaryOperator == "=>")
        parsedQuery.joinBinaryOperator = GEQ;
    else if (binaryOperator == "<=" || binaryOperator == "=<")
        parsedQuery.joinBinaryOperator = LEQ;
    else if (binaryOperator == "==")
        parsedQuery.joinBinaryOperator = EQUAL;
    else if (binaryOperator == "!=")
        parsedQuery.joinBinaryOperator = NOT_EQUAL;
    else
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    return true;
}

bool semanticParseJOIN()
{
    logger.log("semanticParseJOIN");

    if (tableCatalogue.isTable(parsedQuery.joinResultRelationName))
    {
        cout << "SEMANTIC ERROR: Resultant relation already exists" << endl;
        return false;
    }

    if (!tableCatalogue.isTable(parsedQuery.joinFirstRelationName) || !tableCatalogue.isTable(parsedQuery.joinSecondRelationName))
    {
        cout << "SEMANTIC ERROR: Relation doesn't exist" << endl;
        return false;
    }

    if (!tableCatalogue.isColumnFromTable(parsedQuery.joinFirstColumnName, parsedQuery.joinFirstRelationName) || !tableCatalogue.isColumnFromTable(parsedQuery.joinSecondColumnName, parsedQuery.joinSecondRelationName))
    {
        cout << "SEMANTIC ERROR: Column doesn't exist in relation" << endl;
        return false;
    }
    return true;
}

/**
 * @brief Helper function for comparing
 */
typedef struct cmp {

    public:
    int ind1, ind2, op;
    cmp(int ind1, int ind2, int op) {
        this->ind1 = ind1;
        this->ind2 = ind2;
        this->op = op;
    }

    // returns true if condition is satisfied else returns false
    bool compare(vector<int> &x, vector<int> &y) {
        if(op == EQUAL)
            return (x[ind1] == y[ind2]);
        else if(op == GEQ)
            return (x[ind1] >= y[ind2]);
        else if(op == LEQ)
            return (x[ind1] <= y[ind2]);
        else if(op == GREATER_THAN)
            return (x[ind1] > y[ind2]);
        else if(op == LESS_THAN)
            return (x[ind1] < y[ind2]);
        else
        {
            cout << "Unknown operator is being used" << endl;
            return false;
        }
    }

    // compares which row is smaller
    bool JoinIsSmaller(vector<int> &x, vector<int> &y, int ind1, int ind2)
    {
        if(x[ind1] < y[ind2])
            return true;
        return false;
    }
} cmp;


/**
 * @brief Return table ptr if it already exists in tableCatalogue else create new table with that name,
 * load it and insert in table catalogue.
 */
Table* getTable(string &tableName)
{
    if(!tableCatalogue.isTable(tableName))
    {
        Table* t = new Table(tableName);
        if (t->load())
            tableCatalogue.insertTable(t);
        return t;
    }
    else
        return tableCatalogue.getTable(tableName);
}

void sortTablesBasedOnOperator(Table* r1, Table* r2, string &column1, string &column2, int op)
{
    string resultName1 = "SORTED_" + r1->tableName;
    string resultName2 = "SORTED_" + r2->tableName;

    vector<string> sortColumnName1, sortColumnName2;
    sortColumnName1.push_back(column1);
    sortColumnName2.push_back(column2);

    vector<int> sortColumnOrder;

    if(op == EQUAL || op == GREATER_THAN || op == GEQ)  // Both in Ascending order
        sortColumnOrder.push_back(1);
    else                                                // Both in Descending order
        sortColumnOrder.push_back(0);

    r1->sort(sortColumnName1, sortColumnOrder, resultName1);
    r2->sort(sortColumnName2, sortColumnOrder, resultName2);

    return;
}

void storeRecords(vector<int> &r1, Table* table, Cursor &resultCursor, int startPage, int endPage, cmp &comp)
{
    Cursor cursor(table->tableName, startPage, endPage, true);
    vector<int> r2 = cursor.getNext();
    
    while(!r2.empty() && comp.compare(r1, r2))
    {
        vector<int> res;
        for(auto x : r1)
            res.push_back(x);
        for(auto x : r2)
            res.push_back(x);

        resultCursor.setNext(res);
        r2 = cursor.getNext();
    }
}

/*
    @brief joins two table table1 and table2 based on operator op and store the result in 
*/
void JoinTable(Table* table1, Table* table2, int op, cmp &comp, string &resultName)
{
    // Create result Table
    Table* resultTable = new Table(resultName);
    tableCatalogue.insertTable(resultTable);

    vector<string> resultColumns;
    for(auto c1: table1->columns)
        resultColumns.push_back(c1);
    for(auto c2: table2->columns)
        resultColumns.push_back(c2);
    
    resultTable->columns = resultColumns;
    resultTable->columnCount = resultColumns.size();
    resultTable->maxRowsPerBlock = (uint)((BLOCK_SIZE * 1000) / (sizeof(int) * resultTable->columnCount));

    // Create cursors to iterate through these tables
    Cursor cursor1(table1->tableName, 0, table1->blockCount, true);
    Cursor cursor2(table2->tableName, 0, table2->blockCount, true);
    Cursor resultCursor(resultTable->tableName, resultTable->maxRowsPerBlock, resultTable->columnCount, false);

    if(op != EQUAL)
    {
        vector<int> r1, r2;

        r1 = cursor1.getNext();
        r2 = cursor2.getNext();

        while(!r1.empty())
        {
            // keep on incrementing r2 till it satisfies the op condition
            while(!r2.empty() && comp.compare(r1, r2))
            {
                r2 = cursor2.getNext();
            }

            // r1 will satify all records lying from page 0 to curPage
            storeRecords(r1, table2, resultCursor, 0, cursor2.pageIndex, comp);
            r1 = cursor1.getNext();
        }
        resultCursor.page.writePage();
    }
    else // op = EQUAL
    {
        vector<int> r1, r2;
        r1 = cursor1.getNext();
        r2 = cursor2.getNext();

        while(!r1.empty() && !r2.empty())
        {
            if(comp.JoinIsSmaller(r1, r2, comp.ind1, comp.ind2))
            {
                r1 = cursor1.getNext();
                continue;
            }
            else if(comp.JoinIsSmaller(r2, r1, comp.ind2, comp.ind1))
            {
                r2 = cursor2.getNext();
                continue;
            }

            // r1 == r2
            int curPageInd = cursor2.pageIndex;
            int curPagePtr = cursor2.pagePointer;

            vector<int> prev = r1;
            vector<int> prev2 = r2;

            // keep on adding till condition is met
            while(!r2.empty() && comp.compare(r1, r2))
            {
                vector<int> rec;
                for(auto x : r1)
                    rec.push_back(x);
                for(auto x: r2)
                    rec.push_back(x);
 
                resultCursor.setNext(rec);
                r2 = cursor2.getNext();
            }

            r1 = cursor1.getNext();

            if(!r1.empty() && (prev[comp.ind1] == r1[comp.ind1]))
            {
                cursor2 = Cursor(table2->tableName, curPageInd);
                cursor2.pagePointer = curPagePtr;
                r2 = prev2;
            }
        }
        resultCursor.page.writePage();
    }

    resultTable->rowCount = resultCursor.rowCount;
    resultTable->rowsPerBlockCount = resultCursor.rowsPerBlockCount;
    resultTable->blockCount = resultCursor.rowsPerBlockCount.size();
    resultTable->makePermanent();
}

void executeJOIN()
{
    logger.log("executeJOIN");
    string column1 = parsedQuery.joinFirstColumnName;
    string column2 = parsedQuery.joinSecondColumnName;

    Table* table1 = tableCatalogue.getTable(parsedQuery.joinFirstRelationName);
    Table* table2 = tableCatalogue.getTable(parsedQuery.joinSecondRelationName);

    // sort Tables based on operator (==, >=, > , < , <=)
    sortTablesBasedOnOperator(table1, table2, column1, column2, parsedQuery.joinBinaryOperator);
    /*
        Creating an object of cmp to help in compairing rows
    */
    int ind1 = table1->getColumnIndex(column1);
    int ind2 = table2->getColumnIndex(column2);
    cmp comp(ind1, ind2, parsedQuery.joinBinaryOperator);

    string sortedTable1_name = "SORTED_" + parsedQuery.joinFirstRelationName;
    string sortedTable2_name = "SORTED_" + parsedQuery.joinSecondRelationName;

    Table *sortedTable1 = getTable(sortedTable1_name);
    Table *sortedTable2 = getTable(sortedTable2_name);

    JoinTable(sortedTable1, sortedTable2, parsedQuery.joinBinaryOperator, comp, parsedQuery.joinResultRelationName);

    // // cleanup
    bufferManager.deleteFile(sortedTable1->sourceFileName);
    bufferManager.deleteFile(sortedTable2->sourceFileName);
    tableCatalogue.deleteTable(sortedTable1->tableName);
    tableCatalogue.deleteTable(sortedTable2->tableName);
    tableCatalogue.deleteTable(parsedQuery.joinResultRelationName);
    tableCatalogue.deleteTable(table1->tableName);
    tableCatalogue.deleteTable(table2->tableName);

    table1 = new Table(parsedQuery.joinFirstRelationName);
    table2 = new Table(parsedQuery.joinSecondRelationName);
    table1->load();
    table2->load();

    tableCatalogue.insertTable(table1);
    tableCatalogue.insertTable(table2);
    return;
}
