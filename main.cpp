#include <iostream>
#include <cstdio>
#include <fstream>
#include<vector>
// #include <filesystem>
#include <map>
#include <algorithm>
#include <string>
using namespace std;

#define TABLE_DOES_NOT_EXIST "Table Does Not Exist"
#define GENERIC "Invalid statement, try again. MySQL 2022"
#define INSERT_VALUE_COUNT_DONT_MATCH "The value count does not match with the schema"
#define INSERT_INVALID_INT_ERROR "Invalid input for INT data type"
#define INSERT_INVALID_STRING_ERROR "Invalid input for string data type check your quotation marks"

fstream SchemaFile;

void startDatabase();
void processStatement(string sqlStatement);
void convertToVector(string userInput);
void toggleBoolean(bool &value);
void printGenericSyntaxError();
bool checkIfTableExistsInSchema(string tableName);
void printError(string error);
template <typename T>
void printValuesFromVector(vector<T> vec);
void dropTable(vector<string> &sqlStatementArray);
void deleteTableFile(string tableName);
void helpTable();
void fetchSchemaLineForTable(string tableName,string &schemaLine);

// enum ErrorTypeEnum { 
//     GENERIC,
//     TABLE_DOES_NOT_EXIST
//  };


// Documentation 


// Table Creation
// CREATE TABLE Users ( ID INT , FirstName STR , LastName STR , FavArtist STR ) ;

// Example from document
// CREATE TABLE Students ( id INT , name STR , dept STR ) ;

// Table Scheme Looks Like This 
// Persons#Name#STR#Age#INT


// Insert Statement takes input in the form 

// INSERT INTO Students VALUES ( 123 , "Larry" , "CS" ) ;



// TODO

// DELETE
// UPDATE
// SELECT


int main() {    
    startDatabase();
    return 0;
}



// Only used to print values from vector, this is only useful for debugging
template <typename T>
void printValuesFromVector(vector<T> vec) {
    for(unsigned int i = 0; i < vec.size(); i++){
        cout << "Value " << i << " = " << vec[i] << endl;
    }
}




/**
 * This function takes a string and converts it into a vector of strings
 * 
 * @param userInput the user input string
 * @param sqlStatementArray the vector that will hold the words of the SQL statement
 */
void convertToVector(string userInput, vector<string> &sqlStatementArray) {

    string singleWord = "";

    for (int i=0; i < userInput.length(); i ++) {

        if (userInput[i] == ' ') {
            sqlStatementArray.push_back(singleWord);
            singleWord = "";
        }
        else {
            singleWord += userInput[i];
        }

    }

//  Because after the last word is complete we don't go in the if loop
    sqlStatementArray.push_back(singleWord);


}


/**
 * If the value is true, it will be set to false, and if the value is false, it will be set to true
 * 
 * @param value The value to be toggled.
 */

void toggleBoolean(bool &value) {
    if(value == true) {
        value = false;
    }
    else {
        value = true;
    }
}


/**
 * Prints an error message based on the error code
 * 
 * @param error The error message that was returned by the database.
 * 
 * @return Nothing
 */

void printError(string error){

    if(error == "TABLE_DOES_NOT_EXIST"){
        cout<< "Table Does Not Exist"<<endl;
        return;
    }    
    else if(error == "GENERIC"){
        cout << "Invalid statement, try again. Oracle 2021" << endl;
        return;
    }
    


}


/**
 * This function checks if the table name exists in the schema file
 * 
 * @param tableName the name of the table to be created
 * 
 * @return A boolean value.
 */



bool checkIfTableExistsInSchema(string tableName) {


    // struct test n1;

    ifstream schemaFile;

    schemaFile.open("Schema.txt",ios::in);

    // if(SchemaFile.is_open()==false){
    //     SchemaFile.open("Schema.txt",ios::in);
    // }
    // SchemaFile.flush();
    // SchemaFile.clear();

    // SchemaFile.seekg(0);

    string line = "";

    while(getline(schemaFile,line)) {

        string tableNameInSchema = "";
        for(int i=0; i<line.length();i++){
            char value = line[i];
            if(value=='#'){
                if(tableNameInSchema == tableName){
                    return true;
                }
                break;
            }
            tableNameInSchema += value;
        }
    }


    return false;
}

// CREATE TABLE
// DROP TABLE
// SELECT
// INSERT
// DELETE
// UPDATE

void helpCmd(vector<string> &tableSchemaArray) {

    cout << "\n------------HELP----------------" << endl;
    map<string, int> help{ {"CREATETABLE", 1}, {"DROPTABLE", 2}, {"SELECT", 3}, {"INSERT", 4}, {"DELETE", 5}, {"UPDATE", 6} };
    map<string, int>::iterator it1;
    string command = tableSchemaArray[1] + (tableSchemaArray.size() > 2 ? tableSchemaArray[2] : "");

    it1 = help.find(command);

    switch (it1->second)
    {
    case 1:

        cout << "\nCommand : CREATE TABLE" << endl;
        cout << "Info: Creates a new table" << endl;
        cout << "\nFormat: \nCREATE TABLE table_name ( attribute_1 attribute1_type CHECK (constraint1), \nattribute_2 attribute2_type, …, PRIMARY KEY ( attribute_1, attribute_2 ), \nFOREIGN KEY ( attribute_y ) REFERENCES table_x ( attribute_t ), \nFOREIGN KEY ( attribute_w ) REFERENCES table_y ( attribute_z )...);" << endl;
        break;

    case 2:
        cout << "\nCommand : DROP TABLE" << endl;
        cout << "Info: Deletes a table" << endl;
        cout << "\nFormat: \nDROP TABLE table_name;" << endl;
        break;

    case 3:
        cout << "\nCommand : SELECT" << endl;
        cout << "Info: Extracts data from a database" << endl;
        cout << "\nFormat: \nSELECT attribute_list FROM table_list WHERE condition_list;" << endl;
        break;

    case 4:
        cout << "\nCommand : INSERT" << endl;
        cout << "Info: Inserts new data into a database" << endl;
        cout << "\nFormat: \nINSERT INTO table_name VALUES ( val1, val2, ... );" << endl;
        break;

    case 5:
        cout << "\nCommand : DELETE" << endl;
        cout << "Info: Deletes data from a database" << endl;
        cout << "\nFormat: \nDELETE FROM table_name WHERE condition_list;" << endl;
        break;

    case 6:
        cout << "\nCommand : UPDATE" << endl;
        cout << "Info: Updates data in a database" << endl;
        cout << "\nFormat: \nUPDATE table_name SET attr1 = val1, attr2 = val2… WHERE condition_list;" << endl;
        break;

    default:
        cout << "Syntax Error" << endl;
    }

    cout << "\n";
}


void helpTable(){
    string line;

    // if(SchemaFile.is_open() ==false) {
    //     SchemaFile.open("Schema.txt",ios::in);
    // }

    // SchemaFile.flush();

    ifstream file;

    file.open("Schema.txt",ios::in);



    int count = 0;

    while(getline(file,line)){
        // cout << "line: " << line <<endl;
        string l1 = line.substr(0, line.find('#'));
        cout << l1 << endl;
        count++;
    }
    if (!count){
        cout << "No tables found" << endl;
    }

}


//  This does not return the schema line instead it returns the datatypes
//  for eg -> [INT,STR,INT]
void fetchTableSchemaDataTypesArray(vector<string> &tableSchemaArray,string tableName) {

    // if (SchemaFile.is_open()==false){
    //     SchemaFile.open("Schema.txt",ios::in);
    // }
    // SchemaFile.flush();

    // SchemaFile.clear();
    // SchemaFile.seekg(0);

    ifstream file;

    file.open("Schema.txt", ios::in);


    string line = "";

//  We should always be able to extract value since we have aleady checked
//  if the table name exists
    while(getline(file,line)){

        string tableNameInSchema = "";
        for(int i=0; i<line.length();i++){
            char value = line[i];
    //   This is only for selecting the table name so we dont care about
    //   Rest of the schema
            if(value=='#'){


            //  If table name matches than we move on to extract schema
                if(tableNameInSchema == tableName){
                    // Instead of creating a seprate function which just returns the value
                    // I used goto, this just leaves the for loop as well as the while loop
                    // and moves on, as the line with schema is already found 
                    goto break_out;
                }

            //  Table name does not match so we dont care about the rest of 
            //  Schema so we just move on to next line
                break;
            }
            tableNameInSchema += value;
        }
    }

    break_out:

    string singleWord = "";

    for (int i=0; i<line.length();i++){
        
        if(line[i] == '#'){
            tableSchemaArray.push_back(singleWord);
            singleWord = "";
        }
        else {
            singleWord += line[i];
        }
    }

    tableSchemaArray.push_back(singleWord);

    // Persons#Name#STR#Age#INT
    
    
    // Current Vector -> [ Persons , Name , STR , Age , INT  ]

    //Remove the TableName  
    tableSchemaArray.erase(tableSchemaArray.begin());

    // Current Vector -> [ Name , STR , Age , INT  ]


    // This loop removes all the column names like Name, Age, and only keeps the datatype
    for(unsigned int i = 0; i < tableSchemaArray.size(); i++){
        if(tableSchemaArray[i] == "STR" || tableSchemaArray[i] == "INT"){
            continue;
        }
        else {
            tableSchemaArray.erase(tableSchemaArray.begin()+i);
        }
    }

    return;


    //  ONly for printing the values
    // for(unsigned int i = 0; i < tableSchemaArray.size(); i++){
    //     cout << tableSchemaArray[i] << endl;
    // }
    
}

// CREATE TABLE Persons (
//     PersonID int,
//     LastName varchar(255),
//     FirstName varchar(255),
//     Address varchar(255),
//     City varchar(255)
// );


// Scheme Values -> [INT,STR,STR,STR,STR]




void fetchSchemaLineForTable(string tableName,string &schemaLine) {
    
    
    // if (SchemaFile.is_open()==false){
    //     SchemaFile.open("Schema.txt",ios::in);
    // }

    // SchemaFile.clear();
    // SchemaFile.seekg(0);

    ifstream file;

    file.open("Schema.txt", ios::in);


    string line = "";

    while(getline(file,line)) {
        string tableNameInFile = line.substr(0, line.find('#'));
        if(tableNameInFile==tableName){
            schemaLine = line;
            return;
        }
    }

}


/**
 * Given a table name, this function will print out the schema for that table
 * 
 * @param sqlStatementArray This is the array of strings that is passed to the function.
 * 
 * @return Nothing
 */
void describeTable(vector<string> &sqlStatementArray) {

    SchemaFile.flush();

    string tableName = sqlStatementArray[0];

    bool doesTableExist = checkIfTableExistsInSchema(tableName);

    if(doesTableExist == false){
        cout << TABLE_DOES_NOT_EXIST<<endl;
        return;
    }


    string schemaLine;

    fetchSchemaLineForTable(tableName,schemaLine);

    // cout << "Schema Line For Table " << tableName << " VALUE: " << schemaLine << endl;

//  WE  do tableName.length() + 1, because we also want to remove the 
//  # Value which comes after it
    schemaLine.erase(0,tableName.length()+1);

    // cout << "NEW Schema Lines: " << schemaLine << endl;


    while(true){

        // cout << "Schema Line: " << schemaLine << endl;

        string columnName = schemaLine.substr(0, schemaLine.find('#'));
        schemaLine.erase(0,columnName.length()+1);

        // cout << "Column Name: " << columnName << endl;

        string dataType = schemaLine.substr(0,schemaLine.find('#'));
        schemaLine.erase(0,dataType.length()+1);

        // cout << "DataType: " << dataType << endl;
        
        cout << columnName << " -- ";
        cout << dataType << endl;

        if(schemaLine.length() == 0){
            break;
        }

    }

    return;

}


// delete() function needs a char[] instead of string from -> https://www.geeksforgeeks.org/convert-string-char-array-cpp/
void deleteTableFile(string tableName) {

    // string tableFileName = tableName + ".txt";
    char filename[] = "Users.txt";

    string tableFileName = tableName + ".txt";
    int n = tableFileName.length();
    char tableNameCharArray[n + 1];
 

    // copying the contents of the
    // string to char array
    strcpy(tableNameCharArray, tableFileName.c_str());
 
    int result = remove(tableNameCharArray);

    // cout << "Result" << result << endl;

}

/**
 * The function takes in a vector of strings that contains the SQL statement. 
 * It then checks if the table exists in the schema file. If it does, it deletes the table file and the
 * table from the schema file. 
 * If it doesn't, it prints out that the table does not exist
 * 
 * @param sqlStatementArray The array of strings that is passed to the function.
 * 
 * @return Nothing
 */
void dropTable(vector<string> &sqlStatementArray) {


    string tableName = sqlStatementArray[0];

    bool doesTableExist = checkIfTableExistsInSchema(tableName);

    
    if(doesTableExist == false) {
        cout << TABLE_DOES_NOT_EXIST << endl;
        return;
    }

    if (SchemaFile.is_open()==false){
        SchemaFile.open("Schema.txt",ios::in);
    }
    SchemaFile.flush();

    SchemaFile.clear();
    SchemaFile.seekg(0);

    string line = "";

//  Delete the Table File Along With it's data
    // UNCOMMENT LATER
    deleteTableFile(tableName);

    ofstream out("outfile.txt");

    while(getline(SchemaFile,line)) {

        string tableNameInSchema = "";

        for(int i=0; i<line.length();i++){
            char value = line[i];

            if(value == '#'){                
                if(tableNameInSchema != tableName) {
                    out << line << endl;
                }
            //  Since we already wrote the whole line in the
            //  output file we don't need to check every 
            //  other char. So we just move on to the next line
                goto tableNameMatch;
            }
            tableNameInSchema += value;

        }

        tableNameMatch:
        continue;

    }
    SchemaFile.close();
    out.close();
    remove("Schema.txt");
    rename("outfile.txt","Schema.txt");

    cout << "Table dropped successfully" << endl;;

}


/**
 * This function inserts a tuple into a table. 
 * 
 * Takes in a vector of strings that contains the SQL statement. 
 *  
 * @param sqlStatementArray The array of strings that is passed in from the SQL statement.
 * 
 * @return Nothing is being returned.
 */
void insertIntoTable(vector<string> &sqlStatementArray){
    

    fstream TableFile;
    string tableName = sqlStatementArray[0];

    SchemaFile.flush();
    TableFile.flush();

    bool doesTableExist = checkIfTableExistsInSchema(tableName);

    if(doesTableExist==false){
        printError("TABLE_DOES_NOT_EXIST");
        return;
    }

    if(sqlStatementArray[2] == "(" && sqlStatementArray[sqlStatementArray.size()-1] == ";" && sqlStatementArray[sqlStatementArray.size()-2] == ")") {

        vector<string> tableSchemaArray;

    //  Gets the table schema as a vector e.g. [INT,STR,STR,INT]
        fetchTableSchemaDataTypesArray(tableSchemaArray,tableName);
  
    //  The orignal vector has things like (  "," ) and ; which we dont want so
    //  we create a new vector that only contains the values 
        vector<string> insertValuesArray;

//      Start from 3 because 1st value has the table name, second value has term VALUES and third is ( . 
//      We end at size() - 2 because last 2 values are ";" and ")". We know for sure because that is 
//      checked in the if condition  above 

        for(unsigned int i=3; i<sqlStatementArray.size()-2;i++){
            if(sqlStatementArray[i] != ","){
                insertValuesArray.push_back(sqlStatementArray[i]);
            }
        }


        // cout << "INSERT VALUES ARRAY" << endl;
        // printValuesFromVector(insertValuesArray);

        // cout << "TABLE SCHEMA ARRAY" << endl;
        // printValuesFromVector(tableSchemaArray);

    //  Check if the count of input values is equal to number of columns in schema
        if(insertValuesArray.size() != tableSchemaArray.size()){
            cout << INSERT_VALUE_COUNT_DONT_MATCH << endl;
            return;
        }


    // Now value count matches column count so we can loop through it

    vector<string> mainTuple;
    string tuple = "";

    for(unsigned int i = 0; i < tableSchemaArray.size(); i++){

        string currentDataType = tableSchemaArray[i];

        if (currentDataType == "INT") {

            if(isdigit(insertValuesArray[i][0]) == false) {
                cout << INSERT_INVALID_INT_ERROR << endl;
                return;
            }

            // mainTuple.push_back(insertValuesArray[i]);

            tuple += insertValuesArray[i];
        }

        else if(currentDataType == "STR"){
            char firstValue = insertValuesArray[i][0];
            char lastValue = insertValuesArray[i].back();

            char quote = '\"';
        // Checking if string starts with " and ends with "

            if (firstValue != quote || lastValue != quote ) {
                cout << INSERT_INVALID_STRING_ERROR << endl;
                return;
            }

            // Take a substring to remove first and last elements because they are quotation marks
            tuple += insertValuesArray[i].substr(1, insertValuesArray[i].size() - 2);

        }

//      According to the document, all column values should be seprated by #
//      and the last value should not have an # at the end. Hence the condition
        if(i != tableSchemaArray.size() - 1) {
            tuple += "#";
            // mainTuple.push_back("#");
        }

    }

    // printValuesFromVector<string>(mainTuple);
    fstream TableFile;

    string tableFileName = tableName + ".txt";

    TableFile.open(tableFileName,ios::app);

    TableFile << tuple << endl;
    
    TableFile.close();

    cout << "Tuple inserted successfully" << endl;

    }
    else {
        cout << GENERIC << endl;
        return;
    }

}


/**
 * This function creates a table in the schema file
 * 
 * @param sqlStatementArray The array of words that make up the SQL statement.
 * 
 * @return Nothing
 */
void createTable(vector<string> &sqlStatementArray) {
    
    string tableName = sqlStatementArray[0];
    
    // if(SchemaFile.is_open() == false){
        // cout << "ScehemFile was closed so now opening" << endl;
    SchemaFile.open("Schema.txt",ios::app);
    // }

    int parserCounter = 2;
    bool isDataType = false;

    string schemaStatement = "";

    schemaStatement += tableName;

    // printValuesFromVector(sqlStatementArray);

    // cout << "Hello World " << endl;

    // cout << sqlStatementArray[1] << endl;
    // cout << sqlStatementArray[sqlStatementArray.size()-1] << endl;
    // cout << sqlStatementArray[sqlStatementArray.size()-2]<<endl;

    if(sqlStatementArray[1] == "(" && sqlStatementArray[sqlStatementArray.size()-1] == ";" && sqlStatementArray[sqlStatementArray.size()-2] == ")") {

        while(parserCounter != sqlStatementArray.size() - 2) {

            string currentWord = sqlStatementArray[parserCounter];

            // cout << "Current Word is: " << currentWord << endl;

            if(currentWord == ","){
                parserCounter ++;
                continue;
            }

            if((isDataType == true) && (currentWord != "INT") && (currentWord != "STR")){
                cout << "Please Check Your Data Type |" << currentWord << "|" << endl;
                return;
            }

        //  This is the column name in first run
            schemaStatement += "#" + sqlStatementArray[parserCounter];

            toggleBoolean(isDataType);

            parserCounter ++;

        }

    }
    else {
        cout << GENERIC << endl;
        return;
    }

    SchemaFile << schemaStatement << endl;
    cout << "Table created successfully" << endl;

    // The INSERT Statement was not working if the table was created in the same session 
    // as insert, so maybe reopening the file will fix this
    // SchemaFile.close();
    return;

}



void processStatement(string userInput ) {
    
    vector<string> sqlStatementArray;
    convertToVector(userInput,sqlStatementArray);


    if(sqlStatementArray[0] == "CREATE" && sqlStatementArray[1] == "TABLE"){
        // cout << "Nice Statement to create table" << endl;
        // cout << "First Element Current is: " << sqlStatementArray[0] << endl;


        // Removes the first 2 words from vector i.e. CREATE TABLE
        sqlStatementArray.erase(sqlStatementArray.begin());
        sqlStatementArray.erase(sqlStatementArray.begin());

        createTable(sqlStatementArray);

    }

    else if (sqlStatementArray[0] == "INSERT" && sqlStatementArray[1] == "INTO" && sqlStatementArray[3] == "VALUES"){
        
        // Removes the first 2 words from vector i.e. INSERT INTO
        sqlStatementArray.erase(sqlStatementArray.begin());
        sqlStatementArray.erase(sqlStatementArray.begin());

        insertIntoTable(sqlStatementArray);

    }

    else if (sqlStatementArray[0] == "DROP" && sqlStatementArray[1] == "TABLE") {

    //  Removes the first 2 values again i.e. DROP & TABLE
        sqlStatementArray.erase(sqlStatementArray.begin());
        sqlStatementArray.erase(sqlStatementArray.begin());

        dropTable(sqlStatementArray);

    }

    // Displays all the tables or if there are no tables than says so
    else if (sqlStatementArray[0] == "HELP" && sqlStatementArray[1] == "TABLE"){
        helpTable();
    }


    // Describes all the cases for HELP
    // CREATE TABLE
    // DROP TABLE
    // SELECT
    // INSERT
    // DELETE
    // UPDATE
    else if (sqlStatementArray[0] == "HELP" &&  sqlStatementArray[1] != "TABLE" ){
        helpCmd(sqlStatementArray);
    }

    else if (sqlStatementArray[0] == "DESCRIBE") {

        //Removes the first value i.e DESCRIBE
        sqlStatementArray.erase(sqlStatementArray.begin());
        describeTable(sqlStatementArray);
    }

    else if (sqlStatementArray[0] == "QUIT") {
        cout << "All Transactions Closed. Quitting" << endl;
        exit(0);
    }


//  Prints Generic error -> Invalid statement, try again. Oracle 2021 
    else {
        cout << GENERIC << endl;
    }
    


}


void startDatabase() {
    cout << "Start Database Execution" << endl;
    
    while(true){
        SchemaFile.flush();
        string sqlStatement;
        cout << "$mysql ";
        getline(cin, sqlStatement, '\n');

        processStatement(sqlStatement);

//   cout << "Enter two integers: ";

    }
}



