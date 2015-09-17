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

int findWordBreak(const QString& str, int limit)
{
    QString alphabet = "[абвгдеёжзийклмнопрстуфхцчшщъыьэюя]";
    QString vowel = "[аеёиоуыэюя]";
    QString consonant = "[бвгджзйклмнпрстфхцчшщ]";
    QString special = "[ьъй]";
    QString replacementSymbols = "\1";

    // Правила для расстановки мягких переносов.
    typedef QRegularExpression RX;
    QString replaceOn = "\\1" + replacementSymbols + "\\2";
    QVector<QRegularExpression> rules =
    {
        RX("(" + special + ")" + "(" + alphabet + alphabet + ")", RX::CaseInsensitiveOption),
        RX("(" + vowel + ")" + "(" + vowel + alphabet + ")", RX::CaseInsensitiveOption),
        RX("(" + vowel + consonant + ")" + "(" + consonant + vowel + ")", RX::CaseInsensitiveOption),
        RX("(" + consonant + vowel + ")" + "(" + consonant + vowel + ")", RX::CaseInsensitiveOption),
        RX("(" + vowel + consonant + ")" + "(" + consonant + consonant + vowel + ")", RX::CaseInsensitiveOption),
        RX("(" + vowel + consonant + consonant + ")" + "(" + consonant + consonant + vowel + ")", RX::CaseInsensitiveOption)
    };

    // Вспомогательная функция для замены символов в строке str по правилу rx, на
    // replacement. Стандартный подход str.replace(rx, replacement) сделает не полную замену
    // ("об-лачный" вместо "об-лач-ный").
    auto replaceAll = [](QString& str, const QRegularExpression& rx, const QString& replacement)
    {
        QString temp;
        do
        {
            temp = str;
            str.replace(rx, replacement);
        } while (temp != str);
    };

    // Расставить мягкие переносы.
    QString copy = str;
    for (auto rule : rules)
    {
        replaceAll(copy, rule, replaceOn);
    }

    // Найти максимальный индекс, не превышающий limit.
    int breaksAmount = copy.count(replacementSymbols);
    int result = 0;
    do
    {
        int lastIndex = copy.lastIndexOf('\1');
        copy = copy.remove(lastIndex, 1);
        result = lastIndex - breaksAmount--;
    } while (result > limit);

    return result;
}
