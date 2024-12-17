#include "Board.h"
#include "ColorPoint.h"

// Initialize the board with gaps, ladders, and borders
void CBoard::Init()
{
    Spaces* ArrOfNodes[SIZE_LEVEL_STOCK];
    ArrOfNodes[0] = nullptr;
    Spaces* TmpNode;
    ListOfLadders lst;
    lst.head = nullptr;
    Ladder* TmpLadder, * loltr; // List of Ladders tracker

    // Create the gaps and ladders
    CreateNodeArray(ArrOfNodes, SIZE_LEVEL_STOCK);
    CreateListOfLadders(&lst, SIZE_LEVEL_STOCK);
    loltr = lst.head;

    // Initialize the work board with spaces
    for (int y = 0; y < BORDER_HIGHT; y++)
    {
        for (int x = 0; x < BORDER_WIDTH; x++)
            workBoard[y][x] = SPACE_SYMB;
    }

    // Set the borders
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

    // Set the floors
    for (auto floor : levels)
    {
        for (int x = 1; x < BORDER_WIDTH - 1; x++)
            workBoard[floor - 1][x] = FLOOR_SYMB;
    }

    // Remove spaces from floors based on nodes
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

    // Place ladders on the board
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

    // Free memory
    FreeNodeArr(ArrOfNodes, SIZE_LEVEL_STOCK);
}

// Draw the board
void CBoard::Draw(bool color)
{
    if (color)
    {
        for (int y = 0; y < BORDER_HIGHT; y++)
        {
            for (int x = 0; x < BORDER_WIDTH; x++)
                DrawOneChar(workBoard[y][x]);
            cout << "\n";
        }
    }
    else
    {
        for (int y = 0; y < BORDER_HIGHT; y++)
        {
            for (int x = 0; x < BORDER_WIDTH; x++)
                cout << workBoard[y][x];
            cout << "\n";
        }
    }
}

// Draw a single character with color
void CBoard::DrawOneChar(char ch)
{
    if (ch == BOARDER_SYMB)
        CColoredPrint::pr(ch, BoarderColor, CColoredPrint::c_decoration::BOLD);
    else if (ch == LADDER_SYMB)
        CColoredPrint::pr(ch, LadderColor, CColoredPrint::c_decoration::BOLD);
    else if (ch == FLOOR_SYMB)
        CColoredPrint::pr(ch, FloorColor, CColoredPrint::c_decoration::BOLD);
    else if (ch == MOVE_LEFT_SYMB || ch == MOVE_RIGHT_SYMB)
        CColoredPrint::pr(ch, ArrowColor, CColoredPrint::c_decoration::BOLD);
    else
        CColoredPrint::pr(ch);
}

// Create an array of nodes (gaps in the floor)
int CBoard::CreateNodeArray(Spaces** Arr, int size)
{
    int IndexArr[] = { 1,6,14,24,60,79,1,10,64,79,48,52,60,79,1,8,44,79,1,42,1,14,65,79 };
    int AmountOfGaps[] = { 3,2,2,2,1,2 };
    int* Wathcer = IndexArr;
    int StartInd;
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
                FreeNodeArr(Arr, i);
                if (lst.head != nullptr)
                    FreeList(lst.head);
                return 0;
            }
            StartInd += 2;

            if (lst.head == nullptr)
                lst.head = lst.tail = node;
            else
            {
                lst.tail->next = node;
                lst.tail = node;
            }
        }
        Wathcer += AmountOfGaps[i] * 2;
        Arr[i] = lst.head;
    }
    return 1;
}

// Create a single node
Spaces* CBoard::CreateNode(int* Wathcer, int StartInd, int direction)
{
    Spaces* node = new Spaces;
    if (node == nullptr)
        return nullptr;

    node->begining = Wathcer[StartInd];
    node->end = Wathcer[StartInd + 1];
    node->direction = (direction % 2 == 0) ? 1 : 0;
    node->next = nullptr;
    return node;
}

// Free an array of nodes
void CBoard::FreeNodeArr(Spaces** Arr, int size)
{
    for (int i = 0; i < size; i++)
        FreeList(Arr[i]);
}

// Free a single list of nodes
void CBoard::FreeList(Spaces* head)
{
    while (head != nullptr)
    {
        Spaces* tmp = head;
        head = head->next;
        delete tmp;
    }
}

// Validate a point
bool CBoard::ValidatePoint(const CPoint& point)
{
    return point.GetX() >= 0 && point.GetX() < BORDER_WIDTH &&
        point.GetY() >= 0 && point.GetY() < BORDER_HIGHT;
}

// Get the place on the board
enum CBoard::Board_Place CBoard::GetBoardPlace(const CPoint& point)
{
    if (!ValidatePoint(point))
        return OUT_OB;

    int y = point.GetY();
    int x = point.GetX();

    if (workBoard[y][x] == LADDER_SYMB)
        return LADDER;
    if (workBoard[y][x] == BOARDER_SYMB)
        return BOARDER;
    if (workBoard[y][x] == FLOOR_SYMB)
        return FLOOR;

    return FREE;
}
int CBoard::CreateListOfLadders(ListOfLadders* lst, int size)
{
    int counter = 0;
    Ladder* node;
    int IndexOfLaddersArr[] = { 50,43,13,53,30,20,43,64 };
    int AmountOfFLadders[] = { 1,1,2,1,1,1,1 };
    int SizeOfLadderForLevel[] = { 2,3,4,6,5,3,2 };

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < AmountOfFLadders[i]; j++)
        {
            // Create different ladder types (Full, Half, or Default)
            if (i == 1 && j == 0)
                node = CreateNodeLadder(i, IndexOfLaddersArr[counter], SizeOfLadderForLevel[i], Ladder::LadderType::Full);
            else if (i == 3 && j == 0)
                node = CreateNodeLadder(i, IndexOfLaddersArr[counter], SizeOfLadderForLevel[i], Ladder::LadderType::Half);
            else
                node = CreateNodeLadder(i, IndexOfLaddersArr[counter], SizeOfLadderForLevel[i]);

            if (node == nullptr) return 0; // Allocation failure

            if (lst->head == nullptr) // First ladder node
            {
                lst->head = node;
                lst->tail = node;
            }
            else
            {
                lst->tail->next = node;
                lst->tail = node;
            }

            counter++;
        }
    }

    return 1; // Success
}
Ladder* CBoard::CreateNodeLadder(int level, int index, int size, Ladder::LadderType type)
{
    Ladder* node = new Ladder;

    if (node == nullptr) return nullptr; // Allocation failure

    node->level = level;
    node->type = type;
    node->index_of_Ladder = index;

    if (type == Ladder::LadderType::Full)
        node->size = size;
    else if (type == Ladder::LadderType::Half)
        node->size = size / 2;
    else
        node->size = size - 1;

    node->next = nullptr;
    return node;
}


// Get the color and symbol at a specific point
bool CBoard::GetBoardCh(const CPoint& coord, char* symbol, CColorPoint::c_color* color)
{
    if (!ValidatePoint(coord))
        return false;

    *symbol = workBoard[coord.GetY()][coord.GetX()];
    *color = GetCharColor(*symbol);
    return true;
}

// Get color of a specific character
CColorPoint::c_color CBoard::GetCharColor(char ch)
{
    switch (ch)
    {
    case FLOOR_SYMB: return FloorColor;
    case BOARDER_SYMB: return BoarderColor;
    case LADDER_SYMB: return LadderColor;
    case MOVE_RIGHT_SYMB:
    case MOVE_LEFT_SYMB: return ArrowColor;
    }
    return CColoredPrint::c_color::WHITE;
}
