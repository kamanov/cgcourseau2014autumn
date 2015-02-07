
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hw2_2
TEMPLATE = app


SOURCES += main.cpp

qtHaveModule(opengl) {
    QT += opengl

    SOURCES += widget.cpp\
                glwidget.cpp

    HEADERS += widget.h \
                glwidget.h
}

RESOURCES += \
    shaders.qrc \
    textures.qrc
