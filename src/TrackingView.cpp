#include "TrackingView.h"

TrackingView *TrackingView::getInstance()
{
    return Singleton<TrackingView>::instance(TrackingView::createInstance);
}

TrackingView *TrackingView::createInstance()
{
    return new TrackingView;
}

TrackingView::TrackingView(QWidget *parent) : QDialog(parent),
    work_mode(_work_mode)
{
    _work_status = STATUS_WAITING_CAMERA;
    _settings = Settings::getInstance();

    _setupUi();
    _makeConnections();
    _loadSettings();
}

void TrackingView::setWorkMode(const TrackingView::WORK_MODE &mode)
{
    if (mode == MODE_SAMPLING)
    {
        controllingTaskStopped();
        _ui_control_form->hide();
        _ui_sample_form->show();
        setWindowTitle(tr("Collect Samples") + " -- " + tr("Gesture Control System"));
    }
    else
    {
        samplingTaskStopped();
        _ui_sample_form->hide();
        _ui_control_form->show();
        setWindowTitle(tr("Gesture Control System"));
    }
    _work_mode = mode;
}

void TrackingView::updateVideoFrame(const QPixmap &img)
{
    _ui_lbl_video->setPixmap(
                img.scaled(_ui_lbl_video->width(),
                           _ui_lbl_video->height(),
                           Qt::KeepAspectRatio)
                );
}

int TrackingView::getVideoFrameWidth()
{
    return _ui_lbl_video->width();
}

int TrackingView::getVideoFrameHeight()
{
    return _ui_lbl_video->height();
}

void TrackingView::appendText(const QString &text)
{
    _ui_txt_panel->appendPlainText(text);
}

void TrackingView::updateText(const QString &new_text)
{
    QTextCursor cursor = _ui_txt_panel->textCursor();
    _ui_txt_panel->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
    _ui_txt_panel->moveCursor(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
    _ui_txt_panel->moveCursor(QTextCursor::End, QTextCursor::KeepAnchor);
    _ui_txt_panel->textCursor().removeSelectedText();
    _ui_txt_panel->textCursor().deletePreviousChar();
    _ui_txt_panel->setTextCursor(cursor);
    _ui_txt_panel->appendPlainText(new_text);
}

void TrackingView::clearText()
{
    _ui_txt_panel->clear();
}

void TrackingView::setText(const QString &text)
{
    _ui_txt_panel->setPlainText(text);
}

void TrackingView::reloadLabelList()
{
    _ui_box_gesture_list->clear();
    _ui_box_gesture_list->addItems(_settings->gesture_list);
}

void TrackingView::cameraStarted()
{
    _ui_btn_start->setEnabled(true);
    if (_work_mode == MODE_CONTROLLING)
    {
        _ui_txt_model_file->setEnabled(true);
        _ui_btn_choose_model_file->setEnabled(true);
        _ui_txt_keymap_file->setEnabled(true);
        _ui_btn_choose_keymap_file->setEnabled(true);
        _ui_btn_start->setText(tr("Control"));
        _work_status = STATUS_IDLE;
    }
    else
    {
        _ui_btn_start->setText(tr("Sample"));
        _ui_box_gesture_list->setEnabled(true);
        _ui_btn_edit_gesture_list->setEnabled(true);
        _ui_txt_sample_dir->setEnabled(true);
        _ui_btn_choose_sample_folder->setEnabled(true);
        _work_status = STATUS_IDLE;
    }
}

void TrackingView::cameraReleased()
{
    if (_work_status == STATUS_WORKING)
    {
        if (_work_mode == MODE_SAMPLING)
            samplingTaskStopped();
        else
            controllingTaskStopped();
    }
    _work_status = STATUS_WAITING_CAMERA;
    _ui_btn_start->setText(tr("Launch Camera"));
}

void TrackingView::samplingTaskStarted()
{
    _ui_btn_start->setText(tr("Stop"));
    _ui_btn_start->setEnabled(true);
    _work_status = STATUS_WORKING;
}

void TrackingView::controllingTaskStarted()
{
    _ui_btn_start->setText(tr("Stop"));
    _ui_btn_start->setEnabled(true);
    _work_status = STATUS_WORKING;
}

void TrackingView::samplingTaskStopped()
{
    _ui_box_gesture_list->setEnabled(true);
    _ui_btn_edit_gesture_list->setEnabled(true);
    _ui_txt_sample_dir->setEnabled(true);
    _ui_btn_choose_sample_folder->setEnabled(true);
    _ui_btn_start->setEnabled(true);
    if (_work_status != STATUS_WAITING_CAMERA)
    {
        _work_status = STATUS_IDLE;
        _ui_btn_start->setText(tr("Sample"));
    }
}

void TrackingView::controllingTaskStopped()
{
    _ui_txt_model_file->setEnabled(true);
    _ui_btn_choose_model_file->setEnabled(true);
    _ui_txt_keymap_file->setEnabled(true);
    _ui_btn_choose_keymap_file->setEnabled(true);
    _ui_btn_start->setEnabled(true);
    if (_work_status != STATUS_WAITING_CAMERA)
    {
        _work_status = STATUS_IDLE;
        _ui_btn_start->setText(tr("Control"));
    }
}

void TrackingView::_uiBtnStartReleased()
{
    _ui_btn_start->setEnabled(false);
    _ui_box_gesture_list->setEnabled(false);
    _ui_btn_edit_gesture_list->setEnabled(false);
    _ui_txt_sample_dir->setEnabled(false);
    _ui_btn_choose_sample_folder->setEnabled(false);
    _ui_txt_model_file->setEnabled(false);
    _ui_btn_choose_model_file->setEnabled(false);
    _ui_txt_keymap_file->setEnabled(false);
    _ui_btn_choose_keymap_file->setEnabled(false);
    if (_work_status == STATUS_WAITING_CAMERA)
        emit cameraRequest();
    else if (_work_status == STATUS_IDLE)
    {
        if (_work_mode == MODE_CONTROLLING)
            emit controllingTaskStartRequest(_ui_txt_model_file->text(), _ui_txt_keymap_file->text());
        else
            emit samplingTaskStartRequest(_ui_box_gesture_list->currentIndex(), _ui_txt_sample_dir->text());
    }
    else if (_work_mode == MODE_CONTROLLING)
        emit controllingTaskStopRequest();
    else
        emit samplingTaskStopRequest();
}

void TrackingView::_uiBtnChooseSampleDirReleased()
{
    QString dir = _ui_txt_model_file->text();
    dir = QFileDialog::getExistingDirectory(this, tr("Select Sample Storage Path"),
                                            (!dir.isEmpty() && QFileInfo(dir).isDir()) ? dir : QDir::homePath(),
                                            QFileDialog::ShowDirsOnly|QFileDialog::DontUseNativeDialog);
    if (!dir.isEmpty())
        _ui_txt_sample_dir->setText(dir);
}

void TrackingView::_uiBtnChooseModelFileReleased()
{
    QString file = _ui_txt_model_file->text();
    QDir file_dir = QFileInfo(file).dir();
    file = QFileDialog::getOpenFileName(this, tr("Select Model File"),
                                        file_dir.exists() ? file_dir.absolutePath() : QDir::homePath(),
                                        QString(), Q_NULLPTR, QFileDialog::DontUseNativeDialog);
    if (!file.isEmpty())
        _ui_txt_model_file->setText(file);
}

void TrackingView::_uiBtnChooseKeymapFileReleased()
{
    QString file = _ui_txt_keymap_file->text();
    QDir file_dir = QFileInfo(file).dir();
    file = QFileDialog::getOpenFileName(this, tr("Select Keyboard Mapping File"),
                                        file_dir.exists() ? file_dir.absolutePath() : QDir::homePath(),
                                        QString(), Q_NULLPTR, QFileDialog::DontUseNativeDialog);
    if (!file.isEmpty())
        _ui_txt_keymap_file->setText(file);
}

void TrackingView::_uiTxtSampleDirChanged(const QString &folder_path)
{
    if (!folder_path.isEmpty())
    {
        if (QFileInfo(folder_path).isDir())
            _ui_txt_sample_dir->setStyleSheet("QLineEdit {color: auto}");
        else
            _ui_txt_sample_dir->setStyleSheet("QLineEdit {color: red}");
    }
}

void TrackingView::_uiTxtModelFileChanged(const QString &file_path)
{
    if (!file_path.isEmpty())
    {
        if (QFileInfo(file_path).isFile())
            _ui_txt_model_file->setStyleSheet("QLineEdit {color: auto}");
        else
            _ui_txt_model_file->setStyleSheet("QLineEdit {color: red}");
    }
}

void TrackingView::_uiTxtKeymapFileChanged(const QString &file_path)
{
    if (!file_path.isEmpty())
    {
        if (QFileInfo(file_path).isFile())
            _ui_txt_keymap_file->setStyleSheet("QLineEdit {color: auto}");
        else
            _ui_txt_keymap_file->setStyleSheet("QLineEdit {color: red}");
    }
}

void TrackingView::_setupUi()
{
    _ui_lbl_video = new QLabel;
    _ui_lbl_video->setFixedSize(640, 480);
    _ui_lbl_video->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    _ui_txt_panel = new QPlainTextEdit;
    _ui_txt_panel->setFixedSize(250, 300);
    _ui_txt_panel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ui_txt_panel->setReadOnly(true);

    _ui_btn_start = new QPushButton(tr("Launch Camera"));
    _ui_btn_monitor = new QPushButton(tr("Monitor"));
    _ui_btn_setting = new QPushButton(tr("Settings"));
    _ui_btn_start->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ui_btn_start->setAutoDefault(false);
    _ui_btn_start->setFocusPolicy(Qt::NoFocus);
    _ui_btn_monitor->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ui_btn_monitor->setAutoDefault(false);
    _ui_btn_monitor->setFocusPolicy(Qt::NoFocus);
    _ui_btn_monitor->setToolTip(tr("Show monitor window"));
    _ui_btn_setting->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ui_btn_setting->setAutoDefault(false);
    _ui_btn_setting->setFocusPolicy(Qt::NoFocus);
    _ui_btn_setting->setToolTip(tr("Show setting window"));

    QLabel *ui_lbl_gesture = new QLabel(tr("Gesture"));
    QLabel *ui_lbl_sample_folder = new QLabel(tr("Store to"));
    ui_lbl_gesture->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    ui_lbl_sample_folder->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ui_box_gesture_list = new QComboBox;
    _ui_txt_sample_dir = new QLineEdit;
    _ui_box_gesture_list->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    _ui_txt_sample_dir->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    _ui_btn_edit_gesture_list = new QPushButton("...");
    _ui_btn_choose_sample_folder = new QPushButton("...");
    _ui_btn_edit_gesture_list->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ui_btn_edit_gesture_list->setAutoDefault(false);
    _ui_btn_edit_gesture_list->setFocusPolicy(Qt::NoFocus);
    _ui_btn_edit_gesture_list->setToolTip(tr("Edit"));
    _ui_btn_choose_sample_folder->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ui_btn_choose_sample_folder->setAutoDefault(false);
    _ui_btn_choose_sample_folder->setFocusPolicy(Qt::NoFocus);
    _ui_btn_choose_sample_folder->setToolTip(tr("Browse"));
    _ui_sample_form = new QWidget;
    _ui_sample_form->hide();
    QGridLayout *ui_layout_sample_form = new QGridLayout(_ui_sample_form);
    ui_layout_sample_form->setContentsMargins(0, 0, 0, 0);
    ui_layout_sample_form->setSpacing(5);
    ui_layout_sample_form->addWidget(ui_lbl_gesture,               0, 0, 1, 1, Qt::AlignRight);
    ui_layout_sample_form->addWidget(_ui_box_gesture_list,         0, 1, 1, 1);
    ui_layout_sample_form->addWidget(_ui_btn_edit_gesture_list,    0, 2, 1, 1);
    ui_layout_sample_form->addWidget(ui_lbl_sample_folder,         1, 0, 1, 1, Qt::AlignRight);
    ui_layout_sample_form->addWidget(_ui_txt_sample_dir,          1, 1, 1, 1);
    ui_layout_sample_form->addWidget(_ui_btn_choose_sample_folder, 1, 2, 1, 1);

    QLabel *ui_lbl_model_file = new QLabel(tr("Model"));
    QLabel *ui_lbl_keymap_file = new QLabel(tr("Keymap"));
    ui_lbl_model_file->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    ui_lbl_keymap_file->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ui_txt_model_file = new QLineEdit;
    _ui_txt_keymap_file = new QLineEdit;
    _ui_txt_model_file->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    _ui_txt_keymap_file->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    _ui_btn_choose_model_file = new QPushButton("...");
    _ui_btn_choose_keymap_file = new QPushButton("...");
    _ui_btn_choose_model_file->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ui_btn_choose_model_file->setAutoDefault(false);
    _ui_btn_choose_model_file->setFocusPolicy(Qt::NoFocus);
    _ui_btn_choose_model_file->setToolTip(tr("Browse"));
    _ui_btn_choose_keymap_file->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ui_btn_choose_keymap_file->setAutoDefault(false);
    _ui_btn_choose_keymap_file->setFocusPolicy(Qt::NoFocus);
    _ui_btn_choose_keymap_file->setToolTip(tr("Browse"));
    _ui_control_form = new QWidget;
    _ui_control_form->hide();
    QGridLayout *ui_layout_control_form = new QGridLayout(_ui_control_form);
    ui_layout_control_form->setContentsMargins(0, 0, 0, 0);
    ui_layout_control_form->setSpacing(5);
    ui_layout_control_form->addWidget(ui_lbl_model_file,          0, 0, 1, 1, Qt::AlignRight);
    ui_layout_control_form->addWidget(_ui_txt_model_file,         0, 1, 1, 1);
    ui_layout_control_form->addWidget(_ui_btn_choose_model_file,  0, 2, 1, 1);
    ui_layout_control_form->addWidget(ui_lbl_keymap_file,         1, 0, 1, 1, Qt::AlignRight);
    ui_layout_control_form->addWidget(_ui_txt_keymap_file,        1, 1, 1, 1);
    ui_layout_control_form->addWidget(_ui_btn_choose_keymap_file, 1, 2, 1, 1);

    QVBoxLayout *ui_form = new QVBoxLayout;
    ui_form->setContentsMargins(0,0,0,0);
    ui_form->setSpacing(0);
    ui_form->addWidget(_ui_control_form);
    ui_form->addWidget(_ui_sample_form);

    QGridLayout *main_layout = new QGridLayout;
    main_layout->setContentsMargins(20, 20, 20, 20);
    main_layout->setSpacing(20);
    main_layout->addWidget(_ui_lbl_video,   0, 0, 4, 1, Qt::AlignCenter);
    main_layout->addWidget(_ui_txt_panel,   0, 1, 1, 2);
    main_layout->addWidget(_ui_btn_start,   1, 1, 1, 1);
    main_layout->addLayout(ui_form,         2, 1, 1, 2);
    main_layout->addWidget(_ui_btn_setting, 3, 1, 1, 1);
    main_layout->addWidget(_ui_btn_monitor, 3, 2, 1, 1, Qt::AlignLeft);

    setLayout(main_layout);
    setFixedSize(minimumSize());
}

void TrackingView::_makeConnections()
{
    connect(_ui_btn_monitor, SIGNAL(released()), this, SIGNAL(monitorWindowRequest()));
    connect(_ui_btn_setting, SIGNAL(released()), this, SIGNAL(settingWindowRequest()));
    connect(_ui_btn_edit_gesture_list, SIGNAL(released()), this, SIGNAL(settingWindowGestureListRequest()));
    connect(_ui_btn_start, SIGNAL(released()), this, SLOT(_uiBtnStartReleased()));
    connect(_ui_btn_choose_sample_folder, SIGNAL(released()), this, SLOT(_uiBtnChooseSampleDirReleased()));
    connect(_ui_btn_choose_model_file, SIGNAL(released()), this, SLOT(_uiBtnChooseModelFileReleased()));
    connect(_ui_btn_choose_keymap_file, SIGNAL(released()), this, SLOT(_uiBtnChooseKeymapFileReleased()));
    connect(_ui_txt_sample_dir, SIGNAL(textChanged(QString)), this, SLOT(_uiTxtSampleDirChanged(QString)));
    connect(_ui_txt_model_file, SIGNAL(textChanged(QString)), this, SLOT(_uiTxtModelFileChanged(QString)));
    connect(_ui_txt_keymap_file, SIGNAL(textChanged(QString)), this, SLOT(_uiTxtKeymapFileChanged(QString)));
}

void TrackingView::_loadSettings()
{
    _ui_box_gesture_list->addItems(_settings->gesture_list);
    if (_settings->gesture_list.size() > _settings->gesture_selected && _settings->gesture_selected > -1)
        _ui_box_gesture_list->setCurrentIndex(_settings->gesture_selected);
    _ui_txt_sample_dir->setText(_settings->sample_storage_path);
    _ui_txt_model_file->setText(_settings->cnn_model_file);
    _ui_txt_keymap_file->setText(_settings->keymap_file);
}
