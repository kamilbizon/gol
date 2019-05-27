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

    constexpr int MILISECONDS_IN_SECOND = 1000; // never changing value
    int timeToFps = MILISECONDS_IN_SECOND / ui->fpsChanger->value();


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
    qDebug() << ui->startStopCheck->isChecked();
    if(ui->startStopCheck->isChecked()) {
        ui->readFileButton->setEnabled(false);
    }
    else {
        ui->readFileButton->setEnabled(true);
    }

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

        game = GameOfLife(newBoard);
        update();
    }
}
