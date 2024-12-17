#pragma once
#include "Board.h"
#include "Item.h"
#include <vector>
class CGame
{
public: 
	void Start();
	
private:
	enum NeighboorType {NONE, BARREL, DONKEYKONG};
	enum MenuDecision { GAME_START, GAME_END };
	enum LiveStatus { DEAD, ALIVE, WON };
	static constexpr int ESC_KEY = 27;
	static constexpr int MARIO_LIVES = 3;
	static constexpr char AVATAR_MARIO = '@';
	static constexpr char AVATAR_BARREL = 'O';
	static constexpr char AVATAR_DONKEYKONG = '&';
	static constexpr int NUM_OF_BARRELS = 7;
	
	void PrintMenu(bool IsColored);
	MenuDecision GetMenuDecision();
	void PrintInstructions(bool wait = false);
	void PrintGoodbye();
	NeighboorType WhoSomeoneNextToMe(CPoint& point);
	void StartGame();
	void Init();
	bool CanMove(CItem& character, CItem::Directions);
	void PlayLoop();
	MenuDecision Paused();
	void PrintPauseMenu();
	LiveStatus PlayerCheckNextCell(CItem &character);
	LiveStatus CheckWhatIsBelow(CItem& character, bool OnLadder);
	bool m_IsColored = true;
	CItem m_mario;
	CItem m_donkeykong;
	CItem m_barrels;
	void ExplosionAnimation(int x, int y, CItem& mario);
	CBoard m_board;
};


void GameOverScreen();




