#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QGraphicsPixmapItem>//图形元素
#include<QGraphicsView>//视图
#include<QGraphicsScene>//场景
#include<QLabel>
#include<QMovie>
#include <QPropertyAnimation>
#include <QMouseEvent>

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

private:
    enum class Direction { Left, Right, Up, Down, None };

    Ui::Widget *ui;
    QGraphicsView mGameView;
    QGraphicsScene mScene;
    QGraphicsPixmapItem mBackGround;

    QLabel *oblivious;
    QMovie *m_movieLeft;     // 向左动画
    QMovie *m_movieRight;    // 向右动画
    QPropertyAnimation *m_anim;// 控制移动的动画

    int m_targetX;      // 目标X坐标
    bool m_facingRight; // 当前朝向（true=右，false=左）
};




#endif // WIDGET_H
