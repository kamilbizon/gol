#ifndef BOARDPAINTER_H
#define BOARDPAINTER_H

#include <QPainter>
#include <QMainWindow>
#include "gameoflife.h"
#include <memory>

class BoardPainter: QPainter
{
public:
    BoardPainter(QPaintDevice * p_device,
                 const GoLBoard &p_board,
                 size_t p_cellSize,
                 size_t p_xStartBoard, size_t p_yStartBoard);

    void DrawBoard();


private:
    const size_t cellSize;
    const GoLBoard boardWithBorders;
    const size_t width, height;
    const QPoint startBoard, endBoard;

    void drawGrid();

    void drawCell(size_t x, size_t y);
    void drawLivingCell(size_t x, size_t y);
    void drawDeadCell(size_t x, size_t y);

    void drawBoardWithBorders();
};

#endif // BOARDPAINTER_H
