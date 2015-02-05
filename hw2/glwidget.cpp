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
    , distance(5)
    , k_tex_coord(1.0f)
{
    currentObject = &objects[0];
}



QSize GLWidget::sizeHint() const
{
    return QSize(640, 480);
}

void GLWidget::setObjectMode(int i) {
    if (i >= 0 && i < 3) {
        if (i == 2)
            distance = 15.0;
        else
            distance = 7.0;
        currentObject = &objects[i];
        updateGL();
    }
}

void GLWidget::setFilterMode(int i)
{
    if (i == 0) {
        setTexParam(GL_NEAREST, GL_NEAREST);
    } else if (i == 1) {
        setTexParam(GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR);
    } else if (i == 2) {
        setTexParam(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    }
    updateGL();
}

GLWidget::~GLWidget()
{
}

bool GLWidget::loadObjModel(QString filename, Object3D& outObject)
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
            outObject.vertices.push_back(verts[f.v[i] - 1]);
            outObject.texcoords.push_back(textures[f.t[i] - 1]);
            outObject.normals.push_back(normals[f.n[i] - 1]);
        }
    }
    object.close();
    return true;
}

void GLWidget::setTexParam(GLint min, GLint mag)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
}

void GLWidget::initTextures()
{
    glEnable(GL_TEXTURE_2D);
    texture = bindTexture(QImage(":/brickwork-texture.jpg"));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glGenerateMipmap(GL_TEXTURE_2D);
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
    loadObjModel(":/plane.obj", objects[0]);
    loadObjModel(":/sphere.obj", objects[1]);
    loadObjModel(":/cylinder.obj", objects[2]);

    glEnable(GL_DEPTH_TEST);
    qglClearColor(QColor(Qt::gray));
    initShaderProgramm(shaderProgram, ":/vertex.glsl", ":/fragment.glsl");
    initTextures();
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
    shaderProgram.setUniformValue("k", k_tex_coord);
    shaderProgram.setUniformValue("texture", 0);

    shaderProgram.setAttributeArray("texcoord", currentObject->texcoords.constData());
    shaderProgram.setAttributeArray("vert", currentObject->vertices.constData());
    shaderProgram.enableAttributeArray("texcoord");
    shaderProgram.enableAttributeArray("vert");

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, currentObject->vertices.size());

    shaderProgram.disableAttributeArray("vert");
    shaderProgram.disableAttributeArray("texcoord");
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
        while (beta < 0) {
            beta += 360;
        }
        while (beta >= 360) {
            beta -= 360;
        }
        /*
        if (beta < -90) {
            beta = -90;
        }
        if (beta > 90) {
            beta = 90;
        } */

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
