#include "testtextjustifier.h"

void TestTextJustifier::testFillSpaces_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<int>("textWidth");
    QTest::addColumn<QString>("expectation");

    QTest::newRow("One line, line is shorter than text width")
        << "Этот текст меньше восьмидесяти символов"
        << 43
        << "Этот  текст  меньше  восьмидесяти  символов";

    QTest::newRow("One line, line is shorter than text width, there are not only one space after each part")
        << "Этот  текст меньше восьмидесяти символов"
        << 44
        << "Этот   текст  меньше  восьмидесяти  символов";

    QTest::newRow("One line, line is shorter than text width, length of space gaps between parts will be different")
        << "Этот текст меньше восьмидесяти символов."
        << 43
        << "Этот  текст  меньше  восьмидесяти символов.";

    QTest::newRow("One word in line")
        << "Слово"
        << 10
        << "Слово     ";

    QTest::newRow("Empty string")
        << ""
        << 1
        << "";

    QTest::newRow("String length is less than user text width")
        << "Какая-то строка"
        << 20
        << "Какая-то      строка";

    QTest::newRow("There are odd number of words")
        << "Раз, два, три"
        << 14
        << "Раз,  два, три";
}

void TestTextJustifier::testFillSpaces()
{
    QFETCH(QString, text);
    QFETCH(int, textWidth);
    QFETCH(QString, expectation);

    fillSpaces(text, textWidth);
    QString message = QString("\nExpected:\n\"%1\"\n\nReal:\n\"%2\"\n").arg(expectation).arg(text);

    QVERIFY2(text == expectation, message.toLocal8Bit().data());
}

void TestTextJustifier::testJustify_data()
{
    QTest::addColumn<QStringList>("text");
    QTest::addColumn<int>("textWidth");
    QTest::addColumn<QStringList>("expectation");

    QTest::newRow("Two lines, each line is shorter than text width, but last line shouldn't be formatted")
        << QStringList{"Этот  текст меньше восьмидесяти символов.", "И этот тоже."}
        << 43
        << QStringList{"Этот  текст  меньше  восьмидесяти символов.", "И этот тоже."};

    QTest::newRow("Two lines, each line is longer than text width")
        << QStringList{"Очень длинная строка.", "Это тоже очень длинная строка."}
        << 11
        << QStringList{"Очень длин-", "ная   стро-", "ка.Это  то-", "же    очень", "длинная    ", "строка."};

    QTest::newRow("Empty text")
        << QStringList{""}
        << 11
        << QStringList{""};

    QTest::newRow("One line")
        << QStringList{"Какая-то строка"}
        << 80
        << QStringList{"Какая-то строка"};

    QTest::newRow("Multi line")
        << QStringList{"Какая-то строка", "Еще какая-то строка"}
        << 20
        << QStringList{"Какая-то      строка", "Еще какая-то строка"};

    QTest::newRow("Very long multi line")
        << QStringList{"Очень длинная строка", "Еще одна длинная строка"}
        << 20
        << QStringList{"Очень длинная строка", "Еще   одна   длинная", "строка"};

    QTest::newRow("Mixed multi line text")
        << QStringList{"Короткая строка", "Еще одна длинная строка"}
        << 20
        << QStringList{"Короткая      строка", "Еще   одна   длинная", "строка"};

    QTest::newRow("Formatting with breaking")
        << QStringList{"Раз, два, три, четыре, пять."}
        << 20
        << QStringList{"Раз, два, три, четы-", "ре, пять."};
}

void TestTextJustifier::testJustify()
{
    QFETCH(QStringList, text);
    QFETCH(int, textWidth);
    QFETCH(QStringList, expectation);

    justify(text, textWidth);
    QString message = QString("\nExpected:\n\"%1\"\n\nReal:\n\"%2\"\n").arg(expectation.join('\n')).arg(text.join('\n'));

    QVERIFY2(text == expectation, message.toLocal8Bit().data());
}

void TestTextJustifier::testFindWordBreak_data()
{
    QTest::addColumn<QString>("word");
    QTest::addColumn<QString>("expectation");

    QTest::newRow("Облачный")
        << "облачный"
        << "об\1лач\1ный";

    QTest::newRow("Лапа")
        << "лапа"
        << "ла\1па";

    QTest::newRow("Круглый")
        << "круглый"
        << "круг\1лый";

    QTest::newRow("Стрела")
        << "стрела"
        << "стре\1ла";

    QTest::newRow("Жидкость")
        << "жидкость"
        << "жид\1кость";

    QTest::newRow("Подъезд")
        << "подъезд"
        << "подъ\1езд";

    QTest::newRow("Польза")
        << "польза"
        << "поль\1за";

    QTest::newRow("Война")
        << "война"
        << "вой\1на";

    QTest::newRow("Район")
        << "район"
        << "рай\1он";

    QTest::newRow("Разыскать")
        << "разыскать"
        << "ра\1зыс\1кать";

    QTest::newRow("Розыгрыш")
        << "розыгрыш"
        << "ро\1зыг\1рыш";

    QTest::newRow("Классы")
        << "классы"
        << "клас\1сы";

    QTest::newRow("Промышленность")
        << "промышленность"
        << "про\1мыш\1лен\1ность";
}

void TestTextJustifier::testFindWordBreak()
{
    QFETCH(QString, word);
    QFETCH(QString, expectation);
    QString hyphenWord;

    placeHyphens(word, hyphenWord);
    QString message = QString("\nExpected:\n\"%1\"\n\nReal:\n\"%2\"\n").arg(expectation).arg(hyphenWord);

    QVERIFY2(hyphenWord == expectation, message.toLocal8Bit().data());
}

void TestTextJustifier::testBreakLine_data()
{
    typedef QPair<QString, QString> Expectations;

    QTest::addColumn<QString>("str");
    QTest::addColumn<int>("textWidth");
    QTest::addColumn<Expectations>("expectation");

    QTest::newRow("String is longer than textWidth, break is in the middle of the word")
        << "1. Найти слово, которое находится на границе ширины, переданной пользователем"
        << 20
        << Expectations{"1.  Найти слово, ко-", "торое находится на границе ширины, переданной пользователем"};

    QTest::newRow("There is only one word in the string")
        << "Экскаваторы"
        << 10
        << Expectations{"Экскавато-", "ры"};

    QTest::newRow("Punctuation mark is after textWidth")
        << "Привет,"
        << 6
        << Expectations{"При-  ", "вет,"};

    QTest::newRow("String length is more than text width")
        << "Очень большая строка"
        << 8
        << Expectations{"Очень   ", "большая строка"};

    QTest::newRow("Empty string")
        << ""
        << 0
        << Expectations{"", ""};

    QTest::newRow("String length is less than text width")
        << "Короткая строка"
        << 80
        << Expectations{"Короткая строка", ""};

    QTest::newRow("String length is more than text width")
        << "Длинная строка"
        << 10
        << Expectations{"Длинная   ", "строка"};

    QTest::newRow("There are even number of words")
        << "Два слова"
        << 8
        << Expectations{"Два сло-", "ва"};

    QTest::newRow("Break is on the middle of the word")
        << "Длинная строка"
        << 13
        << Expectations{"Длинная стро-", "ка"};

    QTest::newRow("It's no possible to break line in the word")
        << "Длинная строка"
        << 10
        << Expectations{"Длинная   ", "строка"};

    QTest::newRow("Punctuation mark is not fit to text width, but word, which it follows, does")
        << "Длинная, длинная строка"
        << 7
        << Expectations{"Длин-  ", "ная, длинная строка"};

    QTest::newRow("Word length is longer than text width")
        << "Прибавка"
        << 6
        << Expectations{"При-  ", "бавка"};

    QTest::newRow("Word is on the edge of the text width")
        << "Прибавка к пенсии"
        << 10
        << Expectations{"Прибавка к", "пенсии"};

    QTest::newRow("Word is on the edge of the text width 2")
        << "Прибавка к пенсии"
        << 9
        << Expectations{"Прибавка ", "к пенсии"};

    QTest::newRow("Final test 1")
        << "Полное восстановление"
        << 7
        << Expectations{"Полное ", "восстановление"};

    QTest::newRow("Final test 2")
        << "Полное восстановление"
        << 9
        << Expectations{"Полное   ", "восстановление"};

    QTest::newRow("Final test 3")
        << "Полное восстановление"
        << 10
        << Expectations{"Полное    ", "восстановление"};

    QTest::newRow("Final test 4")
        << "Полное восстановление"
        << 11
        << Expectations{"Полное вос-", "становление"};

    QTest::newRow("Final test 5")
        << "Полное восстановление"
        << 12
        << Expectations{"Полное  вос-", "становление"};

    QTest::newRow("Final test 6")
        << "Полное восстановление"
        << 13
        << Expectations{"Полное   вос-", "становление"};

    QTest::newRow("Final test 7")
        << "Полное восстановление"
        << 14
        << Expectations{"Полное восста-", "новление"};

    QTest::newRow("Final test 8")
        << "Полное восстановление"
        << 15
        << Expectations{"Полное  восста-", "новление"};

    QTest::newRow("Final test 9")
        << "Полное восстановление"
        << 16
        << Expectations{"Полное   восста-", "новление"};

    QTest::newRow("Final test 10")
        << "Полное восстановление"
        << 17
        << Expectations{"Полное восстанов-", "ление"};

    QTest::newRow("Final test 11")
        << "Полное восстановление"
        << 18
        << Expectations{"Полное  восстанов-", "ление"};

    QTest::newRow("Final test 12")
        << "Полное восстановление"
        << 19
        << Expectations{"Полное восстановле-", "ние"};

    QTest::newRow("Final test 13")
        << "Полное восстановление"
        << 20
        << Expectations{"Полное  восстановле-", "ние"};

    QTest::newRow("Final test 14")
        << "Полное восстановление"
        << 21
        << Expectations{"Полное восстановление", ""};
}

void TestTextJustifier::testBreakLine()
{
    typedef QPair<QString, QString> Expectations;

    QFETCH(QString, str);
    QFETCH(int, textWidth);
    QFETCH(Expectations, expectation);
    QString after;

    breakLine(str, after, textWidth);
    QString message = QString("\nExpected:\n\"%1\n%2\"\n\nReal:\n\"%3\n%4\"\n").arg(expectation.first).arg(expectation.second).arg(str).arg(after);

    QVERIFY2(str == expectation.first && after == expectation.second, message.toLocal8Bit().data());
}