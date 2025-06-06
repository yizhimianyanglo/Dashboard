#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    void paintEvent(QPaintEvent *event)override;
    QTimer *timer;
    int currentValue;
    int mark=0;
    int startAngle;
    void initCanvas(QPainter& painter);
    void drawMiddleCircle(QPainter &painter,int radius);
    void drawCurrentSpend(QPainter &painter);
    void drawScale(QPainter &painter,int radius);
    double angle;
    void drawScaleText(QPainter &painter,int radius);
    void drawPointLine(QPainter& painter,int len);
    void drawSpeedPie(QPainter &painter,int radius);
    void drawEllipseInnerBlack(QPainter &painter,int radius);
    void startSpeed();
    void drawInnerCircle(QPainter &painter,int radius);
    void drawOuterCircle(QPainter &painter,int radius);
};
#endif // WIDGET_H
