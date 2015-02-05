
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hw2
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
    model.qrc \
    textures.qrc
