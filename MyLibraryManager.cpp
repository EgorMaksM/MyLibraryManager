#include <sqlite3.h>
#include <iostream>
#include <string>
#include <string_view>

int callback(void* data, int argc, char** argv, char** azColName);
void printTable(sqlite3*& DB, std::string name);
void say(std::string x);
void initDB(sqlite3*& DB, char*& messageError);
void IOHandle(sqlite3*& DB);
void addBook(sqlite3*& DB, std::string title, unsigned short int year);
void addAuthor(sqlite3*& DB, std::string name, std::string surname, std::string bio, int birth_year, int death_year);
void addGenre(sqlite3*& DB, std::string name);
void linkAuthorToBook(sqlite3*& DB, int author_id, int book_id);
void linkGenreToBook(sqlite3*& DB, int genre_id, int book_id);

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

void say(std::string x) {
	for (int i = 0; i < x.length() + 4; i++) std::cout << "-";
	std::cout << "\n| " << x << " |\n";
	for (int i = 0; i < x.length() + 4; i++) std::cout << "-";
	std::cout << "\n";
}

// Main part of code
void initDB(sqlite3*& DB, char*& messageError) {

	int exit = 0;
	exit = sqlite3_open("library.db", &DB);

	std::string sql_books = R"(
		CREATE TABLE IF NOT EXISTS BOOKS(
		ID INTEGER PRIMARY KEY AUTOINCREMENT,
		TITLE      TINYTEXT NOT NULL,
		YEAR       INT UNSIGNED NOT NULL);
	)";

	std::string sql_authors = R"(
		CREATE TABLE IF NOT EXISTS AUTHORS(
		ID INTEGER PRIMARY KEY AUTOINCREMENT,
		NAME       TINYTEXT NOT NULL,
		SURNAME    TINYTEXT NOT NULL,
		BIO        TEXT,
		BIRTH      INT NOT NULL,
		DEATH      INT);
	)";

	std::string sql_book_authors = R"(
		CREATE TABLE IF NOT EXISTS BOOK_AUTHORS(
		ID INTEGER PRIMARY KEY AUTOINCREMENT,
		BOOK_ID    INTEGER NOT NULL, 
		AUTHOR_ID  INTEGER NOT NULL, 
		FOREIGN KEY (BOOK_ID) REFERENCES BOOKS(ID) ON DELETE CASCADE, 
		FOREIGN KEY (AUTHOR_ID) REFERENCES AUTHORS(ID) ON DELETE CASCADE);
	)";

	std::string sql_genres = R"(
		CREATE TABLE IF NOT EXISTS GENRES(
		ID INTEGER PRIMARY KEY AUTOINCREMENT, 
		NAME       TINYTEXT NOT NULL);
	)";

	std::string sql_book_genres = R"(
		CREATE TABLE IF NOT EXISTS BOOK_GENRES(
		ID INTEGER PRIMARY KEY AUTOINCREMENT, 
		BOOK_ID    INTEGER NOT NULL, 
		GENRE_ID   INTEGER NOT NULL, 
		FOREIGN KEY (BOOK_ID) REFERENCES BOOKS(ID) ON DELETE CASCADE, 
		FOREIGN KEY (GENRE_ID) REFERENCES GENRES(ID) ON DELETE CASCADE);
	)";


	exit = sqlite3_exec(DB, sql_books.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK) {
		std::cerr << "Error in create BOOKS." << "\n";
		sqlite3_free(messageError);
	}

	exit = sqlite3_exec(DB, sql_authors.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK) {
		std::cerr << "Error in create AUTHORS." << "\n";
		sqlite3_free(messageError);
	}

	exit = sqlite3_exec(DB, sql_book_authors.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK) {
		std::cerr << "Error in create BOOK_AUTHORS." << "\n";
		sqlite3_free(messageError);
	}

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

/*
// n_b - Add new book
// n_a - Add new author
// n_g - Add new genre
// link_b_a - Link an author and a book
// link_b_g - Link a genre and a book
// end - Close program
*/
void IOHandle(sqlite3*& DB) {
	while (true) {
		std::string input;
		std::cin >> input;
		switch (hash(input)) {
		case "n_b"_hash: {
			std::string title;
			int year;
			say("Input book's title:");
			std::cin.ignore();
			std::getline(std::cin, title);
			say("Input book's year of publication:");
			std::cin >> year;
			addBook(DB, title, year);
			break;
		}
		case "n_a"_hash: {
			std::string name, surname, bio;
			unsigned short int b_year, d_year;
			say("Input author's name:");
			std::cin.ignore();
			std::getline(std::cin, name);
			say("Input author's surname:");
			std::cin.ignore();
			std::getline(std::cin, surname);
			say("Input author's bio:");
			std::cin.ignore();
			std::getline(std::cin, bio);
			say("Input author's dates of birth and death:");
			std::cin >> b_year >> d_year;
			addAuthor(DB, name, surname, bio, b_year, d_year);
			break;
		}
		case "n_g"_hash: {
			std::string name;
			say("Input genre's name:");
			std::cin.ignore();
			std::getline(std::cin, name);
			addGenre(DB, name);
			break;
		}
		case "link_b_a"_hash: {
			int book_id, author_id;
			say("Input book's ID:");
			std::cin >> book_id;
			say("Input author's ID:");
			std::cin >> author_id;
			linkAuthorToBook(DB, author_id, book_id);
			break;
		}
		case "link_b_g"_hash: {
			int book_id, genre_id;
			say("Input book's ID:");
			std::cin >> book_id;
			say("Input author's ID:");
			std::cin >> genre_id;
			linkGenreToBook(DB, genre_id, book_id);
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

	exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
	if (exit != SQLITE_OK) {
		std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(DB) << "\n";
		return;
	}

	sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_int(stmt, 2, year);

	exit = sqlite3_step(stmt);
	if (exit != SQLITE_DONE) {
		std::cerr << "Error executing SQL statement: " << sqlite3_errmsg(DB) << "\n";
	}
	else {
		say("Book inserted successfully!");
	}

	sqlite3_finalize(stmt);
	printTable(DB, "BOOKS");
}

void addAuthor(sqlite3*& DB, std::string name, std::string surname, std::string bio, int birth_year, int death_year) {
	sqlite3_stmt* stmt;
	char* messageError;
	const char* sql = "INSERT INTO AUTHORS (NAME, SURNAME, BIO, BIRTH, DEATH) VALUES (?, ?, ?, ?, ?);";
	int exit = 0;
	if (exit != SQLITE_OK) {
		std::cerr << "Error opening database: " << sqlite3_errmsg(DB) << "\n";
		return;
	}

	exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
	if (exit != SQLITE_OK) {
		std::cerr << "Error preparing SQL statement1: " << sqlite3_errmsg(DB) << "\n";
		return;
	}

	sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, surname.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 3, bio.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_int(stmt, 4, birth_year);
	sqlite3_bind_int(stmt, 5, death_year);

	exit = sqlite3_step(stmt);
	if (exit != SQLITE_DONE) {
		std::cerr << "Error executing SQL statement2: " << sqlite3_errmsg(DB) << "\n";
	}
	else {
		say("Author inserted successfully!");
	}

	sqlite3_finalize(stmt);
	printTable(DB, "AUTHORS");
}

void addGenre(sqlite3*& DB, std::string name) {
	sqlite3_stmt* stmt;
	char* messageError;
	const char* sql = "INSERT INTO GENRES (NAME) VALUES (?);";
	int exit = 0;
	if (exit != SQLITE_OK) {
		std::cerr << "Error opening database: " << sqlite3_errmsg(DB) << "\n";
		return;
	}

	exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
	if (exit != SQLITE_OK) {
		std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(DB) << "\n";
		return;
	}

	sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);

	exit = sqlite3_step(stmt);
	if (exit != SQLITE_DONE) {
		std::cerr << "Error executing SQL statement: " << sqlite3_errmsg(DB) << "\n";
	}
	else {
		say("Genre inserted successfully!");
	}

	sqlite3_finalize(stmt);
	printTable(DB, "GENRES");
}

void linkAuthorToBook(sqlite3*& DB, int author_id, int book_id) {
	sqlite3_stmt* stmt;
	char* messageError;
	const char* sql = "INSERT INTO BOOK_AUTHORS (BOOK_ID, AUTHOR_ID) VALUES (?, ?);";
	int exit = 0;
	if (exit != SQLITE_OK) {
		std::cerr << "Error opening database: " << sqlite3_errmsg(DB) << "\n";
		return;
	}

	exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
	if (exit != SQLITE_OK) {
		std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(DB) << "\n";
		return;
	}

	sqlite3_bind_int(stmt, 1, book_id);
	sqlite3_bind_int(stmt, 2, author_id);

	exit = sqlite3_step(stmt);
	if (exit != SQLITE_DONE) {
		std::cerr << "Error executing SQL statement: " << sqlite3_errmsg(DB) << "\n";
	}
	else {
		say("Book<->Author relationship inserted successfully!");
	}

	sqlite3_finalize(stmt);
	printTable(DB, "BOOK_AUTHORS");
}

void linkGenreToBook(sqlite3*& DB, int genre_id, int book_id) {
	sqlite3_stmt* stmt;
	char* messageError;
	const char* sql = "INSERT INTO BOOK_GENRES (BOOK_ID, GENRE_ID) VALUES (?, ?);";
	int exit = 0;
	if (exit != SQLITE_OK) {
		std::cerr << "Error opening database: " << sqlite3_errmsg(DB) << "\n";
		return;
	}

	exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
	if (exit != SQLITE_OK) {
		std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(DB) << "\n";
		return;
	}

	sqlite3_bind_int(stmt, 1, book_id);
	sqlite3_bind_int(stmt, 2, genre_id);

	exit = sqlite3_step(stmt);
	if (exit != SQLITE_DONE) {
		std::cerr << "Error executing SQL statement: " << sqlite3_errmsg(DB) << "\n";
	}
	else {
		say("Book<->Genre relationship inserted successfully!");
	}

	sqlite3_finalize(stmt);
	printTable(DB, "BOOK_GENRES");
}

int main() {
	sqlite3* DB;
	char* messageError;
	int exit = 0;
	initDB(DB, messageError);

	IOHandle(DB);

    return 0;
}