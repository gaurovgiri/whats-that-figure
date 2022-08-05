#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QtWidgets>
#include <QtMath>
class PaintArea : public QWidget
{
    Q_OBJECT
public:
    PaintArea(QWidget* parentWidget = nullptr);

private:
    // Components
    QPixmap pixMap;
    QPointF lastPosition;

    QColor penColor;
    QColor screenColor;
    int penWidth;

    bool paintAreaUnlocked;

    int timerFrequency;
    QTimer* timer;

protected:

    // Main Functionality
    void paintEvent(QPaintEvent*) override;
    void resizeEvent(QResizeEvent*) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void draw(QPointF& newPosition);

private:

    void initialize();

    // Timer Functionality
    void createTimer();
    void startTimer();
    void stopTimer();

private slots:
    void _onTimeOut();

public:

    // Setters
    void setPenColor(const QColor& newColor);
    void setPenWidth(int newWidth);
    void clearScreen();
    void setPixMap(QPixmap newPixmap);
    void unlockPaintArea();
    void lockPaintArea();

    // Getters
    QColor getPenColor();
    int getPenWidth();
    QPixmap getPixMap();
    bool isModified();


signals:
    void painterAreaModifiedSIGNAL();
};

#endif // PAINTAREA_H
