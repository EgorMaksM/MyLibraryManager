#ifndef GENRE_H
#define GENRE_H

#include "CommonUtil.h"

struct Genre {
    int id;
    QString name;

    Genre(int id = NULL, const QString& name = "")
        : id(id), name(name)
    {
    }
};

bool operator==(const Genre& genre, const Genre& other);

void addGenre(sqlite3*& DB, QString name);

bool getGenreByID(sqlite3*& DB, int genre_id, Genre& genre);
std::vector<Genre> getGenres(sqlite3*& DB);

bool setGenreName(sqlite3*& DB, int genre_id, QString newName);

void deleteGenreByID(sqlite3*& DB, int genre_id);

void sortGenres(std::vector<Genre>& genres, Value value, bool bLess = true);

#endif // GENRE_H
