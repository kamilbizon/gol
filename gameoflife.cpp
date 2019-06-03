#include <utility>
#include <iostream>
#include <tuple>
#include <algorithm>
#include <thread>

#include "gameoflife.h"

#include <qdebug.h>

constexpr size_t BOARD_X_SIZE = 140;
constexpr size_t BOARD_Y_SIZE = 140;

void GameOfLife::setBordersConsts()
{
    leftBorder = 0;
    rightBorder = boardX-1;
    firstBoardColumn = 1;
    lastBoardColumn = boardX-2;
}

GameOfLife::GameOfLife(const size_t& sizeX, const size_t& sizeY)
    : boardX(sizeX), boardY(sizeY), boardWithBorders(boardY, (std::vector<bool>(boardX))) {
    setBordersConsts();
}

GameOfLife::GameOfLife(GoLBoard board)
    : boardX(BOARD_X_SIZE), boardY(BOARD_Y_SIZE) {

    boardWithBorders = GoLBoard(BOARD_Y_SIZE, (std::vector<bool>(BOARD_X_SIZE)));

    setDeadBoard();

    for (size_t i=0; i < board.size(); ++i) {
        for (size_t j=0; j < board[0].size(); ++j) {

            boardWithBorders[i+20][j+20] = board[i][j];
        }
    }

    setBordersConsts();
}

void GameOfLife::setDeadBoard() {
    for (auto& row: boardWithBorders) // m:auto
        for (auto &&cell : row)
            cell = false;
}


int GameOfLife::checkLivingNeighbours(size_t x, size_t y) {

    constexpr std::array<std::pair<int, int>, 8> neighbours // m: always the same neighbours, known at compile time
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
            auto livings = checkLivingNeighbours(i, j);

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

void GameOfLife::calcNextGeneration(GoLBoard boardCopy, GoLBoard &nextBoard, size_t start, size_t end)
{   
    size_t iNextBoard = 1;
    for (size_t i = start; i < end-1; i++) {
        for (size_t j = 1; j < boardX-1; j++) {
            auto livings = checkLivingNeighbours(i, j);

            if (boardCopy[i][j]) {
                nextBoard[iNextBoard][j] = isLivingSurvive(livings);
            }
            else if (not boardCopy[i][j]) {
                nextBoard[iNextBoard][j] = isDeadBecomeAlive(livings);
            }
        }
        ++iNextBoard;
    }
}

void GameOfLife::nextIterationThreads()
{
    copyBorders();

//    auto nextBoard = boardWithBorders;

    long half_size = boardY / 2;

    GoLBoard b1(boardWithBorders.begin(), boardWithBorders.begin() + half_size);
    GoLBoard b2(boardWithBorders.begin() + half_size, boardWithBorders.end());

    auto calcFunPtr = std::mem_fn(&GameOfLife::calcNextGeneration);

    std::thread t1(calcFunPtr, this, boardWithBorders, std::ref(b1), 1, half_size);
    std::thread t2(calcFunPtr, this, boardWithBorders, std::ref(b2), half_size, boardY);

    t1.join(); t2.join();

    b1.insert(b1.end(),
              std::make_move_iterator(b2.begin()),
              std::make_move_iterator(b2.end())); // m: no templates names

    boardWithBorders = std::move(b1);
}

void GameOfLife::doNumberOfIterations(int number) {
    while (number > 0) {
        if(currentIteration == numberOfIterations) {
//            nextIterationThreads();
            nextIteration();
            iterSaver.addIterationBoard(boardWithBorders);

            ++numberOfIterations;
            ++currentIteration;
        }
        else {
            boardWithBorders = iterSaver.getIterationBoard(currentIteration);
            ++currentIteration;
        }

        --number;
    }
}

void GameOfLife::previousIteration() {
    if(currentIteration > 0){
        --currentIteration;
        boardWithBorders = iterSaver.getIterationBoard(currentIteration);
    }
}

GoLBoard GameOfLife::getBoardWithBorders() const
{
    return boardWithBorders;
}

void GameOfLife::PreviousIterationsSaver::addIterationBoard(GoLBoard board)
{
    previousIterations.emplace_back(board);
    auto iteration = previousIterations.size() - 1;

    QString filePath = QStringLiteral("./tempBoardFile_%1.rle").arg(iteration);

    QFile file(filePath);

    if(file.open(QFile::WriteOnly | QFile::Truncate | QFile::Text)){
        QTextStream out(&file);
        parser.parseBoardToRLE(board, out);

        file.close();
    }
}

GoLBoard GameOfLife::PreviousIterationsSaver::getIterationBoard(size_t iteration) {
    QString filePath = QStringLiteral("./tempBoardFile_%1.rle").arg(iteration);
    QFile file(filePath);

    QTextStream in(&file);
    qDebug() << "1";
    if(file.open(QFile::ReadOnly)){
        qDebug() << "2";
        auto board = parser.parseFile(in);
        qDebug() << "3";
        file.close();

        return board;
    }

//        return previousIterations[iteration];
}



#ifdef DEBUG

void GameOfLife::printNumberOfIterations() {
    std::cout << "\nXXX " << numberOfIterations << " XXX\n";
}

void GameOfLife::printBoardWithoutBorders() {
    printNumberOfIterations();

    for (size_t i = 1; i < boardY-1; ++i) {
        for (size_t j = 1; j < boardX-1; ++j)
            std::cout << i << ',' << j << ' ';

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

#endif

