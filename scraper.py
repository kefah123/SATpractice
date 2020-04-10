from urllib.request import urlopen as uReq
from bs4 import BeautifulSoup as soup
import os
import sqlite3
from sqlite3 import Error
def main():
    #opening connection and reading page
    my_url = 'https://www.vocabulary.com/lists/191545'
    u_client = uReq(my_url)
    page_html = u_client.read()
    u_client.close()

    #souping page
    page_soup = soup(page_html,"lxml")


    if os.path.exists("dictionary.db"):
        os.remove("dictionary.db")
    #opening database
    conn = create_connection("dictionary.db")
    c = conn.cursor()
    c.execute("CREATE TABLE words (word, definition);")

    #get all word and definition containers
    containers = page_soup.find_all("li",{"class":"entry learnable"})
    for container in containers:
        word_data = container.text.strip().split("\n")
        c.execute("INSERT INTO words VALUES (?,?)",word_data)

    conn.commit()
    conn.close()

""" create a database connection to the SQLite database specified by db_file
    param db_file: database file
    return: Connection object or None
"""
def create_connection(db_file):
    try:
        conn = sqlite3.connect(db_file)
        return conn
    except Error as e:
        print(e)
    return conn



main()
