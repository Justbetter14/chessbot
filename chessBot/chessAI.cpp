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

int pieceType(int num) {
    return num & 7;
}

int pieceColor(int num) {
    return num & 24;
}

struct Move {
    static __readonly int StartSquare;
    static __readonly int TargetSquare;
};

static __readonly int dirOffsets[] = {8, -8, 1, -1, 7, -7, 9, -9};
static __readonly vector<vector<int>> distFromEdge(64);

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

vector<Move> generateMoves(int board[64], int index);

vector<Move> findKnightMoves(int board[64], int index, int color) {
    vector<Move> moves;
}
vector<Move> findBishopMoves(int board[64], int index, int color) {
    vector<Move> moves;
}
vector<Move> findRookMoves(int board[64], int index, int color) {
    vector<Move> moves;
}
vector<Move> findKingMoves(int board[64], int index, int color) {
    vector<Move> moves;
}
vector<Move> findPawnMoves(int board[64], int index, int color) {
    vector<Move> moves;
}

bool checkLegal(Move move, int board[64], int color) {
    int newBoard[64];
    copy(board, board + 64, newBoard);

    int startSquare = move.StartSquare;
    int targetSquare = move.TargetSquare;

    newBoard[targetSquare] = newBoard[startSquare];
    newBoard[startSquare] = Piece().None;

    vector<Move> attacking;
    int kingLoc = -1;
    for (int ind = 0; ind < 64; ind++) {
        if(pieceColor(newBoard[ind]) != color && pieceType(newBoard[ind]) != Piece().None) {
            vector<Move> moves = generateMoves(newBoard, ind);
            attacking.insert(attacking.end(), moves.begin(), moves.end());
        } else if (pieceColor(newBoard[ind]) == color && pieceType(newBoard[ind]) == Piece().King) {
            kingLoc = ind;
        }
    }
    for (Move m: attacking) {
        if (m.TargetSquare == kingLoc) {
            return false;
        }
    }
    return true;
}

vector<Move> generateMoves(int board[64], int index) {
    vector<Move> possibleMoves = {};

    int type = pieceType(board[index]);
    int color = pieceColor(board[index]);

    vector<Move> moves;

    if (type == Piece().None) {
        return {};
    } else if (type == Piece().Knight) {
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

int main(int argc, char **argv) {
    setGlobals();
    int board[64];
    fen2arr("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", board);
    int count = 0;
    for (int num : board) {
        if (num < 10) {
            count++;
            cout << num << "  ";
        } else {
            count++;
            cout << num << " ";
        }
        if (count % 8 == 7) {
            cout << endl;
        }
    }
    return 0;
}