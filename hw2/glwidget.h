#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QGLShaderProgram>
#include <QGLFunctions>
#include <math.h>

struct Object3D {
    QVector<QVector3D> vertices;
    QVector<QVector3D> normals;
    QVector<QVector2D> texcoords;
};

class GLWidget : public QGLWidget, public QGLFunctions
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
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    void initShaderProgramm(QGLShaderProgram& programm, const QString& vertex, const QString& fragment);
    void initTextures();
    bool loadObjModel(QString filename, Object3D& object);
    void setTexParam(GLint min, GLint mag);
    QMatrix4x4 mMatrix;
    QMatrix4x4 vMatrix;
    QMatrix4x4 pMatrix;
    Object3D objects[3];
    Object3D *currentObject;

    GLuint texture;
    QGLShaderProgram shaderProgram;

    double alpha;
    double beta;
    double distance;
    QPoint lastMousePosition;

    float k_tex_coord;
    
public slots:
    void setObjectMode(int i);
    void setFilterMode(int i);
    void setK(int k) {
        k_tex_coord = k;
        updateGL();
    }
    void setV(int v) {
    }

};

#endif // GLWIDGET_H
