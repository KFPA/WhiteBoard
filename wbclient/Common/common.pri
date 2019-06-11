QT += network qml

INCLUDEPATH *= \
    $$PWD/include/

SOURCES += \
    $$PWD/src/fileio.cpp \
    $$PWD/src/fileinfo.cpp \
    $$PWD/src/loghelper.cpp \
    $$PWD/src/util.cpp \
    $$PWD/src/settingshelper.cpp \
    $$PWD/src/fileupload.cpp

HEADERS += \
    $$PWD/src/fileio.h \
    $$PWD/src/fileinfo.h \
    $$PWD/src/loghelper.h \
    $$PWD/src/singleton.h \
    $$PWD/src/util.h \
    $$PWD/src/settingshelper.h \
    $$PWD/src/fileupload.h
