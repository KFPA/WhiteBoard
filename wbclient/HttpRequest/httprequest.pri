QT += network qml

INCLUDEPATH *= \
    $$PWD/include/

SOURCES += $$PWD/src/httprequest.cpp \
    $$PWD/src/networkcookiejar.cpp

HEADERS += $$PWD/src/httprequest.h \
    $$PWD/src/httprequest_p.h \
    $$PWD/src/networkcookiejar.h


