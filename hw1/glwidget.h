#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QGLShaderProgram>
#include <QTimer>
#include <math.h>

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
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    void initShaderProgramm(QGLShaderProgram& programm, const QString& vertex, const QString& fragment);
    bool loadObjModel(QString filename);

    QMatrix4x4 mMatrix;
    QMatrix4x4 vMatrix;
    QMatrix4x4 pMatrix;
    QVector<QVector3D> m_vertices;
    QVector<QVector3D> m_normals;
    QVector<QVector2D> m_texures;

    QGLShaderProgram shaderProgram;

    QTimer timer;
    double alpha;
    double beta;
    double distance;
    QPoint lastMousePosition;

    bool showFrame;
    int rastr_mode;

    float v;
    float t;
    float vt;
    float k_per_max;
    float max_model_distance;
    QVector3D model_center;


public slots:
    void setMode(int i);
    void setK(int k) {
        k_per_max = 2 * M_PI * k / max_model_distance;
        updateGL();
    }
    void setV(int v) {
        this->v = v;
        updateVT();
    }
    void updateVT() {
        vt = 2 * M_PI * v * t;
        updateGL();
    }
    void updateTime();

};

#endif // GLWIDGET_H
