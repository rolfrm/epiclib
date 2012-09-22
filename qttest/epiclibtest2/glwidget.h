#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

class glwidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit glwidget(QWidget *parent = 0);
    void paintGL();
    
signals:
    
public slots:
    
};

#endif // GLWIDGET_H
