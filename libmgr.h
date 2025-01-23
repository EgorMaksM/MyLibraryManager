#ifndef LIBMGR_H
#define LIBMGR_H

#include <vector>

#include "CommonUtil.h"
#include "book.h"
#include "author.h"
#include "genre.h"
#include "user.h"
#include "loan.h"

void initDB(sqlite3*& DB, char*& messageError);

void linkAuthorToBook(sqlite3*& DB, int author_id, int book_id);
void linkGenreToBook(sqlite3*& DB, int genre_id, int book_id);
bool unlinkAuthorFromBook(sqlite3*& DB, int author_id, int book_id);
bool unlinkGenreFromBook(sqlite3*& DB, int genre_id, int book_id);

bool getAuthorsByBookID(sqlite3*& DB, int book_id, std::vector<Author>& authors);
bool getBooksByAuthorID(sqlite3*& DB, int author_id, std::vector<Book>& books);
bool getGenresByBookID(sqlite3*& DB, int book_id, std::vector<Genre>& genres);
bool getBooksByGenreID(sqlite3*& DB, int genre_id, std::vector<Book>& books);
bool getGenresByAuthorID(sqlite3*& DB, int author_id, std::vector<Genre>& genres);
bool getBooksByUserID(sqlite3*& DB, int user_id, std::vector<Book>& books);


#endif // LIBMGR_H
