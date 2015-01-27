#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QGLShaderProgram>
#include <QTimer>

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);
    QSize sizeHint() const;
    ~GLWidget();

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

private:
    void initTextures();
    void initShaderProgramm(QGLShaderProgram& programm, const QString& vertex, const QString& fragment);

    QMatrix4x4 rotationMatrix;
    QVector<QVector3D> vertices;

    QGLShaderProgram programms[3];
    QGLShaderProgram *shaderProgram;

    QTimer timer;
    GLuint texture;

public slots:
    void rotate();
    void setShaderProgramm(int i);

};

#endif // GLWIDGET_H
