QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mycamera.cpp \
    myserial.cpp \
    storedialog.cpp \
    tableinfo.cpp \
    tableitem.cpp

HEADERS += \
    mainwindow.h \
    mycamera.h \
    myhead.h \
    myserial.h \
    storedialog.h \
    tableinfo.h \
    tableitem.h

FORMS += \
    mainwindow.ui \
    storedialog.ui \
    tableitem.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
