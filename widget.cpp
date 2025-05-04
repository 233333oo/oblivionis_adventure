#include "widget.h"
#include "./ui_widget.h"
#include<QPainter>
#include<QMouseEvent>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setFixedSize(800,600);
    this->setMouseTracking(true);//启动鼠标追踪
    //场景
    /*mGameView.setSceneRect(QRect(0,0,800,600));
    mScene.setSceneRect(QRect(0,0,800,600));
    mBackGround.setPixmap(QPixmap(":/scene.png"));//传入背景
    mScene.addItem(&mBackGround);
    mGameView.setScene(&mScene);
    mGameView.setParent(this);
    mGameView.show();*/

    //player初始化
    oblivious = new QLabel(this);
    ob_movie =new QMovie(":/oblivious_left.gif");
    oblivious->setGeometry(100,100,60,60);
    oblivious->setMovie(ob_movie);
    oblivious->setMouseTracking(true);

    ob_movie->start();
    oblivious->show();
    //初始化动画
    m_anim = new QPropertyAnimation(oblivious, "pos", this);
    m_anim->setDuration(1000); // 动画时长 1000ms
    m_anim->setEasingCurve(QEasingCurve::OutCirc);





}
void Widget::mouseMoveEvent(QMouseEvent *event) {
    // 获取鼠标位置（相对于当前窗口的坐标）
    QPoint mousePos = event->pos();

    // 计算 Label 左上角应移动到的位置，使中心对准鼠标
    int labelWidth = oblivious->width();
    int labelHeight = oblivious->height();
    QPoint targetPos = mousePos - QPoint(labelWidth / 2, labelHeight / 2);

    // 如果动画未运行，直接设置起点和终点
    if (m_anim->state() != QPropertyAnimation::Running) {
        m_anim->setStartValue(oblivious->pos());
        m_anim->setEndValue(targetPos);
        m_anim->start();
    } else {
        // 若动画正在运行，动态更新终点以实现连续追踪
        m_anim->setEndValue(targetPos);
    }
}

Widget::~Widget()
{
    delete ui;
}
