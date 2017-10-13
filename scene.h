#ifndef SCENE_H
#define SCENE_H

#include "ball.h"

#include <vector>
#include <mutex>

class Scene
{
public:
    Scene(int width, int height);

    void addBall(int x, int y);
    void removeBall(int x, int y);
    void calculate();

    const std::vector<Ball>& getBalls() const
    {
        return m_balls;
    }

private:
    typedef std::lock_guard<std::mutex> Lock;

    std::vector<Ball> m_balls;
    int m_width;
    int m_height;
    std::mutex m_mutex;
};

#endif // SCENE_H
