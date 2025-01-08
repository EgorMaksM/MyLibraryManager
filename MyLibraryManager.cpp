#include <sqlite3.h>
#include <iostream>
#include <string_view>

// Part of code necessary for debug at this stage
constexpr uint64_t hash(std::string_view str) {
	uint64_t hash = 0;
	for (char c : str) {
		hash = (hash * 131) + c;
	}
	return hash;
}

constexpr uint64_t operator"" _hash(const char* str, size_t len) {
	return hash(std::string_view(str, len));
}

int callback(void* data, int argc, char** argv, char** azColName) {
	for (int i = 0; i < argc; i++) {
		std::cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << " | ";
	}
	std::cout << "\n";
	return 0;
}

void IOHandle() {
	while (true) {
		std::string input;
		std::cin >> input;
		switch (hash(input)) {
		case "new"_hash:
			std::string obj;
			std::cin >> obj;
			switch (hash(obj)) {
			case "book"_hash:
				std::cout << "{ADD A BOOK} => Input the name:";
				break;
			}
			break;
		}
		break;
	}
}

// Main part of code
void initDB(sqlite3*& DB, char*& messageError) {

	int exit = 0;
	exit = sqlite3_open("library.db", &DB);

	std::string sql_books = "CREATE TABLE IF NOT EXISTS BOOKS("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
		"TITLE      TINYTEXT NOT NULL, "
		"AUTHOR     TINYTEXT NOT NULL, "
		"YEAR       INT UNSIGNED NOT NULL);";

	std::string sql_authors = "CREATE TABLE IF NOT EXISTS AUTHORS("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
		"NAME       TINYTEXT NOT NULL, "
		"SURNAME    TINYTEXT NOT NULL, "
		"BIO        TEXT, "
		"BIRTH      DATE NOT NULL, "
		"DEATH      DATE);";

	std::string sql_book_authors = "CREATE TABLE IF NOT EXISTS BOOK_AUTHORS("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
		"BOOK_ID    INTEGER NOT NULL, "
		"AUTHOR_ID  INTEGER NOT NULL, "
		"FOREIGN KEY (BOOK_ID) REFERENCES BOOKS(ID) ON DELETE CASCADE, "
		"FOREIGN KEY (AUTHOR_ID) REFERENCES AUTHORS(ID) ON DELETE CASCADE);";

	std::string sql_genres = "CREATE TABLE IF NOT EXISTS GENRES("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
		"NAME       TINYTEXT NOT NULL);";

	std::string sql_book_genres = "CREATE TABLE IF NOT EXISTS BOOK_GENRES("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
		"BOOK_ID    INTEGER NOT NULL, "
		"GENRE_ID   INTEGER NOT NULL, "
		"FOREIGN KEY (BOOK_ID) REFERENCES BOOKS(ID) ON DELETE CASCADE, "
		"FOREIGN KEY (GENRE_ID) REFERENCES GENRES(ID) ON DELETE CASCADE);";


	exit = sqlite3_exec(DB, sql_books.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK) {
		std::cerr << "Error in create BOOKS." << "\n";
		sqlite3_free(messageError);
	}
	else std::cout << "BOOKS created Successfully" << "\n";

	exit = sqlite3_exec(DB, sql_genres.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK) {
		std::cerr << "Error in create GENRES." << "\n";
		sqlite3_free(messageError);
	}

	exit = sqlite3_exec(DB, sql_book_genres.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK) {
		std::cerr << "Error in create BOOK_GENRES" << "\n";
		sqlite3_free(messageError);
	}
}

int main() {
	sqlite3* DB;
	char* messageError;

	initDB(DB, messageError);

	int exit = 0;
	exit = sqlite3_open("library.db", &DB);

	IOHandle();

	std::string sql = "INSERT INTO BOOKS (TITLE, AUTHOR, YEAR) VALUES ('The Hobbit', 'J.R.R. Tolkien', 1937);";
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK) {
		std::cerr << "Error inserting data: " << messageError << "\n";
		sqlite3_free(messageError);
	}
	else {
		std::cout << "Book inserted successfully!" << "\n";
	}

	sql = "SELECT * FROM BOOKS;";
	exit = sqlite3_exec(DB, sql.c_str(), callback, 0, &messageError);
	if (exit != SQLITE_OK) {
		std::cerr << "Error selecting data from BOOKS: " << messageError << "\n";
		sqlite3_free(messageError);
	}

    sqlite3_close(DB);
    return 0;
}