#ifndef SCENE_H
#define SCENE_H

#include "ball.h"

#include <vector>
#include <mutex>
#include <thread>
#include <condition_variable>

class Scene
{
public:
    explicit Scene(int width, int height, int ballsCount);
    ~Scene();

    void add(int x, int y);
    bool remove(int x, int y);
    Ball* select(int x, int y);
    Ball* getSelected();
    void moveSelected(int dx, int dy);

    void lock() { m_mutex.lock(); }
    void unlock() { m_mutex.unlock(); }

    const std::vector<Ball>& getBalls() const
    {
        return m_balls;
    }

    void update();

private:
    typedef std::lock_guard<std::mutex> Lock;

    void calculate();
    std::vector<Ball>::iterator getBallIt(int x, int y);
    Ball* getBall();

    std::vector<Ball> m_balls;
    int m_selected;

    int m_width;
    int m_height;

    std::mutex m_mutex;
    std::thread m_updateThread;
    std::condition_variable m_condVar;
    bool m_done;
    bool m_isNotified;
};

#endif // SCENE_H
