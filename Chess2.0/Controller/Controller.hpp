#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "../Window/NCursesWindow.hpp"
#include "../Board/Board.hpp"
#include "../Piece/Piece.hpp"
#include "../FileManager/FileManager.hpp"

enum Keys {
    KeyUp = 259,
    KeyDown = 258,
    KeyLeft = 260,
    KeyRight = 261,
    Enter = 10,
    Space = 32,
    Esc = 27
};

class Controller {
    using Menu = std::vector<std::string>;
    using Option = std::string;
    private:
        Board board;
        Window *window;
        Keys key;
        int boardType = 0;
        int blackVictoryCount = 0;
        int whiteVictoryCount = 0;
        //States gameState;
    public:
        Controller();
        ~Controller();
    public:
        void handleGameKeys (Keys pressedKey, bool &exit);
        int handleMenuKeys (const Menu sections, int &selection);
    public:
        void run ();
        void menu ();
        void playGame ();
        void options ();
        void records ();        
        void chooseBoard ();
        Keys getKey () const;
        /*States getGameState () const;
        void setGameState (States);*/
    private:
        void saveRecords () const;
        void saveChosenBoard (int enterCase, const Option option, Menu &sections);
        bool isKeyNewPositionValid (int y, int x);
        void goLeftInGame (Position &newPosition);
        void goRightInGame (Position &newPosition);
        void goUpInGame (Position &newPosition);
        void goDownInGame (Position &newPosition);
        void selectOrMovePiece (Position newPosition);
        void goUpInMenu (int &selection);
        void goDownInMenu (int &selection, const Menu sections);   
        void exitingTheGameIfThereIsVictory (bool &exit);   
        void countVictoriesForRecords ();
        void makeChoiceInOptions (Menu sections, int enterCase, bool &exit);
        void makeChoiceInChoosingBoard (Menu &sections, int enterCase, bool &exit);
        void getRecordsFileContents (Menu &sections);
        void chooseInMenu (Menu sections, int enterCase, bool &exit);
};

#endif // CONTROLLER_HPP