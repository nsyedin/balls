#ifndef SCENE_H
#define SCENE_H

#include "ball.h"

#include <vector>

class Scene
{
public:
    Scene(int width, int height);

    void addBall(int x, int y);
    void removeBall(int x, int y);

    const std::vector<Ball>& getBalls() const
    {
        return m_balls;
    }

private:
    std::vector<Ball> m_balls;
    int m_width;
    int m_heoght;
};

#endif // SCENE_H
