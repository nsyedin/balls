#ifndef SCENE_H
#define SCENE_H

#include "ball.h"

#include <vector>
#include <mutex>
#include <thread>

class Scene
{
public:

    class Listener
    {
    public:
        virtual ~Listener() {}
        virtual void notify() = 0;
    };

    explicit Scene(int width, int height, Listener* listener);
    ~Scene();

    void addBall(int x, int y);
    void removeBall(int x, int y);
    void selectBall(int x, int y);
    bool hasSelection();

    void lock() { m_mutex.lock(); }
    void unlock() { m_mutex.unlock(); }

    const std::vector<Ball>& getBalls() const
    {
        return m_balls;
    }

private:
    typedef std::lock_guard<std::mutex> Lock;

    void calculate();

    std::vector<Ball>::iterator getBallIt(int x, int y);

    std::vector<Ball> m_balls;
    int m_selectedBall;

    int m_width;
    int m_height;

    std::mutex m_mutex;
    std::thread m_updateThread;
    bool m_done;

    Listener* m_listener;
};

#endif // SCENE_H
