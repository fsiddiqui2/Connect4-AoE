#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    const int cols = 5;
    const int rows = 4;
    std::vector<std::vector<int>> grid(rows, std::vector<int>(cols, 0));

    for (const auto& row : grid) {
        for (int cell : row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }

    int turn = 0;
    bool boardFull = false;
    bool win = false;
    std::vector<int> winRows;
    std::vector<int> winCols;
    int player = 0;
    while (!(boardFull || win)) {
        int player = (turn % 2 == 0) ? 1 : 2;

        int move;
        std::cout << "Select a column: ";
        std::cin >> move;

        while (grid[0][move] != 0) {
            std::cout << "Column is full, select another column" << std::endl;
            std::cout << "Select a column: ";
            std::cin >> move;
        }

        int row;
        for (row = rows - 1; row >= 0; --row) {
            if (grid[row][move] == 0) {
                grid[row][move] = player;
                break;
            }
        }

        // Check for win
        // Horizontal check
        for (int c = 0; c <= cols - 4; ++c) {
            if (grid[row][c] == player && grid[row][c + 1] == player &&
                grid[row][c + 2] == player && grid[row][c + 3] == player) {
                win = true;
                for (int i = 0; i < 4; ++i) {
                    winRows.push_back(row);
                    winCols.push_back(c + i);
                }
                break;
            }
        }

        // Vertical check
        if (!win) {
            for (int r = 0; r <= rows - 4; ++r) {
                if (grid[r][move] == player && grid[r + 1][move] == player &&
                    grid[r + 2][move] == player && grid[r + 3][move] == player) {
                    win = true;
                    for (int i = 0; i < 4; ++i) {
                        winRows.push_back(r + i);
                        winCols.push_back(move);
                    }
                    break;
                }
            }
        }

        // Left to right diagonal check
        if (!win) {
            int startRow = row - std::min(move - std::max(0, move - 3), row - std::max(0, row - 3));
            int startCol = move - std::min(move - std::max(0, move - 3), row - std::max(0, row - 3));
            while (!win && startRow + 3 < rows && startCol + 3 < cols) {
                if (grid[startRow][startCol] == player && grid[startRow + 1][startCol + 1] == player &&
                    grid[startRow + 2][startCol + 2] == player && grid[startRow + 3][startCol + 3] == player) {
                    win = true;
                    for (int i = 0; i < 4; ++i) {
                        winRows.push_back(startRow + i);
                        winCols.push_back(startCol + i);
                    }
                }
                ++startRow;
                ++startCol;
            }
        }

        // Right to left diagonal check
        if (!win) {
            int startRow = row + std::min(move - std::max(0, move - 3), std::min(rows - 1, row + 3) - row);
            int startCol = move - std::min(move - std::max(0, move - 3), std::min(rows - 1, row + 3) - row);
            while (!win && startRow - 3 >= 0 && startCol + 3 < cols) {
                if (grid[startRow][startCol] == player && grid[startRow - 1][startCol + 1] == player &&
                    grid[startRow - 2][startCol + 2] == player && grid[startRow - 3][startCol + 3] == player) {
                    win = true;
                    for (int i = 0; i < 4; ++i) {
                        winRows.push_back(startRow - i);
                        winCols.push_back(startCol + i);
                    }
                }
                --startRow;
                ++startCol;
            }
        }

        // Check for tie
        boardFull = std::all_of(grid[0].begin(), grid[0].end(), [](int cell) { return cell != 0; });

        ++turn;

        for (const auto& row : grid) {
            for (int cell : row) {
                std::cout << cell << " ";
            }
            std::cout << std::endl;
        }
    }

    if (boardFull && !win) {
        std::cout << "Tie!" << std::endl;
    } else {
        std::cout << "Player " << player << " wins!" << std::endl;
    }

    for (int i = 0; i < 4; ++i) {
        grid[winRows[i]][winCols[i]] = -1; // Using -1 instead of "X" for winning positions
    }

    for (const auto& row : grid) {
        for (int cell : row) {
            if (cell == -1) {
                std::cout << "X ";
            } else {
                std::cout << cell << " ";
            }
        }
        std::cout << std::endl;
    }

    return 0;
}