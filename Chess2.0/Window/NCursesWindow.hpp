#ifndef NCURSES_WINDOW_HPP
#define NCURSES_WINDOW_HPP

#include "Window.hpp"
#include <algorithm>  //std::find
#include <ncurses.h>

class NCursesWindow : public Window {    
    using array = std::vector<Piece *>;
    using matrix = std::vector<array>;
    using Positions = std::vector<Position>;
    using Menu = std::vector<std::string>;
    public:
        NCursesWindow(const Board &board);
        ~NCursesWindow();
    public:
        void drawBoard ();
        void drawMenu (Menu sections, int selection);
        void drawWinnerText ();
    private:
        bool isColorWhite (int y, int x);
        void printPrettyPartsOfBoard (Piece *piece);
        void drawPrettyBoardColumnLetters(const matrix Matrix);
        void drawPrettyBoardRowNumbers (int &rowNumber);
        void drawPrettyPartsOfTheBoard (int i, int j, const matrix Matrix);
        void drawPrettyBoard();
        void drawNormalBoard();
        bool isPrintedBorderPassed (int i, int j, const matrix Matrix);
        bool isLinePartOfFunctioningMatrix (int i);
        int input ();
        bool isPositionEmpty (Piece *piece);
        bool areIndexesFromFunctioningMatrix (int i, int j);
        bool isPositionPossible (Positions possiblePositions, Position temporaryPosition);
        void printForEmptyPosition (Positions possiblePositions, Position temporaryPosition, Position currentLocation, Position selectedLocation, Piece *piece);
        void printForPiecePosition (Positions possiblePositions, Position temporaryPosition, Position currentLocation, Position selectedLocation, Piece *piece, int i, int j);
};

#endif // NCURSES_WINDOW_HPP