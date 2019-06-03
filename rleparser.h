#ifndef RLEPARSER_H
#define RLEPARSER_H

#include <vector>
#include <QTextStream>
#include <QFile>
//#include "gameoflife.h"

using GoLBoard = std::vector<std::vector<bool>>;

class RLEParser
{
public:
    RLEParser();

    GoLBoard parseFile(QTextStream &fileStream);
    void parseBoardToRLE(GoLBoard, QTextStream &fileStream);

private:
    auto skipComments(QTextStream &fileStream); // m: return auto
    auto readFirstLine(QString& line);
    void fillNewBoard(GoLBoard & newBoard, QTextStream &fileStream, int rowSize, int numRows);
    auto findSections(QString &line);
    auto findEmptyLines(QString section);
    void addCells(GoLBoard& newBoard, size_t row, int numberOfCells, QChar tag);
    void fillEmptyLines(GoLBoard &newBoard, size_t startRow, int rowSize, int numberOfEmptyLines);

    QString buildBasicRLEFile(GoLBoard &boardWithBorders);
    QString addHeader(GoLBoard &boardWithBorders);
    QString parseBoardCells(GoLBoard &boardWithBorders);
    void removeEmptyLines(QString &rleToSave, const unsigned long boardRowSizeWithoutBorders);
    void ignoreHeader(QStringList &lines);
};

#endif // RLEPARSER_H
