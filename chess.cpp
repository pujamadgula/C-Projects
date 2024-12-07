#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <utility> // For std::pair
using namespace std;

// Constants
const int TILE_SIZE = 80;
const int BOARD_SIZE = 8;

// Chessboard Initialization
char board[BOARD_SIZE][BOARD_SIZE] = {
    {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'},
    {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
    {'.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.'},
    {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
    {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'}
};

// Function to draw the chessboard
void drawBoard(sf::RenderWindow& window) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
            tile.setPosition(j * TILE_SIZE, i * TILE_SIZE);
            tile.setFillColor((i + j) % 2 == 0 ? sf::Color(240, 217, 181) : sf::Color(181, 136, 99)); // Light and dark squares
            window.draw(tile);
        }
    }
}

// Function to draw chess pieces
void drawPieces(sf::RenderWindow& window, const vector<sf::Texture>& textures) {
    string pieces = "KQRBNPkqrbnp";
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            char piece = board[i][j];
            if (piece != '.') {
                size_t index = pieces.find(piece);
                sf::Sprite sprite(textures[index]);
                sprite.setPosition(j * TILE_SIZE, i * TILE_SIZE);
                sprite.setScale(TILE_SIZE / 100.0f, TILE_SIZE / 100.0f); // Scale piece images to fit tiles
                window.draw(sprite);
            }
        }
    }
}

// Function to validate moves (simplified for now, add real chess rules here)
bool isValidMove(int x1, int y1, int x2, int y2) {
    // Ensure the move stays on the board
    if (x2 < 0 || x2 >= BOARD_SIZE || y2 < 0 || y2 >= BOARD_SIZE)
        return false;

    // Prevent moving to a square occupied by the same color
    char src = board[x1][y1];
    char dest = board[x2][y2];
    if ((isupper(src) && isupper(dest)) || (islower(src) && islower(dest)))
        return false;

    // Simplified: Allow any move for now (add specific piece logic later)
    return true;
}

/* Function to load chess piece textures
void loadPieceTextures(vector<sf::Texture>& textures) {
    string pieces = "KQRBNPkqrbnp";
    for (char piece : pieces) {
        sf::Texture texture;
        if (!texture.loadFromFile("pieces/" + string(1, piece) + ".png")) {
            cerr << "Failed to load texture for " << piece << endl;
            exit(EXIT_FAILURE);
        }
        textures.push_back(texture);
    }
}
*/

void loadPieceTextures(vector<sf::Texture>& textures) {
    vector<string> pieces = {"king", "queen", "rook", "bishop", "pawn"};
    vector<string> colors = {"white", "black"};

    for (const auto& color : colors) {
        for (const auto& piece : pieces) {
            sf::Texture texture;
            if (!texture.loadFromFile("pieces/" + color + "_" + piece + ".png")) {
                cerr << "Failed to load texture for " << color << " " << piece << endl;
                exit(EXIT_FAILURE);
            }
            textures.push_back(texture);
        }
    }
}

// Main Function
int main() {
    
    sf::RenderWindow window(sf::VideoMode(BOARD_SIZE * TILE_SIZE, BOARD_SIZE * TILE_SIZE), "Chess Game");


    vector<sf::Texture> textures;
    loadPieceTextures(textures);

    // Traking piece movement
    bool pieceSelected = false;
    sf::Vector2i selectedTile(-1, -1); // No piece selected initially

   
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                // Calculate the clicked tile
                int row = event.mouseButton.y / TILE_SIZE;
                int col = event.mouseButton.x / TILE_SIZE;

                if (!pieceSelected) {
                    // Select a piece
                    if (board[row][col] != '.') {
                        pieceSelected = true;
                        selectedTile = sf::Vector2i(row, col);
                    }
                } else {
                    // move the piece
                    if (isValidMove(selectedTile.x, selectedTile.y, row, col)) {
                        // Perform the move
                        board[row][col] = board[selectedTile.x][selectedTile.y];
                        board[selectedTile.x][selectedTile.y] = '.';
                    }

                    // Deselect the piece
                    pieceSelected = false;
                    selectedTile = sf::Vector2i(-1, -1);
                }
            }
        }

 
        window.clear();
        drawBoard(window);
        drawPieces(window, textures);

        if (pieceSelected) {
            sf::RectangleShape highlight(sf::Vector2f(TILE_SIZE, TILE_SIZE));
            highlight.setPosition(selectedTile.y * TILE_SIZE, selectedTile.x * TILE_SIZE);
            highlight.setFillColor(sf::Color(0, 255, 0, 100)); // Semi-transparent green
            window.draw(highlight);
        }

        window.display();
    }

    return 0;
}
