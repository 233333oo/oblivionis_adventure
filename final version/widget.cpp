#include "widget.h"
#include "./ui_widget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QTimerEvent>
#include <QRandomGenerator>
#include <utility>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , m_facingRight(true)
    , m_currentSpawnInterval(2000)
    , m_minSpawnInterval(300)
    ,m_gameActive(false)

{
    ui->setupUi(this);
    this->setWindowTitle("逃离dororis");

    // 初始化主菜单
    m_menuWidget = new MainMenuWidget(this);
    m_menuWidget->setGeometry(0, 0, width(), height());

    connect(m_menuWidget, &MainMenuWidget::startGameClicked,
            this, &Widget::startGame);

    // 场景设置
    mGameView.setSceneRect(QRect(0,0,800,600));
    mScene.setSceneRect(QRect(0,0,800,600));
    mBackGround.setPixmap(QPixmap(":/scene.png"));
    mScene.addItem(&mBackGround);
    mGameView.setScene(&mScene);
    mGameView.setParent(this);
    mGameView.hide();
    mGameView.setMouseTracking(true);  // 允许鼠标追踪
    mGameView.setAttribute(Qt::WA_TransparentForMouseEvents); // 让鼠标事件穿透
    mGameView.viewport()->setMouseTracking(true); // 确保视口也支持鼠标追踪




}
void Widget::startGame()

{
    m_menuWidget->stopmedia();
    m_gameActive = true;
    m_menuWidget->hide();
    mGameView.show();

    // 初始化游戏资源
    this->setFixedSize(800, 600);
    this->setMouseTracking(true);

    // 加载敌人动画帧
    m_enemyFrames.clear();
    for(int i=1; i<=10; i++) {
        m_enemyFrames.append(QPixmap(QString(":/resource/doloris/%1.png").arg(i, 2, 10, QLatin1Char('0'))));
    }

    // 初始化玩家
    oblivionis = new QLabel(this);
    m_movieRight = new QMovie(":/resource/oblivionis_right.gif");
    m_movieLeft = new QMovie(":/resource/oblivionis_left.gif");
    oblivionis->setMovie(m_movieRight);
    m_movieRight->start();
    oblivionis->setGeometry(100, 100, 60, 60);
    oblivionis->show();
    oblivionis->setMouseTracking(true);

    oblivionis->setAttribute(Qt::WA_TransparentForMouseEvents); // 让鼠标事件穿透QLabel

    // 初始化动画
    m_anim = new QPropertyAnimation(oblivionis, "pos");
    m_anim->setDuration(1000);
    m_anim->setEasingCurve(QEasingCurve::OutCirc);

    // 初始化音频
    initBGM();


    // 启动敌人生成定时器
    m_spawnTimerId = startTimer(m_currentSpawnInterval); // 加速生成敌人




    // 添加分数显示
    m_currentScore = 0;
    m_scoreDisplay = new QLCDNumber(this);
    m_scoreDisplay->setSegmentStyle(QLCDNumber::Filled);
    m_scoreDisplay->setGeometry(10, 10, 150, 50);
    m_scoreDisplay->display(m_currentScore);
    m_scoreDisplay->show();

    // 分数更新定时器
    m_scoreTimer = new QTimer(this);
    connect(m_scoreTimer, &QTimer::timeout, [this]() {
        m_currentScore += 1;
        m_scoreDisplay->display(m_currentScore);
    });
    m_scoreTimer->start(1000);
}

void Widget::mouseMoveEvent(QMouseEvent *event) {
    if (!m_gameActive || !oblivionis) return;

    QPoint mousePos = event->pos();
    int labelWidth = oblivionis->width();
    int labelHeight = oblivionis->height();
    QPoint targetPos = mousePos - QPoint(labelWidth / 2, labelHeight / 2);

    if (m_anim->state() != QPropertyAnimation::Running) {
        m_anim->setStartValue(oblivionis->pos());
        m_anim->setEndValue(targetPos);
        m_anim->start();
    } else {
        m_anim->setEndValue(targetPos);
    }

    m_targetX = targetPos.x();
    int currentX = oblivionis->x();
    int delta = m_targetX - currentX;
    bool shouldFaceRight = delta > 0;

    if (shouldFaceRight != m_facingRight) {
        m_facingRight = shouldFaceRight;
        oblivionis->movie()->stop();
        oblivionis->setMovie(m_facingRight ? m_movieRight : m_movieLeft);
        oblivionis->movie()->start();
    }

    // 更新所有敌人的目标位置
    for (AnimatedItem *enemy : std::as_const(m_enemies)) {
        enemy->setTarget(oblivionis->pos());
    }
    QRect playerRect(oblivionis->pos().x()+10, oblivionis->pos().y()+10,40,40);
    //检测碰撞
    for (AnimatedItem *enemy : m_enemies.toList()) {
        QRectF enemyRect = enemy->getcollisionRect();
        enemyRect.moveTopLeft(enemy->pos()+QPoint(15,15));

        if (playerRect.intersects(enemyRect.toRect())) {
            // 确保每个敌人只连接一次信号
            static QMetaObject::Connection connection;
            disconnect(connection); // 断开旧连接
            connection = connect(enemy, &AnimatedItem::collided,
                                 this, &Widget::handleEnemyCollision,
                                 Qt::UniqueConnection); // 唯一连接
            enemy->startBlink();
            this->setMouseTracking(false);

            m_bgmPlayer->stop();
            for (AnimatedItem *enemy : std::as_const(m_enemies)) {
                if (enemy) {
                    enemy->stopMovement(); //所有敌人停止
                }
            }

        }
    }
}

void Widget::timerEvent(QTimerEvent *event) {
    if (event->timerId() == m_spawnTimerId) {
        spawnEnemy();
        updateSpawnRate();
    }
}

void Widget::spawnEnemy() {
    if (m_enemyFrames.isEmpty()) return;

    AnimatedItem *enemy = new AnimatedItem(m_enemyFrames);
    mScene.addItem(enemy);
    m_enemies.append(enemy);

    connect(enemy, &QObject::destroyed, this, [this, enemy]() {
        m_enemies.removeOne(enemy);
    });

    // 设置初始目标为玩家当前位置
    QPoint targetPos = oblivionis->pos() + QPoint(oblivionis->width()/2, oblivionis->height()/2);
    enemy->setTarget(targetPos);
    enemy->startLifeTimer(15000);
}
void Widget::updateSpawnRate()
{
    m_currentSpawnInterval = qMax(m_minSpawnInterval, m_currentSpawnInterval - 50);
    killTimer(m_spawnTimerId);
    m_spawnTimerId = startTimer(m_currentSpawnInterval);
}
void Widget::handleEnemyCollision()
{
    // 停止游戏
    m_gameActive = false;
    killTimer(m_spawnTimerId);



    for (AnimatedItem *enemy : std::as_const(m_enemies)) {
        mScene.removeItem(enemy);

    }
    m_enemies.clear();

    if (m_scoreTimer && m_scoreTimer->isActive()) {
        m_scoreTimer->stop();
    }




    EndGameWidget *endWidget = new EndGameWidget(m_currentScore, this);
    connect(endWidget, &EndGameWidget::returnToMainMenu, this, &Widget::returnToMainMenu);

    endWidget->show();
}

void Widget::initBGM()
{
    m_bgmPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    audioOutput->setVolume(50);
    m_bgmPlayer->setAudioOutput(audioOutput);

    m_bgmPlayer->setSource(QUrl("qrc:/resource/doloris/doloris.mp3"));

    // 设置循环播放
    connect(m_bgmPlayer, &QMediaPlayer::mediaStatusChanged, [this](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::EndOfMedia) {
            m_bgmPlayer->setPosition(0);
            m_bgmPlayer->play();
        }
    });
    m_bgmPlayer->play();

}
void Widget::returnToMainMenu()
{
    // 重置游戏状态
    m_gameActive = false;
    m_menuWidget->show();
    mGameView.hide();
    m_currentScore = 0;
    if(oblivionis) oblivionis->deleteLater();
    qDeleteAll(m_enemies);
    m_enemies.clear();
    m_currentSpawnInterval=2000;//重置敌人重生速度
    if(m_scoreDisplay) {
        delete m_scoreDisplay;   //重置qlcd对象，避免了显示错误的问题
        m_scoreDisplay = nullptr;
    }
    m_menuWidget->startmedia();

}


Widget::~Widget()
{
    killTimer(m_spawnTimerId);
    qDeleteAll(m_enemies);
    delete ui;

}
