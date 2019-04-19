QT -= gui
QT += serialport

CONFIG += c++11 console
CONFIG -= app_bundle

TEMPLATE = app
DEFINES += "AVR_TEST"

release: TARGET = virtualavr
debug: TARGET = virtualavrd

DESTDIR = $$(DEV_BIN)

release: BUILDDIR = build/release
debug:   BUILDDIR = build/debug

OBJECTS_DIR = $$BUILDDIR/.obj
MOC_DIR = $$BUILDDIR/.moc
RCC_DIR = $$BUILDDIR/.qrc
UI_DIR = $$BUILDDIR/.ui

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ../

if(debug){
    LIBS += -L$$(DEV_LIBS) -ldeviced -ltagsystemd
}
else{
    LIBS += ldevice
}


SOURCES += \
        main.cpp \
    app.cpp \
    ../avr/message.cpp \
    ../avr/messagehandler.cpp \
    ../avr/messagetranslationsenter.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    app.h \
    ../avr/message.h \
    ../avr/messagehandler.h \
    ../avr/messagetranslationsenter.h
