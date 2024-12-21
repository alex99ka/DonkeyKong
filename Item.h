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
	void SetCoord(int X, int Y) { m_x = X;  m_y = Y;};
	int GetX() const { return m_x; }
	int GetY() const { return m_y; }
	bool Compare(CPoint& other) { 
		return (m_x == other.GetX() && m_y == other.GetY());
	};

	bool operator==(const CPoint& other) const {
		return (m_x == other.GetX() && m_y == other.GetY());
	}
	
};

class CItem : public CPoint
{
public:
	
	CItem(int x = 0, int y = 0, char c = ' ', CColorPoint::c_color color = CColorPoint::c_color::WHITE)
		: CPoint(x, y), m_symbol(c), m_color(color) { }

	char GetSymbol() { return m_symbol; }
	void SetRestoreSymbol(char ch, CColorPoint::c_color color) { m_RestoreSymbol = ch; m_RestoreColor = color; }
	CColorPoint::c_color GetColor() { return m_color; }
	void SetColor(CColorPoint::c_color color) { m_color = m_color; }
	
	void Draw() { _draw(m_symbol, m_color); }
	void Erase() { _draw(m_RestoreSymbol, m_RestoreColor); }
	char GetNextSymbol() { return m_RestoreSymbol; }
	void ChangeColor(CColorPoint::c_color NewColor) { m_color = NewColor; }

private:
	char m_symbol;
	CColorPoint::c_color m_color;
	char m_RestoreSymbol = ' ';
	CColorPoint::c_color m_RestoreColor = CColoredPrint::c_color::WHITE;

	void _draw(char ch, CColoredPrint::c_color color) {
		GoToXY(m_x, m_y);
		CColoredPrint::pr(ch, color, CColoredPrint::c_decoration::BOLD);
	};
};

class CMovingItem : public CItem
{
public:

	enum Directions { STOP, RIGHT, LEFT, UP, DOWN };

	CMovingItem(int x = 0, int y = 0, char c = ' ', CColorPoint::c_color color = CColorPoint::c_color::WHITE, int MaxFall = 5)
		:CItem(x, y, c, color), m_Xdirection(0), m_Ydirection(0), m_MaxFall(MaxFall) {
		m_direction = STOP;
		m_prevDirection = STOP;
	}

	int GetXDirection() { return m_Xdirection; }
	int GetYDirection()	{ return m_Ydirection; }
	enum Directions GetDirection() { return m_direction; };
	enum Directions GetPrevDirection() { return m_prevDirection; };
	bool IsStop() { return (m_direction == STOP); }
	void SetPrevDirection(Directions direction) { m_prevDirection = direction; }
	void SetDirection(Directions direction) {
		if (m_FallCounter > 0) // we don't change direction during falling the character
			return;
		m_direction = direction;
		switch (direction)
		{
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
			m_Ydirection = -2;
			break;
		case DOWN:
			m_Xdirection = 0;
			m_Ydirection = 1;
			break;
		}
	}
	void ReduceLife() { m_lives--; }
	int GetLives() { return m_lives; }
	void SetLives(int lives) { m_lives = lives; }

	void Falling() { m_FallCounter++; }
	void ResetFalls() { m_FallCounter = 0; };
	int GetFallCounter() { return m_FallCounter; }
	int GetMaxFall() { return m_MaxFall; }

private:
	int m_Xdirection, m_Ydirection;
	int m_FallCounter = 0;
	int m_MaxFall;
	enum Directions m_direction;
	enum Directions m_prevDirection;
	int m_lives = 1;
};

