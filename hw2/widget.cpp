#include "widget.h"
#include <QComboBox>
#include <QSlider>
#include "glwidget.h"
#include <QLabel>

Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
    QComboBox *comboBox = new QComboBox;
    GLWidget *glwidget = new GLWidget;
    comboBox->addItem("1.Plane");
    comboBox->addItem("2.Sphere");
    comboBox->addItem("3.Cylinder");
    QObject::connect(comboBox, SIGNAL(activated(int)), glwidget, SLOT(setObjectMode(int)));
    QComboBox *filterComboBox = new QComboBox;
    filterComboBox->addItem("1.Nearest-neighbour");
    filterComboBox->addItem("2.Bilinear");
    filterComboBox->addItem("3.Trilinear");
    QObject::connect(filterComboBox, SIGNAL(activated(int)), glwidget, SLOT(setFilterMode(int)));

    layout = new QVBoxLayout;
    layout->addWidget(glwidget);
    layout->addWidget(new QLabel("Object:"));
    layout->addWidget(comboBox);
    layout->addWidget(new QLabel("Filtering:"));
    layout->addWidget(filterComboBox);
    setLayout(layout);
    setWindowTitle("LAB 2");

    QSlider *kSlider = new QSlider(Qt::Horizontal);
    kSlider->setRange(1, 100);
    kBox = new QGroupBox("k");
    QVBoxLayout *vl = new QVBoxLayout;
    vl->addWidget(kSlider);
    kBox->setLayout(vl);
    //kBox->hide();
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


