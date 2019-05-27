#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QTime>
#include <memory>
#include "boardpainter.h"
#include "gameoflife.h"

constexpr int CELL_SIZE = 7; // easy to change values known at compile time
constexpr int BOARD_SIZE = 100;
constexpr int X_START_DRAW = 150;
constexpr int Y_START_DRAW = 20;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    virtual void paintEvent(QPaintEvent *event);

private slots:

    void on_nextIterationButton_clicked();

    void on_startStopCheck_stateChanged(int arg1);

    void on_readFileButton_clicked();

private:
    GameOfLife game{BOARD_SIZE, BOARD_SIZE};
    Ui::MainWindow *ui;

    QTime timer;
};

#endif // MAINWINDOW_H
