#include "SettingView.h"

SettingView *SettingView::getInstance()
{
    return Singleton<SettingView>::instance(SettingView::createInstance);
}

SettingView *SettingView::createInstance()
{
    return new SettingView;
}

SettingView::SettingView(QWidget *parent) : QDialog(parent)
{
    _settings = Settings::getInstance();

    _setupUi();
    _makeConnections();

    _ui_lst_gesture->addItems(_settings->gesture_list);
    _ui_tab_widget->setCurrentIndex(0);
}

void SettingView::setToCurrentSettings()
{
    blockSignals(true);
    _ui_sld_hue1->setValue(_settings->skin_color_min_H);
    _ui_sld_hue2->setValue(_settings->skin_color_max_H);
    _ui_sld_saturation1->setValue(_settings->skin_color_min_S);
    _ui_sld_saturation2->setValue(_settings->skin_color_max_S);
    _ui_sld_value1->setValue(_settings->skin_color_min_V);
    _ui_sld_horizon1->setValue(_settings->roi_start_x);
    _ui_sld_horizon2->setValue(_settings->roi_end_x);
    _ui_sld_vertical1->setValue(_settings->roi_start_y);

    blockSignals(false);
    _ui_sld_value2->setValue(_settings->skin_color_max_V);
    _ui_sld_vertical2->setValue(_settings->roi_end_y);
    _ui_box_detection_area->setValue(_settings->skin_detection_area);
    _ui_box_morphology->setChecked(_settings->skin_morphology);
}

void SettingView::setToDefaultSettings()
{
    blockSignals(true);
    _ui_sld_hue1->setValue(DEFAULT_SKIN_COLOR_MIN_H);
    _ui_sld_hue2->setValue(DEFAULT_SKIN_COLOR_MAX_H);
    _ui_sld_saturation1->setValue(DEFAULT_SKIN_COLOR_MIN_S);
    _ui_sld_saturation2->setValue(DEFAULT_SKIN_COLOR_MAX_S);
    _ui_sld_value1->setValue(DEFAULT_SKIN_COLOR_MIN_V);
    _ui_sld_horizon1->setValue(DEFAULT_ROI_START_X);
    _ui_sld_horizon2->setValue(DEFAULT_ROI_END_X);
    _ui_sld_vertical1->setValue(DEFAULT_ROI_START_Y);

    blockSignals(false);
    _ui_sld_value2->setValue(DEFAULT_SKIN_COLOR_MAX_H);
    _ui_sld_vertical2->setValue(DEFAULT_ROI_END_Y);
    _ui_box_detection_area->setValue(DEFAULT_SKIN_DETECTION_AREA);
    _ui_box_morphology->setChecked(DEFAULT_SKIN_MORPHOLOGY);
}

void SettingView::activatePage(const SettingView::SETTING_VIEW_PAGE &page)
{
    _ui_tab_widget->setCurrentIndex(static_cast<int>(page));
}

void SettingView::syncSettings()
{
    if (_flag_change_hue)
    {
        _settings->setSkinColorMinH(_ui_sld_hue1->value() < _ui_sld_hue2->value() ? _ui_sld_hue1->value() : _ui_sld_hue2->value());
        _settings->setSkinColorMaxH(_ui_sld_hue1->value() > _ui_sld_hue2->value() ? _ui_sld_hue1->value() : _ui_sld_hue2->value());
        _flag_change_hue = false;
    }
    if (_flag_change_saturation)
    {
        _settings->setSkinColorMinS(_ui_sld_saturation1->value() < _ui_sld_saturation2->value() ? _ui_sld_saturation1->value() : _ui_sld_saturation2->value());
        _settings->setSkinColorMaxS(_ui_sld_saturation1->value() > _ui_sld_saturation2->value() ? _ui_sld_saturation1->value() : _ui_sld_saturation2->value());
        _flag_change_saturation = false;
    }
    if (_flag_change_value)
    {
        _settings->setSkinColorMinV(_ui_sld_value1->value() < _ui_sld_value2->value() ? _ui_sld_value1->value() : _ui_sld_value2->value());
        _settings->setSkinColorMaxV(_ui_sld_value1->value() > _ui_sld_value2->value() ? _ui_sld_value1->value() : _ui_sld_value2->value());
        _flag_change_value = false;
    }
    if (_flag_change_roi_x)
    {
        _settings->setRoiStartX(_ui_sld_horizon1->value() < _ui_sld_horizon2->value() ? _ui_sld_horizon1->value() : _ui_sld_horizon2->value());
        _settings->setRoiEndX(  _ui_sld_horizon1->value() > _ui_sld_horizon2->value() ? _ui_sld_horizon1->value() : _ui_sld_horizon2->value());
        _flag_change_roi_x = false;
    }
    if (_flag_change_roi_y)
    {
        _settings->setRoiStartY(_ui_sld_vertical1->value() < _ui_sld_vertical2->value() ? _ui_sld_vertical1->value() : _ui_sld_vertical2->value());
        _settings->setRoiEndY(  _ui_sld_vertical1->value() > _ui_sld_vertical2->value() ? _ui_sld_vertical1->value() : _ui_sld_vertical2->value());
        _flag_change_roi_y = false;
    }
    if (_flag_change_detection_area)
    {
        _settings->setSkinDetectionArea(_ui_box_detection_area->value());
        _flag_change_detection_area = false;
    }
    if (_flag_change_morphology)
    {
        _settings->setSkinMorphology(_ui_box_morphology->isChecked());
        _flag_change_morphology = false;
    }
}

void SettingView::_uiSldHue1ValueChanged(const int &val)
{
    _flag_change_hue = true;
    if (val < _ui_sld_hue2->value())
        _ui_lbl_hue_range->setText(QString("%1 - %2").arg(QString::number(val), QString::number(_ui_sld_hue2->value())));
    else
        _ui_lbl_hue_range->setText(QString("%1 - %2").arg(QString::number(_ui_sld_hue2->value()), QString::number(val)));
    _emitSkinColorRangeChangeSignal();
}

void SettingView::_uiSldHue2ValueChanged(const int &val)
{
    _flag_change_hue = true;
    if (val < _ui_sld_hue1->value())
        _ui_lbl_hue_range->setText(QString("%1 - %2").arg(QString::number(val), QString::number(_ui_sld_hue1->value())));
    else
        _ui_lbl_hue_range->setText(QString("%1 - %2").arg(QString::number(_ui_sld_hue1->value()), QString::number(val)));
    _emitSkinColorRangeChangeSignal();
}

void SettingView::_uiSldSaturation1ValueChanged(const int &val)
{
    _flag_change_saturation = true;
    if (val < _ui_sld_saturation2->value())
        _ui_lbl_saturation_range->setText(QString("%1 - %2").arg(QString::number(val), QString::number(_ui_sld_saturation2->value())));
    else
        _ui_lbl_saturation_range->setText(QString("%1 - %2").arg(QString::number(_ui_sld_saturation2->value()), QString::number(val)));
    _emitSkinColorRangeChangeSignal();
}

void SettingView::_uiSldSaturation2ValueChanged(const int &val)
{
    _flag_change_saturation = true;
    if (val < _ui_sld_saturation1->value())
        _ui_lbl_saturation_range->setText(QString("%1 - %2").arg(QString::number(val), QString::number(_ui_sld_saturation1->value())));
    else
        _ui_lbl_saturation_range->setText(QString("%1 - %2").arg(QString::number(_ui_sld_saturation1->value()), QString::number(val)));
    _emitSkinColorRangeChangeSignal();
}

void SettingView::_uiSldValue1ValueChanged(const int &val)
{
    _flag_change_value = true;
    if (val < _ui_sld_value2->value())
        _ui_lbl_value_range->setText(QString("%1 - %2").arg(QString::number(val), QString::number( _ui_sld_value2->value())));
    else
        _ui_lbl_value_range->setText(QString("%1 - %2").arg(QString::number(_ui_sld_value2->value()), QString::number(val)));
    _emitSkinColorRangeChangeSignal();
}

void SettingView::_uiSldValue2ValueChanged(const int &val)
{
    _flag_change_value = true;
    if (val < _ui_sld_value1->value())
        _ui_lbl_value_range->setText(QString("%1 - %2").arg(QString::number(val), QString::number(_ui_sld_value1->value())));
    else
        _ui_lbl_value_range->setText(QString("%1 - %2").arg(QString::number(_ui_sld_value1->value()), QString::number(val)));
    _emitSkinColorRangeChangeSignal();
}

void SettingView::_uiSldHorizon1ValueChanged(const int &val)
{
    _flag_change_roi_x = true;
    if (val < _ui_sld_horizon2->value())
        _ui_lbl_horizon_range->setText(QString("%1% - %2%").arg(QString::number(val), QString::number(_ui_sld_horizon2->value())));
    else
        _ui_lbl_horizon_range->setText(QString("%1% - %2%").arg(QString::number(_ui_sld_horizon2->value()), QString::number(val)));
    _emitRoiRangeChangeSignal();
}

void SettingView::_uiSldHorizon2ValueChanged(const int &val)
{
    _flag_change_roi_x = true;
    if (val < _ui_sld_horizon1->value())
        _ui_lbl_horizon_range->setText(QString("%1% - %2%").arg(QString::number(val), QString::number(_ui_sld_horizon1->value())));
    else
        _ui_lbl_horizon_range->setText(QString("%1% - %2%").arg(QString::number(_ui_sld_horizon1->value()), QString::number(val)));
    _emitRoiRangeChangeSignal();
}

void SettingView::_uiSldVertical1ValueChanged(const int &val)
{
    _flag_change_roi_y = true;
    if (val < _ui_sld_vertical2->value())
        _ui_lbl_vertical_range->setText(QString("%1% - %2%").arg(QString::number(val), QString::number(_ui_sld_vertical2->value())));
    else
        _ui_lbl_vertical_range->setText(QString("%1% - %2%").arg(QString::number(_ui_sld_vertical2->value()), QString::number(val)));
    _emitRoiRangeChangeSignal();
}

void SettingView::_uiSldVertical2ValueChanged(const int &val)
{
    _flag_change_roi_y = true;
    if (val < _ui_sld_vertical1->value())
        _ui_lbl_vertical_range->setText(QString("%1% - %2%").arg(QString::number(val), QString::number(_ui_sld_vertical1->value())));
    else
        _ui_lbl_vertical_range->setText(QString("%1% - %2%").arg(QString::number(_ui_sld_vertical1->value()), QString::number(val)));
    _emitRoiRangeChangeSignal();
}

void SettingView::_uiBoxDetectionAreaValueChanged(const int &val)
{
    _flag_change_detection_area = true;
    emit changeDetectionArea(val);
}

void SettingView::_uiBoxMorphologyToggled(const bool &checked)
{
    _flag_change_morphology = true;
    emit changeMorphology(checked);
}

void SettingView::_emitSkinColorRangeChangeSignal()
{
    emit changeSkinColorLowerBound(_ui_sld_hue1->value()        < _ui_sld_hue2->value()        ? _ui_sld_hue1->value() : _ui_sld_hue2->value(),
                                   _ui_sld_saturation1->value() < _ui_sld_saturation2->value() ? _ui_sld_saturation1->value() : _ui_sld_saturation2->value(),
                                   _ui_sld_value1->value()      < _ui_sld_value2->value()      ? _ui_sld_value1->value() : _ui_sld_value2->value());
    emit changeSkinColorUpperBound(_ui_sld_hue1->value()        > _ui_sld_hue2->value()        ? _ui_sld_hue1->value() : _ui_sld_hue2->value(),
                                   _ui_sld_saturation1->value() > _ui_sld_saturation2->value() ? _ui_sld_saturation1->value() : _ui_sld_saturation2->value(),
                                   _ui_sld_value1->value()      > _ui_sld_value2->value()      ? _ui_sld_value1->value() : _ui_sld_value2->value());
}

void SettingView::_emitRoiRangeChangeSignal()
{
    emit changeRoiRange(_ui_sld_horizon1->value()  < _ui_sld_horizon2->value()  ? _ui_sld_horizon1->value() : _ui_sld_horizon2->value(),
                        _ui_sld_horizon1->value()  > _ui_sld_horizon2->value()  ? _ui_sld_horizon1->value() : _ui_sld_horizon2->value(),
                        _ui_sld_vertical1->value() < _ui_sld_vertical2->value() ? _ui_sld_vertical1->value() : _ui_sld_vertical2->value(),
                        _ui_sld_vertical1->value() > _ui_sld_vertical2->value() ? _ui_sld_vertical1->value() : _ui_sld_vertical2->value());
}

void SettingView::_backgroundSettingRequest()
{
    _ui_btn_bg_clear->setEnabled(false);
    emit backgroundSettingRequest();
}

void SettingView::_backgroundClearingRequest()
{
    _ui_btn_bg_clear->setEnabled(false);
    emit backgroundClearingRequest();
}

void SettingView::enableBackgroundSetting()
{
    _ui_btn_bg_set->setEnabled(true);
}

void SettingView::disableBackgroundSetting()
{
    _ui_btn_bg_set->setEnabled(false);
}

void SettingView::setBackgroundImage(const QPixmap &img)
{
    _ui_lbl_background->setPixmap(img.scaled(
                                      QSize(_ui_lbl_background->width(), _ui_lbl_background->height()),
                                      Qt::KeepAspectRatio
                                      ));
    _ui_btn_bg_clear->setEnabled(true);
}

void SettingView::clearBackgroundImage()
{
    _ui_lbl_background->clear();
}

void SettingView::_gestureListAdd()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Add New Gesture"),
                                         tr("Gesture Name"),
                                         QLineEdit::Normal, QString(),
                                         &ok);
    text = _settings->filterGestureName(text);
    if (ok && !text.isEmpty())
    {
        int indx = _settings->addGestrue(text);
        if (indx == _ui_lst_gesture->count())
        {
            _ui_lst_gesture->addItem(text);
            emit changeLabelList();
        }
        else
        {
            QMessageBox::warning(this, "",
                                 QString(tr("<span style=\"font-weight:100\">Gesture named <span style=\"font-weight:bold\">%1</span> exists already.</span>")).arg(text),
                                 QMessageBox::Ok);
            _ui_lst_gesture->setCurrentRow(indx);
        }
    }
}

void SettingView::_gestureListRemove()
{
    if (_ui_lst_gesture->currentRow() > -1)
    {
        auto res = QMessageBox::question(this, tr("Remove Gesture Category"),
                                         QString(tr("<span style=\"font-weight:100\">"
                                                    "Do you really want to remove the gesture category "
                                                    "<span style=\"font-weight:bold\">%1</span> ?</span>")
                                                 ).arg(_ui_lst_gesture->currentItem()->text()),
                                         QMessageBox::Cancel, QMessageBox::Yes);
        if (res == QMessageBox::Yes)
        {
            _settings->removeGesture(_ui_lst_gesture->currentRow());
            delete _ui_lst_gesture->currentItem();
            emit changeLabelList();
        }
    }
}

void SettingView::_gestureEdit(QListWidgetItem *item)
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Modify Gesture Name"),
                                         QString(tr("New Name for %1")).arg(item->text()),
                                         QLineEdit::Normal,
                                         item->text(), &ok);
    text = _settings->filterGestureName(text);
    if (ok && !text.isEmpty())
    {
        int indx = _settings->gesture_list.indexOf(text);
        if (indx == -1)
        {
            _settings->modifyGestureName(_ui_lst_gesture->currentRow(), text);
            item->setText(text);
            emit changeLabelList();
        }
        else
        {
            QMessageBox::warning(this, "",
                                 QString(tr("<span style=\"font-weight:100\">Gesture named <span style=\"font-weight:bold\">%1</span> exists already.</span>")).arg(text),
                                 QMessageBox::Ok);
            _ui_lst_gesture->setCurrentRow(indx);
        }
    }
}

void SettingView::closeEvent(QCloseEvent *e)
{
    syncSettings();
    e->accept();
}

void SettingView::_setupUi()
{QLabel * ui_lbl_hue = new QLabel(tr("Hue"));
    QLabel * ui_lbl_saturation = new QLabel(tr("Saturation"));
    QLabel * ui_lbl_value = new QLabel(tr("Value"));
    _ui_lbl_hue_range = new QLabel;
    _ui_lbl_saturation_range = new QLabel;
    _ui_lbl_value_range = new QLabel;
    _ui_lbl_hue_range->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ui_lbl_saturation_range->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ui_lbl_value_range->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ui_sld_hue1 = new QSlider(Qt::Horizontal);
    _ui_sld_saturation1 = new QSlider(Qt::Horizontal);
    _ui_sld_value1 = new QSlider(Qt::Horizontal);
    _ui_sld_hue2 = new QSlider(Qt::Horizontal);
    _ui_sld_saturation2 = new QSlider(Qt::Horizontal);
    _ui_sld_value2 = new QSlider(Qt::Horizontal);
    _ui_sld_hue1->setRange(0, 255);
    _ui_sld_saturation1->setRange(0, 255);
    _ui_sld_value1->setRange(0, 255);
    _ui_sld_hue2->setRange(0, 255);
    _ui_sld_saturation2->setRange(0, 255);
    _ui_sld_value2->setRange(0, 255);
    _ui_sld_hue1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ui_sld_saturation1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ui_sld_value1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ui_sld_hue2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ui_sld_saturation2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ui_sld_value2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    QGridLayout * ui_group_skin_color_layout = new QGridLayout;
    ui_group_skin_color_layout->addWidget(ui_lbl_hue,               0, 0, 1, 1, Qt::AlignHCenter);
    ui_group_skin_color_layout->addWidget(ui_lbl_saturation,        0, 1, 1, 1, Qt::AlignHCenter);
    ui_group_skin_color_layout->addWidget(ui_lbl_value,             0, 2, 1, 1, Qt::AlignHCenter);
    ui_group_skin_color_layout->addWidget(_ui_sld_hue1,             1, 0, 1, 1, Qt::AlignHCenter);
    ui_group_skin_color_layout->addWidget(_ui_sld_saturation1,      1, 1, 1, 1, Qt::AlignHCenter);
    ui_group_skin_color_layout->addWidget(_ui_sld_value1,           1, 2, 1, 1, Qt::AlignHCenter);
    ui_group_skin_color_layout->addWidget(_ui_lbl_hue_range,        2, 0, 1, 1, Qt::AlignHCenter);
    ui_group_skin_color_layout->addWidget(_ui_lbl_saturation_range, 2, 1, 1, 1, Qt::AlignHCenter);
    ui_group_skin_color_layout->addWidget(_ui_lbl_value_range,      2, 2, 1, 1, Qt::AlignHCenter);
    ui_group_skin_color_layout->addWidget(_ui_sld_hue2,             3, 0, 1, 1, Qt::AlignHCenter);
    ui_group_skin_color_layout->addWidget(_ui_sld_saturation2,      3, 1, 1, 1, Qt::AlignHCenter);
    ui_group_skin_color_layout->addWidget(_ui_sld_value2,           3, 2, 1, 1, Qt::AlignHCenter);
    QGroupBox * ui_group_skin_color = new QGroupBox(tr("Skin Color Filter"));
    ui_group_skin_color->setLayout(ui_group_skin_color_layout);

    QLabel * ui_lbl_horizon = new QLabel(tr("Horizon"));
    QLabel * ui_lbl_vertical = new QLabel(tr("Vertical"));
    _ui_lbl_horizon_range = new QLabel;
    _ui_lbl_vertical_range = new QLabel;
    _ui_lbl_horizon_range->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ui_lbl_vertical_range->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ui_sld_horizon1 = new QSlider(Qt::Horizontal);
    _ui_sld_vertical1 = new QSlider(Qt::Horizontal);
    _ui_sld_horizon2 = new QSlider(Qt::Horizontal);
    _ui_sld_vertical2 = new QSlider(Qt::Horizontal);
    _ui_sld_horizon1->setRange(0, 100);
    _ui_sld_vertical1->setRange(0, 100);
    _ui_sld_horizon2->setRange(0, 100);
    _ui_sld_vertical2->setRange(0, 100);
    _ui_sld_horizon1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ui_sld_vertical1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ui_sld_horizon2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ui_sld_vertical2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    QGridLayout * ui_group_roi_layout = new QGridLayout;
    ui_group_roi_layout->addWidget(ui_lbl_horizon,         0, 0, 1, 1, Qt::AlignHCenter);
    ui_group_roi_layout->addWidget(ui_lbl_vertical,        0, 1, 1, 1, Qt::AlignHCenter);
    ui_group_roi_layout->addWidget(_ui_sld_horizon1,       1, 0, 1, 1, Qt::AlignHCenter);
    ui_group_roi_layout->addWidget(_ui_sld_vertical1,      1, 1, 1, 1, Qt::AlignHCenter);
    ui_group_roi_layout->addWidget(_ui_lbl_horizon_range,  2, 0, 1, 1, Qt::AlignHCenter);
    ui_group_roi_layout->addWidget(_ui_lbl_vertical_range, 2, 1, 1, 1, Qt::AlignHCenter);
    ui_group_roi_layout->addWidget(_ui_sld_horizon2,       3, 0, 1, 1, Qt::AlignHCenter);
    ui_group_roi_layout->addWidget(_ui_sld_vertical2,      3, 1, 1, 1, Qt::AlignHCenter);
    QLabel * ui_group_roi_spacer = new QLabel;
    ui_group_roi_layout->addWidget(ui_group_roi_spacer, 0, 2, 1, 1);
    QGroupBox * ui_group_roi = new QGroupBox(tr("Region of Interesting"));
    ui_group_roi->setLayout(ui_group_roi_layout);

    QLabel * ui_lbl_detection_area = new QLabel(tr("Detection Area"));
    _ui_box_detection_area = new QSpinBox;
    _ui_box_detection_area->setRange(1000, 100000);
    _ui_box_detection_area->setSingleStep(100);
    _ui_box_detection_area->setFocusPolicy(Qt::StrongFocus);
    _ui_box_detection_area->setStyleSheet("QSpinBox {outline:none}");
    _ui_box_morphology = new QCheckBox(tr("Morphological Transformation"));
    QGridLayout * ui_group_others_layout = new QGridLayout;
    ui_group_others_layout->addWidget(ui_lbl_detection_area,  0, 0, 1, 1);
    ui_group_others_layout->addWidget(_ui_box_detection_area, 0, 1, 1, 1);
    ui_group_others_layout->addWidget(_ui_box_morphology,     1, 0, 1, 2);
    QGroupBox * ui_group_others = new QGroupBox(tr("Miscellanea"));
    ui_group_others->setLayout(ui_group_others_layout);

    _ui_btn_general_reset = new QPushButton(tr("Reset"));
    _ui_btn_general_default = new QPushButton(tr("Reset to Default"));
    _ui_btn_general_reset->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ui_btn_general_reset->setAutoDefault(false);
    _ui_btn_general_reset->setFocusPolicy(Qt::NoFocus);
    _ui_btn_general_default->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ui_btn_general_default->setAutoDefault(false);
    _ui_btn_general_default->setFocusPolicy(Qt::NoFocus);
    QHBoxLayout * ui_general_page_button_box = new QHBoxLayout;
    ui_general_page_button_box->setContentsMargins(0, 0, 0, 0);
    ui_general_page_button_box->setSpacing(0);
    ui_general_page_button_box->addWidget(_ui_btn_general_default, 0, Qt::AlignLeft);
    ui_general_page_button_box->addWidget(_ui_btn_general_reset, 0, Qt::AlignRight);

    QVBoxLayout * ui_general_page_layout = new QVBoxLayout;
    ui_general_page_layout->setContentsMargins(10, 10, 10, 0);
    ui_general_page_layout->setSpacing(5);
    ui_general_page_layout->addWidget(ui_group_skin_color);
    ui_general_page_layout->addWidget(ui_group_roi);
    ui_general_page_layout->addWidget(ui_group_others);
    ui_general_page_layout->addLayout(ui_general_page_button_box);
    QWidget * ui_general_page = new QWidget;
    ui_general_page->setLayout(ui_general_page_layout);

    _ui_lbl_background = new QLabel;
    _ui_lbl_background->setAlignment(Qt::AlignCenter);
    _ui_lbl_background->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _ui_btn_bg_clear = new QPushButton(tr("Clear"));
    _ui_btn_bg_set = new QPushButton(tr("Set Background"));
    _ui_btn_bg_clear->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ui_btn_bg_clear->setAutoDefault(false);
    _ui_btn_bg_clear->setFocusPolicy(Qt::NoFocus);
    _ui_btn_bg_set->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ui_btn_bg_set->setAutoDefault(false);
    _ui_btn_bg_set->setFocusPolicy(Qt::NoFocus);
    QHBoxLayout * ui_background_page_button_box = new QHBoxLayout;
    ui_background_page_button_box->setContentsMargins(10, 0, 10, 0);
    ui_background_page_button_box->setSpacing(0);
    ui_background_page_button_box->addWidget(_ui_btn_bg_clear, 0, Qt::AlignLeft);
    ui_background_page_button_box->addWidget(_ui_btn_bg_set, 0, Qt::AlignRight);
    QVBoxLayout * ui_background_page_layout = new QVBoxLayout;
    ui_background_page_layout->setContentsMargins(0, 0, 0, 0);
    ui_background_page_layout->setSpacing(0);
    ui_background_page_layout->addWidget(_ui_lbl_background);
    ui_background_page_layout->addLayout(ui_background_page_button_box);
    QWidget * ui_background_page = new QWidget;
    ui_background_page->setLayout(ui_background_page_layout);

    _ui_lst_gesture = new QListWidget;
    _ui_lst_gesture->setContentsMargins(0, 0, 0, 0);
    _ui_lst_gesture->setFocusPolicy(Qt::NoFocus);
    _ui_btn_gesture_add = new QPushButton(tr("Add"));
    _ui_btn_gesture_remove = new QPushButton(tr("Remove"));
    _ui_btn_gesture_add->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ui_btn_gesture_add->setAutoDefault(false);
    _ui_btn_gesture_add->setFocusPolicy(Qt::NoFocus);
    _ui_btn_gesture_remove->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ui_btn_gesture_remove->setAutoDefault(false);
    _ui_btn_gesture_remove->setFocusPolicy(Qt::NoFocus);
    QHBoxLayout * ui_gesture_page_button_box = new QHBoxLayout;
    ui_gesture_page_button_box->setContentsMargins(10, 0, 10, 0);
    ui_gesture_page_button_box->setSpacing(0);
    ui_gesture_page_button_box->addWidget(_ui_btn_gesture_remove, 0, Qt::AlignLeft);
    ui_gesture_page_button_box->addWidget(_ui_btn_gesture_add, 0, Qt::AlignRight);
    QVBoxLayout * ui_gesture_page_layout = new QVBoxLayout;
    ui_gesture_page_layout->setContentsMargins(0, 0, 0, 0);
    ui_gesture_page_layout->setSpacing(0);
    ui_gesture_page_layout->addWidget(_ui_lst_gesture);
    ui_gesture_page_layout->addLayout(ui_gesture_page_button_box);
    QWidget * ui_gesture_list_page = new QWidget;
    ui_gesture_list_page->setLayout(ui_gesture_page_layout);

    QVBoxLayout * ui_keymap_page_layout = new QVBoxLayout;
    QWidget * ui_keymap_page = new QWidget;
    ui_keymap_page->setLayout(ui_keymap_page_layout);

    _ui_tab_widget = new QTabWidget;
    _ui_tab_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _ui_tab_widget->addTab(ui_general_page, tr("General"));
    _ui_tab_widget->addTab(ui_background_page, tr("Background"));
    _ui_tab_widget->addTab(ui_gesture_list_page, tr("Gestures"));
    _ui_tab_widget->addTab(ui_keymap_page, tr("Key Map"));

    QVBoxLayout * ui_main_layout = new QVBoxLayout;
    ui_main_layout->addWidget(_ui_tab_widget);
    setLayout(ui_main_layout);

    QFont font;
    if (font.pixelSize() < 0)
    {
        if (font.pointSize() > 10)
            font.setPointSize(font.pointSize()-2);
    }
    else if (font.pixelSize() > 11)
        font.setPixelSize(font.pixelSize()-2);
    ui_lbl_hue->setFont(font);
    ui_lbl_saturation->setFont(font);
    ui_lbl_value->setFont(font);
    ui_lbl_horizon->setFont(font);
    ui_lbl_vertical->setFont(font);
    ui_lbl_detection_area->setFont(font);
    _ui_box_detection_area->setFont(font);
    _ui_box_morphology->setFont(font);
    if (font.pixelSize() < 0)
    {
        if (font.pointSize() > 8)
            font.setPointSize(font.pointSize()-2);
    }
    else if (font.pixelSize() > 9)
        font.setPixelSize(font.pixelSize()-2);
    _ui_lbl_hue_range->setFont(font);
    _ui_lbl_saturation_range->setFont(font);
    _ui_lbl_value_range->setFont(font);
    _ui_lbl_horizon_range->setFont(font);
    _ui_lbl_vertical_range->setFont(font);

    _ui_btn_bg_set->setEnabled(false);
    _ui_btn_bg_clear->setEnabled(false);

    setFixedSize(minimumSize());
    setWindowTitle(tr("Settings"));
}

void SettingView::_makeConnections()
{
    connect(_ui_sld_hue1,           SIGNAL(valueChanged(int)), this, SLOT(_uiSldHue1ValueChanged(int)));
    connect(_ui_sld_hue2,           SIGNAL(valueChanged(int)), this, SLOT(_uiSldHue2ValueChanged(int)));
    connect(_ui_sld_saturation1,    SIGNAL(valueChanged(int)), this, SLOT(_uiSldSaturation1ValueChanged(int)));
    connect(_ui_sld_saturation2,    SIGNAL(valueChanged(int)), this, SLOT(_uiSldSaturation2ValueChanged(int)));
    connect(_ui_sld_value1,         SIGNAL(valueChanged(int)), this, SLOT(_uiSldValue1ValueChanged(int)));
    connect(_ui_sld_value2,         SIGNAL(valueChanged(int)), this, SLOT(_uiSldValue2ValueChanged(int)));
    connect(_ui_sld_horizon1,       SIGNAL(valueChanged(int)), this, SLOT(_uiSldHorizon1ValueChanged(int)));
    connect(_ui_sld_horizon2,       SIGNAL(valueChanged(int)), this, SLOT(_uiSldHorizon2ValueChanged(int)));
    connect(_ui_sld_vertical1,      SIGNAL(valueChanged(int)), this, SLOT(_uiSldVertical1ValueChanged(int)));
    connect(_ui_sld_vertical2,      SIGNAL(valueChanged(int)), this, SLOT(_uiSldVertical2ValueChanged(int)));
    connect(_ui_box_detection_area, SIGNAL(valueChanged(int)), this, SLOT(_uiBoxDetectionAreaValueChanged(int)));
    connect(_ui_box_morphology,     SIGNAL(toggled(bool)),     this, SLOT(_uiBoxMorphologyToggled(bool)));
    connect(_ui_btn_general_reset,  SIGNAL(released()),        this, SLOT(setToCurrentSettings()));
    connect(_ui_btn_general_default,SIGNAL(released()),        this, SLOT(setToDefaultSettings()));
    connect(_ui_btn_bg_set,         SIGNAL(released()),        this, SLOT(_backgroundSettingRequest()));
    connect(_ui_btn_bg_clear,       SIGNAL(released()),        this, SLOT(_backgroundClearingRequest()));
    connect(_ui_btn_gesture_add,    SIGNAL(released()),        this, SLOT(_gestureListAdd()));
    connect(_ui_btn_gesture_remove, SIGNAL(released()),        this, SLOT(_gestureListRemove()));
    connect(_ui_lst_gesture,        SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(_gestureEdit(QListWidgetItem*)));
}
