#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QLabel>
#include <QMovie>
#include <QPropertyAnimation>
#include <QMouseEvent>
#include <QList>
#include "animateditem.h"

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

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

private:
    void spawnEnemy();
    void updateSpawnRate();

    Ui::Widget *ui;
    QGraphicsView mGameView;
    QGraphicsScene mScene;
    QGraphicsPixmapItem mBackGround;

    QLabel *oblivious;
    QMovie *m_movieLeft;
    QMovie *m_movieRight;
    QPropertyAnimation *m_anim;

    int m_targetX;
    bool m_facingRight;
    QList<AnimatedItem*> m_enemies;
    int m_spawnTimerId;
    int m_currentSpawnInterval;
    int m_minSpawnInterval;
    QVector<QPixmap> m_enemyFrames;
};

#endif // WIDGET_H
