QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    author.cpp \
    authorviewwidget.cpp \
    book.cpp \
    bookviewwidget.cpp \
    genre.cpp \
    libmgr.cpp \
    loan.cpp \
    main.cpp \
    mainwindow.cpp \
    smoothscrollarea.cpp \
    sqlite3.c \
    tableviewwidget.cpp \
    user.cpp \
    userviewwidget.cpp

HEADERS += \
    CommonUtil.h \
    author.h \
    authorviewwidget.h \
    book.h \
    bookviewwidget.h \
    genre.h \
    libmgr.h \
    loan.h \
    mainwindow.h \
    smoothscrollarea.h \
    sqlite3.h \
    tableviewwidget.h \
    user.h \
    userviewwidget.h

FORMS += \
    authorviewwidget.ui \
    bookviewwidget.ui \
    mainwindow.ui \
    smoothscrollarea.ui \
    tableviewwidget.ui \
    userviewwidget.ui

TRANSLATIONS += \
    MyLibraryManagerQt_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
