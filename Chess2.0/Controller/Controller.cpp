#include "Controller.hpp"
#include <algorithm> //std::find

Controller::Controller () : window (new NCursesWindow(board)) { //move constructor
    auto fileContent = FileManager::read("records.txt");
    blackVictoryCount = fileContent.at(0); 
    whiteVictoryCount = fileContent.at(1); 
}  

Controller::~Controller () {
    delete window;    
}

void Controller::saveRecords () const {
    auto fileContent = FileManager::read("records.txt");
    fileContent[0] = blackVictoryCount;
    fileContent[1] = whiteVictoryCount;
    FileManager::write("records.txt", fileContent);
}

Keys Controller::getKey() const {
    return this->key;
}

bool Controller::isKeyNewPositionValid (int y, int x) {
    if (x > 0 && x < 9 && y >= 0 && y <= 7)
        return true;
    else
        return false;
}

int Controller::handleMenuKeys (Menu sections, int &selection) {
    Keys inputKey = static_cast<Keys>(window->input());
    switch (inputKey) {
        case Keys::KeyUp:
            goUpInMenu (selection);            
            break;
        case Keys::KeyDown:
            goDownInMenu (selection, sections);            
            break;
        case Keys::Enter:             
            return selection;
            break;
        case Keys::KeyLeft:
        case Keys::KeyRight:
        case Keys::Space:
        case Keys::Esc:
            break;
    }
    return -1;
}

void Controller::handleGameKeys (Keys pressedKey, bool &exit) { 
    Position newPosition = board.getCurrentLocation();
    switch (pressedKey) {        
        case Keys::KeyLeft:  
            goLeftInGame (newPosition);
            break;
        case Keys::KeyRight:
            goRightInGame (newPosition);
            break;
        case Keys::KeyUp:
            goUpInGame (newPosition);
            break;
        case Keys::KeyDown:
            goDownInGame (newPosition);
            break;
        case Keys::Enter: 
            selectOrMovePiece (newPosition);                       
            break;
        case Keys::Esc:
            exit = true;
            board.endGame();
            break;    
        case Keys::Space:
            break;           
    }
}

void Controller::saveChosenBoard (int enterCase, const Option option, Menu &sections) {
    sections[boardType] = option;
    sections[enterCase] = "+" + sections[enterCase] + "+";
    boardType = enterCase;
}

void Controller::goUpInMenu (int &selection) {
    if (selection > 0) 
        --selection;
}

void Controller::goDownInMenu (int &selection, const Menu sections) {
    if (selection < static_cast<int>(sections.size())-1)
        ++selection;
}

void Controller::goLeftInGame (Position &newPosition) {
    if (isKeyNewPositionValid (newPosition.y, newPosition.x-1)) {       
        newPosition.x--;
        board.setCurrentLocation(newPosition);
    }
}

void Controller::goRightInGame (Position &newPosition) {
    if (isKeyNewPositionValid (newPosition.y, newPosition.x+1)) {       
        newPosition.x++;
        board.setCurrentLocation(newPosition);
    }
}        
        
void Controller::goUpInGame (Position &newPosition) {
    if (isKeyNewPositionValid (newPosition.y-1, newPosition.x)) {       
        newPosition.y--;
        board.setCurrentLocation(newPosition);
    }
}

void Controller::goDownInGame (Position &newPosition) {
    if (isKeyNewPositionValid (newPosition.y+1, newPosition.x)) {       
        newPosition.y++;
        board.setCurrentLocation(newPosition);            
    }
}

void Controller::selectOrMovePiece (Position newPosition) {
    const Position selectedPosition = board.getSelectedLocation();
    const auto lastEatenColor = board.getLastEatenColor();
    auto Matrix = board.getMatrix();
    auto possiblePositions = Matrix[selectedPosition.y][selectedPosition.x]->getMovePositions(selectedPosition, Matrix, lastEatenColor);
    if (std::find(possiblePositions.begin(), possiblePositions.end(), newPosition) != possiblePositions.end()) {
        board.move (selectedPosition, newPosition);
        board.setSelectedLocation({0,6});
    }
    else {   
        if (Matrix[newPosition.y][newPosition.x] != 0) 
            board.setSelectedLocation(newPosition);
    } 
}

void Controller::exitingTheGameIfThereIsVictory (bool &exit) {
    if (board.isGameWon()) {
        window->drawWinnerText();
        exit = true;
        window->input();
    } 
} 

void Controller::countVictoriesForRecords () {
    if (board.isGameWonByBlack()) 
        ++blackVictoryCount;
    else if (board.isGameWonByWhite()) 
        ++whiteVictoryCount;
}

void Controller::menu () {
    Menu sections = {"Play", "Options", "Records", "Exit"};
    int selection = 0;
    bool exit = false;
    while (!exit) {
        int enterCase = handleMenuKeys (sections, selection);
        if (enterCase != -1) 
            chooseInMenu (sections, enterCase, exit);    
        window->drawMenu (sections, selection);
    }
}

void Controller::playGame () {
    bool exit = false;
    board.init();
    while (!exit) {
        Keys inputKey = static_cast<Keys>(window->input());
        handleGameKeys(inputKey, exit);
        window->drawBoard();        
        exitingTheGameIfThereIsVictory(exit);       
    } 
    countVictoriesForRecords ();    
    saveRecords();
}

void Controller::options () {
    Menu sections = {"Choose Board", "Go Back"};
    int selection = 0;
    bool exit = false;
    while (!exit) {
        int enterCase = handleMenuKeys (sections, selection);
        if (enterCase != -1) 
            makeChoiceInOptions (sections, enterCase, exit);               
        window->drawMenu (sections, selection);
    }
}

void Controller::chooseBoard () {
    Menu sections = {"Standard Board", "Pretty Board", "Go Back"};
    int selection = 0;
    bool exit = false;
    sections[boardType] = "+" + sections[boardType] + "+";
    while (!exit) {
        int enterCase = handleMenuKeys (sections, selection);
        if (enterCase != -1) 
            makeChoiceInChoosingBoard (sections, enterCase, exit);      
        window->drawMenu (sections, selection);
    }
}

void Controller::makeChoiceInOptions (Menu sections, int enterCase, bool &exit) {
    if (sections[enterCase] == "Choose Board")
        chooseBoard ();
    else if (sections[enterCase] == "Go Back")
        exit = true;
}

void Controller::makeChoiceInChoosingBoard (Menu &sections, int enterCase, bool &exit) {
    if (sections[enterCase] == "Standard Board") {
        saveChosenBoard (enterCase, "Pretty Board", sections);
        window->setPrettyBoardCase(false);
    }
    else if (sections[enterCase] == "Pretty Board") {
        saveChosenBoard (enterCase, "Standard Board", sections);
        window->setPrettyBoardCase(true);
    }
    else if (sections[enterCase] == "Go Back")
        exit = true; 
}

void Controller::getRecordsFileContents (Menu &sections) {
    auto fileContent = FileManager::read("records.txt");
    sections.push_back("Black: " + std::to_string(fileContent[0]));
    sections.push_back("White: " + std::to_string(fileContent[1])); 
}

void Controller::records () {
    Menu sections;    
    getRecordsFileContents(sections);   
    sections.push_back("Go Back");
    int selection = 0;
    bool exit = false;
    while (!exit) {
        int enterCase = handleMenuKeys (sections, selection);
        selection = sections.size()-1;
        if (enterCase != -1)
            exit = true;
        window->drawMenu (sections, selection);
    }
}

void Controller::chooseInMenu (Menu sections, int enterCase, bool &exit) {
    if (sections[enterCase] == "Play")
        playGame ();
    else if (sections[enterCase] == "Options")
        options ();
    else if (sections[enterCase] == "Records")
        records ();
    else if (sections[enterCase] == "Exit")
        exit = true;
}

/*States Controller::getGameState() const {
    return this->gameState;
}

void Controller::setGameState (States someState) {
    this->gameState = someState;
}*/

/*void Controller::run () {
    bool exit = false;
    while (!exit) {
        switch (gameState) {
            case States::Menu:
                menu();
                break;
            case States::Options:
                options();
                break;
            case States::Records:
                records();
                break;
            case States::Play:
                playGame();
                break;
            case States::ChooseBoard:
                chooseBoard();
                break;
            case States::Exit:
                exit = true;
                break;
        }
    }
}*/