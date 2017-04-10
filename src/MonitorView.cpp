#include "MonitorView.h"

MonitorView *MonitorView::getInstance()
{
    return Singleton<MonitorView>::instance(MonitorView::createInstance);
}

MonitorView *MonitorView::createInstance()
{
    return new MonitorView;
}

MonitorView::MonitorView(QWidget *parent) : QDialog(parent)
{
    _ui_lbl_image1 = new QLabel;
    _ui_lbl_image2 = new QLabel;
    _ui_lbl_image3 = new QLabel;
    _ui_lbl_text   = new QLabel;
    _ui_lbl_image1->setAlignment(Qt::AlignCenter);
    _ui_lbl_image2->setAlignment(Qt::AlignCenter);
    _ui_lbl_image3->setAlignment(Qt::AlignCenter);
    _ui_lbl_text->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    QGridLayout * main_layout = new QGridLayout;
    main_layout->setContentsMargins(10, 10, 10, 10);
    main_layout->setSpacing(10);
    main_layout->addWidget(_ui_lbl_image1, 0, 0, 1, 1);
    main_layout->addWidget(_ui_lbl_image2, 0, 1, 1, 1);
    main_layout->addWidget(_ui_lbl_image3, 1, 0, 1, 1);
    main_layout->addWidget(_ui_lbl_text, 1, 1, 1, 1);

    setLayout(main_layout);
    setWindowTitle(tr("Monitor"));
    resize(380, 290);
    setMinimumSize(200, 200);
}

void MonitorView::updateMonitorImage3(const QPixmap &image1,
                                     const QPixmap &image2,
                                     const QPixmap &image3)
{
    _ui_lbl_image1->setPixmap(image1.scaled(
                                  _ui_lbl_image1->width(),
                                  _ui_lbl_image1->height(),
                                  Qt::KeepAspectRatio
                                  ));
    _ui_lbl_image2->setPixmap(image2.scaled(
                                  _ui_lbl_image2->width(),
                                  _ui_lbl_image2->height(),
                                  Qt::KeepAspectRatio
                                  ));
    _ui_lbl_image3->setPixmap(image3.scaled(
                                  _ui_lbl_image3->width(),
                                  _ui_lbl_image3->height(),
                                  Qt::KeepAspectRatio
                                  ));
}

void MonitorView::updateMonitorImage2(const QPixmap &image1, const QPixmap &image2)
{
    _ui_lbl_image1->setPixmap(image1.scaled(
                                  _ui_lbl_image1->width(),
                                  _ui_lbl_image1->height(),
                                  Qt::KeepAspectRatio
                                  ));
    _ui_lbl_image2->setPixmap(image2.scaled(
                                  _ui_lbl_image2->width(),
                                  _ui_lbl_image2->height(),
                                  Qt::KeepAspectRatio
                                  ));
    _ui_lbl_image3->clear();
}

void MonitorView::setMsg(const QString &text)
{
    _ui_lbl_text->setText(text);
}

void MonitorView::closeEvent(QCloseEvent * e)
{
    _ui_lbl_image1->clear();
    _ui_lbl_image2->clear();
    _ui_lbl_image3->clear();
    _ui_lbl_text->clear();
    e->accept();
}

