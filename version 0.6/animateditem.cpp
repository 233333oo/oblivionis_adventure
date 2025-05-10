#include "animateditem.h"
#include <QRandomGenerator>
#include <QDebug>

AnimatedItem::AnimatedItem(const QVector<QPixmap> &frames, QGraphicsItem *parent)
    : QGraphicsObject(parent),
    m_frames(frames),
    m_currentFrame(0),
    m_speed(1.0 + QRandomGenerator::global()->generateDouble() * 2.0), // 随机速度1.0-3.0
    m_lifeTimer(nullptr), // 初始化生命周期计时器
    m_isBlinking(false),
    m_blinkCount(0),
    m_collided(false),
    moveTimer(new QTimer(this))
{
    // 闪烁定时器
    m_blinkTimer = new QTimer(this);
    connect(m_blinkTimer, &QTimer::timeout, [this]() {
        m_isBlinking = !m_isBlinking;
        update();
        if(++m_blinkCount >= 6) { // 闪烁3次
            m_blinkTimer->stop();
            emit collided();
            deleteLater();
        }
    });

    // 设置定时器控制动画帧率
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &AnimatedItem::nextFrame);
    m_timer->start(70);

    // 位置更新定时器

    connect(moveTimer, &QTimer::timeout, this, &AnimatedItem::updatePosition);
    moveTimer->start(16);


    if (!m_frames.isEmpty()) {
        m_boundingRect = m_frames.first().rect();
        collisionRect = m_frames.first().rect().adjusted(15, 15, -15, -15);
    }

    // 初始随机位置
    int side = QRandomGenerator::global()->bounded(4); // 0: top, 1: right, 2: bottom, 3: left
    qreal x, y;

    switch (side) {
    case 0: // top
        x = QRandomGenerator::global()->bounded(-100, 900);
        y = QRandomGenerator::global()->bounded(-200, -50);
        break;
    case 1: // right
        x = QRandomGenerator::global()->bounded(800, 1000);
        y = QRandomGenerator::global()->bounded(-100, 700);
        break;
    case 2: // bottom
        x = QRandomGenerator::global()->bounded(-100, 900);
        y = QRandomGenerator::global()->bounded(600, 800);
        break;
    case 3: // left
        x = QRandomGenerator::global()->bounded(-200, -50);
        y = QRandomGenerator::global()->bounded(-100, 700);
        break;
    }
    setPos(x, y);
}

AnimatedItem::~AnimatedItem()
{
    if (m_lifeTimer) {
        m_lifeTimer->stop();
        delete m_lifeTimer;
    }
    m_timer->stop();
    delete m_timer;
    moveTimer->stop();
    delete moveTimer;//停止移动定时器
}

QRectF AnimatedItem::boundingRect() const
{
    return m_boundingRect;
}
QRectF AnimatedItem::getcollisionRect()
{
    return collisionRect;
}

void AnimatedItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (!m_frames.isEmpty()) {
        if(m_isBlinking) {
            painter->setOpacity(0.3);
        }// 闪烁效果
        painter->drawPixmap(m_boundingRect.topLeft(), m_frames[m_currentFrame]);
        painter->setOpacity(1.0);
    }
}
void AnimatedItem::startBlink()
{
    if(m_collided) return;
    m_collided = true;
    m_blinkTimer->start(200); // 每200ms闪烁一次
    m_timer->stop();
    moveTimer->stop();
}

void AnimatedItem::setFrameInterval(int ms)
{
    m_timer->setInterval(ms);
}

int AnimatedItem::frameInterval() const
{
    return m_timer->interval();
}

void AnimatedItem::setTarget(const QPointF &target)
{
    m_target = target;
}

void AnimatedItem::updatePosition()
{
    if (m_target.isNull()) return;

    QPointF currentPos = pos();
    QPointF direction = m_target - currentPos;

    // 计算距离
    qreal distance = sqrt(direction.x() * direction.x() + direction.y() * direction.y());

    // 如果距离足够近，则直接到达目标
    if (distance < 3.0) {
        setPos(m_target);
        return;
    }

    // 标准化方向向量并应用速度
    direction /= distance;
    setPos(currentPos + direction * m_speed);
}

void AnimatedItem::nextFrame()
{
    if (!m_frames.isEmpty()) {
        m_currentFrame = (m_currentFrame + 1) % m_frames.size();
        update(); // 请求重绘
    }
}
void AnimatedItem::startLifeTimer(int lifetime)
{
    m_lifeTimer = new QTimer(this);
    connect(m_lifeTimer, &QTimer::timeout, this, &AnimatedItem::lifeTimeout);
    m_lifeTimer->setSingleShot(true);
    m_lifeTimer->start(lifetime);
}

void AnimatedItem::lifeTimeout()
{
    if (scene()) scene()->removeItem(this);
    deleteLater();
}
void AnimatedItem::stopMovement() {
    if (moveTimer && moveTimer->isActive()) {
        moveTimer->stop();  // 停止移动定时器
    }
    if (m_timer && m_timer->isActive()) {
        m_timer->stop();     // 停止动画帧更新
    }
}
