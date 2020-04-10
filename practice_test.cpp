
#include <sqlite3.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <vector>
using namespace std;
/** This function populates two vectors with the values within the database DB
 * @param DB The opened database
 * @param words A vector of strings to hold words
 * @param definitions A vector of strings to hold definitions
 */
void get_dictionary_words(sqlite3 *DB, vector<string> &words, vector<string> &definitions);
/** This function simulates a vocabulary practice test
 * @param words A vector of strings containing words
 * @param definitions A vector of strings containing definitions
 */
void begin_test(vector<string> &words, vector<string> &definitions);

int main(int argc, char* argv[]) {
    vector<string> words;
    vector<string> definitions;
    srand(time(NULL));
    //Connect to database
    sqlite3* DB;
    int rc;
    rc = sqlite3_open("dictionary.db",&DB);
    if(rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(DB));
        return(0);
    } else {
        fprintf(stderr, "Opened database successfully\n");
    }
    //Populate vectors
    get_dictionary_words(DB, words, definitions);



    //Close database connection
    sqlite3_close(DB);
    return 0;
}
void get_dictionary_words(sqlite3 *DB, vector<string> &words, vector<string> &definitions){
    int rc;
    char* error_message;
    sqlite3_stmt *stmt;
    string data = "";
    string sql ="SELECT * FROM WORDS;";
    rc = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cerr << "Select Error " << sqlite3_errmsg(DB) << endl;

    }
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const char* word = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        const char* definition = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        words.push_back(word);
        definitions.push_back(definition);

    }
    if (rc != SQLITE_DONE) {
        cerr << "Select Error " << sqlite3_errmsg(DB) << endl;
    }
    sqlite3_finalize(stmt);
}

void begin_test(vector<string> &words, vector<string> &definitions) {
}
