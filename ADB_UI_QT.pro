QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Source/fileexplorer.cpp \
    Source/main.cpp \
    Source/mainwindow.cpp \
    Source/packagemanager.cpp \
    Source/shellterm.cpp \
    Source/settingmanager.cpp \
    Source/aboutdialog.cpp

HEADERS += \
    Header/mainwindow.hpp \
    Header/fileexplorer.hpp \
    Header/packagemanager.hpp \
    Header/shellterm.hpp \
    Header/utilities.hpp \
    Header/settingmanager.hpp \
    Header/aboutdialog.hpp

FORMS += \
    UIView/FileExplorer.ui \
    UIView/PackageManager.ui \
    UIView/ShellTerm.ui \
    UIView/about.ui \
    UIView/mainwindow.ui \  

TRANSLATIONS += \
    ADB_UI_QT_en_IN.ts

INCLUDEPATH += \
    Header/

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources/UI.qrc
