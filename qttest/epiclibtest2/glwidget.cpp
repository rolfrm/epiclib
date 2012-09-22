#include "glwidget.h"
#include <iostream>
glwidget::glwidget(QWidget *parent) :
    QGLWidget(parent)
{
    setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);

}

void glwidget::paintGL(){

    QSize siz = size();
    glViewport(0,0,siz.width(),siz.height());
    std::cout << siz.width() << " " << siz.height() << "\n";
    glClearColor(1,1,0,1);
    glClear(GL_COLOR_BUFFER_BIT);


}
