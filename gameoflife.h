#ifndef GAMEOFLIFE_H
#define GAMEOFLIFE_H

#include <array>
#include <vector>

class GameOfLife {
public:
    GameOfLife(const size_t& sizeX, const size_t& sizeY);
    GameOfLife(std::vector<std::vector<bool>> board);

    void setDeadBoard();
    void setBlinkerBoard();
    void setGliderBoard();

    int checkLivingNeighbours(const int &x, const int &y);
    bool isLivingSurvive(const int& livings);
    bool isDeadBecomeAlive(const int& livings);

    void copyBorders();

    void nextIteration();
    void doNumberOfIterations(const int &number);

    void printNumberOfIterations();
    void printBoardWithoutBorders();
    void printBoardWithBorders();

    std::vector<std::vector<bool> > getBoardWithBorders() const;

private:
    int _numberOfIterations = 0;
    size_t boardX, boardY;
    std::vector<std::vector<bool>> boardWithBorders;
    size_t leftBorder, rightBorder, firstBoardColumn, lastBoardColumn;
};

#endif // GAMEOFLIFE_H
