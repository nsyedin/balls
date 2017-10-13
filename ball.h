#ifndef BALL_H
#define BALL_H

class Ball
{
public:
    explicit Ball(int x, int y, int r) :
        m_x(x), m_y(y), m_r(r), m_xSpeed(0), m_ySpeed(0), m_selected(false)
    {
    }

    void addXSpeed(double xSpeed) { m_xSpeed += xSpeed; }

    void addYSpeed(double ySpeed) { m_ySpeed += ySpeed; }

    void move();

    bool getSelected() { return m_selected; }

    void setSelected(bool isSelected) { m_selected = isSelected; }

    void setCenter(int newX, int newY)
    {
        m_x = newX;
        m_y = newY;
    }

    int getX()
    {
        return m_x;
    }

    void setX(int x)
    {
        m_x = x;
    }

    int getY()
    {
        return m_y;
    }

    void setY(int y)
    {
        m_y = y;
    }

    int getRadius()
    {
        return m_r;
    }

    int getMass()
    {
        return m_r;
    }

private:
    int m_x;
    int m_y;
    int m_r;
    double m_xSpeed;
    double m_ySpeed;
    bool m_selected;
};

#endif // BALL_H
