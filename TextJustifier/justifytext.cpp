#include "justifytext.h"

QString justify(const QString& text, int textWidth)
{
    QStringList lines = text.split(QRegularExpression("(\\r\\n|\\n|\\r)"));
    int linesAmount = lines.size();

    for (int i = 0; i < linesAmount; ++i)
    {
        int length = lines[i].length();

        if (length > textWidth)
        {
        }
        else
        {
            // Удлинить только не последнюю строку.
            if (linesAmount == 1 || i != linesAmount - 1)
            {
                lines[i] = fillSpaces(lines[i], textWidth);
            }
        }
    }

    return linesAmount > 1 ? lines.join('\n') : lines.join("");
}

QString fillSpaces(const QString& str, int textWidth)
{
    if (str.length() >= textWidth)
    {
        return str;
    }

    QStringList parts = str.split(' ', QString::SkipEmptyParts);
    int partsAmount = parts.length();

    // Получить длину непробельных символов.
    int partsLength = 0;
    for (auto part : parts)
    {
        partsLength += part.length();
    }

    int spaceAmountToAdd = textWidth - partsLength;
    int spaceAmountToAddAfterEachPart = spaceAmountToAdd / (partsAmount - 1);

    // Добавить по одному пробелу к частям, количество которых равно remainder.
    int remainder = spaceAmountToAdd % (partsAmount - 1);
    for (auto& part : parts)
    {
        if (remainder != 0)
        {
            part += ' ';
            --remainder;
        }
    }

    // Пройтись по каждой части и добавить к результату эту часть плюс нужное количество пробелов.
    QString formattedLine;
    QString spaces = QString(spaceAmountToAddAfterEachPart, ' ');
    for (int i = 0; i < partsAmount - 1; ++i)
    {
        formattedLine += parts[i] + spaces;
    }
    formattedLine += parts[partsAmount - 1];

    return formattedLine;
}
