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
        ++startRow;
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
        if(section.compare("!") == 0)
            break;

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
            auto nextRow = row + 1;
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

QString RLEParser::addHeader(GoLBoard &boardWithBorders)
{
    const unsigned long rowSizeWithBorders = boardWithBorders[0].size();
    const unsigned long columnSizeWithBorders = boardWithBorders.size();

    return QStringLiteral("x = %1, y = %2\n").arg(rowSizeWithBorders).arg(columnSizeWithBorders);
}

QString RLEParser::parseBoardCells(GoLBoard &boardWithBorders)
{
    QString rleToSave{""};

    bool lastCellSign;
     lastCellSign = boardWithBorders[0][0];

    unsigned numberOfTheSameSignsInRow = 0;

    for(size_t i = 0; i < boardWithBorders.size(); i++) {
        for(size_t j = 0; j < boardWithBorders.size(); j++) {
            if(boardWithBorders[i][j] == lastCellSign)
                ++numberOfTheSameSignsInRow;
            else {
                if(lastCellSign){

                    if(numberOfTheSameSignsInRow == 1)
                        rleToSave += "o";
                    else
                        rleToSave += QString::number(numberOfTheSameSignsInRow) + "o";


                    numberOfTheSameSignsInRow = 1;
                    lastCellSign = boardWithBorders[i][j];
                }
                else {

                    if(numberOfTheSameSignsInRow == 1)
                        rleToSave += "b";
                    else
                        rleToSave += QString::number(numberOfTheSameSignsInRow) + "b";

                    numberOfTheSameSignsInRow = 1;
                    lastCellSign = boardWithBorders[i][j];
                }
            }
        }

        if(numberOfTheSameSignsInRow != 0) {
            if(lastCellSign){
                rleToSave += QString::number(numberOfTheSameSignsInRow) + "o";

                numberOfTheSameSignsInRow = 0;

                if(i < boardWithBorders.size()-1)
                    lastCellSign = boardWithBorders.at(i+1).at(0);
            }
            else {
                rleToSave += QString::number(numberOfTheSameSignsInRow) + "b";

                numberOfTheSameSignsInRow = 0;

                if(i < boardWithBorders.size()-1)
                    lastCellSign = boardWithBorders.at(i+1).at(0);
            }
        }

        rleToSave += "$";
    }

    rleToSave += "!";

    return rleToSave;
}

QString RLEParser::buildBasicRLEFile(GoLBoard &boardWithBorders)
{
    QString rleToSave{""};

    rleToSave += addHeader(boardWithBorders);
    rleToSave += parseBoardCells(boardWithBorders);

    return rleToSave;
}

void RLEParser::ignoreHeader(QStringList &lines)
{
    lines[0].remove(0, lines[0].indexOf("\n") + 1);
}

void RLEParser::removeEmptyLines(QString &rleToSave, const unsigned long boardRowSizeWithBorders)
{
    auto lines = rleToSave.split("$");
    ignoreHeader(lines);

    QString emptyLine = QStringLiteral("%1b").arg(boardRowSizeWithBorders);

    auto numberOfEmptyLines = 0;

    for(auto& line : lines) {

        if(line.compare(emptyLine) == 0)
            ++numberOfEmptyLines;

        else if(numberOfEmptyLines > 0) {

            auto startEmptyLinesBlock = rleToSave.indexOf(emptyLine);
            auto emptyLinesBlockLength = QString(emptyLine + "$").size() * numberOfEmptyLines;
            QString toReplace = QStringLiteral("%1$").arg(numberOfEmptyLines);

            rleToSave.replace(startEmptyLinesBlock, emptyLinesBlockLength, toReplace);

            numberOfEmptyLines = 0;
        }
    }
}

void RLEParser::parseBoardToRLE(GoLBoard boardWithBorders, QTextStream &fileStream)
{
    auto rleToSave = buildBasicRLEFile(boardWithBorders);

    const auto boardRowSizeWithBorders = boardWithBorders[0].size();
    removeEmptyLines(rleToSave, boardRowSizeWithBorders);

    fileStream << rleToSave;
}

