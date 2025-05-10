#ifndef MAINMENUWIDGET_H
#define MAINMENUWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QApplication>
#include <QDialog>
#include <QMouseEvent>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QAudioOutput>
#include <QGraphicsVideoItem>
#include <QGraphicsView>
#include <QGraphicsScene>

class MainMenuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainMenuWidget(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void showTutorial();
signals:
    void startGameClicked();   // 开始游戏信号

    void exitClicked();        // 退出信号

private:
    QGraphicsVideoItem *m_videoitem;
    QMediaPlayer *m_menuPlayer;
    QVideoWidget *m_menuVideo;
    QAudioOutput *m_menuAudio;

    QPushButton *m_startButton;
    QPushButton *m_tutorialButton;
    QPushButton *m_exitButton;
};

#endif // MAINMENUWIDGET_H
