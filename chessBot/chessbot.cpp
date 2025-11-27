#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <SFML/Graphics.hpp>
#include "chess.h"

using namespace std;
using namespace sf;

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
    setGlobals();

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

    Texture red;
    if (! red.loadFromFile("../redSpot.png")){
        cout << "failed to load" << endl;
    }
    Texture gray;
    if (! gray.loadFromFile("../graySpot.png")){
        cout << "failed to load" << endl;
    }
    Sprite redSpot(red);
    Sprite graySpot(gray);
    redSpot.setScale({squareSize/96, squareSize/96});
    graySpot.setScale({squareSize/96, squareSize/96});
    redSpot.setOrigin({64 / 2.0f, 64 / 2.0f});
    graySpot.setOrigin({64 / 2.0f, 64 / 2.0f});

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

        int board[64];
        fen2arr("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", board);
        vector<Move> possibleMoves = generateMoves(board, 62);
        unordered_set<int> attacking = {};
        for (Move m : possibleMoves) {
            attacking.insert(m.TargetSquare);
        }
        for (int row = 0; row < boardSize; row++) {
            for (int col = 0; col < boardSize; col++) {
                Vector2f pos = Vector2f(col * squareSize + squareSize / 2.0f, row * squareSize + squareSize / 2.0f);
                if (attacking.count(row * 8 + col) == 0) {
                    continue;
                }
                if (pieceType(board[row * 8 + col]) == Piece().None) {
                    graySpot.setPosition(pos);
                    window.draw(graySpot);
                } else {
                    redSpot.setPosition(pos);
                    window.draw(redSpot);
                }
            }
        }

        for (int row = 0; row < boardSize; row++) {
            for (int col = 0; col < boardSize; col++) {
                Vector2f pos = Vector2f(col * squareSize, row * squareSize);
                if (pieceType(board[row * 8 + col]) == Piece().None) {
                    continue;
                }
                if (pieceType(board[row * 8 + col]) == Piece().Rook) {
                    int color = pieceColor(board[row * 8 + col]) == Piece().Black ? 1 : 0;
                    pieces[color][4].setPosition(pos);
                    window.draw(pieces[color][4]);
                } else if (pieceType(board[row * 8 + col]) == Piece().Knight) {
                    int color = pieceColor(board[row * 8 + col]) == Piece().Black ? 1 : 0;
                    pieces[color][3].setPosition(pos);
                    window.draw(pieces[color][3]);
                } else if (pieceType(board[row * 8 + col]) == Piece().Bishop) {
                    int color = pieceColor(board[row * 8 + col]) == Piece().Black ? 1 : 0;
                    pieces[color][2].setPosition(pos);
                    window.draw(pieces[color][2]);
                } else if (pieceType(board[row * 8 + col]) == Piece().Pawn) {
                    int color = pieceColor(board[row * 8 + col]) == Piece().Black ? 1 : 0;
                    pieces[color][5].setPosition(pos);
                    window.draw(pieces[color][5]);
                } else if (pieceType(board[row * 8 + col]) == Piece().Queen) {
                    int color = pieceColor(board[row * 8 + col]) == Piece().Black ? 1 : 0;
                    pieces[color][1].setPosition(pos);
                    window.draw(pieces[color][1]);
                } else if (pieceType(board[row * 8 + col]) == Piece().King) {
                    int color = pieceColor(board[row * 8 + col]) == Piece().Black ? 1 : 0;
                    pieces[color][0].setPosition(pos);
                    window.draw(pieces[color][0]);
                }
            }
        }
        window.display();
    }
    return 0;
}