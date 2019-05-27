#ifndef GAMEOFLIFE_H
#define GAMEOFLIFE_H

#include <array>
#include <vector>

using GoLBoard = std::vector<std::vector<bool>>; // m: name that data structure
                                                // no need to own class imo

class GameOfLife {
public:
    GameOfLife(const size_t& sizeX, const size_t& sizeY);
    GameOfLife(GoLBoard board);

    void setDeadBoard();
    void setBlinkerBoard();
    void setGliderBoard();

    int checkLivingNeighbours(int x, int y);
    bool isLivingSurvive(int livings);
    bool isDeadBecomeAlive(int livings);

    void copyBorders();

    void nextIteration();
    void doNumberOfIterations(int number);

    void printNumberOfIterations();
    void printBoardWithoutBorders();
    void printBoardWithBorders();

    GoLBoard getBoardWithBorders() const;

private:
    int numberOfIterations = 0;
    size_t boardX, boardY;
    GoLBoard boardWithBorders;
    size_t leftBorder, rightBorder, firstBoardColumn, lastBoardColumn;

    void setBordersConsts();
};

#endif // GAMEOFLIFE_H
