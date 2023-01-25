#include "Board.hpp"

Board::Board () {
    board = matrix(height, array(width,0));
}

Board::~Board () {
    endGame ();
}

bool Board::isBorder (Position position) const {
    if (position.x != 0 && position.x != 9)
        return false;
    else if ((position.y == 4 || position.y == 3) && (position.x == 0 || position.x == 9))
        return false;
    return true;
}

void Board::switchRookEatState (Position newPosition) {
    if (board[newPosition.y][newPosition.x] != 0) 
        lastEatenColor = board[newPosition.y][newPosition.x]->getColor();
    else
        lastEatenColor = Colour::colorless;
}

void Board::cleanPositionMemory (Position newPosition) {
    if (board[newPosition.y][newPosition.x] != 0) {
        delete board[newPosition.y][newPosition.x];
        board[newPosition.y][newPosition.x] = 0;        
    }
}

void Board::fishToFishyQueen (Piece *piece, Position newPosition) {
    if (piece->getPiece() == PieceType::Fish) {
        if (isPieceWhite(piece) && newPosition.y == 0) {
            cleanPositionMemory(newPosition);
            board[newPosition.y][newPosition.x] = new Piece (PieceType::FishyQueen, Colour::white);
        }
        else if (isPieceBlack(piece) && newPosition.y == 7) {
            cleanPositionMemory(newPosition);
            board[newPosition.y][newPosition.x] = new Piece (PieceType::FishyQueen, Colour::black);
        }
    }
}

bool Board::isPlayerWhite () const {
    return playerTurn == true;
}

bool Board::isPlayerBlack () const {
    return playerTurn == false;
}

bool Board::isPositionEmpty (const Piece *piece) const {
    return (piece == 0);
}

bool Board::isPieceQueen (const Piece *piece) const {
    return (piece->getPiece() == PieceType::Queen);
}

bool Board::isPieceKing (const Piece *piece) const {
    return (piece->getPiece() == PieceType::King || piece->getPiece() == PieceType::KingWBanana);
}

bool Board::isPieceWhite (const Piece *piece) const {
    return (piece!= 0 && piece->getColor() == Colour::white);
}

bool Board::isPieceBlack (const Piece *piece) const {
    return (piece!= 0 && piece->getColor() == Colour::black);
}

bool Board::isGameWon () const {
    return (isGameWonByBlack() || isGameWonByWhite());
}

bool Board::isGameWonByWhite () const {
    return isWhiteWon;
}

bool Board::isGameWonByBlack () const {
    return isBlackWon;
}

Board::matrix Board::getMatrix() const {  
    return this->board;
}

Position Board::getCurrentLocation () const {
    return this->currentLocation;
} 

void Board::setCurrentLocation(Position newCurrentPosition) {
    this->currentLocation = newCurrentPosition;
}

Position Board::getSelectedLocation () const {
    return this->selectedLocation;
} 

void Board::setSelectedLocation(Position newSelectedPosition) {
    this->selectedLocation = newSelectedPosition;
}

Colour Board::getLastEatenColor () const {
    return this->lastEatenColor;
}

bool Board::doesWhiteKingInPrisonHaveABanana (Piece *piece) {
    return (board[4][9]->getPiece() == PieceType::KingWBanana 
            && piece->getPiece() == PieceType::Monkey 
            && piece->getColor() == Colour::white);
}

bool Board::doesBlackKingInPrisonHaveABanana (Piece *piece) {
    return (board[3][0]->getPiece() == PieceType::KingWBanana 
            && piece->getPiece() == PieceType::Monkey 
            && piece->getColor() == Colour::black);
}

bool Board::isWhiteMonkeyAdjacentToKingInPrison () {
    return (board[4][8] != 0 && board[4][8]->getPiece() == PieceType::Monkey 
            && board[4][8]->getColor() == Colour::white);
}

bool Board::isBlackMonkeyAdjacentToKingInPrison () {
    return (board[3][1] != 0 && board[3][1]->getPiece() == PieceType::Monkey 
            && board[3][1]->getColor() == Colour::black);
}

void Board::move (Position currentPosition, Position newPosition) { 
    isGameFinished = false;
    auto piece = board[currentPosition.y][currentPosition.x];     
    if ((isPlayerWhite() && isPieceBlack(piece)) || (isPlayerBlack() && isPieceWhite(piece))) 
        return;

    const auto possiblePositions = piece->getMovePositions(currentPosition, board, lastEatenColor); 
    if (std::find (possiblePositions.begin(), possiblePositions.end(), newPosition) != possiblePositions.end()) {
        switchRookEatState (newPosition);

        if (!isPositionEmpty(board[newPosition.y][newPosition.x]) 
            && (isPieceQueen(board[newPosition.y][newPosition.x]) 
            || isPieceKing(board[newPosition.y][newPosition.x]))) {
            leaderMovedToPrison(currentPosition, newPosition);
        }       

        cleanPositionMemory(newPosition);

        board[newPosition.y][newPosition.x] = board[currentPosition.y][currentPosition.x];
        monkeySavesKingWBananaAndLeavesIfNeeded (possiblePositions, currentPosition, newPosition);
        board[currentPosition.y][currentPosition.x] = 0;

        if (newPosition.y == 0 || newPosition.y == 7)
            fishToFishyQueen (piece, newPosition);
    }    
    if (isGameWon())
        endGame();
    playerTurn = !playerTurn;
}

void Board::init () {
    isBlackWon = false;
    isWhiteWon = false; 
    playerTurn = true;
    isGameFinished = false;

    whiteDeathCount = 16;
    blackDeathCount = 16;

    board[0][1] = new Piece (PieceType::Rook, Colour::black);
    board[0][2] = new Piece (PieceType::Monkey, Colour::black);
    board[0][3] = new Piece (PieceType::Fish, Colour::black);
    board[0][4] = new Piece (PieceType::Queen, Colour::black);
    board[0][5] = new Piece (PieceType::KingWBanana, Colour::black);
    board[0][6] = new Piece (PieceType::Fish, Colour::black);
    board[0][7] = new Piece (PieceType::Monkey, Colour::black);
    board[0][8] = new Piece (PieceType::Rook, Colour::black);
    
    board[7][1] = new Piece (PieceType::Rook, Colour::white);
    board[7][2] = new Piece (PieceType::Monkey, Colour::white);
    board[7][3] = new Piece (PieceType::Fish, Colour::white);
    board[7][4] = new Piece (PieceType::Queen, Colour::white);
    board[7][5] = new Piece (PieceType::KingWBanana, Colour::white);
    board[7][6] = new Piece (PieceType::Fish, Colour::white);
    board[7][7] = new Piece (PieceType::Monkey, Colour::white);
    board[7][8] = new Piece (PieceType::Rook, Colour::white);

    for (int j = 1; j < width-1; ++j) { 
        if (j == 3 || j == 6) {
            board[1][j] = new Piece (PieceType::Elephant, Colour::black);
            board[6][j] = new Piece (PieceType::Elephant, Colour::white);		    
        }
        else {
            board[1][j] = new Piece (PieceType::Fish, Colour::black);
            board[6][j] = new Piece (PieceType::Fish, Colour::white);
        }
	}

    srand((unsigned) time(0));
    const int b_y = 2 + (rand() % 4);
    const int b_x = 1 + (rand() % 8);
    board[b_y][b_x] = new Piece (PieceType::Bear, Colour::gray);

//non-pieces--------------------------------------------------------
    for (int i = 0; i < height; ++i) {
        if (i != 4 || i != 3) {
            board[i][0] = new Piece (PieceType::Border, Colour::colorless);
            board[i][9] = new Piece (PieceType::Border, Colour::colorless);
        }
    }

    board[3][0] = new Piece (PieceType::Prison, Colour::white);
    board[3][9] = new Piece (PieceType::Prison, Colour::black);
    board[4][0] = new Piece (PieceType::Prison, Colour::black);
    board[4][9] = new Piece (PieceType::Prison, Colour::white);      
}

void Board::endGame() {
    for (int i = 0; i < (int)board.size(); ++i) {
        for (int j = 0; j < (int)board[0].size(); ++j) {
            Position positionToClean = {j,i};
            cleanPositionMemory (positionToClean);
        }
    }
    isGameFinished = true;
}

void Board::leaderMovedToPrison (Position currentPosition, Position newPosition) {
    if (isPlayerWhite() && isPieceBlack(board[newPosition.y][newPosition.x])) {  
        if (isPieceQueen(board[newPosition.y][newPosition.x])) {
            *board[4][0] = *board[newPosition.y][newPosition.x];
            if (board[3][0] != 0 && (board[3][0]->getPiece() == PieceType::King 
                || board[3][0]->getPiece() == PieceType::KingWBanana))
                isWhiteWon = true;
        }
        else if (isPieceKing(board[newPosition.y][newPosition.x])) {
            *board[3][0] = *board[newPosition.y][newPosition.x];
            if (board[4][0] != 0 && board[4][0]->getPiece() == PieceType::Queen) 
                isWhiteWon = true;
        }
    }
    else if (isPlayerBlack() && isPieceWhite(board[newPosition.y][newPosition.x])) { 
        if (isPieceQueen(board[newPosition.y][newPosition.x])) {
            *board[3][9] = *board[newPosition.y][newPosition.x];
            if (board[4][9] != 0 && (board[4][9]->getPiece() == PieceType::King 
                || board[4][9]->getPiece() == PieceType::KingWBanana))
                isBlackWon = true;
        }
        else if (isPieceKing(board[newPosition.y][newPosition.x])) {
            *board[4][9] = *board[newPosition.y][newPosition.x];
            if (board[3][9] != 0 && board[3][9]->getPiece() == PieceType::Queen) 
                isBlackWon = true;
        }
    }
}

void Board::monkeySavesKingWBananaAndLeavesIfNeeded (Positions possiblePositions, Position currentPosition, Position newPosition) {
    if (isPieceBlack(board[newPosition.y][newPosition.x]) && !isPositionEmpty(board[4][8])
        && !possiblePositions.empty()
        && isBlackMonkeyAdjacentToKingInPrison() 
        && doesBlackKingInPrisonHaveABanana(board[currentPosition.y][currentPosition.x])) {
        board[newPosition.y][newPosition.x] = new Piece (PieceType::King, Colour::black);
        board[3][0] = 0;
        board[3][0] = new Piece(PieceType::Prison, Colour::colorless);
    }     
    if (isPieceWhite(board[newPosition.y][newPosition.x]) && !isPositionEmpty(board[4][8]) 
        && !possiblePositions.empty() 
        && isWhiteMonkeyAdjacentToKingInPrison() 
        && doesWhiteKingInPrisonHaveABanana(board[currentPosition.y][currentPosition.x])) {
        board[newPosition.y][newPosition.x] = new Piece(PieceType::King, Colour::white);
        board[4][9] = 0;
        board[4][9] = new Piece(PieceType::Prison, Colour::colorless);
    }
}


