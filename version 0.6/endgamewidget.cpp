#include "endgamewidget.h"
#include <QVBoxLayout>




EndGameWidget::EndGameWidget(int finalScore, QWidget *parent)
    : QWidget(parent)
{
    setGeometry(0, 0, parent->width(), parent->height());

    QGraphicsScene *scene = new QGraphicsScene(0, 0, parent->width(), parent->height(), this);
    QGraphicsView *view = new QGraphicsView(scene, this);
    view->setGeometry(0, 0, width(), height());
    view->setFrameShape(QFrame::NoFrame);


    QGraphicsVideoItem *videoItem = new QGraphicsVideoItem;
    scene->addItem(videoItem);
    videoItem->setSize(scene->sceneRect().size());


    videoItem->setSize(view->size()); // 初始设置
    connect(view->scene(), &QGraphicsScene::sceneRectChanged, [=](){
        videoItem->setSize(view->scene()->sceneRect().size());
    });



    // 视频播放

    m_player = new QMediaPlayer(this);
    QAudioOutput *audio = new QAudioOutput(this);
    m_player->setAudioOutput(audio);
    m_player->setVideoOutput(videoItem);
    m_player->setSource(QUrl("qrc:/resource/end.mp4"));



    m_player->play();


    // 分数显示
    QLabel *scoreLabel = new QLabel(QString("你被dororis抓住了!!! score: %1 ").arg(finalScore));
    scoreLabel->setStyleSheet("font: 24pt; color: black;");
    QGraphicsProxyWidget *scoreProxy = scene->addWidget(scoreLabel);
    scoreProxy->setZValue(1);  //
    scoreProxy->setPos(view->width()/2 - scoreLabel->width()/2, 20);


    // 返回按钮
    QPushButton *returnButton = new QPushButton("返回主菜单");
    returnButton->setStyleSheet(
        "QPushButton { background: rgba(0,0,0,150); color: white; border: 2px solid white; padding: 15px; }"
        "QPushButton:hover { background: rgba(255,255,255,50); }");
    QGraphicsProxyWidget *buttonProxy = scene->addWidget(returnButton);
    buttonProxy->setZValue(1); // 设置层级高于视频
    buttonProxy->setPos(view->width()/2 - 25, 540);



    connect(returnButton, &QPushButton::clicked, this, [this]() {
        emit returnToMainMenu();
        deleteLater();
    });



}
