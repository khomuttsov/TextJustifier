#include "justifytext.h"

void justify(QStringList& text, int textWidth)
{
    for (int i = 0; i < text.length(); ++i)
    {
        int length = text[i].length();

        // Разбить строку.
        if (length > textWidth)
        {
            QString after;
            breakLine(text[i], after, textWidth);

            // Если строка не последняя, вставить часть после переноса в
            // начало следующей строки.
            if (i != text.length() - 1)
            {
                text[i + 1] = after + ' ' + text[i + 1];
            }
            else
            {
                text << after;
            }
        }
        // Удлинить только не первую и не последнюю строку.
        else if (text.length() != 1 &&
            (text.length() == 1 || i != text.length() - 1))
        {
            fillSpaces(text[i], textWidth);
        }
    }
}

void fillSpaces(QString& str, int textWidth)
{
    if (str.length() == 0 || str.length() == textWidth)
    {
        return;
    }

    if (str.length() > textWidth)
    {
        throw new std::invalid_argument("Ширина строки не может быть меньше ширины текста");
    }

    QStringList parts = str.split(' ', QString::SkipEmptyParts);
    int partsAmount = parts.length();

    // Получить длину непробельных символов.
    int partsLength = 0;

	QList<QString>::iterator i = parts.begin();
    for (; i != parts.end(); ++i)
    {
        partsLength += i->length();
    }

    int spaceAmountToAdd = textWidth - partsLength;
    int spaceAmountToAddAfterEachPart = partsAmount != 1 ?
        spaceAmountToAdd / (partsAmount - 1) :
        spaceAmountToAdd / partsAmount;

    // Добавить по одному пробелу к частям, количество которых равно remainder.
    int remainder = partsAmount != 1 ?
        spaceAmountToAdd % (partsAmount - 1) :
        spaceAmountToAdd % partsAmount;

    // Пройтись по каждой части и добавить к результату эту часть плюс нужное
    // количество пробелов.
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

    // Граничное слово находится в конце строки.
    if (endWordPosition == -2)
    {
        endWordPosition = str.length() - 1;
    }

    // Слово состоит из одной буквы.
    if (beginWordPosition == endWordPosition + 2)
    {
        endWordPosition = beginWordPosition;
    }

    int wordLength = endWordPosition - beginWordPosition + 1;
    QString word = str.mid(beginWordPosition, wordLength);

    // Слово начинается на границе ширины текста.
    if (beginWordPosition == textWidth - 1 && wordLength != 1)
    {
        QString temp = str.left(textWidth - 1);
        after = str.right(str.length() - temp.length());

		temp = temp.trimmed();
        if (temp.length() < textWidth)
        {
            fillSpaces(temp, textWidth);
        }
        str = temp.trimmed();
        return;
    }

    // Расставить в слове мягкие переносы.
    placeHyphens(word, word);

    // В слове нельзя сделать перенос.
    if (word.count('\1') == 0)
    {
        // Слово целиком умещается в ширину текста.
        if (beginWordPosition + wordLength <= textWidth)
        {
            after = str.right(str.length() - textWidth).trimmed();
            str.chop(str.length() - textWidth);
			str = str.trimmed();
			if (str.length() < textWidth)
			{
				fillSpaces(str, textWidth);
			}
			str = str.trimmed();
            return;
        }
        else
        {
            QString temp = str.left(textWidth);
            after = str.right(str.length() - temp.length());
            str = temp.trimmed();
			if (str.length() < textWidth)
			{
				fillSpaces(str, textWidth);
			}
			str = str.trimmed();
            return;
        }
    }

    // Вычислить максимальную длину слова до переноса, как разницу индексов
    // максимально допустимого символа, умещающегося в ширину текста, и начала
    // слова (символ переноса не входит в эту длину).
    int maxWordLengthBeforeBreak = (textWidth - 1) - beginWordPosition;

    // Так как в слове появились мягкие переносы, то нужно увеличить
    // максимальную длину слова до переноса на количество символов,
    // встретившихся перед прежней длиной.
    for (int i = 0; i < maxWordLengthBeforeBreak; ++i)
    {
        if (word[i] == '\1')
        {
            ++maxWordLengthBeforeBreak;
        }
    }
    if (word[maxWordLengthBeforeBreak] == '\1')
    {
        ++maxWordLengthBeforeBreak;
    }

    // Индекс переноса.
    int breakIndex = word.lastIndexOf('\1', maxWordLengthBeforeBreak - 1);

    // В слове можно сделать перенос с учетом ограничения на длину.
    if (breakIndex != -1)
    {
        // Заменить мягкий перенос на '\2' в позиции, не превышающей
        // максимальную длину слова до переноса. Замена производится именно на
        // '\2', а не на знак дефиса, т. к. в строке уже может быть знак дефиса.
        // Иначе не получится легко определить индекс, где нужно разделить
        // строки на две.
        word[breakIndex] = '\2';

        // Удалить все мягкие переносы.
        word.replace('\1', "");

        // Вставить слово с переносом в строку.
        str.remove(beginWordPosition, wordLength);
        str.insert(beginWordPosition, word);

        // Разделить строку на две.
        QStringList strings = str.split('\2');
        str = strings[0] + "-";
        if (str.length() < textWidth)
        {
            fillSpaces(str, textWidth);
        }
        after = strings[1];
		str = str.trimmed();
    }
    // В слове нельзя сделать перенос с учетом ограничения на длину.
    // Разделяем строку по границе начала граничного слова.
    else
    {
        after = str.right(str.length() - beginWordPosition);
        str.chop(str.length() - beginWordPosition);
        if (str.length() < textWidth)
        {
            fillSpaces(str, textWidth);
        }
		str = str.trimmed();
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
    QVector<QRegularExpression> rules;
    
    rules.push_back(RX("(" + special + ")" + "(" + alphabet + alphabet + ")", RX::CaseInsensitiveOption));
    rules.push_back(RX("(" + vowel + ")" + "(" + vowel + alphabet + ")", RX::CaseInsensitiveOption));
    rules.push_back(RX("(" + vowel + consonant + ")" + "(" + consonant + vowel + ")", RX::CaseInsensitiveOption));
    rules.push_back(RX("(" + consonant + vowel + ")" + "(" + consonant + vowel + ")", RX::CaseInsensitiveOption));
    rules.push_back(RX("(" + vowel + consonant + ")" + "(" + consonant + consonant + vowel + ")", RX::CaseInsensitiveOption));
    rules.push_back(RX("(" + vowel + consonant + consonant + ")" + "(" + consonant + consonant + vowel + ")", RX::CaseInsensitiveOption));
    

    // Вспомогательная функция для замены символов в строке str по правилу rx,
    // на replacement. Стандартный подход str.replace(rx, replacement) сделает
    // не полную замену ("об-лачный" вместо "об-лач-ный").
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
	QVector<QRegularExpression>::Iterator i = rules.begin();
    for (; i != rules.end(); ++i)
    {
        replaceAll(hyphenWord, *i, replaceOn);
    }
}
