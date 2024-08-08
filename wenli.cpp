#include "wenli.h"
unsigned int VAO,VBO,EBO;
#include <QTime>

//float vertex[] = {
//    //positions    //colors       //texture
//    0.5f,0.5f,0.0f,1.0f,0.0f,0.0f,1.0f,1.0f,//top right
//    0.5f,-0.5f,0.0f,0.0f,1.0f,0.0f,1.0f,0.0f,//bottom right
//    -0.5f,-0.5f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,//bottom left
//    -0.5f,0.5f,0.0f,1.0f,1.0f,0.0f,0.0f,1.0f//top left
//};
float vertex[] = {
    //positions    //colors       //texture
    0.5f,0.5f,0.0f,1.0f,0.0f,0.0f,1.0f,1.0f,//top right
    0.5f,-0.5f,0.0f,0.0f,1.0f,0.0f,1.0f,0.0f,//bottom right
    -0.5f,-0.5f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,//bottom left
    -0.5f,0.5f,0.0f,1.0f,1.0f,0.0f,0.0f,1.0f//top left
};

float ratio = 0.5;

unsigned int indices[] = {
    0,1,3,//first trangle
    1,2,3//second trangle
};

wenLi::wenLi(QWidget *parent) : QOpenGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    timer = new QTimer(this);
    timer->start(100);
    connect(timer,&QTimer::timeout,this,[=](){
        update();
    });
}

wenLi::~wenLi()
{
    makeCurrent();
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);
    glDeleteVertexArrays(1,&VAO);
}

void wenLi::initializeGL()
{
    initializeOpenGLFunctions();
    //创建VAO与VBO对象
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);
    //绑定VAO与VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    //为当前绑定到target的缓冲区对象创建一个新的数据存储
    //如果data不是null，则使用来自此指针的数据初始化数据存储
    glBufferData(GL_ARRAY_BUFFER,sizeof (vertex),vertex,GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
    //告知显卡如何解析缓冲里的属性值
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof (float),(void*)0);
    //开启VAO的第一个属性值
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof (float),(void*)(3*sizeof (float)));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof (float),(void*)(6*sizeof (float)));

    glEnableVertexAttribArray(2);



    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,":/shaders.vert");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,":/shaders.frag");
    if(shaderProgram.link())
    {
        qDebug() << "链接成功";
    }

    //    glEnable(GL_BLEND);
    //    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_DST_ALPHA);

    textureWall = new QOpenGLTexture(QImage(":/image/11.png").mirrored());
    textureLe = new QOpenGLTexture(QImage(":/image/22.png").mirrored());
    shaderProgram.bind();
    shaderProgram.setUniformValue("ratio",ratio);
    shaderProgram.setUniformValue("textureWall",0);
    shaderProgram.setUniformValue("textureLe",1);


    textureLe->generateMipMaps();

    glBindVertexArray(0);

    textureWall->bind(0);
    textureLe->bind(1);
    //    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);//Y轴上下填充边框
    //    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);//X轴上下填充边框




    //    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINES);//放大小纹理贴大图,要采用线性,缩小的时候是大纹理贴小图,采用临近采样
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);



}

void wenLi::resizeGL(int w, int h)
{

}

void wenLi::paintGL()
{
    QMatrix4x4 matrix;
    unsigned int time = QTime::currentTime().msec();
    matrix.translate(0.5,-0.5,0);//矩阵向下移动

    matrix.rotate(time,0.0f,0.5f,1.0f);//旋转



    glClearColor(0.2f,0.3f,0.3f,1.0f);//设置清屏颜色（即清除颜色缓冲区时要用的颜色）告诉OpenGL，
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(VAO);
    //shaderProgram.bind();

    textureWall->bind(0);
    textureLe->bind(1);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);//上下不是镜像效果，代表Y轴
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);//

//    float borderColor[] = {1.0f,1.0f,0.0f,1.0f};
//    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);//Y轴上下填充边框
//    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);//X轴上下填充边框
//    glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,borderColor);
    shaderProgram.setUniformValue("RotationMatrix",matrix);//旋转
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,NULL);

    matrix.setToIdentity();
    matrix.translate(-0.5,0.5,0);
    //matrix.scale(sin(time));//会产生旋转；有负数
    matrix.scale(fabs(sin(time)));//会产生旋转；有负数
    shaderProgram.setUniformValue("RotationMatrix",matrix);
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,NULL);
}

void wenLi::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        ratio += 0.1;
        break;
    case Qt::Key_Down:
        ratio -= 0.1;
        break;
    default:
        break;

    }
    if(ratio > 1) ratio =1;
    if(ratio < 0) ratio =0;
    shaderProgram.bind();
    shaderProgram.setUniformValue("ratio",ratio);
    update();
}
