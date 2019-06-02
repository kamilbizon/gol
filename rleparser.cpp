#include "rleparser.h"
#include <QDebug>

using std::vector;

RLEParser::RLEParser()
{

}

auto RLEParser::skipComments(QTextStream &fileStream)
{
    QString line;
    while(not fileStream.atEnd()) {
        line = fileStream.readLine();
        if(line[0] == '#')
            continue;
        break;
    }
    return line;
}

auto RLEParser::readFirstLine(QString &line)
{
    auto xSizePart = line.section(',', 0, 0);
    auto xSize = xSizePart.section('=', 1, 1);

    auto ySizePart = line.section(',', 1, 1);
    auto ySize = ySizePart.section('=', 1, 1);


    auto typeGoLPart = line.section(',', 2, 2);
    if(not typeGoLPart.isEmpty()) {
        auto typeGoL = typeGoLPart.section('=', 1, 1);
        typeGoL.remove(" ");
        if(typeGoL.compare("B3/S23"))
            throw -1;
    }

    return std::make_pair(xSize.toInt(), ySize.toInt());
}

auto RLEParser::findSections(QString &line)
{
    vector<QString> sections;

    for (int i=0 ;; ++i) {
        auto section = line.section('$', i, i);
        if(section.isEmpty())
            break;
        sections.push_back(section);
    }

    return sections;
}

auto RLEParser::findEmptyLines(QString section)
{
    int numberOfEmptyLines = section.toInt();

    constexpr int normalLineEndingSymbol = 1;

    return numberOfEmptyLines - normalLineEndingSymbol;
}

void RLEParser::addCells(GoLBoard &newBoard, size_t row, int numberOfCells, QChar tag)
{
    if(tag == 'b')
        while (numberOfCells--)
            newBoard[row].push_back(false);
    else
        while (numberOfCells--)
            newBoard[row].push_back(true);
}

void RLEParser::fillEmptyLines(GoLBoard &newBoard, size_t startRow, int rowSize, int numberOfEmptyLines)
{
    while(numberOfEmptyLines > 0){
        addCells(newBoard, startRow, rowSize, 'b');
        --numberOfEmptyLines;
    }
}

void RLEParser::fillNewBoard(GoLBoard &newBoard, QTextStream &fileStream, int rowSize, int numRows)
{
    size_t row = 0;

    QString allLines;

    while(not fileStream.atEnd())
        allLines += fileStream.readLine();

    auto sections = findSections(allLines);

    for (auto& section : sections) {


        int numOfUnfilledCells = rowSize;

        while(1) {
            auto tagIndex = section.indexOf(QRegExp("[ob]"));

            int numberOfCells;

            if(tagIndex == -1)
                break;
            else if(tagIndex == 0)
                numberOfCells = 1;
            else
                numberOfCells = section.left(tagIndex).toInt();


            addCells(newBoard, row, numberOfCells, section[tagIndex]);
            section.remove(0, tagIndex+1);

            numOfUnfilledCells -= numberOfCells;
        }

        if(numOfUnfilledCells > 0)
            addCells(newBoard, row, numOfUnfilledCells, 'b');


        auto numberOfEmptyLines = findEmptyLines(section);

        if(numberOfEmptyLines > 0){
            int nextRow = row + 1;
            fillEmptyLines(newBoard, nextRow, rowSize, numberOfEmptyLines);
            row += numberOfEmptyLines;
        }

        ++row;
    }


    while(row < numRows) {
        addCells(newBoard, row, rowSize, 'b');
        ++row;
    }
}

GoLBoard RLEParser::parseFile(QTextStream &fileStream)
{
    auto firstLine = skipComments(fileStream);
    auto boardSize = readFirstLine(firstLine);

    vector<vector<bool>> newBoard(static_cast<size_t>(boardSize.second));
    fillNewBoard(newBoard, fileStream, boardSize.first, boardSize.second);

    return newBoard;
}

