#ifndef ANIMATEDITEM_H
#define ANIMATEDITEM_H

#include <QGraphicsItem>
#include <QPixmap>
#include <QVector>
#include <QTimer>
#include <QObject>
#include <QPainter>

class AnimatedItem : public QGraphicsItem, public QObject
{
public:
    explicit AnimatedItem(const QVector<QPixmap> &frames, QGraphicsItem *parent = nullptr);
    ~AnimatedItem();

    // QGraphicsItem 接口
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    // 动画控制
    void setFrameInterval(int ms);
    int frameInterval() const;
    void setTarget(const QPointF &target);
    void updatePosition();

public slots:
    void nextFrame();

private:
    QVector<QPixmap> m_frames;
    int m_currentFrame;
    QRectF m_boundingRect;
    QTimer *m_timer;
    QPointF m_target;
    QPointF m_velocity;
    qreal m_speed;
};

#endif // ANIMATEDITEM_H
