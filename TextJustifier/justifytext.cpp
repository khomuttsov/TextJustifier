#include "justifytext.h"

void justify(QStringList& text, int textWidth)
{
    int linesAmount = text.size();

    for (int i = 0; i < linesAmount; ++i)
    {
        int length = text[i].length();

        if (length > textWidth)
        {
        }
        // Удлинить только не последнюю строку.
        else if (linesAmount == 1 || i != linesAmount - 1)
        {
            fillSpaces(text[i], textWidth);
        }
    }
}

void fillSpaces(QString& str, int textWidth)
{
    if (str.length() >= textWidth)
    {
        return;
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

    // Пройтись по каждой части и добавить к результату эту часть плюс нужное количество пробелов.
    str.clear();
    QString spaces = QString(spaceAmountToAddAfterEachPart, ' ');
    for (int i = 0; i < partsAmount - 1; ++i)
    {
        if (remainder != 0)
        {
            parts[i] += ' ';
            --remainder;
        }

        str += parts[i] + spaces;
    }
    str += parts[partsAmount - 1];
}

void breakLine(QString& str, QString& after, int textWidth)
{
    // Найти максимальный индекс, не превышающий limit.
    //int breaksAmount = copy.count(replacementSymbols);
    //int result = 0;
    //do
    //{
    //    int lastIndex = copy.lastIndexOf('\1');
    //    copy = copy.remove(lastIndex, 1);
    //    result = lastIndex - breaksAmount--;
    //} while (result > limit);

    //return result;
}

void placeHyphens(const QString& word, QString& hyphenWord)
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
    hyphenWord = word;
    for (auto rule : rules)
    {
        replaceAll(hyphenWord, rule, replaceOn);
    }
}
