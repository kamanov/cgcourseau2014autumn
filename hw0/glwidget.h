#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QGLShaderProgram>
#include <QTimer>
#include <QMouseEvent>

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);
    QSize sizeHint() const;
    ~GLWidget();

protected:
    void initializeGL();
    void resizeGL(int m_width, int m_height);
    void paintGL();

    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);


private:
    void initTextures();
    void initShaderProgramm(QGLShaderProgram& programm, const QString& vertex, const QString& fragment);

    QMatrix4x4 rotationMatrix;
    QVector<QVector3D> vertices;
    QVector<QVector3D> fractal_vertices;

    QGLShaderProgram programms[5];
    QGLShaderProgram *shaderProgram;

    QTimer timer;
    GLuint texture;

    float m_width;
    float m_height;

    float cx;
    float cy;
    int fractal_iter;
public slots:
    void rotate();
    void setShaderProgramm(int i);

};

#endif // GLWIDGET_H
