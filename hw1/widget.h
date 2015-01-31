#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QGroupBox>



class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = 0);
private:
    QVBoxLayout *layout;
    QGroupBox *vBox;
    QGroupBox *kBox;
private slots:
    void showSliders(int i);
};

#endif // WIDGET_H
