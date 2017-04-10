#include "MainView.h"

MainView *MainView::getInstance()
{
    return Singleton<MainView>::instance(MainView::createInstance);
}

MainView *MainView::createInstance()
{
    return new MainView;
}

MainView::MainView(QWidget *parent)
    : QWidget(parent)
{
    _setupUi();
    _makeConnections();
}

void MainView::setStatusBar(const QString &msg)
{
    _ui_status_bar->setText(msg);
}

void MainView::changeWorkStatusToNothing()
{
    _ui_status_bar->setText(tr("Work Status") + ": " + tr("idle"));
    _ui_btn_controlling->setText(tr("Start Gesture Control System"));
    _ui_btn_sampling->setText(tr("Collect Samples"));
    _ui_btn_controlling->setEnabled(true);
    _ui_btn_sampling->setEnabled(true);
}

void MainView::changeWorkStatusToControlling()
{
    _ui_status_bar->setText(tr("Work Status") + ": " + tr("recognizing gestures"));
    _ui_btn_controlling->setText(tr("Gesture Control System is Working"));
    _ui_btn_sampling->setEnabled(false);
    _ui_btn_controlling->setEnabled(true);
}

void MainView::changeWorkStatusToSampling()
{
    _ui_status_bar->setText(tr("Work Status") + ": " + tr("collecting learning materials"));
    _ui_btn_sampling->setText(tr("System is Collecting Samples"));
    _ui_btn_controlling->setEnabled(false);
    _ui_btn_sampling->setEnabled(true);
}

void MainView::_setupUi()
{
    _ui_btn_controlling = new QPushButton;
    _ui_btn_sampling = new QPushButton;
    _ui_btn_setting = new QPushButton;

    _ui_btn_controlling->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    _ui_btn_controlling->setAutoDefault(false);
    _ui_btn_controlling->setFocusPolicy(Qt::NoFocus);
    _ui_btn_sampling->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    _ui_btn_sampling->setAutoDefault(false);
    _ui_btn_sampling->setFocusPolicy(Qt::NoFocus);
    _ui_btn_setting->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    _ui_btn_setting->setAutoDefault(false);
    _ui_btn_setting->setFocusPolicy(Qt::NoFocus);

    _ui_status_bar = new QLabel;
    _ui_status_bar->setContentsMargins(5, 10, 0, 0);
    QFont font;
    if (font.pixelSize() < 0)
    {
        if (font.pointSize() > 10)
            font.setPointSize(font.pointSize()-2);
    }
    else if (font.pixelSize() > 11)
        font.setPixelSize(font.pixelSize()-2);
    _ui_status_bar->setFont(font);
    _ui_status_bar->setText("Work Status: idle");

    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->setContentsMargins(10, 10, 10, 10);
    main_layout->setSpacing(0);
    main_layout->addWidget(_ui_btn_controlling);
    main_layout->addWidget(_ui_btn_sampling);
    main_layout->addWidget(_ui_btn_setting);
    main_layout->addWidget(_ui_status_bar);
    setLayout(main_layout);

    setFixedSize(minimumSize());

    changeWorkStatusToNothing();
    setWindowTitle(tr("Gesture Control System") + " -- " + tr("MTAGA"));
}

void MainView::_makeConnections()
{
    connect(_ui_btn_controlling, SIGNAL(released()), this, SIGNAL(controllingWindowRequest()));
    connect(_ui_btn_sampling, SIGNAL(released()), this, SIGNAL(samplingWindowRequest()));
    connect(_ui_btn_setting, SIGNAL(released()), this, SIGNAL(settingWindowRequest()));
}

void MainView::closeEvent(QCloseEvent *e)
{
    emit mainViewClosing();
    e->accept();
}
