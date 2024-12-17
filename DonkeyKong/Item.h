#pragma once
#include "ColorPoint.h"
#include "Utils.h"

class CPoint
{
protected:
    int m_x = 0; //0 is x and 1 is y 
    int m_y = 0;

public:
    CPoint(int X = 0, int Y = 0) : m_x(X), m_y(Y) {}
    void SetX(int X) { m_x = X; }
    void SetY(int Y) { m_y = Y; }
    int GetX() const { return m_x; }
    int GetY() const { return m_y; }
};

class CItem : public CPoint
{
public:
    enum Directions { STOP, RIGHT, LEFT, UP, DOWN };

    CItem(int x = 0, int y = 0, char c = ' ', CColorPoint::c_color color = CColorPoint::c_color::WHITE, int MaxFall = 5)
        : CPoint(x, y), m_symbol(c), m_color(color), m_Xdirection(0), m_Ydirection(0), m_MaxFall(MaxFall) {
        m_direction = STOP;
    }

    // Added GetDirection method
    Directions GetDirection() const { return m_direction; }

    char GetSymbol() const { return m_symbol; }
    int GetXDirection() const { return m_Xdirection; }
    int GetYDirection() const { return m_Ydirection; }
    bool IsStop() const { return (m_direction == STOP); }
    void SetDirection(enum Directions direction) {
        m_direction = direction;
        switch (direction) {
        case STOP:
            m_Xdirection = 0;
            m_Ydirection = 0;
            break;
        case RIGHT:
            m_Xdirection = 1;
            m_Ydirection = 0;
            break;
        case LEFT:
            m_Xdirection = -1;
            m_Ydirection = 0;
            break;
        case UP:
            m_Ydirection = -1;
            break;
        case DOWN:
            m_Xdirection = 0;
            m_Ydirection = 1;
            break;
        }
    }

    void SeRestoreSymbol(char ch, CColorPoint::c_color color) { m_RestoreSymbol = ch; m_RestoreColor = color; }
    CColorPoint::c_color GetColor() const { return m_color; }
    void ReduceLife() { m_lives--; }
    int GetLives() const { return m_lives; }
    void SetLives(int lives) { m_lives = lives; }
    void Draw() { _draw(m_symbol, m_color); }
    void Erase() { _draw(m_RestoreSymbol, m_RestoreColor); }
    char GetNextSymbol() const { return m_RestoreSymbol; }
    void Falling() { m_FallCounter++; }
    int GetFallCounter() const { return m_FallCounter; }
    int GetMaxFall() const { return m_MaxFall; }
    void ResetFalling() { m_FallCounter = 0; }

private:
    char m_symbol;
    CColorPoint::c_color m_color;
    int m_Xdirection, m_Ydirection;
    int m_FallCounter = 0;
    int m_MaxFall;
    enum Directions m_direction;
    char m_RestoreSymbol = ' ';
    CColorPoint::c_color m_RestoreColor = CColoredPrint::c_color::WHITE;
    int m_lives = 1;

    void _draw(char ch, CColoredPrint::c_color color) {
        GoToXY(m_x, m_y);
        CColoredPrint::pr(ch, color, CColoredPrint::c_decoration::BOLD);
    };
};

class CMovingItem : public CItem
{

};
