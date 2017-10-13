#include "scene.h"

#include <math.h>
#include <time.h>
#include <algorithm>
#include <iostream>

namespace
{
    static const int initialBallsCount = 10;
    static const int ballRadius = 10;
    static const int noBall = -1;
    static const int updateTimeoutMS = 1000;
}

Scene::Scene(int width, int height, Listener* listener)
    : m_width(width), m_height(height), m_selectedBall(noBall), m_listener(listener), m_done(false)
{
    srand(static_cast<unsigned int>(time(nullptr)));

    for (int i = 0; i < initialBallsCount; i++)
    {
        int x = rand() % m_width;
        int y = rand() % m_height;
        m_balls.push_back(Ball(x, y, ballRadius));
    }

    std::thread thread([this]()
                       {
                           std::cout << "Update thread started!" << std::endl;
                           while (!m_done)
                           {
                               calculate();
                               if (m_listener != nullptr)
                               {
                                   m_listener->notify();
                               }
                               std::this_thread::sleep_for(std::chrono::milliseconds(updateTimeoutMS));
                           }
                           std::cout << "Update thread stopped!" << std::endl;
                       });

    m_updateThread.swap(thread);
}

Scene::~Scene()
{
    m_done = true;
    m_updateThread.join();
}

void Scene::addBall(int x, int y)
{
    Lock lock(m_mutex);
    m_balls.push_back(Ball(x, y, ballRadius));
}

void Scene::removeBall(int x, int y)
{
    Lock lock(m_mutex);
    auto it = getBallIt(x, y);
    if (it != m_balls.end())
    {
        m_balls.erase(it);
    }
}

void Scene::selectBall(int x, int y)
{
    Lock lock(m_mutex);
    auto it = getBallIt(x, y);
    m_selectedBall = it != m_balls.end() ? std::distance(m_balls.begin(), it) : noBall;
}

bool Scene::hasSelection()
{
    return m_selectedBall != noBall;
}

std::vector<Ball>::iterator Scene::getBallIt(int x, int y)
{
    return std::find_if(m_balls.begin(), m_balls.end(),
                        [x, y] (const Ball& ball)
                        {
                            int diffX = ball.getX() - x;
                            int diffY = ball.getY() - y;
                            int r = ball.getRadius();
                            return diffX * diffX + diffY * diffY <= r*r;
                        });
}

void Scene::calculate()
{
    Lock lock(m_mutex);
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
                f *= 100;
                // F = m * a;
                double ax = f * cos / ball1.getRadius(); // acceleration X
                double ay = f * sin / ball1.getRadius();  // acceleration Y
                ball1.addXSpeed(ax);
                ball1.addYSpeed(ay);
                std::cout << "F=" << f << " r=" << r << std::endl;
                std::cout << "ax=" << ax << " ay=" << ay << std::endl;
            }
        }
    }

    for (Ball& ball : m_balls)
    {
        ball.move();
    }
}
