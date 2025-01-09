#include <sqlite3.h>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>
#include <optional>

int callback(void* data, int argc, char** argv, char** azColName);
void printTable(sqlite3*& DB, std::string name);
void say(std::string x);
void initDB(sqlite3*& DB, char*& messageError);
void IOHandle(sqlite3*& DB);
void addBook(sqlite3*& DB, std::string title, unsigned short int year);
void addAuthor(sqlite3*& DB, std::string forename, std::string surname, std::string bio, int birth_year, int death_year);
void addGenre(sqlite3*& DB, std::string name);
void linkAuthorToBook(sqlite3*& DB, int author_id, int book_id);
void linkGenreToBook(sqlite3*& DB, int genre_id, int book_id);

std::vector<int> getAuthorsByBookID(sqlite3*& DB, int book_id);
std::vector<int> getBooksByAuthorID(sqlite3*& DB, int author_id);
std::vector<int> getGenresByBookID(sqlite3*& DB, int book_id);
std::vector<int> getBooksByGenreID(sqlite3*& DB, int genre_id);
std::vector<int> getGenresByAuthorID(sqlite3*& DB, int author_id);

std::optional<std::string> getBookName(sqlite3*& DB, int book_id);
std::optional<unsigned short int> getBookYear(sqlite3*& DB, int book_id);
std::optional<std::string> getAuthorForename(sqlite3*& DB, int author_id);
std::optional<std::string> getAuthorSurname(sqlite3*& DB, int author_id);
std::optional<std::string> getAuthorBio(sqlite3*& DB, int author_id);
std::optional<std::string> getAuthorBirthDate(sqlite3*& DB, int author_id);
std::optional<std::string> getAuthorDeathDate(sqlite3*& DB, int author_id);
std::optional<std::string> getGenreName(sqlite3*& DB, int genre_id);

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
		FORENAME   TINYTEXT NOT NULL,
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
			std::string forename, surname, bio;
			unsigned short int b_year, d_year;
			say("Input author's forename:");
			std::cin.ignore();
			std::getline(std::cin, forename);
			say("Input author's surname:");
			std::getline(std::cin, surname);
			say("Input author's bio:");
			std::getline(std::cin, bio);
			say("Input author's dates of birth and death:");
			std::cin >> b_year >> d_year;
			addAuthor(DB, forename, surname, bio, b_year, d_year);
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
		case "a"_hash: {
			std::vector<int> x = getGenresByBookID(DB, 1);
			for (int i = 0; i < x.size(); i++) std::cout << x[i] + 1 << "\n";
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
	const char* sql = "INSERT INTO BOOKS (TITLE, YEAR) VALUES (?, ?);";
	int exit = 0;

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

void addAuthor(sqlite3*& DB, std::string forename, std::string surname, std::string bio, int birth_year, int death_year) {
	sqlite3_stmt* stmt;
	const char* sql = "INSERT INTO AUTHORS (FORENAME, SURNAME, BIO, BIRTH, DEATH) VALUES (?, ?, ?, ?, ?);";
	int exit = 0;

	exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
	if (exit != SQLITE_OK) {
		std::cerr << "Error preparing SQL statement1: " << sqlite3_errmsg(DB) << "\n";
		return;
	}

	sqlite3_bind_text(stmt, 1, forename.c_str(), -1, SQLITE_STATIC);
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
	const char* sql = "INSERT INTO GENRES (NAME) VALUES (?);";
	int exit = 0;

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
	const char* sql = "INSERT INTO BOOK_AUTHORS (BOOK_ID, AUTHOR_ID) VALUES (?, ?);";
	int exit = 0;

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
	const char* sql = "INSERT INTO BOOK_GENRES (BOOK_ID, GENRE_ID) VALUES (?, ?);";
	int exit = 0;

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


std::vector<int> getAuthorsByBookID(sqlite3*& DB, int book_id) {
	sqlite3_stmt* stmt;
	const char* sql = R"(
		SELECT BOOK_AUTHORS.AUTHOR_ID
		FROM BOOK_AUTHORS
		WHERE BOOK_AUTHORS.BOOK_ID = ?;
	)";
	int exit = 0;

	exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);

	sqlite3_bind_int(stmt, 1, book_id);

	std::vector<int> result;
	while ((exit = sqlite3_step(stmt)) == SQLITE_ROW) {
		result.push_back(sqlite3_column_int(stmt, 0));
	}
	sqlite3_finalize(stmt);
	return result;
}

std::vector<int> getBooksByAuthorID(sqlite3*& DB, int author_id) {
	sqlite3_stmt* stmt;
	const char* sql = R"(
		SELECT BOOK_AUTHORS.BOOK_ID
		FROM BOOK_AUTHORS
		WHERE BOOK_AUTHORS.AUTHOR_ID = ?;
	)";
	int exit = 0;

	exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);

	sqlite3_bind_int(stmt, 1, author_id);

	std::vector<int> result;
	while ((exit = sqlite3_step(stmt)) == SQLITE_ROW) {
		result.push_back(sqlite3_column_int(stmt, 0));
	}
	sqlite3_finalize(stmt);
	return result;
}

std::vector<int> getGenresByBookID(sqlite3*& DB, int book_id) {
	sqlite3_stmt* stmt;
	const char* sql = R"(
		SELECT BOOK_GENRES.GENRE_ID
		FROM BOOK_GENRES
		WHERE BOOK_GENRES.BOOK_ID = ?;
	)";
	int exit = 0;

	exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);

	sqlite3_bind_int(stmt, 1, book_id);

	std::vector<int> result;
	while ((exit = sqlite3_step(stmt)) == SQLITE_ROW) {
		result.push_back(sqlite3_column_int(stmt, 0));
	}
	sqlite3_finalize(stmt);
	return result;
}

std::vector<int> getBooksByGenreID(sqlite3*& DB, int genre_id) {
	sqlite3_stmt* stmt;
	const char* sql = R"(
		SELECT BOOK_GENRES.BOOK_ID
		FROM BOOK_GENRES
		WHERE BOOK_GENRES.GENRE_ID = ?;
	)";
	int exit = 0;

	exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);

	sqlite3_bind_int(stmt, 1, genre_id);

	std::vector<int> result;
	while ((exit = sqlite3_step(stmt)) == SQLITE_ROW) {
		result.push_back(sqlite3_column_int(stmt, 0));
	}
	sqlite3_finalize(stmt);
	return result;
}

std::vector<int> getGenresByAuthorID(sqlite3*& DB, int author_id) {
	std::vector<int> books = getBooksByAuthorID(DB, author_id);
	std::vector<int> result;
	for (int i = 0; i < books.size(); i++) {
		std::vector<int> genres = getGenresByBookID(DB, books[i]);
		for (int j = 0; j < genres.size(); j++) {
			if (std::find(result.begin(), result.end(), genres[j]) == result.end())
				result.push_back(genres[j]);
		}
	}
	return result;
}


std::optional<std::string> getBookName(sqlite3*& DB, int book_id) {
	sqlite3_stmt* stmt;
	const char* sql = "SELECT TITLE FROM BOOKS WHERE ID = ?;";

	int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);

	sqlite3_bind_int(stmt, 1, book_id);

	std::optional<std::string> result;
	if (sqlite3_step(stmt) == SQLITE_ROW)
		result = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
	else
		result = std::nullopt;
	sqlite3_finalize(stmt);
	return result;
}

std::optional<unsigned short int> getBookYear(sqlite3*& DB, int book_id) {
	sqlite3_stmt* stmt;
	const char* sql = "SELECT YEAR FROM BOOKS WHERE ID = ?;";

	int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);

	sqlite3_bind_int(stmt, 1, book_id);

	std::optional<unsigned short int> result;
	if (sqlite3_step(stmt) == SQLITE_ROW)
		result = sqlite3_column_int(stmt, 0);
	else
		result = std::nullopt;
	sqlite3_finalize(stmt);
	return result;
}

std::optional<std::string> getAuthorForename(sqlite3*& DB, int author_id) {
	sqlite3_stmt* stmt;
	const char* sql = "SELECT FORENAME FROM AUTHORS WHERE ID = ?;";

	int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);

	sqlite3_bind_int(stmt, 1, author_id);

	std::optional<std::string> result;
	if (sqlite3_step(stmt) == SQLITE_ROW)
		result = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
	else
		result = std::nullopt;
	sqlite3_finalize(stmt);
	return result;
}

std::optional<std::string> getAuthorSurname(sqlite3*& DB, int author_id) {
	sqlite3_stmt* stmt;
	const char* sql = "SELECT SURNAME FROM AUTHORS WHERE ID = ?;";

	int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);

	sqlite3_bind_int(stmt, 1, author_id);

	std::optional<std::string> result;
	if (sqlite3_step(stmt) == SQLITE_ROW)
		result = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
	else
		result = std::nullopt;
	sqlite3_finalize(stmt);
	return result;
}

std::optional<std::string> getAuthorBio(sqlite3*& DB, int author_id) {
	sqlite3_stmt* stmt;
	const char* sql = "SELECT BIO FROM AUTHORS WHERE ID = ?;";

	int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);

	sqlite3_bind_int(stmt, 1, author_id);

	std::optional<std::string> result;
	if (sqlite3_step(stmt) == SQLITE_ROW)
		result = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
	else
		result = std::nullopt;
	sqlite3_finalize(stmt);
	return result;
}

std::optional<std::string> getAuthorBirthDate(sqlite3*& DB, int author_id) {
	sqlite3_stmt* stmt;
	const char* sql = "SELECT BIRTH_DATE FROM AUTHORS WHERE ID = ?;";

	int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);

	sqlite3_bind_int(stmt, 1, author_id);

	std::optional<std::string> result;
	if (sqlite3_step(stmt) == SQLITE_ROW)
		result = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
	else
		result = std::nullopt;
	sqlite3_finalize(stmt);
	return result;
}

std::optional<std::string> getAuthorDeathDate(sqlite3*& DB, int author_id) {
	sqlite3_stmt* stmt;
	const char* sql = "SELECT DEATH_DATE FROM AUTHORS WHERE ID = ?;";

	int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);

	sqlite3_bind_int(stmt, 1, author_id);

	std::optional<std::string> result;
	if (sqlite3_step(stmt) == SQLITE_ROW) {
		const char* dateStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
		(dateStr == nullptr) ? std::nullopt : std::make_optional(std::string(dateStr));
	}
	else
		result = std::nullopt;

	sqlite3_finalize(stmt);
	return result;
}

std::optional<std::string> getGenreName(sqlite3*& DB, int genre_id) {
	sqlite3_stmt* stmt;
	const char* sql = "SELECT NAME FROM GENRES WHERE ID = ?;";

	int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);

	sqlite3_bind_int(stmt, 1, genre_id);

	std::optional<std::string> result;
	if (sqlite3_step(stmt) == SQLITE_ROW)
		result = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
	else
		result = std::nullopt;
	sqlite3_finalize(stmt);
	return result;
}

int main() {
	sqlite3* DB;
	char* messageError;
	int exit = 0;
	initDB(DB, messageError);

	IOHandle(DB);

    return 0;
}