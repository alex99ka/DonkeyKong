
#include <random>

#include "utils.h"

void GoToXY(int x, int y) {
    std::cout.flush();
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void hideCursor()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}
void clrscr()
{
    system("cls");
}

int getRandomInt(int min, int max) {
    
    static std::random_device rd;
    static std::mt19937 generator(rd()); // Mersenne Twister PRNG

    std::uniform_int_distribution<int> distribution(min, max);

    return distribution(generator);
}