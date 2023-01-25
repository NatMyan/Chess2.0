#include "NCursesWindow.hpp"

NCursesWindow::NCursesWindow (const Board &board) : Window(board) {
    initscr();
    curs_set(0);
    keypad(stdscr, true);
    getmaxyx(stdscr, height, width);
    halfdelay(5);
    noecho();
}

NCursesWindow::~NCursesWindow() {
    endwin();
}

int NCursesWindow::input() {
    return getch();
}

void NCursesWindow::drawMenu (Menu sections, int selection) {
    clear();
    const int centreHeight = height / 2 - sections.size();
    const int centreWidth = width / 2 - 5;
    for (int i = 0; i < (int)sections.size(); ++i) {
        //mvprintw(centreHeight + 1 + i, centreWidth, "Chess 2 by Oats Jenkins");
        if (selection == i) 
            mvprintw (centreHeight + 1 + i, centreWidth, "{%s}\n", sections[i].c_str());
        else 
            mvprintw (centreHeight + 1 + i, centreWidth, "[%s]\n", sections[i].c_str());
    }    
    refresh();
}

void NCursesWindow::drawWinnerText () {
    const auto matrix = board.getMatrix();
    clear();
    if (board.isGameWonByWhite()) {
        printw("\\    /\\    / |     |  |  *******  |*****    \\    /\\    /  |****|   |\\  |   |\n");
        printw(" \\  /  \\  /  |-----|  |     |     |-----     \\  /  \\  /   |    |   | \\ |   |\n");
        printw("  \\/    \\/   |     |  |     |     |_____      \\/    \\/    |____|   |  \\|   .\n");
    }
    else if (board.isGameWonByBlack()) {
        printw("|***)   |       /\\    /^***  | /     \\    /\\    /  |****|   |\\  |   |\n");
        printw("|----\\  |      /--\\  |       |/\\      \\  /  \\  /   |    |   | \\ |   |\n");
        printw("|____/  |____ /    \\  \\____  |  \\      \\/    \\/    |____|   |  \\|   .\n");
    }
    getch();
}

void NCursesWindow::drawBoard () {
    if (isBoardPretty)
        drawPrettyBoard();
    else
        drawNormalBoard();
}

void NCursesWindow::drawNormalBoard () {
    const auto matrix = board.getMatrix();
    const auto selectedLocation = board.getSelectedLocation();
    const auto currentLocation = board.getCurrentLocation();
    const auto lastEatenColor = board.getLastEatenColor();
    const auto selection = matrix[selectedLocation.y][selectedLocation.x];
    Positions possiblePositions;

    clear();

    if (selection != 0)
        possiblePositions = selection->getMovePositions(selectedLocation, matrix, lastEatenColor);

    for (int i = 0; i < (int)matrix.size(); ++i) {
        for (int j = 0; j < (int)matrix[0].size(); ++j) {
            Position temporaryPosition = {j, i};
            if (!isPositionEmpty(matrix[i][j])) {                
                printForPiecePosition (possiblePositions, temporaryPosition, currentLocation, selectedLocation, matrix[i][j], i, j);
            }
            else if (isPositionEmpty(matrix[i][j])) {
                printForEmptyPosition (possiblePositions, temporaryPosition, currentLocation, selectedLocation, matrix[i][j]);
            }
        }
        printw("\n\n");
    }
    refresh();
}

void NCursesWindow::drawPrettyBoard() {
    clear();
    const auto matrix = board.getMatrix();
    const auto selectedLocation = board.getSelectedLocation();
    const auto currentLocation = board.getCurrentLocation();
    const auto lastEatenColor = board.getLastEatenColor();
    const auto selection = matrix[selectedLocation.y][selectedLocation.x];
    Positions possiblePositions;
    
    if (selection != 0)
        possiblePositions = selection->getMovePositions(selectedLocation, matrix, lastEatenColor);

    int drawBoardRow = 8;

    drawPrettyBoardColumnLetters(matrix);

    for (int i = 0; i < (int)matrix.size() * 3; ++i) {
        printw("| ");
        for (int j = 0; j < (int)matrix[0].size() * 3; ++j) {
            if (areIndexesFromFunctioningMatrix(i,j)) {
                Position temporaryPosition = {j/3, i/3};
                if (!isPositionEmpty(matrix[i/3][j/3])) {              
                    printw(" ");
                    printForPiecePosition (possiblePositions, temporaryPosition, currentLocation, selectedLocation, matrix[i/3][j/3], i/3, j/3);
                    printw(" ");
                }
                else if (isPositionEmpty(matrix[i/3][j/3])) {
                    printw(" ");
                    printForEmptyPosition (possiblePositions, temporaryPosition, currentLocation, selectedLocation, matrix[i/3][j/3]);
                    printw(" ");
                }
            }              
            if (!isLinePartOfFunctioningMatrix(i)) {
                drawPrettyPartsOfTheBoard (i, j, matrix);
                if (isPrintedBorderPassed (i, j, matrix))
                    break;
            }
        }
        if (isLinePartOfFunctioningMatrix(i)) 
            drawPrettyBoardRowNumbers (drawBoardRow);
        else 
            printw(" |\n");
    }
    printw("------------------------------------------------------");
    refresh();
}

bool NCursesWindow::isLinePartOfFunctioningMatrix (int i) {
    return (i % 3 == 1);
}

bool NCursesWindow::isPrintedBorderPassed (int i, int j, const matrix Matrix) {
    return (j == (int)Matrix.size()*3-1) && (i < 3*3 || i > 4*3+2);
}

bool NCursesWindow::isPositionPossible (Positions possiblePositions, Position tempPos) {
    if (std::find(possiblePositions.begin(), possiblePositions.end(), tempPos) != possiblePositions.end()) 
        return true;
    return false;
}

bool NCursesWindow::isPositionEmpty (Piece *piece) {
    return piece == 0;
}

bool NCursesWindow::isColorWhite(int y, int x) {
    const auto matrix = board.getMatrix();
    return (matrix[y][x]->getColor() == Colour::white);
}

bool NCursesWindow::areIndexesFromFunctioningMatrix (int i, int j) {
    return (i % 3 == 1 && j % 3 == 1);
}

void NCursesWindow::printForEmptyPosition (Positions possiblePositions, Position temporaryPosition, Position currentLocation, 
                                           Position selectedLocation, Piece *piece) {
    if (isPositionPossible(possiblePositions, temporaryPosition)) {
        if (currentLocation == temporaryPosition) 
            printw("< >");
        else
            printw("( )");
    }
    else {
        if (currentLocation == temporaryPosition)
            printw("[ ]");
        else
            printw("   ");
    }
}

void NCursesWindow::printForPiecePosition (Positions possiblePositions, Position temporaryPosition, Position currentLocation, 
                                           Position selectedLocation, Piece *piece, int i, int j) {   
    char symbol = piece->getSymbol();
    if (isColorWhite(i, j))
        symbol = toupper(symbol);

    if (isPositionPossible(possiblePositions, temporaryPosition)) {
        if (currentLocation == temporaryPosition)
            printw("<%c>", symbol);
        else
            printw("(%c)", symbol);
    }
    else  {
        Position tempPos = {0,6};
        if (selectedLocation == temporaryPosition && !(selectedLocation == tempPos)) 
            printw("{%c}", symbol);
        else if (currentLocation == temporaryPosition) 
            printw("[%c]", symbol);
        else
            printw(" %c ", symbol);
    }
}

void NCursesWindow::drawPrettyBoardRowNumbers (int &rowNumber) {
    printw(" |  %d \n", rowNumber);
    --rowNumber;
}

void NCursesWindow::drawPrettyBoardColumnLetters (const matrix Matrix) {
    char drawBoardCol = 'A';
    printw("       ");
    for (int i = 0; i < (int)Matrix.size() * 3; ++i) {
        if (i != (int)Matrix[0].size() * 3 && i % 3 == 1) {
            printw("%c", drawBoardCol);
            drawBoardCol++;
        }
        else 
            printw("  ");
    }
    printw("            Press Esc to exit");
    printw("\n");
    for (int i = 0; i < 18; ++i) 
        printw("---");
    printw("\n");
}

void NCursesWindow::drawPrettyPartsOfTheBoard (int i, int j, const matrix Matrix) {
    if (i < 3*3 || i > 4*3+2) {
        if (j == 0 || j == (int)Matrix.size()*3-1) {
            printw("/////");
        }                   
        if (i % 2 == 0) {
            if (j % 3 == 0 && j % 2 == 1)
                printw("     ");
            else if (j % 3 == 0 && j % 2 == 0)
                printw("#####");
        }
        else if (i % 2 == 1) {
            if (j % 3 == 0  && j % 2 == 1)
                printw("#####"); 
            else if (j % 3 == 0  && j % 2 == 0)
                printw("     ");
        }
    }
    else {
        if (i % 2 == 0) {
            if (j % 3 == 0 && j % 2 == 1)
                printw("#####");
            else if (j % 3 == 0 && j % 2 == 0)
                printw("     ");
        }
        else if (i % 2 == 1) {
            if (j % 3 == 0  && j % 2 == 1)
                printw("     ");
            else if (j % 3 == 0  && j % 2 == 0)
                printw("#####"); 
        
        }
    }
}



