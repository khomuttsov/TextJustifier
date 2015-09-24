#ifndef TEXTJUSTIFIER_H
#define TEXTJUSTIFIER_H

#include <QString>
#include <QStringList>
#include <QVector>
#include <QRegularExpression>

/*!
 * Форматирует текст text по ширине textWidth с расстановкой переносов.
 *
 *\details Если последняя строка короче textWidth, она остается неизменной.
 *
 *\param[in|out] text Текст для форматирования.
 *\param[in] textWidth Количество символов, которое должно быть в каждой строке текста.
 */
void justify(QStringList& text, int textWidth);

/*!
 * Удлиняет строку, длина которой меньше textWidth, засчет вставки пробелов между не пробельными частями.
 *
 *\details Если длина str больше или равна textWidth, функция возвращает str.
 *
 *\param[in|out] str Строка для удлинения.
 *\param[in] textWidth Количество символов, которое должно быть в удлиненной строке.
 */
void fillSpaces(QString& str, int textWidth);

/*!
 * Разбивает строку str на две: до и после переноса. Перенос становится в максимально возможном индексе, не большем textWidth.
 *
 *\param[in|out] str Строка для разбиения.
 *\param[out] after Часть строки после переноса.
 *\param[in] textWidth Количество символов, которое должно быть в каждой строке текста.
 */
void breakLine(QString& str, QString& after, int textWidth);

/*!
 * Расставить мягкие переносы в слове.
 *
 *\details Если в строке нельзя сделать перенос или все позици переноса больше limit, функция возвращает -1.
 *
 * Пример:
 *\code
 * placeHyphens("облачный"); // "об-лач-ный"
 *\endcode
 *
 *\param[in] str Строка, где будет искаться место для переноса. Может включать в себя также знаки препинания.
 *\param[in] limit Число, больше которого не может быть найденный индекс.
 *\return Максимальный индекс в строке, после которого можно сделать перенос.
 */
void placeHyphens(const QString& word, QString& hyphenWord);

#endif // TEXTJUSTIFIER_H
