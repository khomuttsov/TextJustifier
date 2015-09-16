#include <clocale>
#include "testtextjustifier.h"

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "Russian");

    QTest::qExec(&TestTextJustifier(), argc, argv);

    return 0;
}
