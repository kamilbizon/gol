#include <utility>
#include <iostream>
#include <tuple>
#include <algorithm>

#include "gameoflife.h"

GameOfLife::GameOfLife(const size_t& sizeX, const size_t& sizeY)
    : boardX(sizeX), boardY(sizeY), boardWithBorders(boardX, (std::vector<bool>(boardY))) {
    //setBlinkerBoard();
    leftBorder = 0;
    rightBorder = boardX-1;
    firstBoardColumn = 1;
    lastBoardColumn = boardX-2;
}

GameOfLife::GameOfLife(std::vector<std::vector<bool>> board)
    : boardX(board[0].size() + 80), boardY(board.size() + 80) {

    boardWithBorders = std::vector<std::vector<bool>>(board.size() + 80, (std::vector<bool>(board[0].size() + 80)));

    setDeadBoard();

    for (int i=0; i < board.size(); ++i) {
        for (int j=0; j < board[0].size(); ++j) {

            boardWithBorders[i+20][j+20] = board[i][j];
        }
    }

    leftBorder = 0;
    rightBorder = boardX-1;
    firstBoardColumn = 1;
    lastBoardColumn = boardX-2;
}

void GameOfLife::setDeadBoard() {
    for (auto& row: boardWithBorders)
        for (auto &&cell : row)
            cell = false;
}

void GameOfLife::setBlinkerBoard() {
    setDeadBoard();

    boardWithBorders[3][2] = true;
    boardWithBorders[3][3] = true;
    boardWithBorders[3][4] = true;
}

void GameOfLife::setGliderBoard() {
    boardWithBorders[4][3] = true;
    boardWithBorders[4][4] = true;
    boardWithBorders[4][5] = true;
    boardWithBorders[3][5] = true;
    boardWithBorders[2][4] = true;
}

int GameOfLife::checkLivingNeighbours(const int &x, const int &y) {

    constexpr std::array<std::pair<int, int>, 8> neighbours
    {
        std::make_pair(-1, -1),
        std::make_pair(0, -1),
        std::make_pair(1, -1),
        std::make_pair(-1, 0),
        std::make_pair(1, 0),
        std::make_pair(-1, 1),
        std::make_pair(0, 1),
        std::make_pair(1, 1)
    };

    int livings = 0;

    for (auto& neighbour: neighbours)
        if (boardWithBorders[x + neighbour.first][y + neighbour.second])
            livings++;


    return livings;
};

bool GameOfLife::isLivingSurvive(const int& livings) {
    return livings == 2 or livings == 3;
}

bool GameOfLife::isDeadBecomeAlive(const int& livings) {
    return livings == 3;
}

void GameOfLife::copyBorders() {
    for (auto &row : boardWithBorders) {
        row[leftBorder] = row[lastBoardColumn];
        row[rightBorder] = row[firstBoardColumn];
    }
}

void GameOfLife::nextIteration() {
    copyBorders();
    auto nextBoard = boardWithBorders;

    for (auto i = 1; i < boardX-1; i++) {
        for (auto j = 1; j < boardY-1; j++) {
            int livings = checkLivingNeighbours(i, j);

            if (boardWithBorders[i][j]) {
                nextBoard[i][j] = isLivingSurvive(livings);
            }
            else if (not boardWithBorders[i][j]) {
                nextBoard[i][j] = isDeadBecomeAlive(livings);
            }
        }
    }
    boardWithBorders = nextBoard;
}

void GameOfLife::doNumberOfIterations(const int &number) {
    while (_numberOfIterations < number) {
        ++_numberOfIterations;
        nextIteration();
        printBoardWithoutBorders();
    }
}

void GameOfLife::printNumberOfIterations() {
    std::cout << "\nXXX " << _numberOfIterations << " XXX\n";
}

void GameOfLife::printBoardWithoutBorders() {
    printNumberOfIterations();

    for (int i = 1; i < boardX-1; ++i) {
        for (int j = 1; j < boardY-1; ++j)
            std::cout << /*_boardWithBorders[i][j]*/i << ',' << j << ' ';

        std::cout << '\n';
    }
}

void GameOfLife::printBoardWithBorders() {
    printNumberOfIterations();

    for (int i = 1; i < boardX-1; ++i) {
        for (int j = 1; j < boardY-1; ++j)
            std::cout << boardWithBorders[i][j] << ' ';

        std::cout << '\n';
    }
}

std::vector<std::vector<bool> > GameOfLife::getBoardWithBorders() const
{
    return boardWithBorders;
}
