#include "scene.h"

#include <math.h>
#include <time.h>
#include <algorithm>
#include <iostream>
#include <thread>

namespace
{
    static const int ballRadius = 10;
    static const int noBall = -1;
}

Scene::Scene(int width, int height, int ballsCount)
    : m_width(width), m_height(height), m_selected(noBall), m_done(false)
{
    srand(static_cast<unsigned int>(time(nullptr)));

    for (int i = 0; i < ballsCount; i++)
    {
        int x = rand() % m_width;
        int y = rand() % m_height;
        m_balls.push_back(Ball(x, y, ballRadius));
    }

    std::thread tmp(&Scene::calculate, this);
    m_updateThread.swap(tmp);
}

Scene::~Scene()
{
    m_done = true;
    update();
    m_updateThread.join();
}

void Scene::add(int x, int y)
{
    Lock lock(m_mutex);
    m_balls.push_back(Ball(x, y, ballRadius));
}

bool Scene::remove(int x, int y)
{
    Lock lock(m_mutex);
    auto it = getBallIt(x, y);
    if (it != m_balls.end())
    {
        m_balls.erase(it);
        return true;
    }

    return false;
}

Ball* Scene::select(int x, int y)
{
    Lock lock(m_mutex);
    auto it = getBallIt(x, y);
    m_selected = it != m_balls.end() ? std::distance(m_balls.begin(), it) : noBall;
    return getBall();
}

Ball* Scene::getSelected()
{
    Lock lock(m_mutex);
    return getBall();
}

void Scene::moveSelected(int dx, int dy)
{
    Lock lock(m_mutex);
    Ball *ball = getBall();
    if (ball != nullptr)
    {
        int x = ball->getX();
        int y = ball->getY();
        ball->setCenter(x + dx, y + dy);
    }
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

Ball* Scene::getBall()
{
    return m_selected != noBall ? &m_balls[m_selected] : nullptr;
}

void Scene::update()
{
    m_isNotified = true;
    m_condVar.notify_one();
}

void Scene::calculate()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    while (!m_done)
    {
        while (!m_isNotified)
        {
            m_condVar.wait(lock);
        }
        m_isNotified = false;

        auto start = std::chrono::system_clock::now();
        for (Ball& ball1 : m_balls)
        {
            if (ball1.isLocked())
            {
                continue;
            }
            for (const Ball& ball2 : m_balls)
            {
                if (ball2.isLocked())
                {
                    continue;
                }
                if (&ball1 != &ball2)
                {
                    int dx = ball2.getX() - ball1.getX();
                    int dy = ball2.getY() - ball1.getY();
                    double c = sqrt(dx * dx + dy * dy);
                    double r = c - ball1.getRadius() - ball2.getRadius();
                    double f = 1 / r - 1 / (r * r);
                    if (f > 0.000001) // greater 0
                    {
                        double a = f / ball1.getRadius();
                        double cos = dy / c;
                        double sin = dx / c;
                        double ax = a * cos;
                        double ay = a * sin;
                        ball1.addXAcceleration(ax);
                        ball1.addYAcceleration(ay);
                        //std::cout << "F=" << f << " r=" << r << std::endl;
                        //std::cout << "ax=" << ax << " ay=" << ay << std::endl;
                    }
                }
            }
        }

        for (Ball& ball : m_balls)
        {
            ball.move();
        }

        auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);
        std::cout << "Calc=" << diff.count() << "ms" << std::endl;
    }
}
