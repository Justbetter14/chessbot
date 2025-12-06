#include "chess.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

int pieceType(int num) {
    return num & 7;
}

int pieceColor(int num) {
    return num & 24;
}

Move::Move(int startSquare, int targetSquare) : StartSquare(startSquare), TargetSquare(targetSquare) {}

int dirOffsets[] = {-8, 8, 1, -1, -7, -9, 9, 7};
vector<vector<int>> distFromEdge(64);

void edgeDistance() {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            int numNorth = row;
            int numWest = col;
            int numSouth = 7 - row;
            int numEast = 7 - col;

            int index = row * 8 + col;

            distFromEdge[index] = {
                numNorth, numEast, numSouth, numWest,
                min(numNorth, numEast),
                min(numNorth, numWest),
                min(numSouth, numEast),
                min(numSouth, numWest),
            };

        }
    }
}

void fen2arr(string fen, int board[64]){
    Piece piece;
    unordered_map<char, int> letterNum = {
        {'p', piece.Pawn}, {'n' , piece.Knight}, {'b', piece.Bishop},
        {'r', piece.Rook}, {'q' , piece.Queen}, {'k', piece.King}
    };

    int col = 0;
    int row = 0;
    for (char c : fen) {
        if (c == '/') {
            col = 0;
            row++;
        } else if (isdigit(c)) {
            for (int i = col; i < col + (c - '0'); i++) board[row * 8 + i] = piece.None;
            col += c - '0';
        } else {
            board[row * 8 + col] = letterNum[tolower(c)] | (isupper(c) ? piece.White : piece.Black);
            col++;
        }
    }
}

void setGlobals() {
    edgeDistance();
}

vector<Move> findKnightMoves(int board[64], int index, int color) {
    vector<Move> moves = {};

    int row = index / 8;
    int col = index % 8;

    const int rowOff[8] = {-2, -2, -1, -1, 1, 1, 2, 2};
    const int colOff[8] = {-1, 1, -2, 2, -2, 2, -1, 1};

    for (int i = 0; i < 8; i++) {
        int r = row + rowOff[i];
        int c = col + colOff[i];

        if (r < 0 || r > 7 || c < 0 || c > 7) {
            continue;
        }

        int targetSquare = r * 8 + c;

        if (pieceColor(board[targetSquare]) == color) {
            continue;
        }

        moves.push_back(Move(index, targetSquare));
    }

    return moves;
}
vector<Move> findBishopMoves(int board[64], int index, int color) {
    vector<Move> moves = {};
    for (int dir = 4; dir < 8; dir++) {
        for (int dist = 0; dist < distFromEdge[index][dir]; dist++) {
            int targetSquare = index + dirOffsets[dir] * (dist + 1);
            int targetPiece = board[targetSquare];

            if (pieceColor(targetPiece) == color) {
                break;
            }

            Move move(index, targetSquare);
            moves.push_back(move);

            if (pieceColor(targetPiece) != color && pieceType(targetPiece) != Piece().None) {
                break;
            }
        }
    }
    return moves;
}
vector<Move> findRookMoves(int board[64], int index, int color) {
    vector<Move> moves = {};
    for (int dir = 0; dir < 4; dir++) {
        for (int dist = 0; dist < distFromEdge[index][dir]; dist++) {
            int targetSquare = index + dirOffsets[dir] * (dist + 1);
            int targetPiece = board[targetSquare];

            if (pieceColor(targetPiece) == color) {
                break;
            }

            Move move(index, targetSquare);
            moves.push_back(move);

            if (pieceColor(targetPiece) != color && pieceType(targetPiece) != Piece().None) {
                break;
            }
        }
    }
    return moves;
}
vector<Move> findKingMoves(int board[64], int index, int color) {
    vector<Move> moves = {};
    for (int dir = 0; dir < 8; dir++) {
        for (int dist = 0; dist < distFromEdge[index][dir]; dist++) {
            int targetSquare = index + dirOffsets[dir] * (dist + 1);
            int targetPiece = board[targetSquare];

            if (pieceColor(targetPiece) == color) {
                break;
            }

            Move move(index, targetSquare);
            moves.push_back(move);

            if (pieceColor(targetPiece) != color && pieceType(targetPiece) != Piece().None) {
                break;
            }

            break;
        }
    }
    return moves;
}
vector<Move> findPawnMoves(int board[64], int index, int color) {
    vector<Move> moves = {};

    int row = index / 8;
    int col = index % 8;

    if (color == Piece().White) {
        if (row > 0 && pieceType(board[index - 8]) == Piece().None) {
            moves.push_back(Move(index, index - 8));

            if (row == 6 && pieceType(board[index - 16]) == Piece().None) {
                moves.push_back(Move(index, index - 16));
            }
        }
        if (row > 0 && col < 7) {
            int targetSquare = index - 7;
            if (pieceColor(board[targetSquare]) == Piece().Black && pieceType(board[targetSquare]) != Piece().None) {
                moves.push_back(Move(index, targetSquare));
            }
        }
        if (row > 0 && col > 0) {
            int targetSquare = index - 9;
            if (pieceColor(board[targetSquare]) == Piece().Black && pieceType(board[targetSquare]) != Piece().None) {
                moves.push_back(Move(index, targetSquare));
            }
        }
    }
    if (color == Piece().Black) {
        if (row < 7 && pieceType(board[index + 8]) == Piece().None) {
            moves.push_back(Move(index, index + 8));

            if (row == 1 && pieceType(board[index + 16]) == Piece().None) {
                moves.push_back(Move(index, index + 16));
            }
        }
        if (row < 7 && col < 7) {
            int targetSquare = index + 9;
            if (pieceColor(board[targetSquare]) == Piece().White && pieceType(board[targetSquare]) != Piece().None) {
                moves.push_back(Move(index, targetSquare));
            }
        }
        if (row < 7 && col > 0) {
            int targetSquare = index + 7;
            if (pieceColor(board[targetSquare]) == Piece().White && pieceType(board[targetSquare]) != Piece().None) {
                moves.push_back(Move(index, targetSquare));
            }
        }
    }

    return moves;
}

bool checkLegal(Move move, int board[64], int color) {
    int startSquare = move.StartSquare;
    int targetSquare = move.TargetSquare;

    int temp = board[targetSquare];
    board[targetSquare] = board[startSquare];
    board[startSquare] = Piece().None;

    int kingLoc = -1;
    for (int ind = 0; ind < 64; ind++) {
        if (pieceType(board[ind]) == Piece().King && pieceColor(board[ind]) == color) {
            kingLoc = ind;
            break;
        }
    }

    if (kingLoc == -1) {
        board[startSquare] = board[targetSquare];
        board[targetSquare] = temp;
        return false;
    }

    for (int ind = 0; ind < 64; ind++) {
        int type = pieceType(board[ind]);
        int colorNew = pieceColor(board[ind]);

        if (type == Piece().None || colorNew == color) {
            continue;
        }

        vector<Move> moves;

        if (type == Piece().Knight) {
            moves = findKnightMoves(board, ind, colorNew);
        } else if (type == Piece().Bishop) {
            moves = findBishopMoves(board, ind, colorNew);
        } else if (type == Piece().Rook) {
            moves = findRookMoves(board, ind, colorNew);
        } else if (type == Piece().Queen) {
            moves = findRookMoves(board, ind, colorNew);
            vector<Move> moves2 = findBishopMoves(board, ind, colorNew);
            moves.insert(moves.end(), moves2.begin(), moves2.end());
        } else if (type == Piece().King) {
            moves = findKingMoves(board, ind, colorNew);
        } else {
            moves = findPawnMoves(board, ind, colorNew);
        }

        for (Move m: moves) {
            if (m.TargetSquare == kingLoc) {
                board[startSquare] = board[targetSquare];
                board[targetSquare] = temp;
                return false;
            }
        }
    }

    board[startSquare] = board[targetSquare];
    board[targetSquare] = temp;
    return true;
}

vector<Move> generateMoves(int board[64], int index) {
    vector<Move> possibleMoves = {};

    int type = pieceType(board[index]);
    int color = pieceColor(board[index]);

    vector<Move> moves = {};

    if (type == Piece().None) {
        return {};
    }

    if (type == Piece().Knight) {
        moves = findKnightMoves(board, index, color);
    } else if (type == Piece().Bishop) {
        moves = findBishopMoves(board, index, color);
    } else if (type == Piece().Rook) {
        moves = findRookMoves(board, index, color);
    } else if (type == Piece().Queen) {
        moves = findRookMoves(board, index, color);
        vector<Move> moves2 = findBishopMoves(board, index, color);
        moves.insert(moves.end(), moves2.begin(), moves2.end());
    } else if (type == Piece().King) {
        moves = findKingMoves(board, index, color);
    } else {
        moves = findPawnMoves(board, index, color);
    }

    for (Move m : moves) {
        if (checkLegal(m, board, color)) {
            possibleMoves.push_back(m);
        }
    }

    return possibleMoves;
}

vector<Move> generateAllMoves(int board[64], int color) {
    vector<Move> possibleMoves = {};
    for (int index = 0; index < 64; index++) {
        if (pieceColor(board[index]) == color) {
            vector<Move> temp = generateMoves(board, index);
            possibleMoves.insert(possibleMoves.end(), temp.begin(), temp.end());
        }
    }
    return possibleMoves;
}

int eval(int board[64]) {
    // Negative Number is Black Winning
    // Positive Number is White Winning

    Piece piece;
    unordered_map<int, int> pieceVal = {
        {piece.Pawn, 100}, {piece.Knight, 300}, {piece.Bishop, 300},
        {piece.Rook, 500}, {piece.Queen, 900}, {piece.King, 1000}
    };

    int total = 0;

    for (int ind = 0; ind < 64; ind++) {
        if (pieceType(board[ind]) == Piece().None) {
            continue;
        }

        int color = pieceColor(board[ind]);
        if (color == Piece().Black) {
            total -= pieceVal[board[ind]];
        } else if (color == Piece().White) {
            total += pieceVal[board[ind]];
        }
    }

    return total;
}

Move findBestMove(int board[64], int color) {
    vector<Move> possibleMoves = generateAllMoves(board, color);
    int currEval = eval(board);
    vector<Move> best;
    if (color == Piece().White) {
        for (Move m: possibleMoves) {
            int startSquare = board[m.StartSquare];
            int targetSquare = board[m.TargetSquare];

            int temp = board[targetSquare];
            board[targetSquare] = board[startSquare];
            board[startSquare] = Piece().None;

            if (eval(board) > currEval) {
                currEval = eval(board);
                best.clear();
                best.push_back(m);
            } else if (eval(board) == currEval) {
                best.push_back(m);
            }

            board[startSquare] = board[targetSquare];
            board[targetSquare] = temp;
        }
    } else if (color == Piece().Black) {
        for (Move m: possibleMoves) {
            int startSquare = board[m.StartSquare];
            int targetSquare = board[m.TargetSquare];

            int temp = board[targetSquare];
            board[targetSquare] = board[startSquare];
            board[startSquare] = Piece().None;

            if (eval(board) > currEval) {
                currEval = eval(board);
                best.clear();
                best.push_back(m);
            } else if (eval(board) == currEval) {
                best.push_back(m);
            }

            board[startSquare] = board[targetSquare];
            board[targetSquare] = temp;
        }
    }

    return best[0];
}