QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    keyborad.cpp \
    loginwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    managermenu.cpp \
    mybutton.cpp \
    mycamera.cpp \
    myserial.cpp \
    storedialog.cpp \
    tableadddialog.cpp \
    tableinfo.cpp \
    tableitem.cpp

HEADERS += \
    keyborad.h \
    loginwindow.h \
    mainwindow.h \
    managermenu.h \
    mybutton.h \
    mycamera.h \
    myhead.h \
    myserial.h \
    storedialog.h \
    tableadddialog.h \
    tableinfo.h \
    tableitem.h

FORMS += \
    keyborad.ui \
    loginwindow.ui \
    mainwindow.ui \
    managermenu.ui \
    storedialog.ui \
    tableadddialog.ui \
    tableitem.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
