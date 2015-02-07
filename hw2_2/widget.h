#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QGroupBox>



class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = 0);
private:
    QHBoxLayout *layout;
private slots:

};

#endif // WIDGET_H
