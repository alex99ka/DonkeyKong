#include<iostream>
#include<windows.h> // sleep and other stuff
#include <conio.h> // For _kbhit() and _getch and go to 
#include "game.h"
#include "ColorPoint.h"
#include "utils.h"



void CGame::Start()
{
	PrintMenu(true);
	CGame::MenuDecision decision = GetMenuDecision();
	if (decision== GAME_END)
	{
		PrintGoodbye();
		return;
	}
	system("cls");
	StartGame();
	PlayLoop();
}

void CGame::PrintMenu(bool isColored)
{
	system("cls");

	CColoredPrint::prl("Hello, guy!\n");
	CColoredPrint::prl("Welcome to Donkey Kong game\n", CColorPoint::c_color::BLUE, CColorPoint::c_decoration::BOLD);
	CColoredPrint::prl("Please select one of the follwing options : ", CColorPoint::c_color::CYAN, CColorPoint::c_decoration::BOLD);

	CColoredPrint::prl("1 - Start a new game", CColorPoint::c_color::GREEN, CColorPoint::c_decoration::ITALIC);
	if (isColored)
		CColoredPrint::prl("2 - Turn off color", CColorPoint::c_color::GREEN, CColorPoint::c_decoration::ITALIC);
	else
		CColoredPrint::prl("2 - Turn on color", CColorPoint::c_color::GREEN, CColorPoint::c_decoration::ITALIC);
	CColoredPrint::prl("8 - Present instructions and keys", CColorPoint::c_color::GREEN, CColorPoint::c_decoration::ITALIC);
	CColoredPrint::prl("9 - exit", CColorPoint::c_color::GREEN, CColorPoint::c_decoration::ITALIC);
}

CGame::MenuDecision CGame:: GetMenuDecision()
{
	char choice;
	bool color = true;

	while (true)
	{
		if (_kbhit())
		{
		 choice = _getch();
		 switch (choice)
		 {
		 case '1':
			 this->m_IsColored = color;
			 return  CGame::GAME_START;
		 case '2':
			 color = !color; // toggle 
			 PrintMenu(color);
			 break;
		 case '8':
			 PrintInstructions();
			 break;
		 case '9':
			 return CGame::GAME_END;
	
			default:
				cout << "your choice is not legal input"<< endl;
		 }
		}
	}
	return GAME_END;
}

void CGame::PrintInstructions(bool wait)
{
	CColoredPrint::prl("\nGame instructions and keys\n", CColoredPrint::c_color::YELLOW);
	CColoredPrint::prl("When the game starts, Mario(the hero) is positioned at his start position without any movement.");
	CColoredPrint::prl("Once you selects move direction(using the keys, as listed below)");
	CColoredPrint::prl("Mario will continue to move in this direction even if the user does not press any key");
	CColoredPrint::prl("as long as game board boundaries are not reached and the STAY key is not pressed.");
	CColoredPrint::prl("KEYS :", CColoredPrint::c_color::GREEN, CColoredPrint::c_decoration::BLINK);
	CColoredPrint::pr("LEFT :			", CColoredPrint::c_color::GREEN, CColoredPrint::c_decoration::BOLD);
	CColoredPrint::prl("a or A", CColoredPrint::c_color::GREEN, CColoredPrint::c_decoration::ITALIC);
	CColoredPrint::pr("RIGHT :			", CColoredPrint::c_color::GREEN, CColoredPrint::c_decoration::BOLD);
	CColoredPrint::prl("d or D", CColoredPrint::c_color::GREEN, CColoredPrint::c_decoration::ITALIC);
	CColoredPrint::pr("UP / JUMP :		", CColoredPrint::c_color::GREEN, CColoredPrint::c_decoration::BOLD);
	CColoredPrint::prl("w or W", CColoredPrint::c_color::GREEN, CColoredPrint::c_decoration::ITALIC);
	CColoredPrint::pr("DOWN :			", CColoredPrint::c_color::GREEN, CColoredPrint::c_decoration::BOLD);
	CColoredPrint::prl("x or X", CColoredPrint::c_color::GREEN, CColoredPrint::c_decoration::ITALIC);
	CColoredPrint::pr("STAY :			", CColoredPrint::c_color::GREEN, CColoredPrint::c_decoration::BOLD);
	CColoredPrint::prl("s or S", CColoredPrint::c_color::GREEN, CColoredPrint::c_decoration::ITALIC);
	CColoredPrint::prl("\nPausing a game:", CColoredPrint::c_color::GREEN, CColoredPrint::c_decoration::UNDERLINE);
	CColoredPrint::prl("Pressing the ESC key during a game pauses the game.");
	CColoredPrint::prl("When the game is at pause state, pressing ESC again would continue the game");
	CColoredPrint::prl("\nExit a game:", CColoredPrint::c_color::GREEN, CColoredPrint::c_decoration::UNDERLINE);
	CColoredPrint::prl("You can exit the game via the Pause menu");

	while (wait == true)
	{
		if (_kbhit())
		{
			if (_getch() == ESC_KEY) // ASCII code for ESC key
			{
				system("cls");
				PrintPauseMenu();
				return;
			}
		}
	}
}


void CGame::PrintGoodbye()
{
	CColoredPrint::prl("Goodbye, friend!\n", CColoredPrint::c_color::CYAN);
}

void CGame::StartGame()
{
	Init();
	m_board.Draw(m_IsColored);
}

void CGame::Init()
{
	m_board.Init();
	m_mario = CItem(2, m_board.GetBorderHight() - 3, AVATAR_MARIO, m_IsColored ? CColorPoint::c_color::GREEN : CColorPoint::c_color::WHITE); // reset mario to the bottom 
	m_mario.SetLives(MARIO_LIVES);
	m_donkeykong = CItem(m_board.GetBorderWidth()/2,2, AVATAR_DONKEYKONG, m_IsColored ? CColorPoint::c_color::CYAN : CColorPoint::c_color::WHITE);
}

void CGame::PlayLoop()
{
	bool Mario(true), Italian(true);
	bool OnLadder(false);
	char input, previnput('\0');	

	hideCursor();
	m_mario.Draw();
	m_donkeykong.Draw();
	while (Mario = Italian) 
	{
		if (_kbhit())
		{
			input = _getch();
			if (input != previnput)
			{
				switch (input)
				{
				case 'a':
				case 'A':
					m_mario.SetDirection(CItem::LEFT); // 
					break;
				case 'd':
				case 'D':
					m_mario.SetDirection(CItem::RIGHT);
					break;
				case 'w':
				case 'W':
					m_mario.SetDirection(CItem::UP);
					break; 
				case 'x':
				case 'X':
					m_mario.SetDirection(CItem::DOWN);
					break;
				case 's':
				case 'S':
					m_mario.SetDirection(CItem::STOP);
					break;
				case ESC_KEY:
				{///
					if (Paused() == GAME_END)
						return;
					break;
				}
				default:
					// ignore illegal input
					break;
				}
				if (input != ESC_KEY)
					previnput = input; // remember the prev input 
			}
		} 
			// first delete then ask what next and update m_next_symbol then adavance the object and then draw

			// ask what besides mario
			//ask what below mario
			//update mario on the map
			//do the same for the barrles 
			// print them all
		/*if (CheckWhatIsBelow(m_mario, OnLadder) == DEAD)
		{
			m_mario.ReduceLife();
			if (m_mario.GetLives() == 0)
			{
				GameOverScreen();
				return;
			}
		}*/
	

		switch(PlayerCheckNextCell(m_mario))
		{
		case DEAD:
			m_mario.ReduceLife();
			if (m_mario.GetLives() == 0)
			{
				GameOverScreen();
				return;
			}
			break;
		case WON:
			// function that will celebrate the winner
			return;
		case ALIVE:
			if (m_mario.IsStop() == false) {
				m_mario.SetX(m_mario.GetX() + m_mario.GetXDirection());
				m_mario.SetY(m_mario.GetY() + m_mario.GetYDirection());
				m_mario.Draw();
			}
		} 
		std::cout.flush(); 
		Sleep(100);
	}



}


CGame::MenuDecision CGame::Paused()
{
	bool flag(true);
	char choice, prevchioce= '\0';

	system("cls");
	PrintPauseMenu();

	while (flag)
	{
		if (_kbhit())
		{
			choice = _getch();
			if (prevchioce != choice)
			{
				switch (choice)
				{
					case ESC_KEY:
					{
						system("cls");
						m_board.Draw(m_IsColored);
						return GAME_START; // just continue the game
					}
					case '1':
					{
						system("cls");
						PrintInstructions(true);
						break;
					}
					case '2':
					{
						PrintGoodbye();
						return GAME_END;
					}
				}
			}
		}
	}
	return GAME_START;
}

void CGame::PrintPauseMenu() 
{
	const char* PauseMenu[] = {
		"********************************************************************************",
		"*                                                                              *",
		"*                               GAME PAUSED                                    *",
		"*                                                                              *",
		"********************************************************************************",
		"*                                                                              *",
		"*                            [ESC] Resume Game                                 *",
		"*                                                                              *",
		"*                            [1] Instructions                                  *",
		"*                                                                              *",
		"*                            [2] Quit Game                                     *",
		"*                                                                              *",
		"********************************************************************************",
		"*                                                                              *",
		"*                   Use the indicated keys to make your selection              *",
		"*                                                                              *",
		"********************************************************************************"
	};

	for (const char* line : PauseMenu) {
		std::cout << line << std::endl;
	}
}



CGame::NeighboorType CGame::WhoSomeoneNextToMe(CPoint& point)
{
	// here the game will check all the barrels location and boarder /// check if donkey kong there
	return CGame::NeighboorType::NONE;
}
// checks what in cell next to the direction of the player
CGame::LiveStatus CGame::PlayerCheckNextCell(CItem &character)
{
	char symbol;
	CColorPoint::c_color color;
	CPoint newPos(character.GetX() + character.GetXDirection(), character.GetY() + character.GetYDirection());
	enum CBoard::Board_Place place  = m_board.GetBoardPlace(newPos);
	switch (place) {
	case CBoard::Board_Place::BOARDER:
	case CBoard::Board_Place::FLOOR:
		character.SetDirection(CItem::STOP);
		break;
	case CBoard::Board_Place::LADDER:
		//break;
	case CBoard::Board_Place::FREE:
	default:
	{
		CGame::NeighboorType neigboorType;
		neigboorType = WhoSomeoneNextToMe(newPos);
		switch (neigboorType)
		{
		case CGame::NeighboorType::NONE:
			if (character.IsStop() == false) {
				m_board.GetBoardCh(character, &symbol, &color);
				character.SeRestoreSymbol(symbol, color);
				character.Erase();
			}
			break;;
		case CGame::NeighboorType::BARREL:
			return DEAD;
		case CGame::NeighboorType::DONKEYKONG:
			return WON;

		}
		break;
	}

	}
	return ALIVE;
}

CGame :: LiveStatus CGame::CheckWhatIsBelow(CItem& character, bool OnLadder)
{
	CPoint BelowPos(character.GetX(), character.GetY() + 1);
	enum CBoard::Board_Place place = m_board.GetBoardPlace(BelowPos);
	switch (place)
	{
	case CBoard::Board_Place::FLOOR:
	case CBoard::Board_Place::FREE:
		character.SetDirection(CItem::DOWN);
		character.Falling();
		if (character.GetFallCounter() >= character.GetMaxFall())
			return DEAD;
	case CBoard::Board_Place::LADDER:
		OnLadder = true;
		break;
	default:
		OnLadder = false;
	}
	character.ResetFalling();
	return ALIVE;
}


void CGame::GameOverScreen()
{
	// Clear the screen
	system("cls");

	// ASCII art for a gravestone with Mario's name
	const char* art[] = {
		"         _______",
		"      .-'       `-.",
		"     /             \\",
		"    /               \\",
		"   |   RIP MARIO     |",
		"   |                 |",
		"   |    1981-2023    |",
		"   |                 |",
		"    \\               /",
		"     \\_____________/",
		"         ||     ||",
		"         ||     ||",
		"      ###############",
		"      #             #",
		"      ###############",
		"",
		"        GAME OVER!",
		"",
		" PRESS ANY KEY TO RETURN"
	};

	// Print the gravestone
	if (m_IsColored) {
		// Print in color
		for (int i = 0; i < 15; ++i) {
			if (i < 12) {
				CColoredPrint::prl(art[i], CColoredPrint::c_color::WHITE); // Base
			}
			else if (i == 16) {
				CColoredPrint::prl(art[i], CColoredPrint::c_color::RED, CColoredPrint::c_decoration::BOLD); // Game Over
			}
			else if (i == 18) {
				CColoredPrint::prl(art[i], CColoredPrint::c_color::YELLOW, CColoredPrint::c_decoration::ITALIC); // Press any key
			}
			else {
				CColoredPrint::prl(art[i]); // Default
			}
		}
	}
	else {
		// Print in black and white
		for (const char* line : art) {
			CColoredPrint::prl(line); // Default black and white
		}
	}
}

