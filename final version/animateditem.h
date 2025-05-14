#ifndef ANIMATEDITEM_H
#define ANIMATEDITEM_H

#include <QGraphicsItem>
#include <QPixmap>
#include <QVector>
#include <QTimer>
#include <QObject>
#include <QPainter>
#include <QGraphicsScene>

class AnimatedItem : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit AnimatedItem(const QVector<QPixmap> &frames, QGraphicsItem *parent = nullptr);
    ~AnimatedItem();
    ;
    QRectF getcollisionRect();//碰撞边界
    // QGraphicsItem 接口
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    // 动画控制
    void setFrameInterval(int ms);
    int frameInterval() const;
    void setTarget(const QPointF &target);
    void updatePosition();
    void startLifeTimer(int lifetime); // 生命周期计时器

    void startBlink();
    void stopMovement();


signals:
    void collided();

public slots:
    void nextFrame();
    void lifeTimeout(); // 新增生命周期结束槽

private:
    QVector<QPixmap> m_frames;
    int m_currentFrame;
    QRectF m_boundingRect;
    QRectF collisionRect;
    QTimer *m_timer;
    QTimer *m_lifeTimer; // 新增生命周期计时器
    QPointF m_target;
    qreal m_speed;

    //处理碰撞闪烁所需成员变量
    bool m_isBlinking;
    int m_blinkCount;
    QTimer *m_blinkTimer;
    bool m_collided;
    QTimer *moveTimer;

};

#endif // ANIMATEDITEM_H
