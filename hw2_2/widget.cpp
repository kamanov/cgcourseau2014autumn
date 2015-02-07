#include "widget.h"
#include <QComboBox>
#include <QSlider>
#include "glwidget.h"
#include <QLabel>

Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
    GLWidget *glwidget = new GLWidget;
    layout = new QHBoxLayout;

    setWindowTitle("LAB 2_2");

    QVBoxLayout *vl = new QVBoxLayout;

    QGroupBox *lightBox = new QGroupBox("Light");
    vl = new QVBoxLayout;
    vl->addWidget(new QLabel("Light source angle"));
    QSlider *lightAngle = new QSlider(Qt::Horizontal);
    lightAngle->setRange(0, 360);
    vl->addWidget(lightAngle);
    QObject::connect(lightAngle, SIGNAL(valueChanged(int)), glwidget, SLOT(setLightAngle(int)));

    vl->addWidget(new QLabel("Light distance"));
    QSlider *lightDistance = new QSlider(Qt::Horizontal);
    lightDistance->setRange(1, 10);
    vl->addWidget(lightDistance);
    QObject::connect(lightDistance, SIGNAL(valueChanged(int)), glwidget, SLOT(setLightDistance(int)));

    vl->addWidget(new QLabel("Ambient component"));
    QSlider *ambient = new QSlider(Qt::Horizontal);
    ambient->setRange(1, 10);
    vl->addWidget(ambient);
    QObject::connect(ambient, SIGNAL(valueChanged(int)), glwidget, SLOT(setAmbient(int)));


    vl->addWidget(new QLabel("Diffuse component"));
    QSlider *diffuse = new QSlider(Qt::Horizontal);
    diffuse->setRange(1, 10);
    vl->addWidget(diffuse);
    QObject::connect(diffuse, SIGNAL(valueChanged(int)), glwidget, SLOT(setDiffuse(int)));

    vl->addWidget(new QLabel("Specular component"));
    QSlider *specular = new QSlider(Qt::Horizontal);
    specular->setRange(1, 50);
    vl->addWidget(specular);
    QObject::connect(specular, SIGNAL(valueChanged(int)), glwidget, SLOT(setSpecular(int)));

    vl->addWidget(new QLabel("Ambient color"));
    QSlider *ambientColor = new QSlider(Qt::Horizontal);
    ambientColor->setRange(1, 255);
    ambientColor->setValue(32);
    vl->addWidget(ambientColor);
    QObject::connect(ambientColor, SIGNAL(valueChanged(int)), glwidget, SLOT(setAmbientColor(int)));


    vl->addWidget(new QLabel("Diffuse color"));
    QSlider *diffuseColor = new QSlider(Qt::Horizontal);
    diffuseColor->setRange(1, 255);
    diffuseColor->setValue(128);
    vl->addWidget(diffuseColor);
    QObject::connect(diffuseColor, SIGNAL(valueChanged(int)), glwidget, SLOT(setDiffuseColor(int)));

    vl->addWidget(new QLabel("Specular color"));
    QSlider *specularColor = new QSlider(Qt::Horizontal);
    specularColor->setRange(1, 255);
    specularColor->setValue(255);
    vl->addWidget(specularColor);
    QObject::connect(specularColor, SIGNAL(valueChanged(int)), glwidget, SLOT(setSpecularColor(int)));
    lightBox->setLayout(vl);

    QVBoxLayout *slidersLayout = new QVBoxLayout;
    slidersLayout->addWidget(lightBox);

    layout->addLayout(slidersLayout);
    layout->addWidget(glwidget);
    setLayout(layout);




    resize(1000, 640);
}



