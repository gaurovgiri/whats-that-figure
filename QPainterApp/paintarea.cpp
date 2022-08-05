    #include "paintarea.h"

PaintArea::PaintArea(QWidget* parentWidget)
    :
      paintAreaUnlocked{false},
      QWidget(parentWidget)
{
    initialize();
    createTimer();
}

// Main Functionality

void PaintArea::initialize()
{
    penColor = Qt::black;
    screenColor = Qt::white;
    penWidth = 2;
    paintAreaUnlocked = false;
}

void PaintArea::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,pixMap);
}

void PaintArea::resizeEvent(QResizeEvent*)
{
    auto newRect = pixMap.rect().united(rect());
    if(newRect == pixMap.rect())
    {
        return;
    }
    QPixmap newPixmap(newRect.size());
    QPainter painter{&newPixmap};
    painter.fillRect(newPixmap.rect(),Qt::white);
    painter.drawPixmap(0, 0, pixMap);
    pixMap = newPixmap;
}

void PaintArea::mousePressEvent(QMouseEvent *event)
{
    if(paintAreaUnlocked)
    {
        lastPosition = event->pos();
        QPointF newPos = event->pos();
        draw(newPos);
    }
}

void PaintArea::mouseMoveEvent(QMouseEvent *event)
{
    if(paintAreaUnlocked)
    {
        QPointF newPos = event->pos();
        draw(newPos);
    }
}

void PaintArea::draw(QPointF& newPosition)
{
    QPainter painter{&pixMap};
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen({penColor, static_cast<double>(penWidth)});
    painter.drawLine(lastPosition, newPosition);
    lastPosition = newPosition;
    update();
}

// Timer Functionality

void PaintArea::createTimer()
{
    timerFrequency = 500;
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PaintArea::_onTimeOut);
}

void PaintArea::startTimer()
{
    timer->start(timerFrequency);
}

void PaintArea::stopTimer()
{
     timer->stop();
}

void PaintArea::_onTimeOut()
{
    emit painterAreaModifiedSIGNAL();
}
// Setters

void PaintArea::setPenColor(const QColor &newColor)
{
    penColor = newColor;
}

void PaintArea::setPenWidth(int newWidth)
{
    penWidth = newWidth;
}

void PaintArea::clearScreen()
{
    pixMap.fill(screenColor);
    update();
}

void PaintArea::unlockPaintArea()
{
    paintAreaUnlocked = true;
    startTimer();
}

void PaintArea::lockPaintArea()
{
    paintAreaUnlocked = false;
    stopTimer();
}

// Getters

QColor PaintArea::getPenColor()
{
    return penColor;
}

int PaintArea::getPenWidth()
{
    return penWidth;
}

QPixmap PaintArea::getPixMap()
{
    return pixMap;
}

void PaintArea::setPixMap(QPixmap newPixmap)
{
    pixMap = newPixmap;
}
