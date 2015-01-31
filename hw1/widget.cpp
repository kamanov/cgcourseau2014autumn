#include "widget.h"
#include <QComboBox>
#include <QSlider>
#include "glwidget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
    QComboBox *comboBox = new QComboBox;
    GLWidget *glwidget = new GLWidget;
    comboBox->addItem("1.simple model");
    comboBox->addItem("2.model carcass");
    comboBox->addItem("3.model with color=normal.y");
    comboBox->addItem("4.model with color=function");
    QObject::connect(comboBox, SIGNAL(activated(int)), glwidget, SLOT(setMode(int)));
    QObject::connect(comboBox, SIGNAL(activated(int)), this, SLOT(showSliders(int)));
    layout = new QVBoxLayout;
    layout->addWidget(glwidget);
    layout->addWidget(comboBox);
    setLayout(layout);
    setWindowTitle("LAB 1");

    QSlider *kSlider = new QSlider(Qt::Horizontal);
    kSlider->setRange(0, 100);
    kBox = new QGroupBox("k");
    QVBoxLayout *vl = new QVBoxLayout;
    vl->addWidget(kSlider);
    kBox->setLayout(vl);
    kBox->hide();
    QSlider *vSlider = new QSlider(Qt::Horizontal);
    vSlider->setRange(0, 100);
    vBox = new QGroupBox("v");
    vl = new QVBoxLayout;
    vl->addWidget(vSlider);
    vBox->setLayout(vl);
    vBox->hide();
    QObject::connect(kSlider, SIGNAL(valueChanged(int)), glwidget, SLOT(setK(int)));
    QObject::connect(vSlider, SIGNAL(valueChanged(int)), glwidget, SLOT(setV(int)));

    QVBoxLayout *slidersLayout = new QVBoxLayout;
    slidersLayout->addWidget(kBox);
    slidersLayout->addWidget(vBox);
    layout->addLayout(slidersLayout);

    resize(640, 640);
}


void Widget::showSliders(int i)
{
    if (i == 3) {
        kBox->show();
        vBox->show();
    } else {
        kBox->hide();
        vBox->hide();
    }
    update();
}


