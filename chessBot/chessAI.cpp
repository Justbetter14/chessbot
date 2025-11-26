#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

struct Piece {
    static const int None = 0;

    static const int Pawn = 1;
    static const int Knight = 2;
    static const int Bishop = 3;
    static const int Rook = 4;
    static const int Queen = 5;
    static const int King = 6;

    static const int Black = 8;
    static const int White = 16;
};

void fen2arr(string fen, int board[8][8]){
    unordered_map<char, int> letterNum = {
        {'p', Piece::Pawn}, {'n' , Piece::Knight}, {'b', Piece::Bishop},
        {'r', Piece::Rook}, {'q' , Piece::Queen}, {'k', Piece::King}
    };

    int col = 0;
    int row = 0;
    for (char c : fen) {
        if (c == '/') {
            col = 0;
            row++;
        } else if (isdigit(c)) {
            for (int i = col; i < col + (c - '0'); i++) board[row][i] = Piece::None;
            col += c - '0';
        } else {
            board[row][col] = letterNum[tolower(c)] | (isupper(c) ? Piece::White : Piece::Black);
            col++;
        }
    }
}

int main(int argc, char **argv) {
    int board[8][8];
    fen2arr("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR",board);
    for (int (&row)[8] : board) {
        for (int num: row) {
            cout << num << " ";
        }
        cout << endl;
    }
    return 0;
}