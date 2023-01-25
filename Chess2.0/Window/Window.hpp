#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "../Board/Board.hpp"

class Window {
    using Menu = std::vector<std::string>;
    protected:
        bool isBoardPretty = false;
        int width;
        int height;
        const Board &board;
    public:
        Window (const Board &board) : board (board) {} //before constructor, evaluated and stuff
        virtual ~Window () {}
    public:
        virtual void drawBoard () = 0;
        virtual void drawMenu (const Menu, int) = 0;
        virtual int input () = 0;
        virtual void drawWinnerText () = 0;
        void setPrettyBoardCase (bool isPrettyBoard) {
            this->isBoardPretty = isPrettyBoard;
        };
};

#endif // WINDOW_HPP