#include "ball.h"

#include <math.h>

void Ball::move()
{
    m_x += static_cast<int>(round(m_xSpeed));
    m_y += static_cast<int>(round(m_ySpeed));
}
