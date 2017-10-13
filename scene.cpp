#include "scene.h"

#include <math.h>

namespace
{
    static const int initialBallsCount = 10;
    static const int ballRadius = 10;
}

Scene::Scene()
    : m_width(width), m_height(height)
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
    Lock lock;
    m_balls.push_back(Ball(x, y, ballRadius));
}


void Scene::removeBall(int x, int y)
{
    Lock lock;
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

void Scene::calculate()
{
    Lock lock;
    for (Ball& ball1 : m_balls)
    {
        for (const Ball& ball2 : m_balls)
        {
            if (&ball1 != &ball2)
            {
                int dx = ball2.getX() - ball1.getX();
                int dy = ball2.getY() - ball1.getY();
                double c = sqrt(dx * dx + dy * dy);
                double r = c - ball1.getRadius() - ball2.getRadius();
                double cos = dy / c;
                double sin = dx /c;
                double f = 1 / r - 1 / (r * r);
                // F = m * a;
                double ax = f * cos / ball1.getRadius(); // acceleration X
                double ay = f * sin / ball1.getRadius();  // acceleration Y
                ball1.addXSpeed(ax);
                ball1.addYSpeed(ay);
            }
        }
    }

    for (Ball& ball : m_balls)
    {
        ball.move();
    }
}
