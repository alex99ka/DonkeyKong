#include <iostream>
#include <windows.h> // For Sleep and system commands
#include <conio.h>   // For _kbhit() and _getch()
#include "Game.h"
#include "ColorPoint.h"
#include "Utils.h"

void CGame::Start()
{
    PrintMenu(true);
    CGame::MenuDecision decision = GetMenuDecision();

    if (decision == GAME_END)
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

    CColoredPrint::prl("Hello, Player!\n");
    CColoredPrint::prl("Welcome to Donkey Kong Game\n", CColorPoint::c_color::BLUE, CColorPoint::c_decoration::BOLD);
    CColoredPrint::prl("Please select one of the following options:\n", CColorPoint::c_color::CYAN, CColorPoint::c_decoration::BOLD);

    CColoredPrint::prl("1 - Start a New Game", CColorPoint::c_color::GREEN, CColorPoint::c_decoration::ITALIC);
    if (isColored)
        CColoredPrint::prl("2 - Turn Off Color", CColorPoint::c_color::GREEN, CColorPoint::c_decoration::ITALIC);
    else
        CColoredPrint::prl("2 - Turn On Color", CColorPoint::c_color::GREEN, CColorPoint::c_decoration::ITALIC);
    CColoredPrint::prl("8 - Instructions and Keys", CColorPoint::c_color::GREEN, CColorPoint::c_decoration::ITALIC);
    CColoredPrint::prl("9 - Exit", CColorPoint::c_color::GREEN, CColorPoint::c_decoration::ITALIC);
}

CGame::MenuDecision CGame::GetMenuDecision()
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
                return GAME_START;
            case '2':
                color = !color; // Toggle color
                PrintMenu(color);
                break;
            case '8':
                PrintInstructions();
                break;
            case '9':
                return GAME_END;
            default:
                std::cout << "Your choice is not a valid input." << std::endl;
            }
        }
    }
    return GAME_END;
}

void CGame::PrintInstructions(bool wait)
{
    CColoredPrint::prl("\nGame Instructions and Keys:\n", CColoredPrint::c_color::YELLOW);
    CColoredPrint::prl("Mario (the hero) starts at his position and will move according to your commands.");
    CColoredPrint::prl("KEYS:", CColoredPrint::c_color::GREEN, CColoredPrint::c_decoration::BLINK);

    CColoredPrint::pr("LEFT:  ", CColoredPrint::c_color::GREEN, CColoredPrint::c_decoration::BOLD);
    CColoredPrint::prl("a or A");
    CColoredPrint::pr("RIGHT: ", CColoredPrint::c_color::GREEN, CColoredPrint::c_decoration::BOLD);
    CColoredPrint::prl("d or D");
    CColoredPrint::pr("UP:    ", CColoredPrint::c_color::GREEN, CColoredPrint::c_decoration::BOLD);
    CColoredPrint::prl("w or W");
    CColoredPrint::pr("DOWN:  ", CColoredPrint::c_color::GREEN, CColoredPrint::c_decoration::BOLD);
    CColoredPrint::prl("x or X");
    CColoredPrint::pr("STAY:  ", CColoredPrint::c_color::GREEN, CColoredPrint::c_decoration::BOLD);
    CColoredPrint::prl("s or S");

    CColoredPrint::prl("\nTo pause the game: Press ESC.");
    CColoredPrint::prl("To exit: Use the Pause menu.");

    if (wait)
    {
        std::cout << "\nPress ESC to continue..." << std::endl;
        while (true)
        {
            if (_kbhit() && _getch() == ESC_KEY)
                break;
        }
    }
}

void CGame::StartGame()
{
    Init();
    m_board.Draw(m_IsColored);
}

void CGame::Init()
{
    m_board.Init();
    m_mario = CItem(2, m_board.GetBorderHight() - 3, AVATAR_MARIO, m_IsColored ? CColorPoint::c_color::GREEN : CColorPoint::c_color::WHITE);
    m_mario.SetLives(MARIO_LIVES);

    m_donkeykong = CItem(m_board.GetBorderWidth() / 2, 2, AVATAR_DONKEYKONG, m_IsColored ? CColorPoint::c_color::CYAN : CColorPoint::c_color::WHITE);
}


bool CGame::CanMove(CItem & character, CItem::Directions dir)
    {
        CPoint newPos(character.GetX(), character.GetY());

        // Calculate the next position based on the direction
        switch (dir)
        {
        case CItem::LEFT:
            newPos.SetX(newPos.GetX() - 1);
            break;
        case CItem::RIGHT:
            newPos.SetX(newPos.GetX() + 1);
            break;
        case CItem::UP:
            newPos.SetY(newPos.GetY() - 1);
            break;
        case CItem::DOWN:
            newPos.SetY(newPos.GetY() + 1);
            break;
        default:
            return false;
        }

        // Check if the new position is valid (not a border or obstacle)
        CBoard::Board_Place place = m_board.GetBoardPlace(newPos);
        return !(place == CBoard::BOARDER || place == CBoard::FLOOR);
    }

void CGame:: ExplosionAnimation(int x, int y, CItem& mario)
{
    const char* explosionFrames[] = {
        " @ ",
        "@@@",
        " @ ",
        
    };

    for (int i = 0; i < 3; ++i)
    {
        GoToXY(x - 1, y);
        CColoredPrint::prl(explosionFrames[0], CColorPoint::c_color::RED, CColoredPrint::c_decoration::BOLD);
        GoToXY(x - 1, y + 1);
        CColoredPrint::prl(explosionFrames[1], CColorPoint::c_color::YELLOW, CColoredPrint::c_decoration::BOLD);
        GoToXY(x - 1, y + 2);
        CColoredPrint::prl(explosionFrames[2], CColorPoint::c_color::RED, CColoredPrint::c_decoration::BOLD);

        Sleep(100);
        GoToXY(x - 1, y);     std::cout << "   ";
        GoToXY(x - 1, y + 1); 
        CColoredPrint::prl("===", CColorPoint::c_color::YELLOW, CColoredPrint::c_decoration::BOLD);
        GoToXY(x - 1, y + 2); std::cout << "  ";
    }

    // Check if Mario is within explosion radius
    int marioX = mario.GetX();
    int marioY = mario.GetY();
    int distance = abs(marioX - x) + abs(marioY - y);

    if (distance <= 2)
    {
        GameOverScreen();
    }
}

void CGame::PlayLoop()
    {
        bool isGameRunning = true;
        char input = '\0';
        hideCursor();

        // Initial Draw
        m_mario.Draw();
        m_donkeykong.Draw();

        // Get the middle X position of the board
        int centerX = m_board.GetBorderWidth() / 2;
        int topY = 1; // Highest level of the board

        // Barrels Initialization: Both start at the center of the top level
        CItem barrelRight(centerX, topY, AVATAR_BARREL, CColorPoint::c_color::WHITE);
        barrelRight.SetDirection(CItem::RIGHT);

        CItem barrelLeft(centerX, topY, AVATAR_BARREL, CColorPoint::c_color::WHITE);
        barrelLeft.SetDirection(CItem::LEFT);

        int fallCounterRight = 0;
        int fallCounterLeft = 0;

        while (isGameRunning)
        {
            // --- Handle Mario Input ---
            if (_kbhit())
            {
                input = _getch();
                switch (input)
                {
                case 'a': case 'A':
                    if (CanMove(m_mario, CItem::LEFT))
                        m_mario.SetDirection(CItem::LEFT);
                    break;
                case 'd': case 'D':
                    if (CanMove(m_mario, CItem::RIGHT))
                        m_mario.SetDirection(CItem::RIGHT);
                    break;
                case 'w': case 'W':
                {
                    CPoint checkPosUp(m_mario.GetX(), m_mario.GetY() - 1);
                    if (m_board.GetBoardPlace(checkPosUp) == CBoard::LADDER)
                        m_mario.SetDirection(CItem::UP);
                    break;
                }
                case 'x': case 'X':
                {
                    CPoint checkPosDown(m_mario.GetX(), m_mario.GetY() + 1);
                    if (m_board.GetBoardPlace(checkPosDown) == CBoard::LADDER)
                        m_mario.SetDirection(CItem::DOWN);
                    break;
                }
                case 's': case 'S':
                    m_mario.SetDirection(CItem::STOP);
                    break;
                case ESC_KEY:
                    if (Paused() == GAME_END)
                        return;
                    break;
                default:
                    break;
                }
            }

            // --- Update Mario Movement ---
            m_mario.Erase();
            if (CanMove(m_mario, m_mario.GetDirection()))
            {
                m_mario.SetX(m_mario.GetX() + m_mario.GetXDirection());
                m_mario.SetY(m_mario.GetY() + m_mario.GetYDirection());
            }
            else
            {
                m_mario.SetDirection(CItem::STOP);
            }
            m_mario.Draw();

            // --- Barrel Right Movement ---
            barrelRight.Erase();
            CPoint belowRight(barrelRight.GetX(), barrelRight.GetY() + 1);
            if (m_board.GetBoardPlace(belowRight) == CBoard::FREE) // Barrel falls
            {
                barrelRight.SetDirection(CItem::DOWN);
                fallCounterRight++;
            }
            else if (fallCounterRight >= 8 || m_board.GetBoardPlace(CPoint(barrelRight.GetX() + 1, barrelRight.GetY())) == CBoard::BOARDER)
            {
                ExplosionAnimation(barrelRight.GetX(), barrelRight.GetY(), m_mario); // Check explosion radius
                barrelRight.SetX(centerX);
                barrelRight.SetY(topY);
                barrelRight.SetDirection(CItem::RIGHT);
                fallCounterRight = 0;
            }

            // --- Barrel Left Movement ---
            barrelLeft.Erase();
            CPoint belowLeft(barrelLeft.GetX(), barrelLeft.GetY() + 1);
            if (m_board.GetBoardPlace(belowLeft) == CBoard::FREE) // Barrel falls
            {
                barrelLeft.SetDirection(CItem::DOWN);
                fallCounterLeft++;
            }
            else if (fallCounterLeft >= 8 || m_board.GetBoardPlace(CPoint(barrelLeft.GetX() - 1, barrelLeft.GetY())) == CBoard::BOARDER)
            {
                ExplosionAnimation(barrelLeft.GetX(), barrelLeft.GetY(), m_mario); // Check explosion radius
                barrelLeft.SetX(centerX);
                barrelLeft.SetY(topY);
                barrelLeft.SetDirection(CItem::LEFT);
                fallCounterLeft = 0;
            }
            else
            {
                barrelLeft.SetDirection(CItem::LEFT);
            }
            barrelLeft.SetX(barrelLeft.GetX() + barrelLeft.GetXDirection());
            barrelLeft.SetY(barrelLeft.GetY() + barrelLeft.GetYDirection());
            barrelLeft.Draw();

            // --- Check Collision with Mario ---
            if ((barrelRight.GetX() == m_mario.GetX() && barrelRight.GetY() == m_mario.GetY()) ||
                (barrelLeft.GetX() == m_mario.GetX() && barrelLeft.GetY() == m_mario.GetY()))
            {
               GameOverScreen();
                return;
            }

            // --- Frame Delay ---
            std::cout.flush();
            Sleep(100); // Smooth updates
        }
    }




CGame::MenuDecision CGame::Paused()
{
    system("cls");
    PrintPauseMenu();

    while (true)
    {
        if (_kbhit())
        {
            char choice = _getch();
            if (choice == ESC_KEY)
                return GAME_START;
            else if (choice == '2')
                return GAME_END;
        }
    }
}

void CGame::PrintPauseMenu()
{
    std::cout << "*******************************\n";
    std::cout << "*          GAME PAUSED        *\n";
    std::cout << "* [ESC] Resume Game           *\n";
    std::cout << "* [2] Quit Game               *\n";
    std::cout << "*******************************\n";
}

void CGame::PrintGoodbye()
{
    CColoredPrint::prl("Goodbye, Player!\n", CColoredPrint::c_color::CYAN);
}


void GameOverScreen()
{
    system("cls"); // Clear the screen

    // ASCII art for Game Over Screen
    const char* gameOverArt[] = {
        "         _______",
        "      .-'       `-.",
        "     /             \\",
        "    /               \\",
        "   |   GAME OVER!    |",
        "   |                 |",
        "   |    TRY AGAIN    |",
        "   |                 |",
        "    \\               /",
        "     \\_____________/",
        "         ||     ||",
        "         ||     ||",
        "      ###############",
        "      #             #",
        "      ###############",
        "",
        " PRESS ANY KEY TO CONTINUE..."
    };

    // Print the Game Over screen with color
    for (int i = 0; i < 17; ++i)
    {
        if (i == 5 || i == 6) // Highlight GAME OVER and TRY AGAIN lines
            CColoredPrint::prl(gameOverArt[i], CColorPoint::c_color::RED, CColorPoint::c_decoration::BOLD);
        else
            CColoredPrint::prl(gameOverArt[i]);
    }

    // Wait for any key press to continue
    _getch();
    exit(0);
}
