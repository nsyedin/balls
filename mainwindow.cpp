#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

namespace
{
    const int initialBallsCount = 10;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_ballsCount(initialBallsCount),
    m_drag(false)
{
    ui->setupUi(this);
    setFixedSize(width(), height());
    m_scene.reset(new Scene(width(), height(), m_ballsCount));
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(repaint()));
    m_timer->start(20);
    updateCaption();
}

MainWindow::~MainWindow()
{
    m_timer->stop();
    delete ui;
}

void MainWindow::updateCaption()
{
    setWindowTitle(QString("Balls ") + QString::number(m_ballsCount));
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MouseButton::RightButton)
    {
        m_scene->add(event->x(), event->y());
        m_ballsCount++;
        updateCaption();
    }
    else if (event->button() == Qt::MouseButton::LeftButton &&
             m_scene->select(event->x(), event->y()) != nullptr)
    {
        std::cout << "Select!" << std::endl;
        m_scene->getSelected()->lock();
        m_drag = true;
        m_dragPos = event->pos();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    if (m_drag)
    {
        std::cout << "Move!" << std::endl;
        int dx = event->x() - m_dragPos.x();
        int dy = event->y() - m_dragPos.y();
        m_scene->moveSelected( dx, dy);
        m_dragPos = event->pos();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    (void*) event;

    if (m_drag)
    {
        m_scene->getSelected()->unlock();
        m_drag = false;
    }
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent* event)
{
    m_scene->remove(event->x(), event->y());
    m_ballsCount--;
    updateCaption();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    (void*) event;

    std::cout << "Repaint!" << std::endl;

    QPainter painter;
    painter.begin(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::BrushStyle::SolidPattern);
    painter.setPen(Qt::PenStyle::NoPen);

    m_scene->lock();
    for (const Ball& ball : m_scene->getBalls())
    {
        painter.drawEllipse(QPoint(ball.getX(), ball.getY()), ball.getRadius(), ball.getRadius());
    }
    m_scene->unlock();

    m_scene->update();

    painter.end();
}
