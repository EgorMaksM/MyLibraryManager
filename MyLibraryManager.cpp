#include <sqlite3.h>
#include <iostream>

using namespace std;

int callback(void* data, int argc, char** argv, char** azColName) {
	for (int i = 0; i < argc; i++) {
		cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << " | ";
	}
	cout << endl;
	return 0;
}

int main() {
    // Create db
    sqlite3* DB;
	char* messageError;

    int exit = 0;
    exit = sqlite3_open("library.db", &DB);

	string sql_books = "CREATE TABLE IF NOT EXISTS BOOKS("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
		"TITLE      TINYTEXT NOT NULL, "
		"AUTHOR     TINYTEXT NOT NULL, "
		"YEAR       INT UNSIGNED NOT NULL);";

	string sql_authors = "CREATE TABLE IF NOT EXISTS AUTHORS("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
		"NAME       TINYTEXT NOT NULL, "
		"SURNAME    TINYTEXT NOT NULL, "
		"BIO        TEXT, "
		"BIRTH      DATE NOT NULL, "
		"DEATH      DATE);";

	string sql_book_authors = "CREATE TABLE IF NOT EXISTS BOOK_AUTHORS("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
		"BOOK_ID    INTEGER NOT NULL, "
		"AUTHOR_ID  INTEGER NOT NULL, "
		"FOREIGN KEY (BOOK_ID) REFERENCES BOOKS(ID) ON DELETE CASCADE, "
		"FOREIGN KEY (AUTHOR_ID) REFERENCES AUTHORS(ID) ON DELETE CASCADE);";

	string sql_genres = "CREATE TABLE IF NOT EXISTS GENRES("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
		"NAME       TINYTEXT NOT NULL);";

	string sql_book_genres = "CREATE TABLE IF NOT EXISTS BOOK_GENRES("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
		"BOOK_ID    INTEGER NOT NULL, "
		"GENRE_ID   INTEGER NOT NULL, "
		"FOREIGN KEY (BOOK_ID) REFERENCES BOOKS(ID) ON DELETE CASCADE, "
		"FOREIGN KEY (GENRE_ID) REFERENCES GENRES(ID) ON DELETE CASCADE);";


	exit = sqlite3_exec(DB, sql_books.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK) {
		cerr << "Error in createTable function." << endl;
		sqlite3_free(messageError);
	}
	else cout << "Table created Successfully" << endl;

	exit = sqlite3_exec(DB, sql_genres.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK) {
		cerr << "Error in create GENRES." << endl;
		sqlite3_free(messageError);
	}

	exit = sqlite3_exec(DB, sql_book_genres.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK) {
		cerr << "Error in create BOOK_GENRES" << endl;
		sqlite3_free(messageError);
	}

	string sql = "INSERT INTO BOOKS (TITLE, AUTHOR, YEAR) VALUES ('The Hobbit', 'J.R.R. Tolkien', 1937);";
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK) {
		cerr << "Error inserting data: " << messageError << endl;
		sqlite3_free(messageError);
	}
	else {
		cout << "Book inserted successfully!" << endl;
	}

	sql = "SELECT * FROM BOOKS;";
	exit = sqlite3_exec(DB, sql.c_str(), callback, 0, &messageError);
	if (exit != SQLITE_OK) {
		cerr << "Error selecting data from BOOKS: " << messageError << endl;
		sqlite3_free(messageError);
	}

    sqlite3_close(DB);
    return 0;
}