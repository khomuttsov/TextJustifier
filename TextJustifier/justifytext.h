#ifndef TEXTJUSTIFIER_H
#define TEXTJUSTIFIER_H

#include <QString>
#include <QStringList>
#include <QRegularExpression>

/*!
 * Форматирует текст text по ширине textWidth с расстановкой переносов.
 *
 * Если последняя строка короче textWidth, она остается неизменной.
 *
 *\param[in] text Текст для форматирования.
 *\param[in] textWidth Количество символов, которое должно быть в каждой строке текста.
 *\return Отформатированный текст.
 */
QString justify(const QString& text, int textWidth);

/*!
 * Удлиняет строку, длина которой меньше textWidth, засчет вставки пробелов между не пробельными частями.
 * 
 * Если длина str больше или равна textWidth, функция возвращает str.
 *
 *\param[in] str Строка для удлинения.
 *\param[in] textWidth Количество символов, которое должно быть в удлиненной строке.
 *\return Удлиненная строка.
 */
QString fillSpaces(const QString& str, int textWidth);

#endif // TEXTJUSTIFIER_H
