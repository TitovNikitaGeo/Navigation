#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <QGeoCoordinate>

//кольцевой буфер для расчета координат
class CircularBuffer {
public:
    CircularBuffer(size_t size) : size(size), buffer(size), currentIndex(0), isFull(false) {}

    // Добавляем новое значение в буфер
    void add(const QGeoCoordinate& coord) {
        buffer[currentIndex] = coord;
        currentIndex = (currentIndex + 1) % size;

        if (currentIndex == 0) {
            isFull = true;
        }
    }

    // Проверка, заполнен ли буфер
    bool full() const {
        return isFull;
    }

    // Получение текущего количества элементов в буфере
    size_t count() const {
        return isFull ? size : currentIndex;
    }

    // Получение координаты по индексу
    const QGeoCoordinate& get(size_t index) const {
        return buffer[index % size];
    }

    // Функция вычисления среднего азимута движения
    double calculateAzimuth() const {
        if (count() < 2) {
            // std::cerr << "Недостаточно данных для расчета азимута.\n";
            return -1.0;  // Возвращаем некорректное значение, если данных недостаточно
        }

        // Находим индекс самой старой точки
        size_t oldestIndex = isFull ? currentIndex : 0;
        // Самая новая точка - это предыдущая точка перед currentIndex
        size_t newestIndex = (currentIndex == 0) ? size - 1 : currentIndex - 1;

        const QGeoCoordinate& oldestCoord = get(oldestIndex);
        const QGeoCoordinate& newestCoord = get(newestIndex);

        // Вычисляем азимут между самой старой и самой новой точками
        double azimuth = oldestCoord.azimuthTo(newestCoord);

        // Возвращаем азимут в радианах
        if (azimuth >= 0.0) {
            return qDegreesToRadians(azimuth); // Преобразуем из градусов в радианы
        } else {
            return -1.0;
        }
    }

private:
    size_t size;
    QVector<QGeoCoordinate> buffer;
    size_t currentIndex;
    bool isFull;
};
#endif // CIRCULARBUFFER_H
