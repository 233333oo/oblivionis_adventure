#include "widget.h"
#include "./ui_widget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QTimerEvent>
#include <QRandomGenerator>
#include <utility>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , m_facingRight(true)
    , m_currentSpawnInterval(2000)
    , m_minSpawnInterval(300)
{
    ui->setupUi(this);

    this->setFixedSize(800,600);
    this->setMouseTracking(true);

    // 加载敌人动画帧
    m_enemyFrames.append(QPixmap(":/resource/doloris/01.png"));
    m_enemyFrames.append(QPixmap(":/resource/doloris/02.png"));
    m_enemyFrames.append(QPixmap(":/resource/doloris/03.png"));
    m_enemyFrames.append(QPixmap(":/resource/doloris/04.png"));
    m_enemyFrames.append(QPixmap(":/resource/doloris/05.png"));
    m_enemyFrames.append(QPixmap(":/resource/doloris/06.png"));
    m_enemyFrames.append(QPixmap(":/resource/doloris/07.png"));
    m_enemyFrames.append(QPixmap(":/resource/doloris/08.png"));
    m_enemyFrames.append(QPixmap(":/resource/doloris/09.png"));
    m_enemyFrames.append(QPixmap(":/resource/doloris/10.png"));
    // 场景设置
    mGameView.setSceneRect(QRect(0,0,800,600));
    mScene.setSceneRect(QRect(0,0,800,600));
    mBackGround.setPixmap(QPixmap(":/scene.png"));
    mScene.addItem(&mBackGround);
    mGameView.setScene(&mScene);
    mGameView.setParent(this);
    mGameView.show();
    mGameView.setMouseTracking(true);  // 允许鼠标追踪
    mGameView.setAttribute(Qt::WA_TransparentForMouseEvents); // 让鼠标事件穿透
    mGameView.viewport()->setMouseTracking(true); // 确保视口也支持鼠标追踪

    // 加载player动画
    m_movieLeft = new QMovie(":/resource/oblivious_left.gif", QByteArray(), this);
    m_movieRight = new QMovie(":/resource/oblivious_right.gif", QByteArray(), this);

    // player初始化
    oblivious = new QLabel(this);
    oblivious->setMovie(m_movieRight);
    m_movieRight->start();
    oblivious->setGeometry(100,100,60,60);
    oblivious->setMouseTracking(true);
    oblivious->show();
    oblivious->setAttribute(Qt::WA_TransparentForMouseEvents); // 让鼠标事件穿透QLabel

    // 初始化动画
    m_anim = new QPropertyAnimation(oblivious, "pos", this);
    m_anim->setDuration(1000);
    m_anim->setEasingCurve(QEasingCurve::OutCirc);

    // 启动敌人生成定时器
    m_spawnTimerId = startTimer(m_currentSpawnInterval); // 加速生成敌人
}

void Widget::mouseMoveEvent(QMouseEvent *event) {
    QPoint mousePos = event->pos();
    int labelWidth = oblivious->width();
    int labelHeight = oblivious->height();
    QPoint targetPos = mousePos - QPoint(labelWidth / 2, labelHeight / 2);

    if (m_anim->state() != QPropertyAnimation::Running) {
        m_anim->setStartValue(oblivious->pos());
        m_anim->setEndValue(targetPos);
        m_anim->start();
    } else {
        m_anim->setEndValue(targetPos);
    }

    m_targetX = targetPos.x();
    int currentX = oblivious->x();
    int delta = m_targetX - currentX;
    bool shouldFaceRight = delta > 0;

    if (shouldFaceRight != m_facingRight) {
        m_facingRight = shouldFaceRight;
        oblivious->movie()->stop();
        oblivious->setMovie(m_facingRight ? m_movieRight : m_movieLeft);
        oblivious->movie()->start();
    }

    // 更新所有敌人的目标位置
    for (AnimatedItem *enemy : std::as_const(m_enemies)) {
        enemy->setTarget(oblivious->pos() + QPoint(30, 30));
    }
}

void Widget::timerEvent(QTimerEvent *event) {
    if (event->timerId() == m_spawnTimerId) {
        spawnEnemy();
        updateSpawnRate();
    }
}

void Widget::spawnEnemy() {
    if (m_enemyFrames.isEmpty()) return;

    AnimatedItem *enemy = new AnimatedItem(m_enemyFrames);
    mScene.addItem(enemy);
    m_enemies.append(enemy);

    // 设置初始目标为玩家当前位置
    QPoint targetPos = oblivious->pos() + QPoint(oblivious->width()/2, oblivious->height()/2);
    enemy->setTarget(targetPos);
    enemy->startLifeTimer(15000);
}
void Widget::updateSpawnRate()
{
    m_currentSpawnInterval = qMax(m_minSpawnInterval, m_currentSpawnInterval - 50);
    killTimer(m_spawnTimerId);
    m_spawnTimerId = startTimer(m_currentSpawnInterval);
}

Widget::~Widget()
{
    killTimer(m_spawnTimerId);
    qDeleteAll(m_enemies);
    delete ui;
}
