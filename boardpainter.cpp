#include "boardpainter.h"
#include <iostream>

using std::vector;

BoardPainter::BoardPainter(QPaintDevice * p_device,
                           const std::vector<std::vector<bool>> &p_board,
                           size_t p_cellSize,
                           size_t p_xStartBoard, size_t p_yStartBoard)
                : QPainter (p_device),
                  cellSize(p_cellSize),
                  boardWithBorders(p_board),
                  width((boardWithBorders.size()-2) * p_cellSize),
                  height((boardWithBorders[0].size()-2) * p_cellSize),
                  startBoard(p_xStartBoard, p_yStartBoard),
                  endBoard(p_xStartBoard + width, p_yStartBoard + height) {}


void BoardPainter::DrawBoard()
{
    drawGrid();
    drawBoardWithBorders();
}

void BoardPainter::drawGrid()
{
    setPen(QPen(QColor("grey")));

    for (size_t i = 0; i <= width; i+=cellSize) {
        QPoint startLine(startBoard.x(), startBoard.y() + i);
        QPoint endLine(endBoard.x(), startLine.y());

        drawLine(startLine, endLine);
    }

    for (size_t j = 0; j <= height; j+=cellSize) {
        QPoint startLine(startBoard.x() + j, startBoard.y());
        QPoint endLine(startLine.x(), endBoard.y());

        drawLine(startLine, endLine);
    }

    drawLivingCell(1, 1);
    drawDeadCell(1, 2);
}

void BoardPainter::drawCell(size_t cellCoordX, size_t cellCoordY)
{
    auto xStart = startBoard.x() + 1;
    auto yStart = startBoard.y() + 1;

    QPoint startCellCoordToDraw (xStart + cellSize * cellCoordX,
                                 yStart + cellSize * cellCoordY);

    auto cellSizeWithoutGridBorders = cellSize - 2;

    QPoint endCellCoordToDraw(startCellCoordToDraw.x() + cellSizeWithoutGridBorders,
                              startCellCoordToDraw.y() + cellSizeWithoutGridBorders);
    drawRect(startCellCoordToDraw.x(), startCellCoordToDraw.y(), cellSizeWithoutGridBorders, cellSizeWithoutGridBorders);
}

void BoardPainter::drawLivingCell(size_t x, size_t y)
{
    setPen(QPen(QColor("red")));
    setBrush(QBrush(QColor("red")));
    drawCell(x, y);
}

void BoardPainter::drawDeadCell(size_t x, size_t y)
{
    setPen(QPen(QColor("white")));
    setBrush(QBrush(QColor("white")));
    drawCell(x, y);
}

void BoardPainter::drawBoardWithBorders()
{
    constexpr size_t startBoardWithoutBorder = 1;
    const size_t xEndBoardWithoutBorder = boardWithBorders.size() - 1;
    const size_t yEndBoardWithoutBorder = boardWithBorders[0].size() - 1;

    for (size_t i = startBoardWithoutBorder; i < xEndBoardWithoutBorder; ++i)
        for (size_t j = startBoardWithoutBorder; j < yEndBoardWithoutBorder; ++j) {
            const size_t iWithoutBorder = i-1;
            const size_t jWithoutBorder = j-1;

            if(boardWithBorders[i][j])
                drawLivingCell(iWithoutBorder, jWithoutBorder);
            else drawDeadCell(iWithoutBorder, jWithoutBorder);
        }
}


