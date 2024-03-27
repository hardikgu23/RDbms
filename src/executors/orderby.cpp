#include"global.h"
/**
 * @brief File contains method to process ORDER commands.
 * 
 * syntax:
 * RESULT <- ORDER BY <attribute> ASC/DESC ON <Table>
 */
bool syntacticParseORDER(){
    logger.log("syntacticParseORDER");
    
    if(tokenizedQuery.size() != 8)
    {
        cout << "Syntax Error" << endl;
        return false;
    }
    if(tokenizedQuery[3] != "BY" && tokenizedQuery[6] != "ON")
    {
        cout << "Syntax Error" << endl;
        return false;
    }

    parsedQuery.queryType = ORDER;
    parsedQuery.orderResultRelationName = tokenizedQuery[0];
    parsedQuery.orderRelationName = tokenizedQuery[7];

    if(tokenizedQuery[5] == "ASC")
        parsedQuery.sortOrder.push_back(1);
    else if(tokenizedQuery[5] == "DESC")
        parsedQuery.sortOrder.push_back(0);
    else
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.sortColumnName.push_back(tokenizedQuery[4]);

    return true;
}

bool semanticParseORDER(){
    logger.log("semanticParseORDER");

    if(!tableCatalogue.isTable(parsedQuery.orderRelationName)){
        cout<<"SEMANTIC ERROR: Relation doesn't exist"<<endl;
        return false;
    }

    // checking if all column names are valid or not?
    for (auto columnName : parsedQuery.sortColumnName)
    {
        if(!tableCatalogue.isColumnFromTable(columnName, parsedQuery.orderRelationName)){
            cout<<"SEMANTIC ERROR: Column doesn't exist in relation"<<endl;
            return false;
        }
    }
    return true;
}

void executeORDER(){
    logger.log("executeORDER");
    Table* table = tableCatalogue.getTable(parsedQuery.orderRelationName);
    table->sort(parsedQuery.sortColumnName, parsedQuery.sortOrder, parsedQuery.orderResultRelationName);
    table->unload();
    table->reset();
    table->load();
    return;
}