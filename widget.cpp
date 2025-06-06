#include "widget.h"
#include "ui_widget.h"

#include <QTimer>
#include <qpainter.h>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    startAngle=150;//初始角度

    startSpeed();

}

Widget::~Widget()
{
    delete ui;
}

//初始化画布
void Widget::initCanvas(QPainter &painter)
{
    painter.setRenderHint(QPainter::Antialiasing,true); //抗锯齿
    //底色弄成黑色
    painter.setBrush(QBrush(Qt::black));
    painter.drawRect(rect());

    //坐标系 平移到中心
    // 向下偏移 100 像素
    QPointF center = rect().center();
    center.setY(center.y() + 50); // 调整 Y 坐标
    painter.translate(center);
}

//画小圆
void Widget::drawMiddleCircle(QPainter &painter,int radius)
{
    //画小圆
    painter.setPen(QPen(Qt::white,2));
    painter.drawEllipse(QPoint(0,0),radius,radius); //原点 宽 高
}

//显示当前仪表盘值
void Widget::drawCurrentSpend(QPainter &painter)
{
    painter.setPen(Qt::white);
    QFont font("Arial",30);//字体，字体大小
    font.setBold(true); //设置粗体黑体
    painter.setFont(font);
    painter.drawText(QRect(-60,-60,120,85),Qt::AlignCenter,QString::number(currentValue*4));

    QFont font2("Arial",15);//字体，字体大小
    font2.setBold(true); //设置粗体黑体
    painter.setFont(font2);
     painter.drawText(QRect(-60,-60,120,150),Qt::AlignCenter,"Km/h");
}

//画刻度
void Widget::drawScale(QPainter &painter,int radius)
{
    //1.画出一个刻度需要旋转的角度
     angle=240*1.0/60;//270*1.0的作用是扩大作用类型,把int类型扩成double,保留小数
    //2.设置第一个刻度的位置
    painter.save();//保存原点坐标
    painter.setPen(QPen(Qt::white,4));
    painter.rotate(startAngle);
    for(int i=0;i<=60;i++) //分成60份 61根线
    {
        if(i>=40)
        {
            painter.setPen(QPen(Qt::red,4)); //标红
        }
        else
        {
            painter.setPen(QPen(Qt::white,4));
        }
        if(i%5==0) //画长刻度线
        {
            //画长刻度
            painter.drawLine(radius-20,0,radius-2,0);
        }
        else
        {
            //画短刻度
            painter.drawLine(radius-8,0,radius-2,0);
        }
        //画完后旋转
        painter.rotate(angle);//旋转坐标轴（度数）参数表示旋转的角度，而正数角度表示顺时针方向旋转，负数角度则表示逆时针方向旋转。
    }
    painter.restore();
}

//写刻度文字
void Widget::drawScaleText(QPainter &painter,int raduis)
{
    QFont font("Arial",20);//字体，字体大小
    font.setBold(true); //设置粗体黑体
    painter.setFont(font);
    int r=raduis-49;//半径
    for(int i=0;i<=60;i++)
    {
        if(i%5==0)
        {
            //保存坐标系
            painter.save();
            //算出平移点
            int delX=qCos(qDegreesToRadians(210-angle*i))*r;
            int delY=qSin(qDegreesToRadians(210-angle*i))*r;
                //   平移坐标系
            painter.translate(QPoint(delX,-delY));

            //旋转坐标轴
            painter.rotate(-120+angle*i);//anfle=4
            //写上文字
            painter.drawText(-25,-25,50,30,Qt::AlignCenter,QString::number(i*4));
            //恢复坐标系
            painter.restore();
        }
    }
}

//画指针
void Widget::drawPointLine(QPainter &painter, int len)
{
    //1.先让坐标轴回到原点
    painter.save(); //保存当前的坐标轴 当前坐标为刚开始无旋转的坐标
    painter.setBrush(Qt::white);
    painter.setPen(Qt::NoPen);
    //定义一组静态常量点（QPointF）的数组，常用于绘制图形（如三角形、箭头等）QPointF: Qt 提供的浮点型二维坐标点类，支持 x() 和 y() 方法获取坐标。
    static const QPointF points[4]={
        QPointF(0,0),
        QPointF(0,10),
        QPointF(200,1.0),
        QPointF(200,-1.1)
    };

    painter.rotate(startAngle+angle*currentValue);//旋转坐标轴
    painter.drawPolygon(points,4);//points: 点数组（包含 3 个点） 3: 表示多边形的顶点数

    painter.restore();
}

//画扇形
void Widget::drawSpeedPie(QPainter &painter, int radius)
{
    QRect rentangle(-radius,-radius,radius*2,radius*2);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(255,0,0,50));
    painter.drawPie(rentangle,(360-startAngle)*16,-angle*currentValue*16);//正负 表方向 正值为逆时针 负值为顺时针
}

//画黑色内圈
void Widget::drawEllipseInnerBlack(QPainter &painter, int radius)
{
    painter.setBrush(QBrush(Qt::black));
    painter.drawEllipse(QPoint(0,0),radius,radius);
}

//定时器 刷新屏幕
void Widget::startSpeed()
{
    timer =new QTimer(this);
    currentValue=0;
    mark=0;
    connect(timer,&QTimer::timeout,[=]()
            {
                if(mark==0)
                {
                    currentValue++;
                    if(currentValue>=60)
                    {
                        mark=1;
                    }
                }
                else
                {
                    currentValue--;
                    if(currentValue==0)
                    {
                        mark=0;
                    }
                }
                update();//每次调用update()，Qt会安排一个paintEvent，而paintEvent中的代码会被重新执行
                //而每次 paintEvent() 的执行都是独立的。因此，QPainter 的所有设置（如抗锯齿、平移、画刷、画笔等）都会在每次 paintEvent() 中重新初始化并重新应用
            });

    timer->start(50);
}

//画红色内环圆
void Widget::drawInnerCircle(QPainter &painter, int radius)
{
    QRadialGradient radialGradient(0,0,100);//原点 半径
    radialGradient.setColorAt(0.0,QColor(255,0,0,255));
    radialGradient.setColorAt(0.5,QColor(255,0,0,180));
    radialGradient.setColorAt(0.7,QColor(255,0,0,150));
    radialGradient.setColorAt(1.0,QColor(255,0,0,100));
    painter.setBrush(radialGradient);

    painter.drawEllipse(QPoint(0,0),radius,radius);
}

//画红色外环
void Widget::drawOuterCircle(QPainter &painter, int radius)
{
    QRect rentangle(-radius,-radius,radius*2,radius*2);
    painter.setPen(Qt::NoPen);

    QRadialGradient radialGradient(QPoint(0,0),radius);
    radialGradient.setColorAt(1.0,QColor(255,0,0,255));
    radialGradient.setColorAt(0.97,QColor(255,0,0,150));
    radialGradient.setColorAt(0.9,QColor(0,0,0,0));
    radialGradient.setColorAt(0.0,QColor(0,0,0,0));
    painter.setBrush(QBrush(radialGradient));

    painter.drawPie(rentangle,-150*16,-angle*60*16);
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    //初始化画布
    initCanvas(painter);

    //画小圆
    drawMiddleCircle(painter,60);

    //画刻度
    drawScale(painter,height()/2);

    //写刻度文字
    drawScaleText(painter,height()/2);

    //画指针
    drawPointLine(painter,height()/2);

    //画扇形
    drawSpeedPie(painter,height()/2+25);

    //画红色内环圆
    drawInnerCircle(painter,120);

    //画黑色内圈
    drawEllipseInnerBlack(painter,80);

    //显示当前仪表盘速度
    drawCurrentSpend(painter);

    //画红色外环
    drawOuterCircle(painter,height()/2+25);
}
