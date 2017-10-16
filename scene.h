#ifndef SCENE_H
#define SCENE_H

#include "ball.h"

#include <vector>
#include <mutex>
#include <thread>

class Scene
{
public:
    explicit Scene(int width, int height);
    ~Scene() = default;

    void add(int x, int y);
    void remove(int x, int y);
    Ball* select(int x, int y);
    Ball* getSelected();
    void moveSelected(int dx, int dy);

    void lock() { m_mutex.lock(); }
    void unlock() { m_mutex.unlock(); }

    const std::vector<Ball>& getBalls() const
    {
        return m_balls;
    }

    void calculate();

private:
    typedef std::lock_guard<std::mutex> Lock;

    std::vector<Ball>::iterator getBallIt(int x, int y);

    std::vector<Ball> m_balls;
    int m_selected;

    int m_width;
    int m_height;

    std::mutex m_mutex;
};

#endif // SCENE_H
