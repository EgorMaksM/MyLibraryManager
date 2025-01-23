#ifndef COMMONUTIL_H
#define COMMONUTIL_H

#include <QDate>
#include <QString>
#include <sqlite3.h>
#include <algorithm>

enum Value {
    ID,
    TITLE,
    YEAR,
    FORENAME,
    SURNAME,
    BIRTH,
    DEATH,
    NAME
};

inline QDate stringToQDate(const QString& dateStr) {
    if (dateStr.isEmpty()) return QDate();
    return QDate::fromString(dateStr, "yyyy-MM-dd");
}

inline QString QDateToString(const QDate& date) {
    if (date.isNull()) return "";
    return date.toString("yyyy-MM-dd");
}

#endif // COMMONUTIL_H
