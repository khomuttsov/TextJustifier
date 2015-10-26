#include <iostream>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include "justifytext.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Invalid arguments number" << std::endl;
        return -1;
    }

    QFile inputFile(argv[1]);
    if (!inputFile.open(QIODevice::ReadOnly))
    {
        std::cerr << "Couldn't open file: " << argv[1] << std::endl;
        return -1;
    }

    QTextStream in(&inputFile);
    QStringList text = in.readAll().split("\r\n");

    int textWidth = atoi(argv[2]);
    justify(text, textWidth);

    QFile outputFile("result.txt");
    if (!outputFile.open(QIODevice::WriteOnly))
    {
        std::cerr << "Couldn't write to file: " << "result.txt" << std::endl;
        return -1;
    }

    QTextStream out(&outputFile);
    out << text.join("\r\n");

    return 0;
}
