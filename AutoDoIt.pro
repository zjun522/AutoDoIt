#-------------------------------------------------
#
# Project created by QtCreator 2019-06-21T10:51:04
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AutoDoIt
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++14

SOURCES += \
        action/cstartstopprocessaction.cpp \
        common/cappenv.cpp \
        common/csslserver.cpp \
        common/csyswin.cpp \
        common/cmessagebox.cpp \
        action/caction.cpp \
        action/cactioneditdialog.cpp \
        action/cexitautodoitaction.cpp \
        action/cgroupaction.cpp \
        action/cmessageaction.cpp \
        action/cpoweroffaction.cpp \
        action/cstartstoptaskaction.cpp \
        cgetprocessdialog.cpp \
        clistmodel.cpp \
        clistwidgetitem.cpp \
        cnewtaskwizard.cpp \
        condition/ccondition.cpp \
        condition/cconditioneditdialog.cpp \
        condition/cfilecondition.cpp \
        condition/cgroupcondition.cpp \
        condition/cidlecondition.cpp \
        condition/cprocesscondition.cpp \
        condition/ctimecondition.cpp \
        cprocesstablemodel.cpp \
        trigger/cgrouptrigger.cpp \
        trigger/chttptrigger.cpp \
        trigger/cidletrigger.cpp \
        trigger/ctimertrigger.cpp \
        trigger/ctrigger.cpp \
        main.cpp \
        mainwindow.cpp \
        trigger/cudptrigger.cpp \
        trigger/cwatchfiletrigger.cpp \
        trigger/cwatchprocesstrigger.cpp

HEADERS += \
        action/cstartstopprocessaction.h \
        cmakefactory.hpp \
        common/cappenv.hpp \
        common/csslserver.h \
        common/csyswin.h \
        common/cmessagebox.h \
        action/caction.h \
        action/cactioneditdialog.h \
        action/cexitautodoitaction.h \
        action/cgroupaction.h \
        action/cmessageaction.h \
        action/cpoweroffaction.h \
        action/cstartstoptaskaction.h \
        cgetprocessdialog.h \
        clistmodel.h \
        clistwidgetitem.h \
        cnewtaskwizard.h \
        common/data.h \
        condition/ccondition.h \
        condition/cconditioneditdialog.h \
        condition/cfilecondition.h \
        condition/cgroupcondition.h \
        condition/cidlecondition.h \
        condition/cprocesscondition.h \
        condition/ctimecondition.h \
        cprocesstablemodel.h \
        trigger/cgrouptrigger.h \
        trigger/chttptrigger.h \
        trigger/cidletrigger.h \
        trigger/ctimertrigger.h \
        trigger/ctrigger.h \
        mainwindow.h \
        trigger/cudptrigger.h \
        trigger/cwatchfiletrigger.h \
        trigger/cwatchprocesstrigger.h \
        version.h

FORMS += \
        action/cactioneditdialog.ui \
        cgetprocessdialog.ui \
        cnewtaskwizard.ui \
        condition/cconditioneditdialog.ui \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32{
    RC_FILE += ICO.rc
#    QMAKE_LFLAGS += /MANIFESTUAC:"level='requireAdministrator'uiAccess='false'"
}

RESOURCES += \
    qrc.qrc

LIBS += -ladvapi32
LIBS += -luser32

DEFINES += USE_CUSTOM_DIR_MODEL
CONFIG(release, debug|release) {
    DEFINES += QT_NO_DEBUG_OUTPUT
    DEFINES += ZJ_NO_DEBUG_OUTPUT
}
