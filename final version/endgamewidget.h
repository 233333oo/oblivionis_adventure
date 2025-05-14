
#ifndef ENDGAMEWIDGET_H
#define ENDGAMEWIDGET_H

#include <QWidget>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QLabel>
#include <QPushButton>
#include <QAudioOutput>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsVideoItem>
#include <QGraphicsProxyWidget>

class EndGameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EndGameWidget(int finalScore, QWidget *parent = nullptr);

signals:
    void returnToMainMenu();

private:




    QMediaPlayer *m_player;
    QVideoWidget *m_video;
    QLabel *m_scoreLabel;
};

#endif // ENDGAMEWIDGET_H


