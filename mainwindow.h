#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "scene.h"

#include <QMainWindow>
#include <QPainter>
#include <QTimer>
#include <QMouseEvent>
#include <QPoint>

#include <memory>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);

private:
    void updateCaption();

    Ui::MainWindow *ui;
    std::unique_ptr<Scene> m_scene;
    QTimer* m_timer;
    int m_ballsCount;
    bool m_drag;
    QPoint m_dragPos;
};

#endif // MAINWINDOW_H
