#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(width(), height());
    m_scene.reset(new Scene(width(), height(), this));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::notify()
{
    QApplication::postEvent(this, new QEvent(QEvent::Paint));
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    (void*) event;

    std::cout << "Repaint!" << std::endl;

    QPainter painter;

    painter.begin(this);
    m_scene->lock();
    for (const Ball& ball : m_scene->getBalls())
    {
        painter.drawEllipse(QPoint(ball.getX(), ball.getY()), ball.getRadius(), ball.getRadius());
    }
    m_scene->unlock();
    painter.end();
}
