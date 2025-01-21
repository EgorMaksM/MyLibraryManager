#include "libmgr.h"

QDate stringToQDate(const std::string& dateStr) {
    if (dateStr.empty()) return QDate();
    return QDate::fromString(QString::fromStdString(dateStr), "yyyy-MM-dd");
}

std::string QDateToString(const QDate& date) {
    if (date.isNull()) return "";
    return date.toString("yyyy-MM-dd").toStdString();
}


bool operator==(const Book& book, const Book& other) {
    return book.id == other.id &&
           book.title == other.title &&
           book.year == other.year;
}

bool operator==(const Author& author, const Author& other) {
    return author.id == other.id &&
           author.forename == other.forename &&
           author.surname == other.surname &&
           author.bio == other.bio &&
           author.birth == other.birth &&
           author.death == other.death;
}

bool operator==(const Genre& genre, const Genre& other) {
    return genre.id == other.id &&
           genre.name == other.name;
}

bool operator==(const User& user, const User& other) {
    return user.id == other.id &&
           user.forename == other.forename &&
           user.surname == other.surname &&
           user.birth == other.birth &&
           user.email == other.email &&
           user.phone == other.phone;
}

bool operator==(const Loan& loan, const Loan& other) {
    return loan.id == other.id &&
           loan.user_id == other.user_id &&
           loan.book_id == other.book_id &&
           loan.start == other.start &&
           loan.end == other.end;
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
        BIRTH      DATE NOT NULL,
        DEATH      DATE);
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

    std::string sql_users = R"(
        CREATE TABLE IF NOT EXISTS USERS(
        ID INTEGER PRIMARY KEY AUTOINCREMENT,
        FORENAME       TINYTEXT NOT NULL,
        SURNAME        TINYTEXT NOT NULL,
        BIRTH          DATE NOT NULL,
        EMAIL          TINYTEXT NOT NULL,
        PHONE          TINYTEXT);
    )";

    std::string sql_loans = R"(
        CREATE TABLE IF NOT EXISTS LOANS(
        ID INTEGER PRIMARY KEY AUTOINCREMENT,
        USER_ID       INTEGER NOT NULL,
        BOOK_ID       INTEGER NOT NULL,
        START         DATE NOT NULL,
        END           DATE NOT NULL,
        FOREIGN KEY (USER_ID) REFERENCES USERS(ID) ON DELETE CASCADE,
        FOREIGN KEY (BOOK_ID) REFERENCES BOOKS(ID) ON DELETE CASCADE);
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

    exit = sqlite3_exec(DB, sql_users.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        std::cerr << "Error in create USERS" << "\n";
        sqlite3_free(messageError);
    }

    exit = sqlite3_exec(DB, sql_loans.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        std::cerr << "Error in create LOANS" << "\n";
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
/*void IOHandle(sqlite3*& DB) {
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
            unsigned short int b_dd, b_mm, b_yyyy, d_dd, d_mm, d_yyyy;
            say("Input author's forename:");
            std::cin.ignore();
            std::getline(std::cin, forename);
            say("Input author's surname:");
            std::getline(std::cin, surname);
            say("Input author's bio:");
            std::getline(std::cin, bio);
            say("Input author's date of birth (dd-mm-yyyy):");
            std::cin >> b_dd >> b_mm >> b_yyyy;
            say("Input author's date of death (0 0 0 if not dead yet):");
            std::cin >> d_dd >> d_mm >> d_yyyy;
            //addAuthor(DB, forename, surname, bio, b_year, d_year);
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
*/

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

void addAuthor(sqlite3*& DB, std::string forename, std::string surname, std::string bio, QDate q_birth, QDate q_death) {
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO AUTHORS (FORENAME, SURNAME, BIO, BIRTH, DEATH) VALUES (?, ?, ?, ?, ?);";
    int exit = 0;

    std::string birth = QDateToString(q_birth);
    std::string death = QDateToString(q_death);

    exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        std::cerr << "Error preparing SQL statement1: " << sqlite3_errmsg(DB) << "\n";
        return;
    }

    sqlite3_bind_text(stmt, 1, forename.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, surname.c_str(), -1, SQLITE_STATIC);

    if (bio.empty()) sqlite3_bind_null(stmt, 3);
    else sqlite3_bind_text(stmt, 3, bio.c_str(), -1, SQLITE_STATIC);

    sqlite3_bind_text(stmt, 4, birth.c_str(), -1, SQLITE_STATIC);

    if (death.empty()) sqlite3_bind_null(stmt, 5);
    else sqlite3_bind_text(stmt, 5, death.c_str(), -1, SQLITE_STATIC);

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

void addUser(sqlite3*& DB, std::string forename, std::string surname, QDate q_birth, std::string email, std::string phone) {
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO USERS (FORENAME, SURNAME, BIRTH, EMAIL, PHONE) VALUES (?, ?, ?, ?, ?);";
    int exit = 0;

    std::string birth = QDateToString(q_birth);
    exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(DB) << "\n";
        return;
    }

    sqlite3_bind_text(stmt, 1, forename.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, surname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, birth.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, email.c_str(), -1, SQLITE_STATIC);
    if (phone.empty()) sqlite3_bind_null(stmt, 5);
    else sqlite3_bind_text(stmt, 5, phone.c_str(), -1, SQLITE_STATIC);
    //sqlite3_bind_text(stmt, 5, phone.c_str(), -1, SQLITE_STATIC);

    exit = sqlite3_step(stmt);
    if (exit != SQLITE_DONE) {
        std::cerr << "Error executing SQL statement: " << sqlite3_errmsg(DB) << "\n";
    }
    else {
        say("User inserted successfully!");
    }

    sqlite3_finalize(stmt);
    printTable(DB, "USERS");
}

void addLoan(sqlite3*& DB, int user_id, int book_id, QDate q_start, QDate q_end) {
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO LOANS (USER_ID, BOOK_ID, START, END) VALUES (?, ?, ?, ?);";
    int exit = 0;

    std::string start = QDateToString(q_start);
    std::string end = QDateToString(q_end);

    exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(DB) << "\n";
        return;
    }

    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_int(stmt, 2, book_id);
    sqlite3_bind_text(stmt, 3, start.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, end.c_str(), -1, SQLITE_STATIC);

    exit = sqlite3_step(stmt);
    if (exit != SQLITE_DONE) {
        std::cerr << "Error executing SQL statement: " << sqlite3_errmsg(DB) << "\n";
    }
    else {
        say("Loan inserted successfully!");
    }

    sqlite3_finalize(stmt);
    printTable(DB, "LOANS");
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

bool unlinkAuthorFromBook(sqlite3*& DB, int author_id, int book_id) {
    sqlite3_stmt* stmt;
    const char* sql = R"(
        DELETE FROM BOOK_AUTHORS
        WHERE BOOK_ID = ? AND AUTHOR_ID = ?;
    )";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr);
    if (exit != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(DB) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, book_id);
    sqlite3_bind_int(stmt, 2, author_id);

    exit = sqlite3_step(stmt);
    if (exit != SQLITE_DONE) {
        std::cerr << "Error executing statement: " << sqlite3_errmsg(DB) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

bool unlinkGenreFromBook(sqlite3*& DB, int genre_id, int book_id) {
    sqlite3_stmt* stmt;
    const char* sql = R"(
        DELETE FROM BOOK_GENRES
        WHERE BOOK_ID = ? AND GENRE_ID = ?;
    )";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr);
    if (exit != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(DB) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, book_id);
    sqlite3_bind_int(stmt, 2, genre_id);

    exit = sqlite3_step(stmt);
    if (exit != SQLITE_DONE) {
        std::cerr << "Error executing statement: " << sqlite3_errmsg(DB) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}


bool getAuthorsByBookID(sqlite3*& DB, int book_id, std::vector<Author>& authors) {
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
    authors.clear();
    while ((exit = sqlite3_step(stmt)) == SQLITE_ROW) {
        result.push_back(sqlite3_column_int(stmt, 0));
    }
    sqlite3_finalize(stmt);
    for (int i = 0; i < result.size(); i++) {
        Author author;
        if (!getAuthorByID(DB, result[i], author)) return false;
        authors.push_back(author);
    }
    return true;
}

bool getBooksByAuthorID(sqlite3*& DB, int author_id, std::vector<Book>& books) {
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
    for (int i = 0; i < result.size(); i++) {
        Book book;
        if (!getBookByID(DB, result[i], book)) return false;
        books.push_back(book);
    }
    return true;
}

bool getGenresByBookID(sqlite3*& DB, int book_id, std::vector<Genre>& genres) {
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
    for (int i = 0; i < result.size(); i++) {
        Genre genre;
        if (!getGenreByID(DB, result[i], genre)) return false;
        genres.push_back(genre);
    }
    return true;
}

bool getBooksByGenreID(sqlite3*& DB, int genre_id, std::vector<Book>& books) {
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
    for (int i = 0; i < result.size(); i++) {
        Book book;
        if (!getBookByID(DB, result[i], book)) return false;
        books.push_back(book);
    }
    return true;
}

bool getGenresByAuthorID(sqlite3*& DB, int author_id, std::vector<Genre>& genres) {
    genres.clear();
    std::vector<Book> authorBooks;
    if (!getBooksByAuthorID(DB, author_id, authorBooks)) return false;

    for (int i = 0; i < authorBooks.size(); i++) {
        std::vector<Genre> searchVec;
        if (!getGenresByBookID(DB, authorBooks[i].id, searchVec)) return false;
        for (int j = 0; j < searchVec.size(); j++) {
            if (std::find(genres.begin(), genres.end(), searchVec[j]) == genres.end())
                genres.push_back(searchVec[j]);
        }
    }
    return true;
}

bool getBooksByUserID(sqlite3*& DB, int user_id, std::vector<Book>& books) {
    sqlite3_stmt* stmt;
    const char* sql = R"(
        SELECT LOANS.BOOK_ID
        FROM LOANS
        WHERE LOANS.USER_ID = ?;
    )";
    int exit = 0;

    exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);

    sqlite3_bind_int(stmt, 1, user_id);

    std::vector<int> result;
    while ((exit = sqlite3_step(stmt)) == SQLITE_ROW) {
        result.push_back(sqlite3_column_int(stmt, 0));
    }
    sqlite3_finalize(stmt);
    for (int i = 0; i < result.size(); i++) {
        Book book;
        if (!getBookByID(DB, result[i], book)) return false;
        books.push_back(book);
    }
    return true;
}


bool getBookByID(sqlite3*& DB, int book_id, Book& book) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT TITLE, YEAR FROM BOOKS WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);

    sqlite3_bind_int(stmt, 1, book_id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        int year = sqlite3_column_int(stmt, 1);
        book = Book(book_id, title, year);
        sqlite3_finalize(stmt);
        return true;
    }
    sqlite3_finalize(stmt);
    return false;
}

bool getAuthorByID(sqlite3*& DB, int author_id, Author& author) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT FORENAME, SURNAME, BIO, BIRTH, DEATH FROM AUTHORS WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);

    sqlite3_bind_int(stmt, 1, author_id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* forename = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        const char* surname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

        const char* value = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        std::string bio = (value == nullptr) ? "" : std::string(value);

        const char* birth = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

        value = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        std::string death = (value == nullptr) ? "" : std::string(value);

        author = Author(author_id, forename, surname, bio, stringToQDate(birth), stringToQDate(death));
        sqlite3_finalize(stmt);
        return true;
    }
    sqlite3_finalize(stmt);
    return false;
}

bool getGenreByID(sqlite3*& DB, int genre_id, Genre& genre) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT NAME FROM GENRES WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);

    sqlite3_bind_int(stmt, 1, genre_id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        genre = Genre(genre_id, name);
        sqlite3_finalize(stmt);
        return true;
    }
    sqlite3_finalize(stmt);
    return false;
}

bool getUserByID(sqlite3*& DB, int user_id, User& user) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT FORENAME, SURNAME, BIRTH, EMAIL, PHONE FROM USERS WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);

    sqlite3_bind_int(stmt, 1, user_id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* forename = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        const char* surname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        const char* birth = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        const char* email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

        const char* value = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        std::string phone = (value == nullptr) ? "" : std::string(value);
        user = User(user_id, forename, surname, stringToQDate(birth), email, phone);
        sqlite3_finalize(stmt);
        return true;
    }
    sqlite3_finalize(stmt);
    return false;
}


bool setBookName(sqlite3*& DB, int book_id, std::string newName) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE BOOKS SET TITLE = ? WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, newName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, book_id);

    exit = sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    return exit == SQLITE_DONE;
}

bool setBookYear(sqlite3*& DB, int book_id, unsigned short int newYear) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE BOOKS SET YEAR = ? WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_int(stmt, 1, newYear);
    sqlite3_bind_int(stmt, 2, book_id);

    exit = sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    return exit == SQLITE_DONE;
}

bool setAuthorForename(sqlite3*& DB, int author_id, std::string newForename) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE AUTHORS SET FORENAME = ? WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, newForename.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, author_id);

    exit = sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    return exit == SQLITE_DONE;
}

bool setAuthorSurname(sqlite3*& DB, int author_id, std::string newSurname) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE AUTHORS SET SURNAME = ? WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, newSurname.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, author_id);

    exit = sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    return exit == SQLITE_DONE;
}

bool setAuthorBio(sqlite3*& DB, int author_id, std::string newBio) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE AUTHORS SET BIO = ? WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        return false;
    }

    if (newBio.empty()) sqlite3_bind_null(stmt, 1);
    else sqlite3_bind_text(stmt, 1, newBio.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, author_id);

    exit = sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    return exit == SQLITE_DONE;
}

bool setAuthorBirthDate(sqlite3*& DB, int author_id, QDate q_newBirthDate) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE AUTHORS SET BIRTH_DATE = ? WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        return false;
    }

    std::string newBirthDate = QDateToString(q_newBirthDate);
    if (newBirthDate.empty()) sqlite3_bind_null(stmt, 1);
    else sqlite3_bind_text(stmt, 1, newBirthDate.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, author_id);

    exit = sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    return exit == SQLITE_DONE;
}

bool setAuthorDeathDate(sqlite3*& DB, int author_id, QDate q_newDeathDate) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE AUTHORS SET DEATH_DATE = ? WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        return false;
    }

    std::string newDeathDate = QDateToString(q_newDeathDate);
    if (newDeathDate.empty()) sqlite3_bind_null(stmt, 1);
    else sqlite3_bind_text(stmt, 1, newDeathDate.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, author_id);

    exit = sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    return exit == SQLITE_DONE;
}

bool setGenreName(sqlite3*& DB, int genre_id, std::string newName) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE GENRES SET NAME = ? WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, newName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, genre_id);

    exit = sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    return exit == SQLITE_DONE;
}

bool setUserForename(sqlite3*& DB, int user_id, std::string newForename) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE USERS SET FORENAME = ? WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, newForename.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, user_id);

    exit = sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    return exit == SQLITE_DONE;
}

bool setUserSurname(sqlite3*& DB, int user_id, std::string newSurname) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE USERS SET SURNAME = ? WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, newSurname.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, user_id);

    exit = sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    return exit == SQLITE_DONE;
}

bool setUserBirth(sqlite3*& DB, int user_id, QDate q_newBirth) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE USERS SET BIRTH = ? WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        return false;
    }

    std::string newBirth = QDateToString(q_newBirth);
    sqlite3_bind_text(stmt, 1, newBirth.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, user_id);

    exit = sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    return exit == SQLITE_DONE;
}

bool setUserEmail(sqlite3*& DB, int user_id, std::string newEmail) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE USERS SET EMAIL = ? WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, newEmail.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, user_id);

    exit = sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    return exit == SQLITE_DONE;
}

bool setUserPhone(sqlite3*& DB, int user_id, std::string newPhone) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE USERS SET PHONE = ? WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        return false;
    }

    if (newPhone.empty()) sqlite3_bind_null(stmt, 1);
    else sqlite3_bind_text(stmt, 1, newPhone.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, user_id);

    exit = sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    return exit == SQLITE_DONE;
}


void deleteBookByID(sqlite3*& DB, int book_id) {
    sqlite3_stmt* stmt;
    const char* sql = "DELETE FROM BOOKS WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(DB) << "\n";
        return;
    }

    sqlite3_bind_int(stmt, 1, book_id);

    exit = sqlite3_step(stmt);
    if (exit != SQLITE_DONE) {
        std::cerr << "Error executing SQL statement: " << sqlite3_errmsg(DB) << "\n";
    }

    sqlite3_finalize(stmt);
}

void deleteAuthorByID(sqlite3*& DB, int author_id) {
    sqlite3_stmt* stmt;
    const char* sql = "DELETE FROM AUTHORS WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(DB) << "\n";
        return;
    }

    sqlite3_bind_int(stmt, 1, author_id);

    exit = sqlite3_step(stmt);
    if (exit != SQLITE_DONE) {
        std::cerr << "Error executing SQL statement: " << sqlite3_errmsg(DB) << "\n";
    }

    sqlite3_finalize(stmt);
}

void deleteGenreByID(sqlite3*& DB, int genre_id) {
    sqlite3_stmt* stmt;
    const char* sql = "DELETE FROM GENRES WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(DB) << "\n";
        return;
    }

    sqlite3_bind_int(stmt, 1, genre_id);

    exit = sqlite3_step(stmt);
    if (exit != SQLITE_DONE) {
        std::cerr << "Error executing SQL statement: " << sqlite3_errmsg(DB) << "\n";
    }

    sqlite3_finalize(stmt);
}

void deleteUserByID(sqlite3*& DB, int user_id) {
    sqlite3_stmt* stmt;
    const char* sql = "DELETE FROM USERS WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(DB) << "\n";
        return;
    }

    sqlite3_bind_int(stmt, 1, user_id);

    exit = sqlite3_step(stmt);
    if (exit != SQLITE_DONE) {
        std::cerr << "Error executing SQL statement: " << sqlite3_errmsg(DB) << "\n";
    }

    sqlite3_finalize(stmt);
}


std::vector<Book> sortBooks(sqlite3*& DB, Value value, bool bLess) {
    std::vector<Book> books;
    sqlite3_stmt* stmt;
    const char* sql = "SELECT ID, TITLE, YEAR FROM BOOKS;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char* title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        int year = sqlite3_column_int(stmt, 2);

        Book book(id, title, year);
        books.push_back(book);
    }
    sqlite3_finalize(stmt);
    switch (value) {
    case ID: {
        std::sort(books.begin(), books.end(), [bLess](const Book& a, const Book& b) {
            return bLess ? a.id < b.id : a.id > b.id;
        });
        break;
    }
    case TITLE: {
        std::sort(books.begin(), books.end(), [bLess](const Book& a, const Book& b) {
            return bLess ? a.title < b.title : a.title > b.title;
        });
        break;
    }
    case YEAR: {
        std::sort(books.begin(), books.end(), [bLess](const Book& a, const Book& b) {
            return bLess ? a.year < b.year : a.year > b.year;
        });
        break;
    }
    default: {
        say("WRONG VALUE FIELD");
        break;
    }
    }
    return books;
}

void sortBooks(std::vector<Book>& books, Value value, bool bLess) {
    switch (value) {
    case ID: {
        std::sort(books.begin(), books.end(), [bLess](const Book& a, const Book& b) {
            return bLess ? a.id < b.id : a.id > b.id;
        });
        break;
    }
    case TITLE: {
        std::sort(books.begin(), books.end(), [bLess](const Book& a, const Book& b) {
            return bLess ? a.title < b.title : a.title > b.title;
        });
        break;
    }
    case YEAR: {
        std::sort(books.begin(), books.end(), [bLess](const Book& a, const Book& b) {
            return bLess ? a.year < b.year : a.year > b.year;
        });
        break;
    }
    default: {
        say("WRONG VALUE FIELD");
        break;
    }
    }
}

std::vector<Author> sortAuthors(sqlite3*& DB, Value value, bool bLess) {
    std::vector<Author> authors;
    sqlite3_stmt* stmt;
    const char* sql = "SELECT ID, FORENAME, SURNAME, BIO, BIRTH_DATE, DEATH_DATE FROM AUTHORS;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char* forename = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        const char* surname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        const char* bio = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        const char* b_date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        const char* d_date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));

        Author author(id, forename, surname, bio, stringToQDate(b_date), stringToQDate(d_date));
        authors.push_back(author);
    }
    sqlite3_finalize(stmt);
    switch (value) {
    case ID: {
        std::sort(authors.begin(), authors.end(), [bLess](const Author& a, const Author& b) {
            return bLess ? a.id < b.id : a.id > b.id;
        });
        break;
    }
    case FORENAME: {
        std::sort(authors.begin(), authors.end(), [bLess](const Author& a, const Author& b) {
            return bLess ? a.forename < b.forename : a.forename > b.forename;
        });
        break;
    }
    case SURNAME: {
        std::sort(authors.begin(), authors.end(), [bLess](const Author& a, const Author& b) {
            return bLess ? a.surname < b.surname : a.surname > b.surname;
        });
        break;
    }
    case BIRTH: {
        std::sort(authors.begin(), authors.end(), [bLess](const Author& a, const Author& b) {
            return bLess ? a.birth < b.birth : a.birth > b.birth;
        });
        break;
    }
    case DEATH: {
        std::sort(authors.begin(), authors.end(), [bLess](const Author& a, const Author& b) {
            return bLess ? a.death < b.death : a.death > b.death;
        });
        break;
    }
    default: {
        say("WRONG VALUE FIELD");
        break;
    }
    }
    return authors;
}

void sortAuthors(std::vector<Author>& authors, Value value, bool bLess) {
    switch (value) {
    case ID: {
        std::sort(authors.begin(), authors.end(), [bLess](const Author& a, const Author& b) {
            return bLess ? a.id < b.id : a.id > b.id;
        });
        break;
    }
    case FORENAME: {
        std::sort(authors.begin(), authors.end(), [bLess](const Author& a, const Author& b) {
            return bLess ? a.forename < b.forename : a.forename > b.forename;
        });
        break;
    }
    case SURNAME: {
        std::sort(authors.begin(), authors.end(), [bLess](const Author& a, const Author& b) {
            return bLess ? a.surname < b.surname : a.surname > b.surname;
        });
        break;
    }
    case BIRTH: {
        std::sort(authors.begin(), authors.end(), [bLess](const Author& a, const Author& b) {
            return bLess ? a.birth < b.birth : a.birth > b.birth;
        });
        break;
    }
    case DEATH: {
        std::sort(authors.begin(), authors.end(), [bLess](const Author& a, const Author& b) {
            return bLess ? a.death < b.death : a.death > b.death;
        });
        break;
    }
    default: {
        say("WRONG VALUE FIELD");
        break;
    }
    }
}

std::vector<Genre> sortGenres(sqlite3*& DB, Value value, bool bLess) {
    std::vector<Genre> genres;
    sqlite3_stmt* stmt;
    const char* sql = "SELECT ID, NAME FROM GENRES;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

        Genre genre(id, name);
        genres.push_back(genre);
    }
    sqlite3_finalize(stmt);
    switch (value) {
    case ID: {
        std::sort(genres.begin(), genres.end(), [bLess](const Genre& a, const Genre& b) {
            return bLess ? a.id < b.id : a.id > b.id;
        });
        break;
    }
    case NAME: {
        std::sort(genres.begin(), genres.end(), [bLess](const Genre& a, const Genre& b) {
            return bLess ? a.name < b.name : a.name > b.name;
        });
        break;
    }
    default: {
        say("WRONG VALUE FIELD");
        break;
    }
    }
    return genres;
}

void sortGenres(std::vector<Genre>& genres, Value value, bool bLess) {
    switch (value) {
    case ID: {
        std::sort(genres.begin(), genres.end(), [bLess](const Genre& a, const Genre& b) {
            return bLess ? a.id < b.id : a.id > b.id;
        });
        break;
    }
    case NAME: {
        std::sort(genres.begin(), genres.end(), [bLess](const Genre& a, const Genre& b) {
            return bLess ? a.name < b.name : a.name > b.name;
        });
        break;
    }
    default: {
        say("WRONG VALUE FIELD");
        break;
    }
    }
}

std::vector<User> sortUsers(sqlite3*& DB, Value value, bool bLess) {
    std::vector<User> users;
    sqlite3_stmt* stmt;
    const char* sql = "SELECT ID, FORENAME, SURNAME, BIRTH, EMAIL, PHONE FROM USERS;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char* forename = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        const char* surname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        const char* birth = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        const char* email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        const char* phone = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));

        User user = User(id, forename, surname, stringToQDate(birth), email, phone);
        users.push_back(user);
    }
    sqlite3_finalize(stmt);
    switch (value) {
    case ID: {
        std::sort(users.begin(), users.end(), [bLess](const User& a, const User& b) {
            return bLess ? a.id < b.id : a.id > b.id;
        });
        break;
    }
    case FORENAME: {
        std::sort(users.begin(), users.end(), [bLess](const User& a, const User& b) {
            return bLess ? a.forename < b.forename : a.forename > b.forename;
        });
        break;
    }
    case SURNAME: {
        std::sort(users.begin(), users.end(), [bLess](const User& a, const User& b) {
            return bLess ? a.surname < b.surname : a.surname > b.surname;
        });
        break;
    }
    case BIRTH: {
        std::sort(users.begin(), users.end(), [bLess](const User& a, const User& b) {
            return bLess ? a.birth < b.birth : a.birth > b.birth;
        });
        break;
    }
    default: {
        say("WRONG VALUE FIELD");
        break;
    }
    }
    return users;
}

void sortUsers(std::vector<User>& users, Value value, bool bLess) {
    switch (value) {
    case ID: {
        std::sort(users.begin(), users.end(), [bLess](const User& a, const User& b) {
            return bLess ? a.id < b.id : a.id > b.id;
        });
        break;
    }
    case FORENAME: {
        std::sort(users.begin(), users.end(), [bLess](const User& a, const User& b) {
            return bLess ? a.forename < b.forename : a.forename > b.forename;
        });
        break;
    }
    case SURNAME: {
        std::sort(users.begin(), users.end(), [bLess](const User& a, const User& b) {
            return bLess ? a.surname < b.surname : a.surname > b.surname;
        });
        break;
    }
    case BIRTH: {
        std::sort(users.begin(), users.end(), [bLess](const User& a, const User& b) {
            return bLess ? a.birth < b.birth : a.birth > b.birth;
        });
        break;
    }
    default: {
        say("WRONG VALUE FIELD");
        break;
    }
    }
}
