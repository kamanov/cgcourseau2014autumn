#include "glwidget.h"
#include <QTimer>
#include <qmath.h>
#include <QGLBuffer>
#include <iostream>

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent)
{
    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(rotate()));
}

QSize GLWidget::sizeHint() const
{
return QSize(480, 480);
}

void GLWidget::setShaderProgramm(int i) {
    if (i >= 0 && i < 3) {
        shaderProgram = programms + i;
        updateGL();
    }
}

void GLWidget::rotate()
{
    rotationMatrix.rotate(1, QVector3D(0,0,1));
    updateGL();
}

GLWidget::~GLWidget() {
    deleteTexture(texture);
}

void GLWidget::initTextures()
{
    glEnable(GL_TEXTURE_2D);
    texture = bindTexture(QImage(":/chess.png"));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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
    qglClearColor(QColor(Qt::gray));
    initShaderProgramm(programms[0], ":/vertex.glsl", ":/fragment.glsl");
    initShaderProgramm(programms[1], ":/vertex_chess1.glsl", ":/fragment_chess.glsl");
    initShaderProgramm(programms[2], ":/vertex_chess2.glsl", ":/fragment_chess.glsl");
    initTextures();
    float h = 1/qSqrt(3);
    vertices << QVector3D(1.0f, -h, 0.0f) << QVector3D(-1.0f, -h, 0.0f) << QVector3D(0.0f, 2 * h, 0.0f);
    shaderProgram = &programms[0];
    timer.start(30);
}

void GLWidget::resizeGL(int width, int height)
{
    if (height == 0) {
        height = 1;
    }
    glViewport(0, 0, width, height);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    QMatrix4x4 scaleMatrix (
                            0.5, 0, 0, 0,
                            0, 0.5, 0, 0,
                            0, 0, 0.5, 0,
                            0, 0, 0, 1
                           );
    shaderProgram->bind();
    shaderProgram->setUniformValue("mvpMatrix", rotationMatrix * scaleMatrix);
    shaderProgram->setUniformValue("color", QColor(Qt::blue));
    shaderProgram->setUniformValue("texture", 0);
    shaderProgram->setAttributeArray("vert", vertices.constData());
    shaderProgram->enableAttributeArray("vert");
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    shaderProgram->disableAttributeArray("vert");
    shaderProgram->release();
}
