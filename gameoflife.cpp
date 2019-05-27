#include <utility>
#include <iostream>
#include <tuple>
#include <algorithm>

#include "gameoflife.h"

void GameOfLife::setBordersConsts()
{
    leftBorder = 0;
    rightBorder = boardX-1;
    firstBoardColumn = 1;
    lastBoardColumn = boardX-2;
}

GameOfLife::GameOfLife(const size_t& sizeX, const size_t& sizeY)
    : boardX(sizeX), boardY(sizeY), boardWithBorders(boardY, (std::vector<bool>(boardX))) {
    //setBlinkerBoard();
    setBordersConsts();
}

GameOfLife::GameOfLife(GoLBoard board)
    : boardX(board[0].size() + 80), boardY(board.size() + 80) {

    boardWithBorders = GoLBoard(board.size() + 80, (std::vector<bool>(board[0].size() + 80)));

    setDeadBoard();

    for (size_t i=0; i < board.size(); ++i) {
        for (size_t j=0; j < board[0].size(); ++j) {

            boardWithBorders[i+20][j+20] = board[i][j];
        }
    }

    setBordersConsts();
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

int GameOfLife::checkLivingNeighbours(int x, int y) {

    constexpr std::array<std::pair<int, int>, 8> neighbours // m: always the same neighbours
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

    for (auto& neighbour: neighbours) // m: iterations
        if (boardWithBorders[x + neighbour.first][y + neighbour.second])
            ++livings;


    return livings;
};

bool GameOfLife::isLivingSurvive(int livings) {
    return livings == 2 or livings == 3;
}

bool GameOfLife::isDeadBecomeAlive(int livings) {
    return livings == 3;
}

void GameOfLife::copyBorders() {
    for (auto &row : boardWithBorders) { // m: every row
        row[leftBorder] = row[lastBoardColumn];
        row[rightBorder] = row[firstBoardColumn];
    }
}

void GameOfLife::nextIteration() {
    copyBorders();
    auto nextBoard = boardWithBorders; // m: no templates names

    for (size_t i = 1; i < boardY-1; i++) {
        for (size_t j = 1; j < boardX-1; j++) {
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

void GameOfLife::doNumberOfIterations(int number) {
    while (numberOfIterations < number) {
        ++numberOfIterations;
        nextIteration();
//        printBoardWithoutBorders();
    }
}

void GameOfLife::printNumberOfIterations() {
    std::cout << "\nXXX " << numberOfIterations << " XXX\n";
}

void GameOfLife::printBoardWithoutBorders() {
    printNumberOfIterations();

    for (size_t i = 1; i < boardY-1; ++i) {
        for (size_t j = 1; j < boardX-1; ++j)
            std::cout << /*_boardWithBorders[i][j]*/i << ',' << j << ' ';

        std::cout << '\n';
    }
}

void GameOfLife::printBoardWithBorders() {
    printNumberOfIterations();

    for (size_t i = 1; i < boardY-1; ++i) {
        for (size_t j = 1; j < boardX-1; ++j)
            std::cout << boardWithBorders[i][j] << ' ';

        std::cout << '\n';
    }
}

GoLBoard GameOfLife::getBoardWithBorders() const
{
    return boardWithBorders;
}
