#ifndef TESTTEXTJUSTIFIER_H
#define TESTTEXTJUSTIFIER_H

#include <QObject>
#include <QTest>
#include <QVector>
#include "../TextJustifier/justifytext.h"

class TestTextJustifier : public QObject
{
    Q_OBJECT

private slots:
    void testFillSpaces_data();
    void testFillSpaces();

    void testJustify_data();
    void testJustify();

    void testPlaceHyphens_data();
    void testPlaceHyphens();

    void testBreakLine_data();
    void testBreakLine();

	void testExceptions();
};

#endif // TESTTEXTJUSTIFIER_H
