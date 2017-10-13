#include "scene.h"

namespace
{
    static const int initialBallsCount = 10;
    static const int ballRadius = 10;
}

Scene::Scene()
    : m_width(width), m_heoght(height)
{
    for (int i = 0; i < initialBallsCount; i++)
    {
        int x;
        int y;
        m_balls.push_back(Ball(x, y, ballRadius));
    }
}

void Scene::addBall(int x, int y)
{
    m_balls.push_back(Ball(x, y, ballRadius));
}


void Scene::removeBall(int x, int y)
{
    auto it = std::find_if(m_balls.begin(), m_balls.end(),
                           [x, y] (const Ball& ball)
                           {
                               int diffX = ball.getX() - x;
                               int diffY = ball.getY() - y;
                               int r = ball.getRadius();
                               return diffX * diffX + diffY * diffY <= r*r;
                           });

    if (it != m_balls.end())
    {
        m_balls.erase(it);
    }
}
