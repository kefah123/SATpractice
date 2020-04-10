#include <sqlite3.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <chrono>
using namespace std;
using namespace std::chrono;
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
void begin_test(vector<string> &words, vector<string> &definitions, int number_of_questions);

/** This function displays questions in multiple choice format
 * @param definitions A vector of strings containing definitions
 * @param question A vector of strings containing questions to ask
 * @param answers A vector of strings containing answers to questions
 * @param question_no The specific question number
 */
int display_question(vector<string> &definitions, vector<string> &questions,
        vector<string> &answers, int question_no);

/** This function gets the user's input
 * @param prompt The prompt for the user
 * @param invalid The message shown for invalid input
 * @param low The lower bound of acceptable values
 * @param high the higher bound of acceptable values
 * @return The user's choice
*/
int get_int_input(string prompt, string invalid, int low, int high);
int main(int argc, char* argv[]) {
    system("python3 scraper.py");
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

    int number_of_questions = get_int_input("How many questions would you like to try: ",
            "Enter a number between 1 and 300",1,300);
    begin_test(words,definitions,number_of_questions);


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

void begin_test(vector<string> &words, vector<string> &definitions, int number_of_questions) {
    int score;
    int user_input;
    int correct_answer;
    vector<int> rand_numbers;
    vector<string> questions;
    vector<string> answers;
    //populating two vectors with random words and their definitions
    for (int i=0;i<number_of_questions;i++){
        rand_numbers.push_back(rand() % words.size());
        questions.push_back(words.at(rand_numbers.at(i)));
        answers.push_back(definitions.at(rand_numbers.at(i)));
    }
    cout << "Welcome to your SAT vocabulary practice exam!\nThe level of difficulty is HARD." << endl;
    cout << "You will be shown a word and must choose the correct definition." << endl;
    cout << "Enter '1','2','3' or '4' to make a selection." << endl;
    cout << "Your score and time will be calculated at the end." << endl;
    cout << "Begin!\n" << endl;
    auto start = high_resolution_clock::now();
    for (int i=0;i<number_of_questions;i++) {
       correct_answer = display_question(definitions, questions, answers, i);
       user_input = get_int_input("Your answer: ","You must enter a valid number between 1 and 4: ",1,4);
       if (user_input != correct_answer) {
           cout << "Incorrect. The correct answer is: " << correct_answer << endl;
           cout << "--------------------------------------------------\n" << endl;
       } else {
           cout << "Correct." << endl;
           cout << "--------------------------------------------------\n" << endl;
           score++;
       }
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    double minutes = duration.count()/60000.0;
    double seconds = duration.count()/1000.0;
    cout << "Your score:" << endl;
    printf("%.1f%%\n",(float(score)/number_of_questions)*100);
    if (duration.count() >= 60000)
        printf("Time: %.2f minutes",minutes);
    else
        printf("Time: %.2f seconds",seconds);
}
int display_question(vector<string> &definitions, vector<string> &questions,
        vector<string> &answers, int question_no){
    int randomizer = rand()%4 + 1;
    cout << question_no+1 << ") " << questions.at(question_no) << ":\n" << endl;
    if (randomizer == 1) {
        cout << "1) " << answers.at(question_no) << endl;
        cout << "2) " << definitions.at(rand() % definitions.size()) << endl;
        cout << "3) " << definitions.at(rand() % definitions.size()) << endl;
        cout << "4) " << definitions.at(rand() % definitions.size()) << endl;
    } else if (randomizer == 2) {
        cout << "1) " << definitions.at(rand() % definitions.size()) << endl;
        cout << "2) " << answers.at(question_no) << endl;
        cout << "3) " << definitions.at(rand() % definitions.size()) << endl;
        cout << "4) " << definitions.at(rand() % definitions.size()) << endl;
    } else if (randomizer == 3) {
        cout << "1) " << definitions.at(rand() % definitions.size()) << endl;
        cout << "2) " << definitions.at(rand() % definitions.size()) << endl;
        cout << "3) " << answers.at(question_no) << endl;
        cout << "4) " << definitions.at(rand() % definitions.size()) << endl;
    } else if (randomizer == 4) {
        cout << "1) " << definitions.at(rand() % definitions.size()) << endl;
        cout << "2) " << definitions.at(rand() % definitions.size()) << endl;
        cout << "3) " << definitions.at(rand() % definitions.size()) << endl;
        cout << "4) " << answers.at(question_no) << endl;
    }
    return randomizer;
}

int get_int_input(string prompt, string invalid, int low, int high) {
    cout << prompt;
    int choice = 0;
    string junk;
    while (!(cin >> choice) || choice < low || choice > high) {
        cout << invalid;
        cin.clear();
        getline(cin, junk);
    }
    return choice;
}