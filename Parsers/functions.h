#include <QString>

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

QString floatToQString(float value, int totalLength, int decimalPlaces) {
    QString str = QString::number(value, 'f', decimalPlaces);

    // Если строка длиннее необходимого, обрезаем ее
    if (str.length() > totalLength) {
        str = str.left(totalLength);
    }
    // Если строка короче необходимого, добавляем пробелы в начало
    else if (str.length() < totalLength) {
        str = str.rightJustified(totalLength, ' ');
    }

    return str;
}

#endif // FUNCTIONS_H
