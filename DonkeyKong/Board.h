#pragma once
#include<iostream>
#include "Item.h"

// a node that states when to start and end a gap on the platform
typedef struct SPACES {
	int begining;
	int end;
	struct SPACES* next;
	enum directions { Left, Right };
	int direction;//direction type for the barrels for each floor '0'=Left '1'=right
}Spaces;

typedef struct LIST
{
	Spaces* head;
	Spaces* tail;
}List;


//a node that states where to put each Ladder and wich type of Ladder we need?
typedef struct Ladder {
	int level;
	enum LadderType { Full, Half, Broken };
	LadderType type;//FULL Ladder='0',HALF_A_Ladder='1',BROKEN Ladder='2';
	int index_of_Ladder;
	int size; //lenght of thr Ladder
	struct Ladder* next;
}Ladder;

typedef struct ListOfLadders
{
	Ladder* head;
	Ladder* tail;
}ListOfLadders;

class CBoard
{
	static constexpr int BORDER_WIDTH = 80;
	static constexpr int BORDER_HIGHT = 25;
	static constexpr int SIZE_LEVEL_STOCK = 7;
	static constexpr char BOARDER_SYMB = 'Q';
	static constexpr char FLOOR_SYMB = '=';
	static constexpr char MOVE_RIGHT_SYMB = '>';
	static constexpr char MOVE_LEFT_SYMB = '<';
	static constexpr char LADDER_SYMB = 'H';
	static constexpr char SPACE_SYMB = ' ';
	static constexpr CColorPoint::c_color BoarderColor = CColoredPrint::c_color::RED;
	static constexpr CColorPoint::c_color FloorColor = CColoredPrint::c_color::YELLOW;
	static constexpr CColorPoint::c_color ArrowColor = CColoredPrint::c_color::MAGENTA;
	static constexpr CColorPoint::c_color LadderColor = CColoredPrint::c_color::BLUE;

	int levels[SIZE_LEVEL_STOCK] = { 24,22,19,15,9,7,4 };
public:
	enum Board_Place { BOARDER, FLOOR, LADDER, FREE, OUT_OB };

	void Init();
	void Draw(bool color);
	int GetBorderHight() { return BORDER_HIGHT; };
	int GetBorderWidth() { return BORDER_WIDTH; };
	bool ValidatePoint(const CPoint& point);
	enum Board_Place GetBoardPlace(const CPoint& point);
	bool GetBoardCh(const CPoint& coord, char* symbol, CColorPoint::c_color* color);

private:
	//void RestoreBoard
	void DrawOneChar(char ch);
	// creates an array that inicates for each floor where to create white spaces
	int CreateNodeArray(Spaces** Arr, int size);
	// creat single node
	Spaces* CreateNode(int* Wathcer, int StartInd, int direction);
	// free the whole array of lists
	void FreeNodeArr(Spaces** Arr, int size);
	void FreeList(Spaces* head);
	bool ValidatePoint(CPoint& const point);

	Ladder* CreateNodeLadder(int level, int index, int size, Ladder::LadderType type = Ladder::LadderType::Full);
	int CreateListOfLadders(ListOfLadders* lst, int size);
	void FreeListLadderNode(ListOfLadders* lst);
	CColorPoint::c_color GetCharColor(char ch);
	char workBoard[BORDER_HIGHT][BORDER_WIDTH];
	

};



