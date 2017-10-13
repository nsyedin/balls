#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "scene.h"

#include <QMainWindow>
#include <QPainter>

#include <memory>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public Scene::Listener
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    virtual void notify();

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::MainWindow *ui;
    std::unique_ptr<Scene> m_scene;
};

#endif // MAINWINDOW_H
