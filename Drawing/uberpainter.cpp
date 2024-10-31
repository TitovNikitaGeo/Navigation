#include "uberpainter.h"


UberPainter::UberPainter(ItemsStorage *storage, QWidget *parent) :
    QGraphicsView(parent), MyVault(storage), graphicsScene(new QGraphicsScene)
{
    isDrawingLine = false;
    setScene(graphicsScene);
    this->resetTransform();         // Сброс трансформаций
    this->scale(1, -1);
    createContainer();
}

UberPainter::~UberPainter() ///some comments
{
    delete graphicsScene;
}

void UberPainter::run()
{//reading item's position and building map
    collectItems();
    updateView();

}

// Метод для обновления координат в видимой области
void UberPainter::updateCoordinates()
{
    QTransform trans;
    trans.scale(1, -1);
    for (auto item : graphicsScene->items()) {
        if (dynamic_cast<QGraphicsTextItem*>(item)) {
            graphicsScene->removeItem(item);
            delete item;
        }
    }

    QRectF visibleArea = mapToScene(viewport()->geometry()).boundingRect();
    QFont font;
    font.setPointSizeF(10.0 / transform().m11());

    for (double x = visibleArea.left(); x <= visibleArea.right(); x += 50.0) {
        QGraphicsTextItem* textX = graphicsScene->addText(QString::number(x), font);
        // textX->setTransform(trans);
        textX->setPos(x, visibleArea.bottom() + 100);
        textX->setDefaultTextColor(Qt::black);
    }

    qDebug() <<__FUNCTION__<<visibleArea.top() <<visibleArea.bottom();
    for (double y = visibleArea.top(); y <= visibleArea.bottom(); y += 50.0) {
        QGraphicsTextItem* textY = graphicsScene->addText(QString::number(y), font);
        // textY->setTransform(trans);
        textY->setPos(visibleArea.left() + 70, y);
        textY->setDefaultTextColor(Qt::black);
    }
}

void UberPainter::drawCoordinatesNet(double minX, double minY, double maxX, double maxY)
{
    const double stepSize = 50.0;

    QRectF visibleArea = mapToScene(viewport()->geometry()).boundingRect();
    double startX = std::max(minX - 500, visibleArea.left() - stepSize);
    double endX = std::min(maxX + 500, visibleArea.right() + stepSize);
    double startY = std::max(minY - 500, visibleArea.top() - stepSize);
    double endY = std::min(maxY + 500, visibleArea.bottom() + stepSize);

    for (double x = startX; x <= endX; x += stepSize) {
        graphicsScene->addLine(x, startY, x, endY, QPen(Qt::lightGray));
    }

    for (double y = startY; y <= endY; y += stepSize) {
        graphicsScene->addLine(startX, y, endX, y, QPen(Qt::lightGray));
    }

    updateCoordinates();
}

void UberPainter::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isPanning = true;
        lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor); // Изменяем курсор при захвате
    }
    QGraphicsView::mousePressEvent(event);
}

void UberPainter::mouseMoveEvent(QMouseEvent *event)
{
    if (isPanning) {
        QPoint delta = event->pos() - lastMousePos;
        lastMousePos = event->pos();
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());

        updateCoordinates(); // Обновляем координаты при сдвиге
    }
    QGraphicsView::mouseMoveEvent(event);
}

void UberPainter::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isPanning = false;
        setCursor(Qt::ArrowCursor); // Возвращаем курсор обратно
    }
    QGraphicsView::mouseReleaseEvent(event);
}

// Функция для обработки масштабирования.
void UberPainter::wheelEvent(QWheelEvent *event)
{
    double scaleFactor = 1.15; // Коэффициент масштабирования

    if (event->angleDelta().y() > 0) {
        scale(scaleFactor, scaleFactor); // Увеличиваем масштаб
    } else {
        scale(1.0 / scaleFactor, 1.0 / scaleFactor); // Уменьшаем масштаб
    }

    updateCoordinates(); // Обновляем координаты при масштабировании
    QGraphicsView::wheelEvent(event);
}

void UberPainter::updateView()
{
    QMutexLocker locker(&sceneMutex);
    graphicsScene->clear();

    minX = std::numeric_limits<double>::max();
    minY = std::numeric_limits<double>::max();
    maxX = std::numeric_limits<double>::lowest();
    maxY = std::numeric_limits<double>::lowest();

    for (auto i: recentPositions) {
        for (QPointF coor: i) {
            if (coor.x() < 10 || coor.y() < 10) continue;
            minX = std::min(minX, coor.x());
            minY = std::min(minY, coor.y());
            maxX = std::max(maxX, coor.x());
            maxY = std::max(maxY, coor.y());
        }
    }

    graphicsScene->setSceneRect(minX - 50, minY - 50, (maxX - minX) + 100, (maxY - minY) + 100);
    drawCoordinatesNet(minX, minY, maxX, maxY);

    for (auto it = recentPositions.begin(); it != recentPositions.end(); ++it) {
        FixedItem* item = it.key();
        const QQueue<QPointF>& queue = it.value();
        QColor color;
        if (item->itemType == "Fixed") color = QColor(0, 0, 0);
        else if (item->itemType == "Source") color = QColor(255, 0, 0);
        else if (item->itemType == "Streamer") color = QColor(0, 0, 255);
        else if (item->itemType == "Buoy") color = QColor(0, 255, 0);
        else color = QColor(255, 0, 255);

        int subItemsCount = (item->itemType == "Streamer") ? dynamic_cast<Streamer*>(item)->getChanCount() : 1;
        int totalSteps = queue.size() / subItemsCount;
        int step = totalSteps > 1 ? 100 / (totalSteps - 1) : 100;

        int index = 0;
        for (int i = 0; i < queue.size(); i += subItemsCount) {
            int alpha = qMin(100, (step * index) / 2);
            QColor pointColor = color;
            pointColor.setAlpha(alpha * 255 / 100);

            for (int j = 0; j < subItemsCount; ++j) {
                const QPointF& pos = queue[i + j];
                graphicsScene->addEllipse(pos.x() - 2, pos.y() - 2, 4, 4, QPen(Qt::NoPen), QBrush(pointColor));
            }
            ++index;
        }
    }
}

void UberPainter::collectItems()
{
    //QMap<FixedItem*, QQueue<QPointF>> recentPositions;

    for (FixedItem* item: MyVault->ItemsVault) {
        QQueue<QPointF>* curQueue = &recentPositions[item];
        if (item->itemType == "Streamer") {
            Streamer* streamer = dynamic_cast<Streamer*>(item);
            QVector<QPointF> chansPos = getCoor(streamer);
            // qDebug() << __FUNCTION__ << QUEUES_SIZE * streamer->getChanCount();
            if (curQueue->size() >= (QUEUES_SIZE*streamer->getChanCount())) {
                for (uint i = 0; i < streamer->getChanCount(); i++) {
                    curQueue->dequeue();
                }
            }
            for (auto i:chansPos) {
                curQueue->enqueue(i);
            }
        } else {
            QPointF itemPos = getCoor(item);
            if (curQueue->size() >= QUEUES_SIZE) curQueue->dequeue();
            curQueue->enqueue(itemPos);
        }
    }

}

QPointF UberPainter::getCoor(FixedItem *item)
{
    return QPointF(item->x_coor, item->y_coor);
}

QVector<QPointF> UberPainter::getCoor(Streamer *streamer)
{
    QVector<QPointF> res(streamer->getChanCount());
    for (uint i = 0; i < streamer->getChanCount(); i++) {
        QPointF chanCoor = QPointF(streamer->getChan(i+1)->x_coor, streamer->getChan(i+1)->y_coor);
        res[i] = chanCoor;
        // qDebug() << __FUNCTION__ << res[i];
    }
    return res;
}

void UberPainter::createContainer()
{
    bool doWeReallyNeedToReload = false;
    for (auto i: MyVault->ItemsVault) {
        if (!recentPositions.contains(i)) doWeReallyNeedToReload = true;
    }
    if (doWeReallyNeedToReload) {
        recentPositions.clear();
        for (auto item: MyVault->ItemsVault) {
            QQueue<QPointF> queue;
            recentPositions.insert(item, queue);
        }
    } else {
        return;
    }
}



