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
 *\param[in] text Текст для форматирования.
 *\param[in] textWidth Количество символов, которое должно быть в каждой строке текста.
 *\return Отформатированный текст.
 */
QString justify(const QString& text, int textWidth);

/*!
 * Удлиняет строку, длина которой меньше textWidth, засчет вставки пробелов между не пробельными частями.
 *
 *\details Если длина str больше или равна textWidth, функция возвращает str.
 *
 *\param[in] str Строка для удлинения.
 *\param[in] textWidth Количество символов, которое должно быть в удлиненной строке.
 *\return Удлиненная строка.
 */
QString fillSpaces(const QString& str, int textWidth);

/*!
 * Найти максимальный индекс в строке, не превышающиий limit, после которого можно сделать перенос.
 *
 *\details Если в строке нельзя сделать перенос или все позици переноса больше limit, функция возвращает -1.
 *
 * Пример:
 *\code
 * int maxPos = findWordBreak("облачный", 100); // 4 (позиция буквы "ч"), "облач-ный"
 *\endcode
 *
 *\param[in] str Строка, где будет искаться место для переноса. Может включать в себя также знаки препинания.
 *\param[in] limit Число, больше которого не может быть найденный индекс.
 *\return Максимальный индекс в строке, после которого можно сделать перенос.
 */
int findWordBreak(const QString& str, int limit);

#endif // TEXTJUSTIFIER_H
