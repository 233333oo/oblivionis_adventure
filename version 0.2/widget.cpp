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

    //加载player动画
    m_movieLeft = new QMovie(":/oblivious_left.gif", QByteArray(), this);
    m_movieRight = new QMovie(":/oblivious_right.gif", QByteArray(), this);

    //player初始化
    oblivious = new QLabel(this);
    oblivious->setMovie(m_movieRight);
    m_movieRight->start();
    oblivious->setGeometry(100,100,60,60);
    oblivious->setMouseTracking(true);//不知道为什么但不加这句追踪时会很卡
    oblivious->show();



    //初始化动画
    m_anim = new QPropertyAnimation(oblivious, "pos", this);
    m_anim->setDuration(1000); // 动画时长 1000ms
    m_anim->setEasingCurve(QEasingCurve::OutCirc);





}
void Widget::mouseMoveEvent(QMouseEvent *event) {
    // 获取鼠标位置
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
    //

    m_targetX=targetPos.x();
    int currentX = oblivious->x();
    int delta = m_targetX - currentX;
    bool shouldFaceRight = delta > 0;

    // 方向变化时切换动画
    if (shouldFaceRight != m_facingRight) {
        m_facingRight = shouldFaceRight;
        oblivious->movie()->stop();
        oblivious->setMovie(m_facingRight ? m_movieRight : m_movieLeft);
        oblivious->movie()->start();
    }


}




Widget::~Widget()
{
    delete ui;
}
