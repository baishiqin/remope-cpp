#ifndef WENLI_H
#define WENLI_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_0_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QKeyEvent>
#include <QTimer>

class wenLi : public QOpenGLWidget,QOpenGLFunctions_4_0_Core
{
    Q_OBJECT
    enum Shape{None,Rect,Circle,Triangle};
public:
    explicit wenLi(QWidget *parent = nullptr);
    ~wenLi();

protected:
    virtual void initializeGL()override;
    virtual void resizeGL(int w,int h)override;
    virtual void paintGL() override;
    virtual void keyPressEvent(QKeyEvent* event)override;

signals:
private:
    QOpenGLShaderProgram shaderProgram;
    QOpenGLTexture* textureWall;
    QOpenGLTexture* textureLe;
    Shape m_shape;
    QTimer* timer;

};

#endif // WENLI_H
