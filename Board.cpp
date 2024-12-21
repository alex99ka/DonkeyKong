#include "Board.h"
#include "ColorPoint.h"

void CBoard::Init(bool isColored)
{
	Spaces* ArrOfNodes[SIZE_LEVEL_STOCK];
	ArrOfNodes[0] = nullptr;
	Spaces* TmpNode;
	ListOfLadders lst;
	lst.head = nullptr;
	Ladder* TmpLadder, * loltr;//list of Ladders tracker
	int move_left_floor[SIZE_LEVEL_STOCK] = { UNUSED_LEVEL,UNUSED_LEVEL,46,57,41,65,38 };
	int move_right_floor[SIZE_LEVEL_STOCK] = { UNUSED_LEVEL,8,49,7,11,UNUSED_LEVEL,42 };

	m_IsColored = isColored;

	CreateNodeArray(ArrOfNodes, SIZE_LEVEL_STOCK);
	CreateListOfLadders(&lst, SIZE_LEVEL_STOCK);
	loltr = lst.head;
	for (int y = 0; y < BORDER_HIGHT; y++)
	{
		for (int x = 0; x < BORDER_WIDTH; x++)
			workBoard[y][x] = SPACE_SYMB;
	}
	for (int x = 0; x < BORDER_WIDTH; x++)
	{
		workBoard[0][x] = BOARDER_SYMB;
		workBoard[BORDER_HIGHT - 1][x] = BOARDER_SYMB;

	}
	for (int y = 0; y < BORDER_HIGHT; y++)
	{
		workBoard[y][0] = BOARDER_SYMB;
		workBoard[y][BORDER_WIDTH - 1] = BOARDER_SYMB;
	}

	for (auto floor : levels)
	{
		for (int x = 1; x < BORDER_WIDTH - 1; x++)
		{
			workBoard[floor - 1][x] = FLOOR_SYMB;
		}
	}

	for (int y = 0, x = 1; y < SIZE_LEVEL_STOCK; y++, x++)
	{
		TmpNode = ArrOfNodes[y];
		auto floor = levels[x];
		while (TmpNode != nullptr)
		{
			while (TmpNode->begining < TmpNode->end)
			{
				workBoard[floor - 1][TmpNode->begining] = SPACE_SYMB;
				TmpNode->begining++;

			}
			TmpNode = TmpNode->next;
		}
	}

	while (loltr != nullptr)
	{
		int size = loltr->size;
		int y_place = levels[loltr->level];
		int x_place = loltr->index_of_Ladder;
		for (int i = 0; i < size; i++)
		{
			workBoard[y_place - 2][x_place] = LADDER_SYMB;
			y_place--;
		}
		TmpLadder = loltr;
		loltr = loltr->next;
	}
	FreeNodeArr(ArrOfNodes, SIZE_LEVEL_STOCK);
	FreeListLadderNode(&lst);

	for (int i = 0; i < SIZE_LEVEL_STOCK; i++)
	{
		if (move_left_floor[i] != UNUSED_LEVEL)
			AddArrow(move_left_floor[i], levels[i] - 1, MOVE_LEFT_SYMB);
		if (move_right_floor[i] != UNUSED_LEVEL)
			AddArrow(move_right_floor[i], levels[i] - 1, MOVE_RIGHT_SYMB);
	}

}

void CBoard::AddArrow(int x, int y, char symb)
{
	workBoard[y][x - 1] = symb;
	workBoard[y][x] = symb;
	workBoard[y][x + 1] = symb;
}

void CBoard::Draw()
{
	if (m_IsColored)
	{
		for (int y = 0; y < BORDER_HIGHT; y++)
		{
			for (int x = 0; x < BORDER_WIDTH; x++)
			{
				DrawOneChar(workBoard[y][x]);
			}
			cout << "\n";
		}
	}
	else
	{
		for (int y = 0; y < BORDER_HIGHT; y++)
		{
			for (int x = 0; x < BORDER_WIDTH; x++)
			{
				cout << workBoard[y][x];
			}
			cout << "\n";
		}
	}
}

void CBoard::DrawOneChar(char ch)
{
	if (ch == BOARDER_SYMB)
		CColoredPrint::pr(ch, BoarderColor, CColoredPrint::c_decoration::BOLD);
	else if (ch == LADDER_SYMB)
		CColoredPrint::pr(ch, LadderColor, CColoredPrint::c_decoration::BOLD);
	else  if (ch == FLOOR_SYMB)
		CColoredPrint::pr(ch, FloorColor, CColoredPrint::c_decoration::BOLD);
	else  if (ch == MOVE_LEFT_SYMB || ch == MOVE_RIGHT_SYMB)
		CColoredPrint::pr(ch, ArrowColor, CColoredPrint::c_decoration::BOLD);
	else
		CColoredPrint::pr(ch);
}

/////////////////////// spaces //////////////////////////////////////////

int CBoard::CreateNodeArray(Spaces** Arr, int size)
{
	int IndexArr[] = { 1,6,14,24,60,79,1,10,64,79,48,52,60,79,1,8,57,79,1,42,1,14,65,79 }; // even starts the gap and odd ends the gap. between 0 and 6 there will be a gap on the bottom floor
	int AmountOfGaps[] = { 3,2,2,2,1,2 };// helper array to loop the nodes to the array
	int* Wathcer = IndexArr;
	int mover, StartInd;
	Spaces* node;
	List lst;
	for (int i = 0; i < size; i++)
	{
		lst.head = nullptr;
		lst.tail = nullptr;
		StartInd = 0;
		for (int j = 0; j < AmountOfGaps[i]; j++)
		{
			node = CreateNode(Wathcer, StartInd, i);
			if (node == nullptr)
			{
				if (*Arr != nullptr)
					FreeNodeArr(Arr, i);
				if (lst.head != nullptr)
					FreeList(lst.head);
				return 0;
			}
			StartInd += 2;
			if (lst.head == nullptr) // first node
			{
				lst.head = node;
				lst.tail = node;
			}
			else
			{
				lst.tail->next = node;
				lst.tail = node;
			}
		}
		mover = AmountOfGaps[i] * 2;  // how much to move the Wathcer each time;
		Wathcer += mover; // watch the next gap
		Arr[i] = lst.head;
	}
	return 1;
}
// create a single node
Spaces* CBoard::CreateNode(int* Wathcer, int StartInd, int direction)
{
	Spaces* node = new Spaces;

	if (node == nullptr) // alocation has failed
		return nullptr;
	node->begining = Wathcer[StartInd];
	node->end = Wathcer[StartInd + 1];
	if (direction % 2 == 0)
		node->direction = 1;
	else
		node->direction = 0;
	node->next = nullptr;
	return node;
}

void CBoard::FreeNodeArr(Spaces** Arr, int size)
{
	for (int i = 0; i < size; i++)
		FreeList(Arr[i]);
}

void CBoard::FreeList(Spaces* head)
{
	void* tmp;
	while (head != nullptr)
	{
		tmp = head;
		head = head->next;
		delete(tmp);
	}
}

/// /////////////////////////////////////Ladders////////////////////////////////////////////

int CBoard::CreateListOfLadders(ListOfLadders* lst, int size)
{
	int counter = 0;
	Ladder* node;
	int IndexOfLaddersArr[] = { 10,50,43,13,53,30,20,50,63 }; //
	int AmountOfFLadders[] = { 2,1,2,1,1,1,1 };
	int SizeOfLadderForLevel[] = { 2,3,4,12,5,3,2 };
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < AmountOfFLadders[i]; j++) // need to add a cheker for allocation and free functions for the list and nodes
		{
			if (i == 1 && j == 0)
			{
				node = CreateNodeLadder(i, IndexOfLaddersArr[counter], SizeOfLadderForLevel[i], Ladder::LadderType::Full);
			}
			else if (i == 3 && j == 0)
			{
				node = CreateNodeLadder(i, IndexOfLaddersArr[counter], SizeOfLadderForLevel[i], Ladder::LadderType::Half);
			}
			else
			{

				node = CreateNodeLadder(i, IndexOfLaddersArr[counter], SizeOfLadderForLevel[i]);
			}
			if (lst->head == nullptr)
			{
				lst->head = node;
				lst->tail = node;
				//cout << lst->head->index_of_Ladder;
			}
			else
			{
				lst->tail->next = node;
				lst->tail = node;
				//cout << lst->tail->index_of_Ladder;
			}
			counter++;
		}
	}
	return 1;
}
//create a single Ladder node
Ladder* CBoard::CreateNodeLadder(int level, int index, int size, Ladder::LadderType type)
{
	Ladder* node = new Ladder;
	if (node == nullptr)
	{
		return nullptr;
	}
	node->level = level;
	node->type = type;
	node->index_of_Ladder = index;
	if (type == Ladder::LadderType::Full)
	{
		node->size = size;
	}
	else if (type == Ladder::LadderType::Half)
	{
		node->size = size / 2;
	}
	else
	{
		node->size = size - 1;
	}
	node->next = nullptr;

	return node;

}

void CBoard::FreeListLadderNode(ListOfLadders* lst)
{
	Ladder* node = lst->head;
	while (lst->head != nullptr)
	{
		lst->head = lst->head->next;
		free(node);
		node = lst->head;
	}
	free(lst);
}

bool CBoard::ValidatePoint(CPoint& const point)
{
	if (point.GetX() >= 0 && point.GetX() < BORDER_WIDTH &&
		point.GetY() >= 0 && point.GetY() < BORDER_HIGHT)
		return true;

	return false;
}

enum CBoard::Board_Place CBoard::GetBoardPlace(CPoint& const point)
{
	int y = point.GetY();
	int x = point.GetX();

	if (ValidatePoint(point) == false)
		return OUT_OB;
	if (workBoard[y][x] == LADDER_SYMB)
		return LADDER;
	if (workBoard[y][x] == BOARDER_SYMB)
		return BOARDER;
	if (workBoard[y][x] == FLOOR_SYMB)
		return FLOOR;
	if (workBoard[y][x] == MOVE_LEFT_SYMB)
		return ARROW_LEFT;
	if (workBoard[y][x] == MOVE_RIGHT_SYMB)
		return ARROW_RIGHT;

	return FREE;
}

CColorPoint::c_color CBoard::GetCharColor(char ch)
{
	switch (ch)
	{
	case FLOOR_SYMB:
		return FloorColor;
	case BOARDER_SYMB:
		return  BoarderColor;
	case LADDER_SYMB:
		return LadderColor;
	case MOVE_RIGHT_SYMB:
	case MOVE_LEFT_SYMB:
		return ArrowColor;
	}
	return CColoredPrint::c_color::WHITE;
}

// a function that helps to reprint to the og board after somthing runs over it 
bool CBoard::GetBoardSymbol(CPoint& coord, char* symbol, CColorPoint::c_color* color)
{
	if (ValidatePoint(coord) == false)
		return false;

	*symbol = workBoard[coord.GetY()][coord.GetX()];
	if (m_IsColored)
		*color = GetCharColor(*symbol);
	else
		*color = CColoredPrint::c_color::WHITE;
	return true;
}

void CBoard::UpdateWorkBoard(int x, int y, char symbol)
{
	workBoard[y][x] = symbol;
}