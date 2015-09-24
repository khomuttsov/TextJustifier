#include "justifytext.h"

void justify(QStringList& text, int textWidth)
{
    int linesAmount = text.size();

    for (int i = 0; i < linesAmount; ++i)
    {
        int length = text[i].length();

        if (length > textWidth)
        {
            QString after;
            breakLine(text[i], after, textWidth);

            if (i != linesAmount - 1)
            {
                text[i + 1] = after + text[i + 1];
            }
            else
            {
                text << after;

                if (text.length() != linesAmount)
                {
                    justify(text, textWidth);
                }
            }
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
    int spaceAmountToAddAfterEachPart = partsAmount != 1 ?
        spaceAmountToAdd / (partsAmount - 1) :
        spaceAmountToAdd / partsAmount;

    // Добавить по одному пробелу к частям, количество которых равно remainder.
    int remainder = partsAmount != 1 ?
        spaceAmountToAdd % (partsAmount - 1) :
        spaceAmountToAdd % partsAmount;

    // Пройтись по каждой части и добавить к результату эту часть плюс нужное количество пробелов.
    str.clear();
    QString spaces = QString(spaceAmountToAddAfterEachPart, ' ');
    int limit = partsAmount != 1 ? partsAmount - 1 : partsAmount;
    for (int i = 0; i < limit; ++i)
    {
        if (remainder != 0)
        {
            parts[i] += ' ';
            --remainder;
        }

        str += parts[i] + spaces;
    }

    if (partsAmount != 1)
    {
        str += parts[partsAmount - 1];
    }
}

void breakLine(QString& str, QString& after, int textWidth)
{
    if (str.length() <= textWidth)
    {
        return;
    }

    // Найти слово, которое находится на границе textWidth.
    int beginWordPosition = str.lastIndexOf(' ', textWidth - 1) + 1;
    int endWordPosition = str.indexOf(' ', textWidth - 1) - 1;

    // Если в строке одно слово.
    if (beginWordPosition == 0)
    {
        beginWordPosition = 0;
    }
    if (endWordPosition == -2)
    {
        endWordPosition = str.length() - 1;
    }

    int wordLength = endWordPosition - beginWordPosition + 1;
    QString word = str.mid(beginWordPosition, wordLength);

    // Расставить в слове мягкие переносы.
    placeHyphens(word, word);

    // Вычислить максимальную длину слова до переноса, как разницу ширины текста и индекса начала слова.
    int maxWordLengthBeforeBreak = textWidth - beginWordPosition - 1;

    // Заменить последний мягкий перенос на '\2' в позиции, не превышающей максимальную длину слова до переноса. Замена производится именно на '\2', а не на знак дефиса, т. к. в строке уже может быть знак дефиса. Иначе не получится легко определить индекс, где нужно разделить строки на две.
    int breaksCount = word.count('\1');
    int breakIndex = word.lastIndexOf('\1', maxWordLengthBeforeBreak - 1 + breaksCount);

    // В слове можно сделать перенос с учетом ограничения на длину.
    if (breakIndex != -1)
    {
        word[breakIndex] = '\2';
        word.replace('\1', "");

        // Вставить слово с переносом в строку.
        str.remove(beginWordPosition, wordLength);
        str.insert(beginWordPosition, word);

        // Разделить строку на две.
        QStringList strings = str.split('\2');
        str = strings[0] + "-";
        fillSpaces(str, textWidth);
        after = strings[1];
    }
    // В слове нельзя сделать перенос с учетом ограничения на длину.
    // Разделяем строку по границе начала граничного слова.
    else
    {
        after = str.right(str.length() - beginWordPosition);
        str.remove(beginWordPosition, beginWordPosition + 1000);
        fillSpaces(str, textWidth);
    }
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
