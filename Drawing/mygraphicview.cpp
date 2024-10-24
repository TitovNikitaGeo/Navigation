#include "mygraphicview.h"


MyGraphicView::MyGraphicView(QWidget* parent)
: QGraphicsView(parent), scene(new QGraphicsScene(this))
{
    setScene(scene);
    QGraphicsView view(scene);
    QTransform transform;
    transform.scale(1, -1);  // Ось Y теперь направлена вниз
    view.setTransform(transform);

    // this->rotate(90);
    drawShip();
    drawAxes();
    getSceneProperties();
    drawCenter();
    view.show();
    // drawGrid();
}

MyGraphicView::~MyGraphicView(){
    for (auto i: VectorGraphicsTextVault){
        delete i;
    }
    for (auto i: VectorAllOtherGraphicsVault){
        delete i;
    }
}



void MyGraphicView::addPoint(float x, float y, float z, QString name)
{
    Q_UNUSED(z); //wow
    QGraphicsEllipseItem *pointItem =
        new QGraphicsEllipseItem(x-2.5, y-2.5, 5, 5);
    pointItem->setBrush(QBrush(Qt::red));
    scene->addItem(pointItem);
    QGraphicsTextItem* newItemNameLabel = new QGraphicsTextItem(name);
    newItemNameLabel->setPos(x,y);
    VectorGraphicsTextVault.append(newItemNameLabel);

    scene->addItem(newItemNameLabel);
}

void MyGraphicView::drawShip() {
    ///get ship configuration
    QPolygonF fig;
    fig << QPointF(-100, -40)
        << QPointF(-100, 40)
        << QPointF(110, 40)
        << QPointF(150, 0)
        << QPointF(110, -40);
    MyShip = new QGraphicsPolygonItem(fig);
    MyShip->setBrush(QBrush(Qt::blue));
    ///SHIP DRAWING
    scene->addItem(MyShip);
}

void MyGraphicView::drawCenter() {
    QGraphicsEllipseItem *pointItem =
        new QGraphicsEllipseItem(-2.5, -2.5, 5, 5);
    pointItem->setBrush(QBrush(Qt::green));
    scene->addItem(pointItem);

    ///LABEL "SHIP CENTER"
    QGraphicsTextItem* сenterLabel = new QGraphicsTextItem("Ship Center");
    сenterLabel->setPos(10,-20);
    сenterLabel->setDefaultTextColor(Qt::green);
    QFont font = сenterLabel->font();
    font.setBold(true);
    сenterLabel->setFont(font);
    scene->addItem(сenterLabel);
    VectorGraphicsTextVault.append(сenterLabel);
    ///LABEL "SHIP CENTER"
}

void MyGraphicView::drawAxes()
{
    // Ось X
    QGraphicsLineItem* xAxis = new QGraphicsLineItem(-200, 0, 200, 0);
    scene->addItem(xAxis);
    QGraphicsTextItem* xLabel = new QGraphicsTextItem("X");
    xLabel->setPos(190, -30);
    scene->addItem(xLabel);

    // Ось Y
    QGraphicsLineItem* yAxis = new QGraphicsLineItem(0, -200, 0, 200);
    scene->addItem(yAxis);
    QGraphicsTextItem* yLabel = new QGraphicsTextItem("Y");
    yLabel->setPos(20, 190);
    scene->addItem(yLabel);

    VectorGraphicsTextVault.append(xLabel);
    VectorGraphicsTextVault.append(yLabel);

    int Step = 50;
    // Добавление отметок на оси X
    for (int i = -200; i <= 200; i += Step) {
        QGraphicsLineItem* tick = new QGraphicsLineItem(i, -5, i, 5);
        scene->addItem(tick);
        QGraphicsTextItem* label = new QGraphicsTextItem(QString::number(i));
        label->setPos(i - 5, 10);
        scene->addItem(label);
        VectorGraphicsTextVault.append(label);
    }

    // Добавление отметок на оси Y
    for (int i = -200; i <= 200; i += Step) {
        QGraphicsLineItem* tick = new QGraphicsLineItem(-5, i, 5, i);
        scene->addItem(tick);
        VectorAllOtherGraphicsVault.append(tick);
        QGraphicsTextItem* label = new QGraphicsTextItem(QString::number(i));
        label->setPos(-20, i - 5);
        scene->addItem(label);
        VectorGraphicsTextVault.append(label);
    }

    // // Ось Z (не в 3D, но для обозначения)
    // QGraphicsLineItem* zAxis = new QGraphicsLineItem(0, 0, -70, -70);
    // scene->addItem(zAxis);
    // QGraphicsTextItem* zLabel = new QGraphicsTextItem("Z");
    // zLabel->setPos(-80, -80);
    // scene->addItem(zLabel);
}


void MyGraphicView::drawGrid(){ ///TODO:сделать нормально
    // Horizontal lines
    for (int i = bottom; i <= top; i += (top-bottom)/20)
    {
        QGraphicsLineItem* line = new QGraphicsLineItem(left, i, right, i);
        scene->addItem(line);
        VectorAllOtherGraphicsVault.append(line);
    }

    // Vertical lines
    for (int i = left; i <= right; i += (right-left)/20)
    {
        QGraphicsLineItem* line = new QGraphicsLineItem(i, bottom, i, top);
        scene->addItem(line);
        VectorAllOtherGraphicsVault.append(line);
    }
}

void MyGraphicView::getSceneProperties() {
    left = scene->sceneRect().left();
    right = scene->sceneRect().right();
    top = scene->sceneRect().top();
    bottom = scene->sceneRect().bottom();
}

void MyGraphicView::drawLineToTowed(int x1, int y1, int x2, int y2) {
    QGraphicsLineItem* line = new QGraphicsLineItem(x1, y1, x2, y2);
    QPen pen;
    pen.setColor(Qt::red);
    pen.setStyle(Qt::DotLine);
    line->setPen(pen);
    scene->addItem(line);
    VectorAllOtherGraphicsVault.append(line);
}

void MyGraphicView::drawStreamer(int x1, int y1, int x2, int y2, uint n)
{
    QGraphicsLineItem* line = new QGraphicsLineItem(x1, y1, x2, y2);
    QPen pen;
    pen.setColor(Qt::cyan);
    pen.setStyle(Qt::DotLine);
    float dx = (x2-x1)/(n-1);
    float dy = (y2-y1)/(n-1);
    for (float x = x1, y = y1; x <=x2 && y <= y2; x+=dx, y += dy) {
        QGraphicsEllipseItem *pointItem =
            new QGraphicsEllipseItem(x-0.5, y-0.5, 1, 1);
        pointItem->setBrush(QBrush(Qt::magenta));
        scene->addItem(pointItem);
        VectorAllOtherGraphicsVault.append(pointItem);
    }
    line->setPen(pen);
    scene->addItem(line);
    VectorAllOtherGraphicsVault.append(line);
}


void MyGraphicView::wheelEvent(QWheelEvent *event) {
    const double scaleFactor = 1.15;

    if (event->angleDelta().y() > 0) {
        // Zoom in
        scale(scaleFactor, scaleFactor);
    } else {
        // Zoom out
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
}


void MyGraphicView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isPanning = true;
        lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor); // Изменяем курсор при захвате
    }
    // else if (event->button() == Qt::RightButton) { //чисто по приколу
    //     QPointF scenePos = mapToScene(event->pos());
    //     // QMessageBox::information(this, "Coordinates", QString("X: %1, Y: %2").arg(scenePos.x()).arg(scenePos.y()));

    // }
    QGraphicsView::mousePressEvent(event);
}

void MyGraphicView::mouseMoveEvent(QMouseEvent *event)
{
    if (isPanning) {
        QPoint delta = event->pos() - lastMousePos;
        lastMousePos = event->pos();
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
    }
    QGraphicsView::mouseMoveEvent(event);
}

void MyGraphicView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isPanning = false;
        setCursor(Qt::ArrowCursor); // Возвращаем курсор обратно
    }
    QGraphicsView::mouseReleaseEvent(event);
}



