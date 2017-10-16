#include "ball.h"

#include <math.h>

void Ball::move()
{
    m_x += static_cast<int>(round(m_xSpeed));
    m_y += static_cast<int>(round(m_ySpeed));
}

void Ball::lock()
{
    m_isLocked = true;
    m_xSpeed = 0;
    m_ySpeed = 0;
}

void Ball::unlock()
{
    m_isLocked = false;
}
