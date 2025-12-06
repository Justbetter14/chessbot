#ifndef CHESSBOT_CHESS_H
#define CHESSBOT_CHESS_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

using namespace std;
class Piece {
public:
    const int None = 0;

    const int Pawn = 1;
    const int Knight = 2;
    const int Bishop = 3;
    const int Rook = 4;
    const int Queen = 5;
    const int King = 6;

    const int Black = 8;
    const int White = 16;
};
int pieceType(int num);
int pieceColor(int num);
struct Move {
    int StartSquare;
    int TargetSquare;

    Move(int startSquare, int targetSquare);
};
extern int dirOffsets[8];
extern vector<vector<int>> distFromEdge;

void edgeDistance();

void fen2arr(string fen, int board[64]);
void setGlobals();
vector<Move> findKnightMoves(int board[64], int index, int color);
vector<Move> findBishopMoves(int board[64], int index, int color);
vector<Move> findRookMoves(int board[64], int index, int color);
vector<Move> findKingMoves(int board[64], int index, int color);
vector<Move> findPawnMoves(int board[64], int index, int color);
bool checkLegal(Move move, int board[64], int color);
vector<Move> generateMoves(int board[64], int index);
vector<Move> generateAllMoves(int board[64], int color);
int eval(int board[64]);
Move findBestMove(int board[64], int color);
#endif //CHESSBOT_CHESS_H