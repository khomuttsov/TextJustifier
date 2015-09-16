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
    puts("");
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
    puts("");
}
