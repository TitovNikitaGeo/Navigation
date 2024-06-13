#ifndef MYGRAPHICVIEW_CPP
#define MYGRAPHICVIEW_CPP

#include <QGraphicsView>
#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QPolygonF>
#include <QBrush>
#include <QGraphicsScene>
#include <QGraphicsPolygonItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QWheelEvent>
#include <QScrollBar>
#include <QString>
#include <QMessageBox>

class MyGraphicView : public QGraphicsView
{
public:
    MyGraphicView(QWidget* parent = nullptr);
    ~MyGraphicView();

    void addPoint(float x, float y, float z, QString name);

    ///init drawing
    void drawShip();
    void drawCenter();
    void drawAxes();
    void drawGrid();
    void getSceneProperties();
    ///init drawing

    ///Drawing properties
    int X_center = 0;
    int Y_center = 0;
    float left;
    float right;
    float top;
    float bottom;
    ///Drawing properties

    ///Draw line for towed Item
    void drawLineToTowed(int x1, int y1, int x2, int y2);

private:
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    bool isPanning = false;
    QPoint lastMousePos;


    QGraphicsScene* scene;
    QGraphicsPolygonItem* MyShip;


    QVector<QGraphicsTextItem*> VectorGraphicsTextVault;
    QVector<QGraphicsItem*> VectorAllOtherGraphicsVault;

};

#endif // MYGRAPHICVIEW_CPP
