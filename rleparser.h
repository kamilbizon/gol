#ifndef RLEPARSER_H
#define RLEPARSER_H

#include <vector>
#include <QTextStream>

class RLEParser
{
public:
    RLEParser();

    std::vector<std::vector<bool>> parse(QTextStream &fileStream);
private:
    auto skipComments(QTextStream &fileStream);
    auto readFirstLine(QString& line);
    void fillNewBoard(std::vector<std::vector<bool>>& newBoard, QTextStream &fileStream, int rowSize, int numRows);
    auto findSections(QString &line);
    auto findEmptyLines(QString section);
    void addCells(std::vector<std::vector<bool>>& newBoard, size_t row, int numberOfCells, QChar tag);
    void fillEmptyLines(std::vector<std::vector<bool> > &newBoard, size_t startRow, int rowSize, int numberOfEmptyLines);
};

#endif // RLEPARSER_H
