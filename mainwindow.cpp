#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <unistd.h>
#include <thread>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include "rleparser.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    game.setGliderBoard();
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::paintEvent(QPaintEvent *event)
{
    BoardPainter painter(this, game.getBoardWithBorders(), CELL_SIZE, X_START_DRAW, Y_START_DRAW);
    painter.DrawBoard();

    constexpr int milisecondsInSecond = 1000;
    int timeToFps = milisecondsInSecond / ui->fpsChanger->value();


    if(ui->startStopCheck->isChecked()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(timeToFps));
        game.nextIteration();
        update();
    }
}

void MainWindow::on_nextIterationButton_clicked()
{
    game.nextIteration();
    update();
}

void MainWindow::on_startStopCheck_stateChanged(int arg1)
{
    update();
}

void MainWindow::on_readFileButton_clicked()
{
    const QString filter("RLE (*.rle)");
    QString filePath = QFileDialog::getOpenFileName(this, "Open RLE file", ".", filter);
    QFile file(filePath);

    if(not file.open(QFile::ReadOnly))
        QMessageBox::warning(this, "File not open", "Can't open this file");
    else {
        QTextStream fileStream(&file);

        auto newBoard = RLEParser().parse(fileStream);

        for (int i = 0; i < newBoard.size(); ++i) {
            for (int j = 0; j < newBoard[0].size(); ++j)
                qDebug() << newBoard[i][j] << ' ';

            qDebug() << '\n';
        }

        game = GameOfLife(newBoard);
        update();
    }
}
