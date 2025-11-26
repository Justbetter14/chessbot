#include <iostream>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

vector<vector<string>> fen2board(string fen) {
    vector<vector<string>> board(8);
    vector<string> rows;

    string temp = "";
    for (char ch : fen) {
        if (ch == '/') {
            rows.push_back(temp);
            temp = "";
        } else {
            temp += ch;
        }
    }
    rows.push_back(temp);

    for (int r = 0; r < rows.size(); r++) {
        for (char ch : rows[r]) {
            if (isdigit(ch)) {
                for (int i = 0; i < ch - '0'; i++) {
                    board[r].push_back(" ");
                }
            } else {
                string piece = isupper(ch) ? "w" : "b";
                piece += tolower(ch);
                board[r].push_back(piece);
            }
        }
    }
    return board;
}

vector<vector<Sprite>> allPieces(int squareSize, vector<Texture>& spriteSheet) {
    vector<vector<Sprite>> pieces(2);
    string array[] = { "king" , "queen" , "bishop" , "knight" , "rook" , "pawn"};
    for (int i = 0; i < 12; i++) {
        float scale = squareSize / 128.0f;
        Sprite temp(spriteSheet[i]);
        temp.setScale({scale, scale});
        pieces[i/6].push_back(temp);
    }
    return pieces;
}

int main(int argc, char **argv) {
    const int boardSize = 8;
    const float squareSize = 75.0f;
    const unsigned int boardWidth = boardSize * squareSize;
    const unsigned int boardHeight = boardSize * squareSize;

    vector<Texture> spriteSheet;
    string array[] = { "king" , "queen" , "bishop" , "knight" , "rook" , "pawn"};
    for (string piece: array) {
        Texture white;
        if (! white.loadFromFile("../pieces-basic-png/white-" + piece + ".png")){
            cout << "failed to load" << endl;
        }
        spriteSheet.push_back(white);
    }
    for (string piece: array) {
        Texture black;
        if (! black.loadFromFile("../pieces-basic-png/black-" + piece + ".png")){
            cout << "failed to load" << endl;
        }
        spriteSheet.push_back(black);
    }
    vector<vector<Sprite>> pieces = allPieces(squareSize, spriteSheet);

    RenderWindow window(VideoMode({boardWidth, boardHeight}), "Chess Board");

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<Event::Closed>()) {
                window.close();
            }
        }
        window.clear(Color::Black);

        for (int row = 0; row < boardSize; row++) {
            for (int col = 0; col < boardSize; col++) {
                RectangleShape square(Vector2f(squareSize, squareSize));
                if ((row + col) % 2 == 0) {
                    square.setFillColor(Color(222, 181, 140));
                } else {
                    square.setFillColor(Color(158, 99, 40));
                }
                Vector2f pos = Vector2f(col * squareSize, row * squareSize);
                square.setPosition(pos);
                window.draw(square);
            }
        }

        vector<vector<string>> board = fen2board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
        for (int row = 0; row < boardSize; row++) {
            for (int col = 0; col < boardSize; col++) {
                Vector2f pos = Vector2f(col * squareSize, row * squareSize);
                if (board[row][col] == " ") {
                    continue;
                }
                if (board[row][col] == "wr") {
                    pieces[0][4].setPosition(pos);
                    window.draw(pieces[0][4]);
                } else if (board[row][col] == "br") {
                    pieces[1][4].setPosition(pos);
                    window.draw(pieces[1][4]);
                } else if (board[row][col] == "wn") {
                    pieces[0][3].setPosition(pos);
                    window.draw(pieces[0][3]);
                } else if (board[row][col] == "bn") {
                    pieces[1][3].setPosition(pos);
                    window.draw(pieces[1][3]);
                } else if (board[row][col] == "wb") {
                    pieces[0][2].setPosition(pos);
                    window.draw(pieces[0][2]);
                } else if (board[row][col] == "bb") {
                    pieces[1][2].setPosition(pos);
                    window.draw(pieces[1][2]);
                } else if (board[row][col] == "wp") {
                    pieces[0][5].setPosition(pos);
                    window.draw(pieces[0][5]);
                } else if (board[row][col] == "bp") {
                    pieces[1][5].setPosition(pos);
                    window.draw(pieces[1][5]);
                } else if (board[row][col] == "wq") {
                    pieces[0][1].setPosition(pos);
                    window.draw(pieces[0][1]);
                } else if (board[row][col] == "bq") {
                    pieces[1][1].setPosition(pos);
                    window.draw(pieces[1][1]);
                } else if (board[row][col] == "wk") {
                    pieces[0][0].setPosition(pos);
                    window.draw(pieces[0][0]);
                } else if (board[row][col] == "bk") {
                    pieces[1][0].setPosition(pos);
                    window.draw(pieces[1][0]);
                }
            }
        }

        window.display();
    }
    return 0;
}