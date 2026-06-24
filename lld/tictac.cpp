#include <iostream>
#include <vector>

// ==========================================
// 1. The Rules (Enums)
// ==========================================
enum class Player {
    EMPTY, 
    X, 
    O
};

enum class GameState {
    PLAYING,
    X_WON,
    O_WON,
    DRAW
};

// ==========================================
// 2. The Board Class (The Physical Space)
// ==========================================
class Board {
private:
    std::vector<std::vector<Player>> grid;
    const int size = 3;

public:
    // Constructor initializes 3x3 grid with EMPTY
    Board() {
        grid = std::vector<std::vector<Player>>(size, std::vector<Player>(size, Player::EMPTY));
    }

    // Try to place a piece
    bool makeMove(Player player, int row, int col) {
        if (row >= 0 && row < size && col >= 0 && col < size && grid[row][col] == Player::EMPTY) {
            grid[row][col] = player;
            return true;
        }
        return false;
    }

    // Check if anyone has 3 in a row
    Player checkWinner() const {
        // Check Rows and Columns
        for (int i = 0; i < size; i++) {
            if (grid[i][0] != Player::EMPTY && grid[i][0] == grid[i][1] && grid[i][1] == grid[i][2])
                return grid[i][0];
            if (grid[0][i] != Player::EMPTY && grid[0][i] == grid[1][i] && grid[1][i] == grid[2][i])
                return grid[0][i];
        }
        // Check Diagonals
        if (grid[0][0] != Player::EMPTY && grid[0][0] == grid[1][1] && grid[1][1] == grid[2][2])
            return grid[0][0];
        if (grid[0][2] != Player::EMPTY && grid[0][2] == grid[1][1] && grid[1][1] == grid[2][0])
            return grid[0][2];

        return Player::EMPTY; 
    }

    // Check if the board is completely full
    bool isFull() const {
        for (int row = 0; row < size; row++) {
            for (int col = 0; col < size; col++) {
                if (grid[row][col] == Player::EMPTY) return false; 
            }
        }
        return true; 
    }

    // Clear the board for a new game
    void reset() {
        for (int row = 0; row < size; row++) {
            for (int col = 0; col < size; col++) {
                grid[row][col] = Player::EMPTY;
            }
        }
    }
};

// ==========================================
// 3. The Manager Class (The Referee)
// ==========================================
class TicTacToe {
private:
    Board board;          
    Player currentPlayer; 
    GameState gameState;  

public:
    // Constructor
    TicTacToe() {
        currentPlayer = Player::X;     
        gameState = GameState::PLAYING; 
    }

    // The core function the user interacts with
    void playMove(int row, int col) {
        if (gameState != GameState::PLAYING) {
            std::cout << "Game is already over!\n";
            return; 
        }

        if (board.makeMove(currentPlayer, row, col)) {
            std::cout << "Player " << (currentPlayer == Player::X ? "X" : "O") 
                      << " moved to (" << row << ", " << col << ")\n";

            Player winner = board.checkWinner();
            
            if (winner != Player::EMPTY) {
                gameState = (winner == Player::X) ? GameState::X_WON : GameState::O_WON;
                std::cout << ">>> Player " << (winner == Player::X ? "X" : "O") << " WINS! <<<\n";
            } 
            else if (board.isFull()) {
                gameState = GameState::DRAW;
                std::cout << ">>> Game is a DRAW! <<<\n";
            } 
            else {
                // Switch turn to the other player
                currentPlayer = (currentPlayer == Player::X) ? Player::O : Player::X;
            }
        } else {
            std::cout << "Invalid move! Spot taken or out of bounds. Try again.\n";
        }
    }
    
    // Reset the whole game via the manager
    void resetGame() {
        board.reset();
        currentPlayer = Player::X;
        gameState = GameState::PLAYING;
        std::cout << "\n--- Game reset. Player X's turn. ---\n";
    }
};

// ==========================================
// 4. Main Execution (The Interview Demo)
// ==========================================
int main() {
    TicTacToe game;

    std::cout << "Starting new game...\n";
    
    // Simulate a game where X wins across the top row
    game.playMove(0, 0); // X moves
    game.playMove(1, 0); // O moves
    
    game.playMove(0, 1); // X moves
    game.playMove(1, 1); // O moves
    
    game.playMove(0, 2); // X moves (X gets 3 in a row and wins)
    
    // Try to move after the game is over to prove the Referee works
    game.playMove(2, 2); 

    return 0;
}