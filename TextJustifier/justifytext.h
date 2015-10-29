/*!
 *\file justifytext.h
 *\brief Файл с функциями для форматирования текста по ширине с расстановкой переносов.
 *\author Хомутцов Виктор homutshov@mail.ru.
 *
 * Данный файл содержит в себе декларацию функций для форматирования текста по
 * ширине с расстановкой переносов.
 */

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
 *\param[in,out] text Текст для форматирования.
 *\param[in] textWidth Количество символов, которое должно быть в каждой строке текста.
 */
void justify(QStringList& text, int textWidth);

/*!
 * Удлиняет строку, длина которой меньше textWidth, засчет вставки пробелов между не пробельными частями.
 *
 *\details Если длина str больше или равна textWidth, функция бросает
 * исключение.
 *
 *\param[in,out] str Строка для удлинения.
 *\param[in] textWidth Количество символов, которое должно быть в удлиненной строке.
 *\throw Длина строки больше ширины текста.
 */
void fillSpaces(QString& str, int textWidth) throw (std::invalid_argument);

/*!
 * Разбивает строку str на две: до и после переноса. Перенос становится в
 * максимально возможном индексе, не большем textWidth.
 *
 *\param[in,out] str Строка для разбиения.
 *\param[out] after Часть строки после переноса.
 *\param[in] textWidth Количество символов, которое должно быть в каждой
 * строке текста.
 */
void breakLine(QString& str, QString& after, int textWidth);

/*!
 * Расставить мягкие переносы в слове.
 *
 *\details В качестве символа переноса используется символ '\1'.
 *
 * Пример:
 *\code
 * placeHyphens("облачный", result); // result -> "об\1лач\1ный"
 *\endcode
 *
 *\param[in] word Слово, где будут расставляться переносы.
 *\param[in,out] hyphenWord Слово с расставленными переносами.
 */
void placeHyphens(const QString& word, QString& hyphenWord);

#endif // TEXTJUSTIFIER_H
