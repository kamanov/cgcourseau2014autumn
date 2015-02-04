#include "widget.h"
#include <QVBoxLayout>
#include <QComboBox>
#include "glwidget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
    QComboBox *comboBox = new QComboBox;
    GLWidget *glwidget = new GLWidget;
    comboBox->addItem("simple shader");
    comboBox->addItem("chess display shader");
    comboBox->addItem("chess non-display shader");
    comboBox->addItem("chess gl_FragCoord");
    comboBox->addItem("fractal Julia");
    QObject::connect(comboBox, SIGNAL(activated(int)), glwidget, SLOT(setShaderProgramm(int)));
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(glwidget);
    layout->addWidget(comboBox);
    setLayout(layout);
    setWindowTitle("LAB 0");

    resize(480, 520);
}


