#ifndef BOARD_HPP
#define BOARD_HPP

#include "../Piece/Piece.hpp"  
#include <algorithm> //std::find
#include <cstdlib>   //std::srand()
#include <iostream>
#include <vector>

class Board {
    using Positions = std::vector<Position>;
    using array = std::vector<Piece *>;
    using matrix = std::vector<array>;
    private:
        const int width = 10;     
        const int height = 8;
        matrix board; 
        Position currentLocation = {1,6};
        Position selectedLocation = {0,6}; 
        Colour lastEatenColor = colorless;
        int blackDeathCount = 0;
        int whiteDeathCount = 0;
        bool playerTurn = true;
        bool isGameFinished = false;
        bool isBlackWon = false;
        bool isWhiteWon = false;
    public:
        Board();
        ~Board();
    public:
        void init ();
        void move (Position currentPosition, Position newPosition);    
        void endGame ();
    public:
        bool isBorder (Position pos) const;
        void fishToFishyQueen (Piece *piece, Position newPosition);
        void leaderMovedToPrison (Position currentPosition, Position newPosition);  
        bool isGameWon () const;    
        bool isGameWonByWhite () const;
        bool isGameWonByBlack () const;
    private:
        void cleanPositionMemory (Position newPosition);
        void switchRookEatState (Position newPosition);
        bool isPlayerWhite () const;
        bool isPlayerBlack () const;
        bool isPieceWhite (const Piece *piece) const;         
        bool isPieceBlack (const Piece *piece) const;
        bool isPieceQueen (const Piece *piece) const;
        bool isPieceKing (const Piece *piece) const;
        bool isPositionEmpty (const Piece *piece) const;
        bool doesWhiteKingInPrisonHaveABanana (Piece *piece);    
        bool doesBlackKingInPrisonHaveABanana (Piece *piece);
        bool isWhiteMonkeyAdjacentToKingInPrison ();
        bool isBlackMonkeyAdjacentToKingInPrison ();
        void monkeySavesKingWBananaAndLeavesIfNeeded (Positions possiblePositions, Position currentPosition, Position newPosition);
    public:
        matrix getMatrix () const;
        int getWidth () const;
        int getHeight () const;
        Position getCurrentLocation() const;
        void setCurrentLocation (Position);
        Position getSelectedLocation() const;
        void setSelectedLocation (Position);
        Colour getLastEatenColor () const;
};

#endif // BOARD_HPP