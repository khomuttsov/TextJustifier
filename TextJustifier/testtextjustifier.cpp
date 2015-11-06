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

	QStringList l1;
	l1.push_back("Этот  текст меньше восьмидесяти символов.");
	l1.push_back("И этот тоже.");
	QStringList l1Expect;
	l1Expect.push_back("Этот  текст  меньше  восьмидесяти символов.");
	l1Expect.push_back("И этот тоже.");
    QTest::newRow("Two lines, each line is shorter than text width, but last line shouldn't be formatted")
        << l1
        << 43
        << l1Expect;

	QStringList l2;
	l2.push_back("Очень длинная строка.");
	l2.push_back("Это тоже очень длинная строка.");
	QStringList l2Expect;
	l2Expect.push_back("Очень длин-");
	l2Expect.push_back("ная   стро-");
	l2Expect.push_back("ка. Это то-");
	l2Expect.push_back("же    очень");
	l2Expect.push_back("длинная");
	l2Expect.push_back("строка.");
    QTest::newRow("Two lines, each line is longer than text width")
        << l2
        << 11
        << l2Expect;

	QStringList l3;
	l3.push_back("");
	QStringList l3Expect;
	l3Expect.push_back("");
    QTest::newRow("Empty text")
        << l3
        << 11
        << l3Expect;

	QStringList l4;
	l4.push_back("Какая-то строка");
	QStringList l4Expect;
	l4Expect.push_back("Какая-то строка");
    QTest::newRow("One line")
        << l4
        << 80
        << l4Expect;

	QStringList l5;
	l5.push_back("Какая-то строка");
	l5.push_back("Еще какая-то строка");
	QStringList l5Expect;
	l5Expect.push_back("Какая-то      строка");
	l5Expect.push_back("Еще какая-то строка");
    QTest::newRow("Multi line")
        << l5
        << 20
        << l5Expect;

	QStringList l6;
	l6.push_back("Очень длинная строка");
	l6.push_back("Еще одна длинная строка");
	QStringList l6Expect;
	l6Expect.push_back("Очень длинная строка");
	l6Expect.push_back("Еще   одна   длинная");
	l6Expect.push_back("строка");
    QTest::newRow("Very long multi line")
        << l6
        << 20
        << l6Expect;

	QStringList l7;
	l7.push_back("Короткая строка");
	l7.push_back("Еще одна длинная строка");
	QStringList l7Expect;
	l7Expect.push_back("Короткая      строка");
	l7Expect.push_back("Еще   одна   длинная");
	l7Expect.push_back("строка");
    QTest::newRow("Mixed multi line text")
        << l7
        << 20
        << l7Expect;

	QStringList l8;
	l8.push_back("Раз, два, три, четыре, пять.");
	QStringList l8Expect;
	l8Expect.push_back("Раз, два, три, четы-");
	l8Expect.push_back("ре, пять.");
    QTest::newRow("Formatting with breaking")
        << l8
        << 20
        << l8Expect;

	QStringList l9;
	l9.push_back("Привет пока");
	l9.push_back("пока");
	QStringList l9Expect;
	l9Expect.push_back("Привет по-");
	l9Expect.push_back("ка пока");
    QTest::newRow("Formatting with breaking")
        << l9
        << 10
        << l9Expect;
}

void TestTextJustifier::testJustify()
{
    QFETCH(QStringList, text);
    QFETCH(int, textWidth);
    QFETCH(QStringList, expectation);

    justify(text, textWidth);
    QString message = QString("\nСтрока: %1\nОжидалось:\n\"%2\"\n\nReal:\n\"%3\"\n");

    int resultLength = text.length();
    int expectationLength = expectation.length();
    int length = std::max(resultLength, expectationLength);
    for (int i = 0; i < length; ++i)
    {
        QVERIFY2(text[i] == expectation[i],
            message
            .arg(i + 1)
            .arg(i <= expectationLength ? expectation[i] : "")
            .arg(i <= resultLength ? text[i] : "")
            .toLocal8Bit().data()
            );
    }
}

void TestTextJustifier::testPlaceHyphens_data()
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

void TestTextJustifier::testPlaceHyphens()
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

	Expectations t1;
	t1.first = "1.  Найти слово, ко-";
	t1.second = "торое находится на границе ширины, переданной пользователем";
    QTest::newRow("String is longer than textWidth, break is in the middle of the word")
        << "1. Найти слово, которое находится на границе ширины, переданной пользователем"
        << 20
        << t1;

	Expectations t2;
	t2.first = "Экскавато-";
	t2.second = "ры";
    QTest::newRow("There is only one word in the string")
        << "Экскаваторы"
        << 10
        << t2;

	Expectations t3;
	t3.first = "При-";
	t3.second = "вет,";
    QTest::newRow("Punctuation mark is after textWidth")
        << "Привет,"
        << 6
        << t3;

	Expectations t4;
	t4.first = "Очень";
	t4.second = "большая строка";
    QTest::newRow("String length is more than text width")
        << "Очень большая строка"
        << 8
        << t4;

	Expectations t5;
	t5.first = "";
	t5.second = "";
    QTest::newRow("Empty string")
        << ""
        << 0
        << t5;

	Expectations t6;
	t6.first = "Короткая строка";
	t6.second = "";
    QTest::newRow("String length is less than text width")
        << "Короткая строка"
        << 80
        << t6;

	Expectations t7;
	t7.first = "Длинная";
	t7.second = "строка";
    QTest::newRow("String length is more than text width")
        << "Длинная строка"
        << 10
        << t7;

	Expectations t8;
	t8.first = "Два сло-";
	t8.second = "ва";
    QTest::newRow("There are even number of words")
        << "Два слова"
        << 8
        << t8;

	Expectations t9;
	t9.first = "Длинная стро-";
	t9.second = "ка";
    QTest::newRow("Break is on the middle of the word")
        << "Длинная строка"
        << 13
        << t9;

	Expectations t10;
	t10.first = "Длинная";
	t10.second = "строка";
    QTest::newRow("It's no possible to break line in the word")
        << "Длинная строка"
        << 10
        << t10;

	Expectations t11;
	t11.first = "Длин-";
	t11.second = "ная, длинная строка";
    QTest::newRow("Punctuation mark is not fit to text width, but word, which it follows, does")
        << "Длинная, длинная строка"
        << 7
        << t11;

	Expectations t12;
	t12.first = "При-";
	t12.second = "бавка";
    QTest::newRow("Word length is longer than text width")
        << "Прибавка"
        << 6
        << t12;

	Expectations t13;
	t13.first = "Прибавка к";
	t13.second = "пенсии";
    QTest::newRow("Word is on the edge of the text width")
        << "Прибавка к пенсии"
        << 10
        << t13;

	Expectations t14;
	t14.first = "Прибавка";
	t14.second = "к пенсии";
    QTest::newRow("Word is on the edge of the text width 2")
        << "Прибавка к пенсии"
        << 9
        << t14;

	Expectations t15;
	t15.first = "Полное";
	t15.second = "восстановление";
    QTest::newRow("Final test 1")
        << "Полное восстановление"
        << 7
        << t15;

	Expectations t16;
	t16.first = "Полное";
	t16.second = "восстановление";
    QTest::newRow("Final test 2")
        << "Полное восстановление"
        << 9
        << t16;

	Expectations t17;
	t17.first = "Полное";
	t17.second = "восстановление";
    QTest::newRow("Final test 3")
        << "Полное восстановление"
        << 10
        << t17;

	Expectations t18;
	t18.first = "Полное вос-";
	t18.second = "становление";
    QTest::newRow("Final test 4")
        << "Полное восстановление"
        << 11
        << t18;

	Expectations t19;
	t19.first = "Полное  вос-";
	t19.second = "становление";
    QTest::newRow("Final test 5")
        << "Полное восстановление"
        << 12
        << t19;

	Expectations t20;
	t20.first = "Полное   вос-";
	t20.second = "становление";
    QTest::newRow("Final test 6")
        << "Полное восстановление"
        << 13
        << t20;

	Expectations t21;
	t21.first = "Полное восста-";
	t21.second = "новление";
    QTest::newRow("Final test 7")
        << "Полное восстановление"
        << 14
        << t21;

	Expectations t22;
	t22.first = "Полное  восста-";
	t22.second = "новление";
    QTest::newRow("Final test 8")
        << "Полное восстановление"
        << 15
        << t22;

	Expectations t23;
	t23.first = "Полное   восста-";
	t23.second = "новление";
    QTest::newRow("Final test 9")
        << "Полное восстановление"
        << 16
        << t23;

	Expectations t24;
	t24.first = "Полное восстанов-";
	t24.second = "ление";
    QTest::newRow("Final test 10")
        << "Полное восстановление"
        << 17
        << t24;

	Expectations t25;
	t25.first = "Полное  восстанов-";
	t25.second = "ление";
    QTest::newRow("Final test 11")
        << "Полное восстановление"
        << 18
        << t25;

	Expectations t26;
	t26.first = "Полное восстановле-";
	t26.second = "ние";
    QTest::newRow("Final test 12")
        << "Полное восстановление"
        << 19
        << t26;

	Expectations t27;
	t27.first = "Полное  восстановле-";
	t27.second = "ние";
    QTest::newRow("Final test 13")
        << "Полное восстановление"
        << 20
        << t27;

	Expectations t28;
	t28.first = "Полное восстановление";
	t28.second = "";
    QTest::newRow("Final test 14")
        << "Полное восстановление"
        << 21
        << t28;

	Expectations t29;
	t29.first = "Не  следует,  однако";
	t29.second = "забывать, что консультация";
    QTest::newRow("Some additional test")
        << "Не следует, однако забывать, что консультация"
        << 20
        << t29;

	Expectations t30;
	t30.first = "Раз  два";
	t30.second = "три";
    QTest::newRow("Some additional test")
        << "Раз два три"
        << 8
        << t30;
}

void TestTextJustifier::testBreakLine()
{
    typedef QPair<QString, QString> Expectations;

    QFETCH(QString, str);
    QFETCH(int, textWidth);
    QFETCH(Expectations, expectation);
    QString after;

    breakLine(str, after, textWidth);
    QString message1 = QString("\nПервая пара\nExpected:\n\"%1\"\n\nReal:\n\"%2\"\n")
        .arg(expectation.first)
        .arg(str);
    QString message2 = QString("\nВторая пара\nExpected:\n\"%1\"\n\nReal:\n\"%2\"\n")
        .arg(expectation.second)
        .arg(after);

    QVERIFY2(str == expectation.first, message1.toLocal8Bit().data());
    QVERIFY2(after == expectation.second, message2.toLocal8Bit().data());
}

void TestTextJustifier::testExceptions()
{
	bool result = false;
	try
	{
		fillSpaces(QString("12345"), 3);
	}
	catch (...)
	{
		result = true;
	}
	QVERIFY(true == result);
}
