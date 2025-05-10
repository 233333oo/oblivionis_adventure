#include "mainmenuwidget.h"
#include <QLabel>
#include <QFile>

MainMenuWidget::MainMenuWidget(QWidget *parent)
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

    m_menuPlayer = new QMediaPlayer(this);
    QAudioOutput *audio = new QAudioOutput(this);
    m_menuPlayer->setAudioOutput(audio);
    m_menuPlayer->setVideoOutput(videoItem);
    m_menuPlayer->setSource(QUrl("qrc:/resource/op.mp4"));



    m_menuPlayer->play();

    // 创建按钮
    m_startButton = new QPushButton("开始游戏", this);
    m_tutorialButton = new QPushButton("游戏教程", this);
    m_exitButton = new QPushButton("退出游戏", this);

    // 设置按钮样式
    QString buttonStyle = "QPushButton {"
                          "  background-color: rgba(76, 175, 80, 0);" // 半透明绿色
                          "  border: 2px solid #000000;"
                          "  color: black;"
                          "  padding: 15px 32px;"
                          "  font-size: 18px;"
                          "  border-radius: 8px;"
                          "}"
                          "QPushButton:hover {"
                          "  background-color: rgba(0, 255, 255, 80);"
                          "}";
    m_startButton->setStyleSheet(buttonStyle);
    m_tutorialButton->setStyleSheet(buttonStyle);
    m_exitButton->setStyleSheet(buttonStyle);
    m_startButton->setFixedSize(200, 60);
    m_tutorialButton->setFixedSize(200, 60);
    m_exitButton->setFixedSize(200, 60);


    QVBoxLayout *layout_1=new QVBoxLayout(this);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(m_startButton);
    layout->addWidget(m_tutorialButton);
    layout->addWidget(m_exitButton);
    layout->setAlignment(Qt::AlignHCenter);

    layout_1->addStretch();
    layout_1->addLayout(layout);

    setLayout(layout_1);

    // 连接信号槽
    connect(m_startButton, &QPushButton::clicked,
            this, &MainMenuWidget::startGameClicked);
    connect(m_tutorialButton, &QPushButton::clicked,
            this, &MainMenuWidget::showTutorial);
    connect(m_exitButton, &QPushButton::clicked,
            []() { QApplication::quit(); }); // Lambda直接退出

}
void MainMenuWidget::showTutorial()
{
    QDialog tutorialDialog(this);
    tutorialDialog.setWindowTitle("游戏教程");

    QLabel *label = new QLabel(
        "操作指南：\n"
        "1. 移动鼠标控制角色\n"
        "2. 躲避doloris\n"
        "3. 生存时间越长敌人生成越快",
        &tutorialDialog
        );

    QVBoxLayout *layout = new QVBoxLayout(&tutorialDialog);
    layout->addWidget(label);
    tutorialDialog.exec(); // 模态显示对话框
}
void MainMenuWidget::mousePressEvent(QMouseEvent *event) {
    event->accept(); // 阻止事件传递到父窗口
}

void MainMenuWidget::mouseMoveEvent(QMouseEvent *event) {
    event->accept(); // 阻止鼠标移动事件传递
}
