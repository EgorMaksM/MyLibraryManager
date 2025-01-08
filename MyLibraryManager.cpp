#include <sqlite3.h>
#include <iostream>
#include <string>
#include <string_view>

int callback(void* data, int argc, char** argv, char** azColName);
void printTable(sqlite3*& DB, std::string name);
void initDB(sqlite3*& DB, char*& messageError);
void IOHandle(sqlite3*& DB);
void addBook(sqlite3*& DB, std::string title, unsigned short int year);

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

void printTable(sqlite3*& DB, std::string name) {
	char* messageError;
	std::string sql1 = "SELECT * FROM " + name + ";";
	int exit = sqlite3_exec(DB, sql1.c_str(), callback, 0, &messageError);
}

// Main part of code
void initDB(sqlite3*& DB, char*& messageError) {

	int exit = 0;
	exit = sqlite3_open("library.db", &DB);

	std::string sql_books = "CREATE TABLE IF NOT EXISTS BOOKS("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
		"TITLE      TINYTEXT NOT NULL, "
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

void IOHandle(sqlite3*& DB) {
	while (true) {
		std::string input;
		std::cin >> input;
		switch (hash(input)) {
		case "n_b"_hash: {
			std::string title;
			int year;
			std::cout << "Input book's title:\n";
			std::cin.ignore();
			std::getline(std::cin, title);
			std::cout << "Input book's year of publication:\n";
			std::cin >> year;
			addBook(DB, title, year);
			break;
		}
		case "end"_hash: {
			sqlite3_close(DB);
			exit(0);
			break;
		}
		}
	}
}

void addBook(sqlite3*& DB, std::string title, unsigned short int year) {
	sqlite3_stmt* stmt;
	char* messageError;
	const char* sql = "INSERT INTO BOOKS (TITLE, YEAR) VALUES (?, ?);";
	int exit = 0;
	if (exit != SQLITE_OK) {
		std::cerr << "Error opening database: " << sqlite3_errmsg(DB) << "\n";
		return;
	}

	// Prepare the SQL statement
	exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
	if (exit != SQLITE_OK) {
		std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(DB) << "\n";
		return;
	}

	// Bind the parameters to the statement
	sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_int(stmt, 2, year);

	exit = sqlite3_step(stmt);
	if (exit != SQLITE_DONE) {
		std::cerr << "Error executing SQL statement: " << sqlite3_errmsg(DB) << "\n";
	}
	else {
		std::cout << "Book inserted successfully!" << "\n";
	}

	sqlite3_finalize(stmt);
	printTable(DB, "BOOKS");
}

int main() {
	sqlite3* DB;
	char* messageError;
	int exit = 0;
	initDB(DB, messageError);

	IOHandle(DB);

    return 0;
}