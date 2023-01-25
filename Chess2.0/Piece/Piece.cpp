#include "Piece.hpp"

Piece::Piece (PieceType p, Colour c) {
    this->type = p;
    this->color = c;
}

PieceType Piece::getPiece() const {
    return this->type;
}

Colour Piece::getColor() const {
    return this->color;
}

void Piece::setPiece(PieceType p) {
    this->type = p;
}

void Piece::setColor(Colour c) {
    this->color = c;
}

char Piece::getSymbol () const {
    switch (type) {
        case KingWBanana:
            return 'x';
        case King:
            return 'k';
        case Elephant: 
            return 'e';
        case Monkey:
            return 'm';
        case Queen:
            return 'q';
        case Fish:
            return 'f';
        case FishyQueen:
            return 'p';
        case Rook:
            return 'r';
        case Bear:
            return 'B';
        case Border:
            return '/';
        case Prison:
            return ' ';        
    }
    return 'Y'; 
}

//---------------------------------------------------
// x IS j >
// y IS i \/
//---------------------------------------------------

Piece::Positions Piece::getMovePositions (Position currentPosition, const matrix &Matrix, Colour lastEatenColor) {
    Positions possiblePositions;
    switch (type) {
        case Fish:
            getFishPositions (currentPosition, Matrix, possiblePositions);
            break;
        case King:
        case KingWBanana:
            getKingPositions (currentPosition, Matrix, possiblePositions);
            break;
        case Queen:
        case FishyQueen:   
            getQueenPositions (currentPosition, Matrix, possiblePositions);
            break;
        case Elephant:
            getElephantPositions (currentPosition, Matrix, possiblePositions);
            break;
        case Monkey:                                                           
            getMonkeyPositions (currentPosition, Matrix, possiblePositions);
            break;
        case Rook: 
            getRookPositions (currentPosition, Matrix, lastEatenColor, possiblePositions);   
            break;
        case Bear:
            getBearPositions (currentPosition, Matrix, possiblePositions);
            break;
        case Border:
        case Prison:
            break;
    }
    return possiblePositions;
}

bool Piece::isPositionValid (Position position, const matrix &Matrix) const {
    const auto pos = position;
    if (pos.x > 0 && pos.x < 9 && pos.y >= 0 && pos.y <= 7) { 
        if ((Matrix[pos.y][pos.x] != 0 && color != Matrix[pos.y][pos.x]->getColor()) || Matrix[pos.y][pos.x] == 0)
            return true;
    }
    return false;
}

void Piece::getFishPositions(Position currentPosition, const matrix &Matrix, Positions &possiblePositions) {
    Position futurePosition;
    futurePosition = {currentPosition.x - 1, currentPosition.y};         // left
    if (isPositionValid(futurePosition, Matrix) && Matrix[futurePosition.y][futurePosition.x] == 0)         
        possiblePositions.push_back(futurePosition);
    
    futurePosition = {currentPosition.x + 1, currentPosition.y};         // right
    if (isPositionValid(futurePosition, Matrix) && Matrix[futurePosition.y][futurePosition.x] == 0)     
        possiblePositions.push_back(futurePosition);

    if (color == white) {        
        futurePosition = {currentPosition.x - 1, currentPosition.y - 1}; // diagonal left
        if (isPositionValid(futurePosition, Matrix)) {
            possiblePositions.push_back(futurePosition);
        }
        futurePosition = {currentPosition.x + 1, currentPosition.y - 1}; // diagonal right
        if (isPositionValid(futurePosition, Matrix)) {
            possiblePositions.push_back(futurePosition);
        }
        futurePosition = {currentPosition.x, currentPosition.y - 1};     // front 
        if (isPositionValid(futurePosition, Matrix) && Matrix[futurePosition.y][futurePosition.x] == 0) {
            possiblePositions.push_back(futurePosition);
        }
    }
    else if (color == black) {        
        futurePosition = {currentPosition.x - 1, currentPosition.y + 1}; // diagonal left
        if (isPositionValid(futurePosition, Matrix)) {
            possiblePositions.push_back(futurePosition);
        }
        futurePosition = {currentPosition.x + 1, currentPosition.y + 1}; // diagonal right
        if (isPositionValid(futurePosition, Matrix)) {
            possiblePositions.push_back(futurePosition);
        }
        futurePosition = {currentPosition.x, currentPosition.y + 1};     // front 
        if (isPositionValid(futurePosition, Matrix) && Matrix[futurePosition.y][futurePosition.x] == 0) {
            possiblePositions.push_back(futurePosition);
        }
    }
}

void Piece::getKingPositions(Position currentPosition, const matrix &Matrix, Positions &possiblePositions) {
    Position futurePosition;
    futurePosition = {currentPosition.x - 1, currentPosition.y - 1}; // diagonal left up
        if (isPositionValid(futurePosition, Matrix)) {
            possiblePositions.push_back(futurePosition);
        }
        futurePosition = {currentPosition.x + 1, currentPosition.y - 1}; // diagonal right up
        if (isPositionValid(futurePosition, Matrix)) {
            possiblePositions.push_back(futurePosition);
        }
        futurePosition = {currentPosition.x, currentPosition.y - 1}; // front up
        if (isPositionValid(futurePosition, Matrix)) {
            possiblePositions.push_back(futurePosition);
        }
        futurePosition = {currentPosition.x - 1, currentPosition.y + 1}; // diagonal left down
        if (isPositionValid(futurePosition, Matrix)) {
            possiblePositions.push_back(futurePosition);
        }
        futurePosition = {currentPosition.x + 1, currentPosition.y + 1}; // diagonal right down
        if (isPositionValid(futurePosition, Matrix)) {
            possiblePositions.push_back(futurePosition);
        }
        futurePosition = {currentPosition.x, currentPosition.y + 1}; // front down
        if (isPositionValid(futurePosition, Matrix)) {
            possiblePositions.push_back(futurePosition);
        }
        futurePosition = {currentPosition.x - 1, currentPosition.y}; // left
        if (isPositionValid(futurePosition, Matrix)) {
            possiblePositions.push_back(futurePosition);
        }
        futurePosition = {currentPosition.x + 1, currentPosition.y}; // right
        if (isPositionValid(futurePosition, Matrix)) {
            possiblePositions.push_back(futurePosition);
        }
}

void Piece::getQueenPositions(Position currentPosition, const matrix &Matrix, Positions &possiblePositions) {
    Position futurePosition;
    int tempY, tempX;
    tempY = currentPosition.y; 
    tempX = currentPosition.x;
    while (tempY < 7) {                                    // down
        ++tempY;
        futurePosition = {currentPosition.x, tempY};
        if (isPositionValid (futurePosition, Matrix)) {
            possiblePositions.push_back(futurePosition);
            if (Matrix[tempY][currentPosition.x] != 0)
                break;
        }
        else
            break;
    }
    tempY = currentPosition.y;                             // up
    while (tempY > 0) {
        --tempY;
        futurePosition = {currentPosition.x, tempY};
        if (isPositionValid (futurePosition, Matrix)) {
            possiblePositions.push_back(futurePosition);
            if (Matrix[tempY][currentPosition.x] != 0)
                break;
        }
        else
            break;
    }
    tempY = currentPosition.y;                             // right
    tempX = currentPosition.x;
    while (tempX < 8) {  
        ++tempX;     
        futurePosition = {tempX, currentPosition.y};
        if (isPositionValid (futurePosition, Matrix)) {
            possiblePositions.push_back(futurePosition);
            if (Matrix[currentPosition.y][tempX] != 0)
                break;
        }
        else
            break;
    }
    tempX = currentPosition.x;                              // left
    while (tempX > 1) {
        --tempX;         
        futurePosition = {tempX, currentPosition.y};
        if (isPositionValid (futurePosition, Matrix)) {
            possiblePositions.push_back(futurePosition);
            if (Matrix[currentPosition.y][tempX] != 0)
                break;
        }
        else
            break;
    }
 
    tempY = currentPosition.y;                              // top left diagonal
    tempX = currentPosition.x;
    while (tempX != 1 && tempY != 0) {            
        --tempX;
        --tempY;  
        futurePosition = {tempX, tempY};
        if (isPositionValid (futurePosition, Matrix)) {
            possiblePositions.push_back(futurePosition);
            if (Matrix[tempY][tempX] != 0)
                break;
        }
        else
            break;
    }  
    tempY = currentPosition.y;                              // bottom left diagonal
    tempX = currentPosition.x;
    while (tempX != 1 && tempY != 7) {
        --tempX;
        ++tempY;
        futurePosition = {tempX, tempY};
        if (isPositionValid (futurePosition, Matrix)) {
            possiblePositions.push_back(futurePosition);
            if (Matrix[tempY][tempX] != 0)
                break;
        }
        else
            break;
    } 
    tempY = currentPosition.y;                              // top right diagonal
    tempX = currentPosition.x;
    while (tempX != 8 && tempY != 0) {
        ++tempX;
        --tempY;
        futurePosition = {tempX, tempY};
        if (isPositionValid (futurePosition, Matrix)) {
            possiblePositions.push_back(futurePosition);
            if (Matrix[tempY][tempX] != 0)
                break;
        }
        else
            break;
    } 
    tempY = currentPosition.y;                              // bottom right diagonal
    tempX = currentPosition.x;
    while (tempX != 8 && tempY != 7) {
        ++tempX;
        ++tempY;      
        futurePosition = {tempX, tempY};
        if (isPositionValid (futurePosition, Matrix)) {
            possiblePositions.push_back(futurePosition);
            if (Matrix[tempY][tempX] != 0)
                break;
        }
        else
            break;
    }
}

void Piece::getElephantPositions(Position currentPosition, const matrix &Matrix, Positions &possiblePositions) {
    Position futurePosition;
    futurePosition = {currentPosition.x - 2, currentPosition.y - 2}; // top diagonal left
    if (isPositionValid(futurePosition, Matrix) && Matrix[currentPosition.y - 1][currentPosition.x - 1] == 0) {
        possiblePositions.push_back(futurePosition);
    }
    futurePosition = {currentPosition.x + 2, currentPosition.y - 2}; // top diagonal right
    if (isPositionValid(futurePosition, Matrix) && Matrix[currentPosition.y - 1][currentPosition.x + 1] == 0) {
        possiblePositions.push_back(futurePosition);
    }
    futurePosition = {currentPosition.x - 2, currentPosition.y + 2}; // bottom diagonal left
    if (isPositionValid(futurePosition, Matrix) && Matrix[currentPosition.y + 1][currentPosition.x - 1] == 0) {
        possiblePositions.push_back(futurePosition);
    }
    futurePosition = {currentPosition.x + 2, currentPosition.y + 2}; // bottom diagonal right
    if (isPositionValid(futurePosition, Matrix) && Matrix[currentPosition.y + 1][currentPosition.x + 1] == 0) {
        possiblePositions.push_back(futurePosition);
    }
}

void Piece::getMonkeyPositions(Position currentPosition, const matrix &Matrix, Positions &possiblePositions) {
    Position futurePosition;
    if (isPositionValid({currentPosition.x + 1, currentPosition.y + 1}, Matrix)     // bottom diagonal right
        && Matrix[currentPosition.y + 1][currentPosition.x + 1] == 0) {        
        futurePosition = {currentPosition.x + 1, currentPosition.y + 1};
    }
    else if (isPositionValid({currentPosition.x + 2, currentPosition.y + 2}, Matrix) // swing bdr
                && Matrix[currentPosition.y + 2][currentPosition.x + 2] == 0) {        
        futurePosition = {currentPosition.x + 2, currentPosition.y + 2};
    }
    possiblePositions.push_back(futurePosition);

    if (isPositionValid({currentPosition.x + 1, currentPosition.y}, Matrix)          // right
        && Matrix[currentPosition.y][currentPosition.x + 1] == 0) {        
        futurePosition = {currentPosition.x + 1, currentPosition.y};
    }
    else if (isPositionValid({currentPosition.x + 2, currentPosition.y}, Matrix)     // swing r
                && Matrix[currentPosition.y][currentPosition.x + 2] == 0) {
        futurePosition = {currentPosition.x + 2, currentPosition.y};
    }
    possiblePositions.push_back(futurePosition);

    if (isPositionValid({currentPosition.x + 1, currentPosition.y - 1}, Matrix)      // top diagonal right
        && Matrix[currentPosition.y - 1][currentPosition.x + 1] == 0) {
        futurePosition = {currentPosition.x + 1, currentPosition.y - 1};
    }
    else if (isPositionValid({currentPosition.x + 2, currentPosition.y - 2}, Matrix) // swing tdr
                && Matrix[currentPosition.y - 2][currentPosition.x + 2] == 0) {
        futurePosition = {currentPosition.x + 2, currentPosition.y - 2};
    }
    possiblePositions.push_back(futurePosition);

    if (isPositionValid({currentPosition.x, currentPosition.y - 1}, Matrix)          // up
        && Matrix[currentPosition.y - 1][currentPosition.x] == 0) {
        futurePosition = {currentPosition.x, currentPosition.y - 1};
    }
    else if (isPositionValid({currentPosition.x, currentPosition.y - 2}, Matrix)     // swing up
                && Matrix[currentPosition.y - 2][currentPosition.x] == 0) {
        futurePosition = {currentPosition.x, currentPosition.y - 2};
    }
    possiblePositions.push_back(futurePosition);

    if (isPositionValid({currentPosition.x - 1, currentPosition.y - 1}, Matrix)      // top diagonal left
        && Matrix[currentPosition.y - 1][currentPosition.x - 1] == 0) {
        futurePosition = {currentPosition.x - 1, currentPosition.y - 1};
    }
    else if (isPositionValid({currentPosition.x - 2, currentPosition.y - 2}, Matrix) // swing tdl
                && Matrix[currentPosition.y - 2][currentPosition.x - 2] == 0) {
        futurePosition = {currentPosition.x - 2, currentPosition.y - 2};
    }
    possiblePositions.push_back(futurePosition);

    if (isPositionValid({currentPosition.x - 1, currentPosition.y}, Matrix)          // left
        && Matrix[currentPosition.y][currentPosition.x - 1] == 0) {
        futurePosition = {currentPosition.x - 1, currentPosition.y};
    }
    else if (isPositionValid({currentPosition.x - 2, currentPosition.y}, Matrix)     // swing l
                && Matrix[currentPosition.y][currentPosition.x - 2] == 0) {
        futurePosition = {currentPosition.x - 2, currentPosition.y};
    }
    possiblePositions.push_back(futurePosition);

    if (isPositionValid({currentPosition.x - 1, currentPosition.y + 1}, Matrix)      // bottom diagonal left
        && Matrix[currentPosition.y + 1][currentPosition.x - 1] == 0) {
        futurePosition = {currentPosition.x - 1, currentPosition.y + 1};
    }
    else if (isPositionValid({currentPosition.x - 2, currentPosition.y + 2}, Matrix) // swing bdl
                && Matrix[currentPosition.y + 2][currentPosition.x - 2] == 0) {
        futurePosition = {currentPosition.x - 2, currentPosition.y + 2};
    }
    possiblePositions.push_back(futurePosition);

    if (isPositionValid({currentPosition.x, currentPosition.y + 1}, Matrix)          // down
        && Matrix[currentPosition.y + 1][currentPosition.x] == 0) {
        futurePosition = {currentPosition.x, currentPosition.y + 1};
    }
    else if (isPositionValid({currentPosition.x, currentPosition.y + 2}, Matrix)     // swing down
                && Matrix[currentPosition.y + 2][currentPosition.x] == 0) {
        futurePosition = {currentPosition.x, currentPosition.y + 2};
    }
    possiblePositions.push_back(futurePosition);
}

void Piece::getRookPositions(Position currentPosition, const matrix &Matrix, Colour lastEatenColor, Positions &possiblePositions) {
    Position futurePosition;
    for (int y = 0; y < 8; ++y) {
        for (int x = 1; x < 9; ++x) {
            futurePosition = {y,x};   
            if (isPositionValid(futurePosition, Matrix) && Matrix[futurePosition.y][futurePosition.x] == 0) {
                possiblePositions.push_back(futurePosition);
            }
        }
    }
    if (lastEatenColor == color) {
        possiblePositions.push_back({currentPosition.x-1, currentPosition.y - 1});    
        possiblePositions.push_back({currentPosition.x + 1, currentPosition.y - 1}); 
        possiblePositions.push_back({currentPosition.x, currentPosition.y - 1});
        possiblePositions.push_back({currentPosition.x - 1, currentPosition.y + 1});
        possiblePositions.push_back({currentPosition.x + 1, currentPosition.y + 1});
        possiblePositions.push_back({currentPosition.x, currentPosition.y + 1});
        possiblePositions.push_back({currentPosition.x - 1, currentPosition.y});
        possiblePositions.push_back({currentPosition.x + 1, currentPosition.y});
    }
}

void Piece::getBearPositions(Position currentPosition, const matrix &Matrix, Positions &possiblePositions) {
    Position futurePosition;
    futurePosition = {currentPosition.x - 1, currentPosition.y - 1}; // diagonal left up
    if (isPositionValid(futurePosition, Matrix) && Matrix[futurePosition.y][futurePosition.x] == 0) {
        possiblePositions.push_back(futurePosition);
    }
    futurePosition = {currentPosition.x + 1, currentPosition.y - 1}; // diagonal right up
    if (isPositionValid(futurePosition, Matrix) && Matrix[futurePosition.y][futurePosition.x] == 0) {
        possiblePositions.push_back(futurePosition);
    }
    futurePosition = {currentPosition.x, currentPosition.y - 1};     // front up
    if (isPositionValid(futurePosition, Matrix) && Matrix[futurePosition.y][futurePosition.x] == 0) {
        possiblePositions.push_back(futurePosition);
    }
    futurePosition = {currentPosition.x - 1, currentPosition.y + 1}; // diagonal left down
    if (isPositionValid(futurePosition, Matrix) && Matrix[futurePosition.y][futurePosition.x] == 0) {
        possiblePositions.push_back(futurePosition);
    }
    futurePosition = {currentPosition.x + 1, currentPosition.y + 1}; // diagonal right down
    if (isPositionValid(futurePosition, Matrix) && Matrix[futurePosition.y][futurePosition.x] == 0) {
        possiblePositions.push_back(futurePosition);
    }
    futurePosition = {currentPosition.x, currentPosition.y + 1};     // front down
    if (isPositionValid(futurePosition, Matrix) && Matrix[futurePosition.y][futurePosition.x] == 0) {
        possiblePositions.push_back(futurePosition);
    }
    futurePosition = {currentPosition.x - 1, currentPosition.y};     // left
    if (isPositionValid(futurePosition, Matrix) && Matrix[futurePosition.y][futurePosition.x] == 0) {
        possiblePositions.push_back(futurePosition);
    }
    futurePosition = {currentPosition.x + 1, currentPosition.y};     // right
    if (isPositionValid(futurePosition, Matrix) && Matrix[futurePosition.y][futurePosition.x] == 0) {
        possiblePositions.push_back(futurePosition);
    }
}
