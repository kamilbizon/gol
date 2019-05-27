#ifndef RLEPARSER_H
#define RLEPARSER_H

#include <vector>
#include <QTextStream>
#include "gameoflife.h"

class RLEParser
{
public:
    RLEParser();

    GoLBoard parse(QTextStream &fileStream);
private:
    auto skipComments(QTextStream &fileStream);
    auto readFirstLine(QString& line);
    void fillNewBoard(GoLBoard & newBoard, QTextStream &fileStream, int rowSize, int numRows);
    auto findSections(QString &line);
    auto findEmptyLines(QString section);
    void addCells(GoLBoard& newBoard, size_t row, int numberOfCells, QChar tag);
    void fillEmptyLines(GoLBoard &newBoard, size_t startRow, int rowSize, int numberOfEmptyLines);
};

#endif // RLEPARSER_H
