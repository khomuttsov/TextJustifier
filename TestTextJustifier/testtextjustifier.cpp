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

    QString result = fillSpaces(text, textWidth);
    QString message = QString("\nExpected:\n\"%1\"\n\nReal:\n\"%2\"\n").arg(expectation).arg(result);

    QVERIFY2(result == expectation, message.toLocal8Bit().data());
}

void TestTextJustifier::testJustify_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<int>("textWidth");
    QTest::addColumn<QString>("expectation");

    QTest::newRow("Two lines, each line is shorter than text width, but last line shouldn't be formatted")
        << R"(Этот  текст меньше восьмидесяти символов.
И  этот тоже.)"
<< 43
<< R"(Этот  текст  меньше  восьмидесяти символов.
И  этот тоже.)";
}

void TestTextJustifier::testJustify()
{
    QFETCH(QString, text);
    QFETCH(int, textWidth);
    QFETCH(QString, expectation);

    QString result = justify(text, textWidth);
    QString message = QString("\nExpected:\n\"%1\"\n\nReal:\n\"%2\"\n").arg(expectation).arg(result);

    QVERIFY2(result == expectation, message.toLocal8Bit().data());
}

void TestTextJustifier::testFindWordBreak_data()
{
    QTest::addColumn<QString>("word");
    QTest::addColumn<QVector<int>>("limits");
    QTest::addColumn<QVector<int>>("expectations");

    QTest::newRow("Облачный")
        << "облачный"
        << QVector<int>{6, 8, 3, 1, 0}
        << QVector<int>{4, 4, 1, 1, -1};

    QTest::newRow("Лапа")
        << "лапа"
        << QVector<int>{100, 0}
        << QVector<int>{1, -1};
}

void TestTextJustifier::testFindWordBreak()
{
    QFETCH(QString, word);
    QFETCH(QVector<int>, limits);
    QFETCH(QVector<int>, expectations);

    int length = limits.length();

    for (int i = 0; i < length; ++i)
    {
        int result = findWordBreak(word, limits[i]);
        QString message = QString("\nExpected:\n\"%1\"\n\nReal:\n\"%2\"\n").arg(expectations[i]).arg(result);

        QVERIFY2(result == expectations[i], message.toLocal8Bit().data());
    }
}
