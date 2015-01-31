#include "glwidget.h"
#include <qmath.h>
#include <QGLBuffer>
#include <QFile>
#include <QMouseEvent>

struct Face {
    QVector<int> v;
    QVector<int> t;
    QVector<int> n;
};

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent)
    , alpha(0)
    , beta(0)
    , distance(15)
    , showFrame(false)
    , rastr_mode(0)
    , v(0)
    , t(0)
    , vt(0)
    , k_per_max(0)
    , max_model_distance(0)
{
    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(updateTime()));
}

void GLWidget::updateTime() {
    t += timer.interval() / 1000.0;
    updateVT();
}


QSize GLWidget::sizeHint() const
{
    return QSize(640, 480);
}

void GLWidget::setMode(int i) {
    int mode = i;
    if (i == 1) {
        rastr_mode = 0;
        showFrame = true;
    } else {
        showFrame = false;
        if (i > 1) i--;
        rastr_mode = i;
    }
    if (mode == 3) {
        t = 0;
        timer.start(30);
    }
    else
        timer.stop();

    updateGL();
}

GLWidget::~GLWidget()
{
}

bool GLWidget::loadObjModel(QString filename)
{
    QFile object(filename);
    if(!object.open(QIODevice::ReadOnly)) {
        qDebug() << "error read file";
        return false;
    }
    QVector<QVector3D> verts;
    QVector<QVector2D> textures;
    QVector<QVector3D> normals;
    QVector<Face> faces;

    while (!object.atEnd()) {
        QString line = object.readLine().trimmed();
        QString val = line.left(2).trimmed();
        QString data = line.mid(2).trimmed();
        QStringList valsList = data.split(' ');
        if (val == "v") {
            Q_ASSERT(valsList.size() == 3);
            verts.push_back(QVector3D(valsList[0].toFloat(), valsList[1].toFloat(), valsList[2].toFloat()));
        } else if (val == "vt") {
            Q_ASSERT(valsList.size() == 2);
            textures.push_back(QVector2D(valsList[0].toFloat(), valsList[1].toFloat()));
        } else if (val == "vn") {
            Q_ASSERT(valsList.size() == 3);
            normals.push_back(QVector3D(valsList[0].toFloat(), valsList[1].toFloat(), valsList[2].toFloat()));
        } else if (val == "f") {
            Q_ASSERT(valsList.size() == 3);
            Face face;
            foreach (QString str, valsList) {
                QStringList fList = str.split('/');
                face.v.push_back(fList[0].toInt());
                face.t.push_back(fList[1].toInt());
                face.n.push_back(fList[2].toInt());
            }
            faces.push_back(face);
        } else {
            qDebug() << "invalid obj file format";
            return false;
        }
    }

    foreach (Face f, faces) {
        for (int i = 0; i < 3; i++) {
            m_vertices.push_back(verts[f.v[i] - 1]);
            m_texures.push_back(textures[f.t[i] - 1]);
            m_normals.push_back(normals[f.n[i] - 1]);
        }
    }
    object.close();
    return true;
}

void GLWidget::initShaderProgramm(QGLShaderProgram& programm, const QString& vertex, const QString& fragment) {
    if (!programm.addShaderFromSourceFile(QGLShader::Vertex, vertex))
        close();
    if (!programm.addShaderFromSourceFile(QGLShader::Fragment, fragment))
        close();
    programm.link();
}

void GLWidget::initializeGL()
{
    loadObjModel(":/model.obj");
    foreach (QVector3D v, m_vertices) {
        model_center += v;
    }

    model_center = model_center / m_vertices.size();

    foreach (QVector3D v, m_vertices) {
        max_model_distance = std::max(max_model_distance, (v - model_center).length());
    }

    glEnable(GL_DEPTH_TEST);

    qglClearColor(QColor(Qt::gray));
    initShaderProgramm(shaderProgram, ":/vertex.glsl", ":/fragment.glsl");
}

void GLWidget::resizeGL(int width, int height)
{
    if (height == 0) {
        height = 1;
    }

    pMatrix.setToIdentity();
    pMatrix.perspective(60.0, (float) width / (float) height, 0.001, 1000);
    glViewport(0, 0, width, height);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    vMatrix.setToIdentity();
    QMatrix4x4 cameraTransformation;
    cameraTransformation.rotate(alpha, 0, 1, 0);
    cameraTransformation.rotate(beta, 1, 0, 0);
    QVector3D cameraPosition = cameraTransformation * QVector3D(0, 0, distance);
    QVector3D cameraUpDirection = cameraTransformation * QVector3D(0, 1, 0);
    vMatrix.lookAt(cameraPosition, QVector3D(0, 0, 0), cameraUpDirection);

    shaderProgram.bind();
    shaderProgram.setUniformValue("mvpMatrix", pMatrix * vMatrix * mMatrix);
    shaderProgram.setUniformValue("rastr_mode", rastr_mode);
    shaderProgram.setUniformValue("vt", vt);
    shaderProgram.setUniformValue("c", model_center);
    shaderProgram.setUniformValue("k_per_max", k_per_max);

    shaderProgram.setAttributeArray("norm", m_normals.constData());
    shaderProgram.setAttributeArray("vert", m_vertices.constData());
    shaderProgram.enableAttributeArray("norm");
    shaderProgram.enableAttributeArray("vert");
    if (showFrame) {
        glEnable(GL_POLYGON_OFFSET_LINE);
        glPolygonOffset(-2, -2);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
    }
    shaderProgram.disableAttributeArray("vert");
    shaderProgram.disableAttributeArray("norm");
    shaderProgram.release();
}


void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastMousePosition = event->pos();
    event->accept();
}
void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int deltaX = event->x() - lastMousePosition.x();
    int deltaY = event->y() - lastMousePosition.y();
    if (event->buttons() & Qt::LeftButton) {
        alpha -= deltaX;
        while (alpha < 0) {
            alpha += 360;
        }
        while (alpha >= 360) {
            alpha -= 360;
        }
        beta -= deltaY;
        if (beta < -90) {
            beta = -90;
        }
        if (beta > 90) {
            beta = 90;
        }

        updateGL();
    }
    lastMousePosition = event->pos();
    event->accept();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    int delta = event->delta();
    if (delta < 0) {
        distance *= 1.1;
    } else if (delta > 0) {
        distance *= 0.9;
    }
    updateGL();

    event->accept();
}
