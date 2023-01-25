#include "Chess2.hpp"
#include <vector>

Chess2::Chess2 () : window (new NCursesWindow(board)) {}  

Chess2::~Chess2 () {
    delete window;
}

void Chess2::run () {
    Controller game;
    game.menu();
}
















































































/*void Chess2::run () {
    bool exit = false;
    while (!exit) {
        switch (gameState) {
            case States::Menu:
                controller.menu();
                break;
            case States::Options:
                controller.options();
                break;
            case States::Records:
                controller.records();
                break;
            case States::Play:
                controller.playGame();
                break;
            case States::ChooseBoard:
                controller.chooseBoard();
                break;
            case States::Exit:
                exit = true;
                break;
        }
    }
}*/