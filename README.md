# KE-Final-Project-MPL
SAT Words Practice Exam

Team Members: Kwasi Efah

References: 
1) Author: CL. 
Date: April 6, 2013
Site: https://stackoverflow.com/questions/15836253/c-populate-vector-from-sqlite3-callback-function

2) Author: tutorialspoint.com
Date:
https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm

Installations: 
1) Python3
2) Beautifulsoup (pip3 install bs4)
3) Lxml (pip3 install lxml)
4) sqlite3 (pip3 install sqlite3)

To compile and run program(Mac):

Open Terminal, then enter:

 g++ practice_test.cpp -l sqlite3
 
 ./a.out
 
 
 Summary:
 
 This program is a practice vocabulary examination based on common SAT words. Out of the 1000 most common SAT words,
 the 300 most difficult words were scraped from https://www.vocabulary.com/lists/191545 using the python script
  scraper.py. The script parses through all of the data, creates an sqlite3 database called dictionary.db and saves to 
 a table two fields: word and definition. The c++ program practice_test.cpp calls the python script and then
  opens the database and stores the data in two vectors. It allows the user to choose the number of questions they would
   like to attempt, then generates a set of random multiple choice questions. After answering all of the questions, 
   the user's score and time taken is displayed.
   
 Known bugs: 
 There is a 1/299 chance that duplicate answer choices will be displayed in a question. 
 In the case where the correct answer is duplicated, only one of the answer choices will result in a correct answer,
 so the user has a 50/50 chance, assuming they know the correct answer.
 