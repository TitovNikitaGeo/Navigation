#ifndef UBERPAINTER_H
#define UBERPAINTER_H

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsView>
#include <QMutex>
#include <QScrollBar>
#include <QThread>
#include <QTimer>
#include <QPointF>
#include <QWheelEvent>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <cmath>
#include <deque>

#include "itemsstorage.h"

// #include "structures.h"


#define QUEUES_SIZE 15
class UberPainter : public QGraphicsView
{
    Q_OBJECT
public:
    explicit UberPainter(ItemsStorage* storage, QWidget* parent = nullptr);
    ~UberPainter();

    void run();
private:
    void drawCoordinatesNet(double minX, double minY, double maxX, double maxY);

    void updateView();
    void collectItems();
    void updateCoordinates();


    QPointF getCoor(FixedItem* item);
    QVector<QPointF> getCoor (Streamer* streamer);

    void createContainer();

    ItemsStorage* MyVault;
    QGraphicsScene* graphicsScene;
    QMutex sceneMutex;  // Защита доступа к сцене

    // QVector<FixedItem*> items;
    QMap<FixedItem*, QQueue<QPointF>> recentPositions;

    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    bool isPanning = false;
    QPoint lastMousePos;

    double minX;
    double minY;
    double maxX;
    double maxY;

    bool isDrawingLine;

    QVector<QGraphicsItem*> coordinateItems;
};

#endif // UBERPAINTER_H
