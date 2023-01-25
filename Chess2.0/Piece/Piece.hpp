#ifndef PIECE_HPP
#define PIECE_HPP

#include <vector>

enum Colour {
    black = 0,
    white,
    gray,
    colorless        
};

enum PieceType {
    KingWBanana = 0,  // X
    King,             // K
    Queen,            // Q
    Monkey,           // M
    Fish,             // F
    FishyQueen,       // P
    Elephant,         // E
    Rook,             // R
    Bear,             // B
    Prison,           // ' '
    Border            // '/'
};

struct Position {
    int x;
    int y;
    bool operator==(const Position &obj) const {  
        return (x == obj.x && y == obj.y);            
    } 
};

class Piece {
    using Positions = std::vector<Position>;
    using array = std::vector<Piece *>;
    using matrix = std::vector<array>;
    private:
        PieceType type;
        Colour color;
    public:
        Piece (PieceType, Colour);
    public:        
        bool isPositionValid (Position position, const matrix &Matrix) const;
        Positions getMovePositions(Position currentPosition, const matrix &Matrix, Colour lastEatenColor);  
    private:
        void getFishPositions(Position currentPosition, const matrix &Matrix, Positions &possiblePositions);
        void getKingPositions(Position currentPosition, const matrix &Matrix, Positions &possiblePositions);
        void getQueenPositions(Position currentPosition, const matrix &Matrix, Positions &possiblePositions);
        void getElephantPositions(Position currentPosition, const matrix &Matrix, Positions &possiblePositions);
        void getMonkeyPositions(Position currentPosition, const matrix &Matrix, Positions &possiblePositions);
        void getRookPositions(Position currentPosition, const matrix &Matrix, Colour lastEatenColor, Positions &possiblePositions);
        void getBearPositions(Position currentPosition, const matrix &Matrix, Positions &possiblePositions);
    public:
        PieceType getPiece() const;                                   
        Colour getColor() const;                                     
        void setPiece(PieceType);
        void setColor(Colour);         
        char getSymbol () const;  
};

#endif  // PIECE_HPP

