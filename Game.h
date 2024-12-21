#pragma once
#include "Board.h"
#include "Item.h"
#include <vector>
class CGame
{
public: 
	void Start();
private:
	enum NeighboorType {NONE, BARREL, DONKEYKONG, PRINCESS};
	enum MenuDecision { GAME_START, GAME_END };
	enum LiveStatus { DEAD, ALIVE, WON };
    enum ScreenType { MAIN_MENU, PAUSE_MENU };
	
	static constexpr int ESC_KEY = 27;
	static constexpr int MARIO_LIVES = 3;
	static constexpr char AVATAR_MARIO = '@';
	static constexpr char AVATAR_BARREL = 'O';
	static constexpr char AVATAR_DONKEYKONG = '&';
	static constexpr char AVATAR_PRINCESS = '$';
	static constexpr int NUM_OF_BARRELS = 7;
	static constexpr int SLEEP_TIME = 100;
	static constexpr int MAX_NUM_BARRELS = 10;
	static constexpr int BARREL_FREQUENCY_BIRTH = 25;
	static constexpr int MAX_FALL_BARREL = 8;
	static constexpr char EXPL_SYMB = '*';
	static constexpr char FLOOR_SYMB = '=';

	void StartGame();
	void Init();
	void PlayLoop();
	MenuDecision Paused();
	void ResetScreen();

	void PrintMenu();
	MenuDecision GetMenuDecision();
	void PrintInstructions(ScreenType type);
	void PrintGoodbye();
	void PrintCongratulation();
	void PrintPauseMenu();
	bool GameOver();
	void GameOverScreen();
	void CharacterDeathAnimation(CMovingItem& character);
	void DrawHearts();
	void CreatePrincess();
	
	CGame::LiveStatus AddBarrel();
	LiveStatus BarrelMoving(CMovingItem& barrel);
	LiveStatus BarrelsMoving();
	bool BarrelFlowCollision(CMovingItem& barrel, CMovingItem::Directions direction, CPoint& newPos);
	void ResetBarrel(CMovingItem& barrel);
	CGame::LiveStatus ExplosionBarrel(CMovingItem& barrel);
	bool IsHitPlayer(CMovingItem& barrel);

	LiveStatus PlayerCheckNextCell(CMovingItem& character);
	CGame::LiveStatus MovePlayer(CMovingItem& character, CPoint& newPos);
	NeighboorType WhoSomeoneNextToMe(CPoint& point);
	void FallCharacter(CMovingItem& character);
	void ResetPlayer();
	void EraseCharacter(CMovingItem& character);
	
	bool m_DonkeyIsDead = false;
	int m_nBarrels = MAX_NUM_BARRELS;
	bool m_IsColored = true;
	CMovingItem m_mario;
	CItem m_donkeykong;
	CItem m_princess;
	std::vector<CMovingItem> m_barrels;
	CBoard m_board;
};






