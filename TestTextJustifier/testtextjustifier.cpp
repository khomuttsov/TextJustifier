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
