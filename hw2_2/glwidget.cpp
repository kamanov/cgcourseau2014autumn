#include "glwidget.h"
#include <qmath.h>
#include <QGLBuffer>
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
    , distance(3)
    , lightAngle(0.0)
    , lightDistance(1.0)
    , ambient(1.0)
    , diffuse(1.0)
    , specular(1.0)
    , ambientColor(32)
    , diffuseColor(128)
    , specularColor(255)
{
}



QSize GLWidget::sizeHint() const
{
    return QSize(640, 480);
}



void GLWidget::initTextures()
{
    glActiveTexture(GL_TEXTURE0);
    texture = bindTexture(QPixmap(":/brickwork-texture.jpg"));
    initTexture();

    glActiveTexture(GL_TEXTURE1);
    normTexture = bindTexture(QPixmap(":/brickwork_normal-map.jpg"));
    initTexture();
}

void GLWidget::initTexture()
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
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
    initializeGLFunctions();
    glEnable(GL_DEPTH_TEST);
    qglClearColor(QColor(Qt::gray));
    initShaderProgramm(lightProgramm, ":/vertex_light.glsl", ":/fragment_light.glsl");
    initShaderProgramm(lightSourceProgramm, ":/vertex_light_source.glsl", ":/fragment_light_source.glsl");
    initTextures();

    cube.vertices << QVector3D(-0.5, -0.5, 0.5) << QVector3D( 0.5, -0.5, 0.5) << QVector3D( 0.5, 0.5, 0.5)
    << QVector3D( 0.5, 0.5, 0.5) << QVector3D(-0.5, 0.5, 0.5) << QVector3D(-0.5, -0.5, 0.5)
    << QVector3D( 0.5, -0.5, -0.5) << QVector3D(-0.5, -0.5, -0.5) << QVector3D(-0.5, 0.5, -0.5)
    << QVector3D(-0.5, 0.5, -0.5) << QVector3D( 0.5, 0.5, -0.5) << QVector3D( 0.5, -0.5, -0.5)
    << QVector3D(-0.5, -0.5, -0.5) << QVector3D(-0.5, -0.5, 0.5) << QVector3D(-0.5, 0.5, 0.5)
    << QVector3D(-0.5, 0.5, 0.5) << QVector3D(-0.5, 0.5, -0.5) << QVector3D(-0.5, -0.5, -0.5)
    << QVector3D( 0.5, -0.5, 0.5) << QVector3D( 0.5, -0.5, -0.5) << QVector3D( 0.5, 0.5, -0.5)
    << QVector3D( 0.5, 0.5, -0.5) << QVector3D( 0.5, 0.5, 0.5) << QVector3D( 0.5, -0.5, 0.5)
    << QVector3D(-0.5, 0.5, 0.5) << QVector3D( 0.5, 0.5, 0.5) << QVector3D( 0.5, 0.5, -0.5)
    << QVector3D( 0.5, 0.5, -0.5) << QVector3D(-0.5, 0.5, -0.5) << QVector3D(-0.5, 0.5, 0.5)
    << QVector3D(-0.5, -0.5, -0.5) << QVector3D( 0.5, -0.5, -0.5) << QVector3D( 0.5, -0.5, 0.5)
    << QVector3D( 0.5, -0.5, 0.5) << QVector3D(-0.5, -0.5, 0.5) << QVector3D(-0.5, -0.5, -0.5);

    cube.normals << QVector3D( 0, 0, 1) << QVector3D( 0, 0, 1) << QVector3D( 0, 0, 1)
    << QVector3D( 0, 0, 1) << QVector3D( 0, 0, 1) << QVector3D( 0, 0, 1)
    << QVector3D( 0, 0, -1) << QVector3D( 0, 0, -1) << QVector3D( 0, 0, -1)
    << QVector3D( 0, 0, -1) << QVector3D( 0, 0, -1) << QVector3D( 0, 0, -1)
    << QVector3D(-1, 0, 0) << QVector3D(-1, 0, 0) << QVector3D(-1, 0, 0)
    << QVector3D(-1, 0, 0) << QVector3D(-1, 0, 0) << QVector3D(-1, 0, 0)
    << QVector3D( 1, 0, 0) << QVector3D( 1, 0, 0) << QVector3D( 1, 0, 0)
    << QVector3D( 1, 0, 0) << QVector3D( 1, 0, 0) << QVector3D( 1, 0, 0)
    << QVector3D( 0, 1, 0) << QVector3D( 0, 1, 0) << QVector3D( 0, 1, 0)
    << QVector3D( 0, 1, 0) << QVector3D( 0, 1, 0) << QVector3D( 0, 1, 0)
    << QVector3D( 0, -1, 0) << QVector3D( 0, -1, 0) << QVector3D( 0, -1, 0)
    << QVector3D( 0, -1, 0) << QVector3D( 0, -1, 0) << QVector3D( 0, -1, 0);

    cube.texcoords << QVector2D(0, 0) << QVector2D(1, 0) << QVector2D(1, 1)
    << QVector2D(1, 1) << QVector2D(0, 1) << QVector2D(0, 0)
    << QVector2D(0, 0) << QVector2D(1, 0) << QVector2D(1, 1)
    << QVector2D(1, 1) << QVector2D(0, 1) << QVector2D(0, 0)
    << QVector2D(0, 0) << QVector2D(1, 0) << QVector2D(1, 1)
    << QVector2D(1, 1) << QVector2D(0, 1) << QVector2D(0, 0)
    << QVector2D(0, 0) << QVector2D(1, 0) << QVector2D(1, 1)
    << QVector2D(1, 1) << QVector2D(0, 1) << QVector2D(0, 0)
    << QVector2D(0, 0) << QVector2D(1, 0) << QVector2D(1, 1)
    << QVector2D(1, 1) << QVector2D(0, 1) << QVector2D(0, 0)
    << QVector2D(0, 0) << QVector2D(1, 0) << QVector2D(1, 1)
    << QVector2D(1, 1) << QVector2D(0, 1) << QVector2D(0, 0);

    lsVertices << QVector3D( 0, 1, 0) << QVector3D(-0.5, 0, 0.5) << QVector3D( 0.5, 0, 0.5)
    << QVector3D( 0, 1, 0) << QVector3D( 0.5, 0, -0.5) << QVector3D(-0.5, 0, -0.5)
    << QVector3D( 0, 1, 0) << QVector3D(-0.5, 0, -0.5) << QVector3D(-0.5, 0, 0.5)
    << QVector3D( 0, 1, 0) << QVector3D( 0.5, 0, 0.5) << QVector3D( 0.5, 0, -0.5)
    << QVector3D(-0.5, 0, -0.5) << QVector3D( 0.5, 0, -0.5) << QVector3D( 0.5, 0, 0.5)
    << QVector3D( 0.5, 0, 0.5) << QVector3D(-0.5, 0, 0.5) << QVector3D(-0.5, 0, -0.5);

    lsColors << QVector3D(0.2, 0.2, 0.2) << QVector3D(0.2, 0.2, 0.2) << QVector3D(0.2, 0.2, 0.2)
    << QVector3D(0.2, 0.2, 0.2) << QVector3D(0.2, 0.2, 0.2) << QVector3D(0.2, 0.2, 0.2)
    << QVector3D(0.2, 0.2, 0.2) << QVector3D(0.2, 0.2, 0.2) << QVector3D(0.2, 0.2, 0.2)
    << QVector3D(0.2, 0.2, 0.2) << QVector3D(0.2, 0.2, 0.2) << QVector3D(0.2, 0.2, 0.2)
    << QVector3D( 1, 1, 1) << QVector3D( 1, 1, 1) << QVector3D( 1, 1, 1)
    << QVector3D( 1, 1, 1) << QVector3D( 1, 1, 1) << QVector3D( 1, 1, 1);
    initTangentAndBitangents();
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

    mMatrix.setToIdentity();
    vMatrix.setToIdentity();
    QMatrix4x4 cameraTransformation;
    cameraTransformation.rotate(alpha, 0, 1, 0);
    cameraTransformation.rotate(beta, 1, 0, 0);
    QVector3D cameraPosition = cameraTransformation * QVector3D(0, 0, distance);
    QVector3D cameraUpDirection = cameraTransformation * QVector3D(0, 1, 0);
    vMatrix.lookAt(cameraPosition, QVector3D(0, 0, 0), cameraUpDirection);
    QMatrix4x4 mvMatrix;
    mvMatrix = vMatrix * mMatrix;
    QMatrix3x3 normalMatrix;
    normalMatrix = mvMatrix.normalMatrix();
    QMatrix4x4 lightTransformation;
    lightTransformation.rotate(lightAngle, 1, 0, 0);
    QVector3D lightPosition = lightTransformation * QVector3D(0, 0, lightDistance);

    lightProgramm.bind();
    lightProgramm.setUniformValue("mvpMatrix", pMatrix * vMatrix * mMatrix);
    lightProgramm.setUniformValue("texture", 0);
    lightProgramm.setUniformValue("normTexture", 1);
    lightProgramm.setUniformValue("mvMatrix", mvMatrix);
    lightProgramm.setUniformValue("normalMatrix", normalMatrix);
    lightProgramm.setUniformValue("lightPosition", vMatrix * lightPosition);
    lightProgramm.setUniformValue("ambientColor", QColor(ambientColor, ambientColor, ambientColor));
    lightProgramm.setUniformValue("diffuseColor", QColor(diffuseColor, diffuseColor, diffuseColor));
    lightProgramm.setUniformValue("specularColor", QColor(specularColor, specularColor, specularColor));
    lightProgramm.setUniformValue("ambientReflection", (GLfloat) ambient);
    lightProgramm.setUniformValue("diffuseReflection", (GLfloat) diffuse);
    lightProgramm.setUniformValue("specularReflection", (GLfloat) specular);


    lightProgramm.setAttributeArray("tangents", tangents.constData());
    lightProgramm.enableAttributeArray("tangents");
    lightProgramm.setAttributeArray("bitangents", bitangents.constData());
    lightProgramm.enableAttributeArray("bitangents");
    lightProgramm.setAttributeArray("texcoord", cube.texcoords.constData());
    lightProgramm.enableAttributeArray("texcoord");
    lightProgramm.setAttributeArray("vert", cube.vertices.constData());
    lightProgramm.enableAttributeArray("vert");
    lightProgramm.setAttributeArray("norm", cube.normals.constData());
    lightProgramm.enableAttributeArray("norm");

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, cube.vertices.size());

    lightProgramm.disableAttributeArray("vert");
    lightProgramm.disableAttributeArray("texcoord");
    lightProgramm.disableAttributeArray("norm");
    lightProgramm.disableAttributeArray("tangents");
    lightProgramm.disableAttributeArray("bitangents");
    lightProgramm.release();


    mMatrix.setToIdentity();
    mMatrix.translate(lightPosition);
    mMatrix.rotate(lightAngle, 1, 0, 0);
    mMatrix.rotate(90, 1, 0, 0);
    mMatrix.scale(0.05);
    lightSourceProgramm.bind();
    lightSourceProgramm.setUniformValue("mvpMatrix", pMatrix * vMatrix * mMatrix);
    lightSourceProgramm.setAttributeArray("vert", lsVertices.constData());
    lightSourceProgramm.enableAttributeArray("vert");
    lightSourceProgramm.setAttributeArray("color", lsColors.constData());
    lightSourceProgramm.enableAttributeArray("color");

    glDrawArrays(GL_TRIANGLES, 0, lsVertices.size());
    lightSourceProgramm.disableAttributeArray("vert");
    lightSourceProgramm.disableAttributeArray("color");
    lightSourceProgramm.release();

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

void GLWidget::initTangentAndBitangents()
{
    for (unsigned int i = 0; i < cube.vertices.size(); i += 3 ) {
        QVector3D & v0 = cube.vertices[i + 0];
        QVector3D & v1 = cube.vertices[i + 1];
        QVector3D & v2 = cube.vertices[i + 2];
        QVector2D & uv0 = cube.texcoords[i + 0];
        QVector2D & uv1 = cube.texcoords[i + 1];
        QVector2D & uv2 = cube.texcoords[i + 2];

        QVector3D deltaPos1 = v1 - v0;
        QVector3D deltaPos2 = v2 - v0;
        QVector2D deltaUV1 = uv1 - uv0;
        QVector2D deltaUV2 = uv2 - uv0;
        float r = 1.0f / (deltaUV1.x() * deltaUV2.y() - deltaUV1.y() * deltaUV2.x());
        QVector3D tangent = (deltaPos1 * deltaUV2.y()   - deltaPos2 * deltaUV1.y()) * r;
        QVector3D bitangent = (deltaPos2 * deltaUV1.x()   - deltaPos1 * deltaUV2.x()) * r;

        tangents << tangent << tangent << tangent;
        bitangents << bitangent << bitangent << bitangent;

    }

}
