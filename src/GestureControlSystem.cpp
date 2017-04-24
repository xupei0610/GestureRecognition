#include "GestureControlSystem.h"

GestureControlSystem::GestureControlSystem(HandDetector *hand_detector, SampleCollector *sample_collector, GestureAnalystInterface *gesture_analyst, CommandInputterInterface *command_inputter, QObject *parent) :
    QObject(parent),
    main_view(MainView::getInstance()),
    tracking_view(TrackingView::getInstance()),
    setting_view(SettingView::getInstance()),
    monitor_view(MonitorView::getInstance()),
    camera_fps(_camera_fps),
    work_status(_work_status),
    roi(_roi),
    cursor_roi(_cursor_roi),
    _hand_detector(hand_detector),
    _sample_collector(sample_collector),
    _gesture_analyst(gesture_analyst),
    _command_inputter(command_inputter),
    _work_status(STATUS_IDLE),
    _camera_capture_timer(new QTimer),
    _settings(Settings::getInstance()),
    _camera(new cv::VideoCapture)
{
    connect(main_view, SIGNAL(mainViewClosing()), this, SLOT(windowClosing()));
    connect(main_view, SIGNAL(settingWindowRequest()), this, SLOT(openSettingWindow()));
    connect(main_view, SIGNAL(controllingWindowRequest()), this, SLOT(openControllingWindow()));
    connect(main_view, SIGNAL(samplingWindowRequest()), this, SLOT(openSamplingWindow()));

    connect(tracking_view, SIGNAL(cameraRequest()), this, SLOT(openCamera()));
    connect(tracking_view, SIGNAL(samplingTaskStartRequest(int,QString)), this, SLOT(startSamplingTask(int,QString)));
    connect(tracking_view, SIGNAL(samplingTaskStopRequest()), this, SLOT(stopSamplingTask()));
    connect(tracking_view, SIGNAL(controllingTaskStartRequest(QString,QString)), this, SLOT(startControllingTask(QString,QString)));
    connect(tracking_view, SIGNAL(controllingTaskStopRequest()), this, SLOT(stopControllingTask()));
    connect(tracking_view, SIGNAL(monitorWindowRequest()), this, SLOT(openMonitorWindow()));
    connect(tracking_view, SIGNAL(settingWindowRequest()), this, SLOT(openSettingWindow()));
    connect(tracking_view, SIGNAL(settingWindowGestureListRequest()), this, SLOT(openSettingWindowGestureList()));

    connect(setting_view, SIGNAL(changeSkinColorLowerBound(int,int,int)), _hand_detector, SLOT(setSkinColorFilterLowerBound(int,int,int)));
    connect(setting_view, SIGNAL(changeSkinColorUpperBound(int,int,int)), _hand_detector, SLOT(setSkinColorFilterUpperBound(int,int,int)));
    connect(setting_view, SIGNAL(changeRoiRange(int,int,int,int)), this, SLOT(verifyRoiRange(int,int,int,int)));
    connect(setting_view, SIGNAL(changeDetectionArea(int)), _hand_detector, SLOT(setDetectionArea(int)));
    connect(setting_view, SIGNAL(changeMorphology(bool)), _hand_detector, SLOT(setMorphology(bool)));
    connect(setting_view, SIGNAL(backgroundSettingRequest()), _hand_detector, SLOT(setBackgroundImage()));
    connect(setting_view, SIGNAL(backgroundClearingRequest()), _hand_detector, SLOT(clearBackgroundImage()));
    connect(setting_view, SIGNAL(changeLabelList()), tracking_view, SLOT(reloadLabelList()));

    connect(_hand_detector, SIGNAL(backgroundImageSet()), this, SLOT(updateBackgroundImage()));
    connect(_hand_detector, SIGNAL(backgroundImageCleared()), setting_view, SLOT(clearBackgroundImage()));

    connect(_command_inputter, SIGNAL(commandMade(QString)), this, SLOT(informActionMade(QString)));

    connect(this, SIGNAL(cameraOpened()), tracking_view, SLOT(cameraStarted()));
    connect(this, SIGNAL(cameraOpened()), setting_view, SLOT(enableBackgroundSetting()));
    connect(this, SIGNAL(cameraReleased()), tracking_view, SLOT(cameraReleased()));
    connect(this, SIGNAL(cameraReleased()), main_view, SLOT(changeWorkStatusToNothing()));
    connect(this, SIGNAL(cameraReleased()), setting_view, SLOT(disableBackgroundSetting()));
    connect(this, SIGNAL(samplingTaskStarted()), tracking_view, SLOT(samplingTaskStarted()));
    connect(this, SIGNAL(samplingTaskStarted()), main_view, SLOT(changeWorkStatusToSampling()));
    connect(this, SIGNAL(samplingTaskStopped()), tracking_view, SLOT(samplingTaskStopped()));
    connect(this, SIGNAL(samplingTaskStopped()), main_view, SLOT(changeWorkStatusToNothing()));
    connect(this, SIGNAL(controllingTaskStarted()), tracking_view, SLOT(controllingTaskStarted()));
    connect(this, SIGNAL(controllingTaskStarted()), main_view, SLOT(changeWorkStatusToControlling()));
    connect(this, SIGNAL(controllingTaskStopped()), tracking_view, SLOT(controllingTaskStopped()));
    connect(this, SIGNAL(controllingTaskStopped()), main_view, SLOT(changeWorkStatusToNothing()));

    connect(_camera_capture_timer, SIGNAL(timeout()), this, SLOT(captureFrame()));

    setting_view->setToCurrentSettings();
}

void GestureControlSystem::run()
{
    main_view->show();
}

void GestureControlSystem::openControllingWindow()
{
    tracking_view->setWorkMode(TrackingView::MODE_CONTROLLING);
    tracking_view->show();
    tracking_view->raise();
}

void GestureControlSystem::openSamplingWindow()
{
    tracking_view->setWorkMode(TrackingView::MODE_SAMPLING);
    tracking_view->show();
    tracking_view->raise();
}

void GestureControlSystem::openSettingWindow()
{
    setting_view->activatePage(SettingView::PAGE_GENERAL);
    setting_view->show();
    setting_view->raise();
}

void GestureControlSystem::openSettingWindowGestureList()
{
    setting_view->activatePage(SettingView::PAGE_GESTURE_LIST);
    setting_view->show();
    setting_view->raise();
}

void GestureControlSystem::openMonitorWindow()
{
    monitor_view->show();
    monitor_view->raise();
}

void GestureControlSystem::verifyRoiRange(const int &start_x, const int &end_x, const int &start_y, const int &end_y)
{
    _roi.x = tracking_view->getVideoFrameWidth() * start_x/100;
    _roi.y = tracking_view->getVideoFrameHeight() * start_y/100;
    _roi.width = tracking_view->getVideoFrameWidth() * (end_x-start_x)/100;
    _roi.height = tracking_view->getVideoFrameHeight() * (end_y-start_y)/100;

    if (_roi.width < DEFAULT_ROI_MARGIN_LEFT + DEFAULT_ROI_MARGIN_RIGHT + 1)
    {
        _roi.width = DEFAULT_ROI_MARGIN_LEFT + DEFAULT_ROI_MARGIN_RIGHT + 1;
        if (_roi.x + _roi.width > tracking_view->getVideoFrameWidth())
            _roi.x = tracking_view->getVideoFrameWidth() - _roi.width;
    }
    if (_roi.height < DEFAULT_ROI_MARGIN_TOP + DEFAULT_ROI_MARGIN_BOTTOM + 1)
    {
        _roi.height = DEFAULT_ROI_MARGIN_TOP + DEFAULT_ROI_MARGIN_BOTTOM + 1;
        if (_roi.y + _roi.height > tracking_view->getVideoFrameHeight())
            _roi.y = tracking_view->getVideoFrameHeight() - _roi.height;
    }
    _cursor_roi.x = _roi.x + DEFAULT_ROI_MARGIN_LEFT;
    _cursor_roi.y = _roi.y + DEFAULT_ROI_MARGIN_TOP;
    _cursor_roi.width = _roi.width - DEFAULT_ROI_MARGIN_LEFT - DEFAULT_ROI_MARGIN_RIGHT;
    _cursor_roi.height= _roi.height- DEFAULT_ROI_MARGIN_TOP - DEFAULT_ROI_MARGIN_BOTTOM;
}

void GestureControlSystem::updateBackgroundImage()
{
    setting_view->setBackgroundImage(
                ImgConvertor::cvMat2QPixmap(
                    this->_hand_detector->background_img
                    )
                );
}

void GestureControlSystem::informActionMade(const QString &action)
{
    tracking_view->appendText(action);
}

void GestureControlSystem::openCamera()
{
    if (!_camera->isOpened())
    {
        _camera->open(0);
        if (!_camera->isOpened())
        {
            _handleCameraError();
            return;
        }
    }
    emit cameraOpened();
    _camera_capture_timer->start(1000/_camera_fps);
}

void GestureControlSystem::releaseCamera()
{
    _camera_capture_timer->stop();
    // FIXME exception caused by opencv when releasing the camera
    //    if (_camera->isOpened())
    //        _camera->release();
}

void GestureControlSystem::captureFrame()
{
    if (_camera->isOpened())
    {
        cv::Mat captured_frame;
        _camera->read(captured_frame);
        if (!captured_frame.empty())
        {
            // resize and keep aspect ratio
            cv::resize(captured_frame, captured_frame,
                       cv::Size(captured_frame.cols*tracking_view->getVideoFrameHeight()/captured_frame.rows,
                                tracking_view->getVideoFrameHeight()));
            captured_frame(
                        cv::Rect(
                            (captured_frame.cols - tracking_view->getVideoFrameWidth())/2, 0,
                            tracking_view->getVideoFrameWidth(), tracking_view->getVideoFrameHeight()
                            )
                        ).copyTo(captured_frame);
            cv::flip(captured_frame, captured_frame, 1);

            _processCapturedFrame(captured_frame);
            return;
        }
    }
    _handleCameraError();
}

void GestureControlSystem::startSamplingTask(const int &label_index, const QString &folder_path)
{
    if (_work_status == STATUS_CONTROLLING)
        stopControllingTask();
    if (!_camera->isOpened())
    {
        _handleCameraError();
        return;
    }
    if (label_index < 0 || label_index >= _settings->gesture_list.size())
    {
        _handleSamplingError(SAMPLING_ERROR_GESTURE_INDEX);
        return;
    }

    if (_sample_collector->setStoragePath(folder_path, _settings->gesture_list.at(label_index)))
    {
        if (QMessageBox::Ok == QMessageBox::information(
                    tracking_view, tr("Start Sampling"),
                    QString(tr("<div style=\"font-weight:100\">"
                               "Sampling will start.<br />"
                               "It will automatically terminate after %1 of samples are collected and take about %2 seconds. "
                               "All samples collected will be stored to"
                               "<pre style=\"padding-left:10px\">%3</pre>"
                               "Please pose your gesture in the region of interesting."
                               "</div>")
                            ).arg(QString::number(_settings->sampling_amount_per_time),
                                  QString::number(3*_settings->sampling_amount_per_time*_settings->sampling_interval/1000),
                                  _sample_collector->storage_path.toHtmlEscaped()),
                    QMessageBox::Cancel, QMessageBox::Ok))
        {
            _work_status = STATUS_SAMPLING;
            _sampling_trails = 0;
            _samples_collected = 0;
            _settings->setSampleStoragePath(folder_path);
            _settings->setSelectedGesture(label_index);
            emit samplingTaskStarted();
        }
        else
            emit samplingTaskStopped();
        return;
    }
    _handleSamplingError(SAMPLING_ERROR_STORAGE_PATH);
}

void GestureControlSystem::stopSamplingTask()
{
    _samplingCompleted();
}

void GestureControlSystem::startControllingTask(const QString &model_file, const QString &keymap_file)
{
    if (_work_status == STATUS_SAMPLING)
        stopSamplingTask();
    if (_command_inputter->load(keymap_file))
    {
        if (_command_inputter->labels().empty())
        {
            _handleControllingError(CONTROLLING_ERROR_ILLEGAL_KEYMAP_FILE);
            return;
        }
        int num_labels = _gesture_analyst->load(model_file,"");

        if (num_labels == _command_inputter->labels().size())
        {
            _work_status = STATUS_CONTROLLING;
            _settings->setCnnModelFile(model_file);
            _settings->setKeymapFile(keymap_file);
            emit controllingTaskStarted();
            return;
        }
        _handleControllingError(CONTROLLING_ERROR_INVALID_KEYMAP_FILE);
        return;
    }
    _handleControllingError(CONTROLLING_ERROR_ILLEGAL_KEYMAP_FILE);
}

void GestureControlSystem::stopControllingTask()
{
    _work_status = STATUS_IDLE;
    emit controllingTaskStopped();
}

void GestureControlSystem::windowClosing()
{
    tracking_view->close();
    monitor_view->close();
    setting_view->close();
}

void GestureControlSystem::_handleCameraError()
{
    _camera_capture_timer->stop();
    emit cameraReleased();
    QMessageBox::critical(main_view, tr("Error"), tr("Failed to open camera."));
}

void GestureControlSystem::_processCapturedFrame(cv::Mat &captured_frame)
{
    bool detected = false;
    if (monitor_view->isVisible() || _hand_detector->waitting_bg || _work_status == STATUS_CONTROLLING || _work_status == STATUS_SAMPLING)
    {
        detected = _hand_detector->detect(captured_frame(_roi));

        if (detected)
        {
            if (_work_status == STATUS_CONTROLLING)
            {
                _recognize(_hand_detector->extracted_img, _hand_detector->tracked_point);
                if (monitor_view->isVisible())
                    monitor_view->updateMonitorImage3(
                                ImgConvertor::cvMat2QPixmap(_hand_detector->interesting_img),
                                ImgConvertor::cvMat2QPixmap(_hand_detector->filtered_img),
                                ImgConvertor::cvMat2QPixmap(_hand_detector->convexity_img)
//                                ImgConvertor::cvMat2QPixmap(_hand_detector->extracted_img)
                                );

            }
            else
            {
                if (_work_status == STATUS_SAMPLING && !_sample_collector->deny())
                    _sample(_hand_detector->extracted_img);

                if (monitor_view->isVisible())
                    monitor_view->updateMonitorImage3(
                                ImgConvertor::cvMat2QPixmap(_hand_detector->interesting_img),
                                ImgConvertor::cvMat2QPixmap(_hand_detector->filtered_img),
//                                ImgConvertor::cvMat2QPixmap(_hand_detector->convexity_img),
                                ImgConvertor::cvMat2QPixmap(_hand_detector->extracted_img)
                                );
            }
        }
        else
        {
            if (_work_status == STATUS_SAMPLING)
            {
                if (!_sample_collector->deny())
                    tracking_view->appendText(tr("[Error] Sampling failed. Nothing detected."));

            }
            if (monitor_view->isVisible())
                monitor_view->updateMonitorImage2(
                            ImgConvertor::cvMat2QPixmap(_hand_detector->interesting_img),
                            ImgConvertor::cvMat2QPixmap(_hand_detector->filtered_img)
                            );
        }

    }
    if (tracking_view->isVisible())
    {
        cv::rectangle(captured_frame, _roi, HandDetector::COLOR_GREEN, 2);
        if (tracking_view->work_mode == TrackingView::MODE_CONTROLLING)
        {
            cv::rectangle(captured_frame, _cursor_roi, HandDetector::COLOR_RED, 3);
            if (detected)
                cv::circle(captured_frame(_roi), _hand_detector->tracked_point, 3, HandDetector::COLOR_BLUE, -1);
        }
        tracking_view->updateVideoFrame(ImgConvertor::cvMat2QPixmap(captured_frame));
    }
}

void GestureControlSystem::_sample(const cv::Mat &image)
{
    if (_sample_collector->sample(image))
    {
        _samples_collected++;
        tracking_view->updateText(QString(tr("[Info] Collected: %1 / %2")).arg(
                                      QString::number(++_sampling_trails).rightJustified(3, ' '),
                                      QString::number(_settings->sampling_amount_per_time))
                                  );
        if (_sampling_trails >= _settings->sampling_amount_per_time)
            _samplingCompleted();
    }
    else
        _handleSamplingError(SAMPLING_ERROR_STORAGE_IMAGE);
}

void GestureControlSystem::_handleSamplingError(const SAMPLING_ERROR &e)
{
    QString msg;
    if (e == SAMPLING_ERROR_GESTURE_INDEX)
    {
        msg = QString(tr("Illegal Gesture Information."));
    }
    else
        msg = QString(tr("Failed to store sample images at <pre style=\"padding-left:10px\">%1</pre>")).arg(
                    _sample_collector->storage_path.toHtmlEscaped());
    QMessageBox::critical(tracking_view, tr("Error"),
                          QString(tr("<div style=\"font-weight:100\">"
                                     "Sampling terminated.<br /><br />"
                                     "%1"
                                     "</div>")).arg(msg)
                          );
    emit samplingTaskStopped();
}

void GestureControlSystem::_samplingCompleted()
{
    _work_status = STATUS_IDLE;
    emit samplingTaskStopped();
    tracking_view->appendText(QString(tr("[Info] Sampling Completed.\n"
                                         "[Info] %1 Samples were stored at\n%2\n"
                                         )).arg(
                                  QString::number(_samples_collected),
                                  _sample_collector->storage_path.toHtmlEscaped()
                                  )
                              );
    QMessageBox::information(tracking_view, tr("Sampling Completed"),
                             QString(tr("<div style=\"font-weight:100\">%1 samples were collected. All samples were stored at<pre style=\"padding-left:10px\">%3</pre></div>")).arg(
                                 QString::number(_samples_collected),
                                 _sample_collector->storage_path.toHtmlEscaped()
                                 )
                             );
}

void GestureControlSystem::_recognize(const cv::Mat &image, const cv::Point &tracked_point)
{
    auto res = _gesture_analyst->analyze(_sample_collector->resizeSample(image), 5);

    _command_inputter->input(res[0].label_id,
            static_cast<float>(tracked_point.x-DEFAULT_ROI_MARGIN_LEFT)/_cursor_roi.width,
            static_cast<float>(tracked_point.y-DEFAULT_ROI_MARGIN_TOP)/_cursor_roi.height);
    QStringList msg;
    for (const auto &p : res)
    {
        msg << (QString("[%1]: %2").arg(_command_inputter->labels().at(p.label_id),
                                        QString::number(p.prob)));
    }
    monitor_view->setMsg(msg.join("\n"));
}

void GestureControlSystem::_handleControllingError(const CONTROLLING_ERROR &e)
{
    if (e == CONTROLLING_ERROR_ILLEGAL_KEYMAP_FILE)
        QMessageBox::critical(tracking_view, tr("Error"),
                              QString(tr("<div style=\"font-weight:100\">"
                                         "Unable to parse the keymap file."
                                         "</div>"))
                              );
    else
        QMessageBox::critical(tracking_view, tr("Error"),
                              QString(tr("<div style=\"font-weight:100\">"
                                         "The keymap file is unmatched with the model file."
                                         "</div>"))
                              );
    emit controllingTaskStopped();
}
