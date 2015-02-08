#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QGLShaderProgram>
#include <QTimer>
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
    void initTexture();
    QMatrix4x4 mMatrix;
    QMatrix4x4 vMatrix;
    QMatrix4x4 pMatrix;
    Object3D cube;
    GLuint texture;
    GLuint normTexture;
    QGLShaderProgram lightSourceProgramm;
    QGLShaderProgram lightProgramm;
    QVector<QVector3D> lsVertices;
    QVector<QVector3D> lsColors;


    QVector<QVector3D> tangents;
    QVector<QVector3D> bitangents;

    double alpha;
    double beta;
    double distance;
    QPoint lastMousePosition;

    float k_tex_coord;
    float lightAngle;
    float lightDistance;

    float ambient;
    float diffuse;
    float specular;

    float ambientColor;
    float diffuseColor;
    float specularColor;

public slots:
    void initTangentAndBitangents();
    void setAmbient(int i) {
        ambient = i;
        updateGL();
    }
    void setDiffuse(int i) {
        diffuse = i;
        updateGL();
    }
    void setSpecular(int i) {
        specular = i;
        updateGL();
    }

    void setLightAngle(int i) {
        lightAngle = i;
        updateGL();
    }
    void setLightDistance(int i) {
        lightDistance = i;
        updateGL();
    }

    void setAmbientColor(int i) {
        ambientColor = i;
        updateGL();
    }
    void setDiffuseColor(int i) {
        diffuseColor = i;
        updateGL();
    }
    void setSpecularColor(int i) {
        specularColor = i;
        updateGL();
    }
};

#endif // GLWIDGET_H
