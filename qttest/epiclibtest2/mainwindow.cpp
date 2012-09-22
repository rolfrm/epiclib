#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "glwidget.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    glwidget * glwid = new glwidget(ui->frame);
    glwid->setParent(ui->frame);

}

MainWindow::~MainWindow()
{
    delete ui;
}
