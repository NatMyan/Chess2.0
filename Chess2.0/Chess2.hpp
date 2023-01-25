#ifndef CHESS2_HPP
#define CHESS2_HPP

#include "Window/NCursesWindow.hpp"
#include "Controller/Controller.hpp"
#include "Board/Board.hpp"

class Chess2 {
    private:
        Board board;
        Window *window;
        Controller controller;
    public:
        Chess2 ();
        ~Chess2 ();
    public:
        void run ();
};  

#endif  // CHESS2_HPP