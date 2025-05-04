#include "animateditem.h"
#include <QRandomGenerator>
#include <QDebug>

AnimatedItem::AnimatedItem(const QVector<QPixmap> &frames, QGraphicsItem *parent)
    : QGraphicsItem(parent),
    m_frames(frames),
    m_currentFrame(0),
    m_speed(1.0 + QRandomGenerator::global()->generateDouble() * 2.0) // 随机速度1.0-3.0
{
    // 设置定时器控制动画帧率
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &AnimatedItem::nextFrame);
    m_timer->start(100); // 默认每100毫秒切换一帧 (10FPS)

    // 位置更新定时器
    QTimer *moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, this, &AnimatedItem::updatePosition);
    moveTimer->start(16); // 约60FPS的移动更新

    // 如果提供了帧，设置初始边界矩形
    if (!m_frames.isEmpty()) {
        m_boundingRect = m_frames.first().rect();
    }

    // 初始随机位置
    qreal x = QRandomGenerator::global()->bounded(-200, 800);
    qreal y = QRandomGenerator::global()->bounded(-200, 600);
    setPos(x, y);
}

AnimatedItem::~AnimatedItem()
{
    m_timer->stop();
    delete m_timer;
}

QRectF AnimatedItem::boundingRect() const
{
    return m_boundingRect;
}

void AnimatedItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (!m_frames.isEmpty()) {
        painter->drawPixmap(m_boundingRect.topLeft(), m_frames[m_currentFrame]);
    }
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
    if (distance < 5.0) {
        setPos(m_target);
        return;
    }

    // 标准化方向向量并应用速度
    direction /= distance;
    QPointF newPos = currentPos + direction * m_speed;

    setPos(newPos);
}

void AnimatedItem::nextFrame()
{
    if (!m_frames.isEmpty()) {
        m_currentFrame = (m_currentFrame + 1) % m_frames.size();
        update(); // 请求重绘
    }
}
