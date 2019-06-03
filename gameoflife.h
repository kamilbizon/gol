#ifndef GAMEOFLIFE_H
#define GAMEOFLIFE_H

#include <array>
#include <vector>
#include "rleparser.h"

using GoLBoard = std::vector<std::vector<bool>>; // m: name that data structure
                                                // no need to own class imo

class GameOfLife {
public:
    GameOfLife(const size_t& sizeX, const size_t& sizeY);
    GameOfLife(GoLBoard board);

    void setDeadBoard();

    int checkLivingNeighbours(size_t x, size_t y);
    bool isLivingSurvive(int livings);
    bool isDeadBecomeAlive(int livings);

    void copyBorders();
    void calcNextGeneration(GoLBoard boardCopy,
                            GoLBoard &nextBoard,
                            size_t start, size_t end);

    void nextIteration();
    void nextIterationThreads();

    void previousIteration();
    void doNumberOfIterations(int number);

    void printNumberOfIterations();
    void printBoardWithoutBorders();
    void printBoardWithBorders();

    GoLBoard getBoardWithBorders() const;


private:
    unsigned numberOfIterations = 0;
    unsigned currentIteration = 0;
    size_t boardX, boardY;
    GoLBoard boardWithBorders;
    size_t leftBorder, rightBorder, firstBoardColumn, lastBoardColumn;

    class PreviousIterationsSaver {
    public:
        void addIterationBoard(GoLBoard);
        GoLBoard getIterationBoard(size_t iteration);

        void saveIterationToFile(size_t iteration);
        void removeFiles();

    private:
        unsigned iterations = 0;
        std::vector<GoLBoard> previousIterations{};
        RLEParser parser{};
    };

    PreviousIterationsSaver iterSaver{};

    void setBordersConsts();
};

#endif // GAMEOFLIFE_H
