#ifndef GESTURECONTROLSYSTEM_H
#define GESTURECONTROLSYSTEM_H
/**
 * @file
 * @author Pei Xu, xupei0610 at gmail.com
 * @brief The GestureControlSystem.h file contains the class who is the controller of the whole system.
 */
#include <QObject>
#include <QTimer>
#include <QString>
#include <opencv2/opencv.hpp>

#include "global.h"
#include "MainView.h"
#include "SettingView.h"
#include "TrackingView.h"
#include "MonitorView.h"
#include "Settings.h"
#include "HandDetector.h"
#include "SampleCollector.h"
#include "GestureAnalystInterface.h"
#include "CommandInputterInterface.h"

/**
 * @brief The GestureControlSystem class is the controller of the whole system.
 */
class GestureControlSystem : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief WORK_STATUS represents the current work status of the system.
     */
    enum WORK_STATUS
    {
        STATUS_IDLE,         //!< idle
        STATUS_CONTROLLING,  //!< controlling
        STATUS_SAMPLING,     //!< sampling
    };

    /**
     * @brief SAMPLING_ERROR represents the error appearing during samples.
     *
     * @see #GestureControlSystem::startSamplingTask
     * @see #GestureControlSystem::_handleSamplingError
     */
    enum SAMPLING_ERROR
    {
        SAMPLING_ERROR_GESTURE_INDEX, //!< illegal gesture index
        SAMPLING_ERROR_STORAGE_PATH,  //!< illegal storage path or no permissions to make directory at the given path
        SAMPLING_ERROR_STORAGE_IMAGE  //!< failed to store sample images at the given path
    };

    /**
     * @brief The CONTROLLING_ERROR represents the error appearing during controlling.
     *
     * @see #GestureControlSystem::_handleControllingError
     */
    enum CONTROLLING_ERROR
    {
        CONTROLLING_ERROR_ILLEGAL_KEYMAP_FILE, //!< illegal or invalid keymap file
        CONTROLLING_ERROR_INVALID_KEYMAP_FILE  //!< the model file is unmatched with the provide network structure
    };

    /**
     * @brief main_view is the GUI of the main window.
     *
     * @see #MainView
     */
    MainView *main_view;
    /**
     * @brief tracking_view is the GUI of the tracking window.
     *
     * @see #TrackingView
     */
    TrackingView *tracking_view;
    /**
     * @brief setting_view is the GUI of the setting window.
     *
     * @see #SettingView
     */
    SettingView *setting_view;
    /**
     * @brief monitor_view is the GUI of the monitor window.
     *
     * @see #MonitorView
     */
    MonitorView *monitor_view;
    /**
     * @brief camera_fps is the FPS of the camera.
     */
    const size_t &camera_fps;
    /**
     * @brief work_status is the current work status. This is a reference to #GestureControlSystem::_work_status.
     *
     * @see #GestureControlSystem::WORK_STATUS
     */
    const WORK_STATUS &work_status;
    /**
     * @brief _roi is the region of interesting on the frame captured by the camera.
     */
    const cv::Rect &roi;
    /**
     * @brief _cursor_roi is a modified ROI in the interior of _ROI to mapping cursor position from tracked point estimated by HandDetector.
     */
    const cv::Rect &cursor_roi;

    explicit GestureControlSystem(HandDetector *hand_detector,
                                  SampleCollector *sample_collector,
                                  GestureAnalystInterface *gesture_analyst,
                                  CommandInputterInterface *command_inputter,
                                  QObject *parent = 0);
    /**
     * @brief run is the main function to run the system
     */
    void run();

signals:
    /**
     * @brief cameraOpened is the signal emitted right after the camera launched.
     */
    void cameraOpened();
    /**
     * @brief cameraReleased is the signal emitted right after the camera is released.
     */
    void cameraReleased();

    /**
     * @brief controllingTaskStarted is the signal to indicate the start of controlling task.
     */
    void controllingTaskStarted();
    /**
     * @brief controllingTaskStopped is the signal to indicate the stop of controlling task.
     */
    void controllingTaskStopped();
    /**
     * @brief controllingTaskStarted is the signal to indicate the start of sampling task.
     */
    void samplingTaskStarted();
    /**
     * @brief controllingTaskStarted is the signal to indicate the stop of sampling task.
     */
    void samplingTaskStopped();

public slots:
    /**
     * @brief openControllingWindow opens the tracking window in controlling mode.
     */
    void openControllingWindow();
    /**
     * @brief openSamplingWindow opens the tracking window in sampling mode.
     */
    void openSamplingWindow();
    /**
     * @brief openSettingWindow opens the setting window.
     */
    void openSettingWindow();
    /**
     * @brief openSettingWindowGestureList opens the gesture list page of the setting window
     */
    void openSettingWindowGestureList();
    /**
     * @brief openMonitorWindow opens the monitor window.
     */
    void openMonitorWindow();
    /**
     * @brief verifyRoiRange verifies the given ROI range and stores the verified range.
     *
     * @param start_x : the left bound of the ROI in percent
     * @param end_x : the right bound of the ROI in percent
     * @param start_y : the top bound of the ROI in percent
     * @param end_y : the bottom bound of the ROI in percent
     */
    void verifyRoiRange(const int &start_x, const int &end_x, const int &start_y, const int &end_y);
    /**
     * @brief updateBackgroundImage updates the background image shown in the setting window.
     */
    void updateBackgroundImage();
    /**
     * @brief informActionMade informs what action made by the control system.
     * @param action : name of the action made
     */
    void informActionMade(const QString &action);
    /**
     * @brief openCamera opens the camera and shows error message if the camera cannot be open.
     * @see #GestureControlSystem::_handleCameraError
     */
    void openCamera();
    /**
     * @brief releaseCamera closes the camera.
     *
     * **ATTENTION**:
     *   This function cannot work properly on Mac OS (at least at Mac OS 10.12.3) due to the exception caused by opencv when releasing the camera on MAC OS
     */
    void releaseCamera();
    /**
     * @brief captureFrame captures frame from the camera and analyzes the captured frame. It will show an error message if it failed to capture frame from the camera.
     *
     * @see #GestureControlSystem::_processCapturedFrame
     * @see #GestureControlSystem::_handleCameraError
     */
    void captureFrame();
    /**
     * @brief startSamplingTask starts a sampling task.
     *
     * It sets #GestureControlSystem::_work_status and emits the signal of #GestureControlSystem::samplingTaskStarted if everything goes will, or shows error message if something wrong.
     * It should reset #GestureControlSystem::_sampling_trails and #GestureControlSystem::_samples_collected to 0.
     *
     * @param label_index : index of the label into which samples will be added
     * @param folder_path : path of the folder containing the sample
     *
     */
    virtual void startSamplingTask(const int &label_index, const QString &folder_path);
    /**
     * @brief stopSamplingTask handles with the request of stopping sampling.
     *
     * It, like #GestureControlSystem::_samplingCompleted, resets #GestureControlSystem::_work_status and emit the signal of #GestureControlSystem::controllingTaskStopped .
     *
     * @see #GestureControlSystem::_samplingCompleted
     */
    virtual void stopSamplingTask();
    /**
     * @brief startControllingTask starts a controlling task.
     *
     * It sets #GestureControlSystem::_work_status and emits the signal of #GestureControlSystem::controllingTaskStarted if everything goes will, or shows error message if something wrong.
     *
     * @param model_file : file path of the model file
     * @param keymap_file : file path of the keymap file
     */
    virtual void startControllingTask(const QString &model_file, const QString &keymap_file);
    /**
     * @brief stopControllingTask handles with the request of stopping controlling.
     *
     * It resets #GestureControlSystem::_work_status and emit the signal of #GestureControlSystem::controllingTaskStopped .
     */
    virtual void stopControllingTask();

protected:
    /**
     * @brief _hand_detector is the #HandDetector used by this instance.
     */
    HandDetector *_hand_detector;
    /**
     * @brief _sample_collector is the #SampleCollector used by this instance.
     */
    SampleCollector *_sample_collector;
    /**
     * @brief _gesture_analyst is the #GestureAnalyst used by this instance.
     */
    GestureAnalystInterface *_gesture_analyst;
    /**
     * @brief _command_inputter is the #CommandInputter used by this instance.
     */
    CommandInputterInterface *_command_inputter;
    /**
     * @brief _work_status is the current work status.
     *
     * @see #GestureControlSystem::WORK_STATUS
     */
    WORK_STATUS _work_status;
    /**
     * @brief _camera_capture_timer is the timer of capturing frame form the camera.
     *
     * @see #GestureControlSystem::captureFrame
     */
    QTimer *_camera_capture_timer;
    /**
     * @brief _sampling_trails is an indicator of how many sampling trails have been conducted.
     *
     * **ATTENTION**:
     *  It should be reset to 0 everytime before doing a batch of sampling.
     *
     * @see #GestureControlSystem::startSamplingTask
     */
    int _sampling_trails;
    /**
     * @brief _samples_collected is an indicator of how many samples have benn collected.
     *
     * **ATTENTION**:
     *  It should be reset to 0 everytime before doing a batch of sampling.
     *
     * @see #GestureControlSystem::startSamplingTask
     */
    int _samples_collected;
    /**
     * @brief _handleCameraError is the callback function to handle the camera error.
     *
     * It do the following by default:
     *  - stop #GestureControlSystem::_camera_capture_timer
     *  - emit #GestureControlSystem::cameraReleased() to inform the release of the camera.
     */
    virtual void _handleCameraError();
    /**
     * @brief _processCapturedFrame is the callback function to deal with the frame captured by the camera.
     *
     * It do the following by default:
     *
     *  - detect the hand/gesture via #HandDetector from the region of interesting on the captured frame,
     *  - make command via #CommandMaker using the analyst result obtained by #GestureAnalyst, and
     *  - draw the region of interesting on the captured frame and show the captured frame on the tracking window.
     *
     * @param captured_frame : the captured frame
     *
     * @see #GestureControlSystem::captureFrame
     */
    virtual void _processCapturedFrame(cv::Mat &captured_frame);

    /**
     * @brief _sample does sampling via #SampleCollector::sample
     * @param image : the sample image.
     */
    virtual void _sample(const cv::Mat &image);
    /**
     * @brief _sampling_completed is the callback function when sampling completed.
     *
     * It informs the success of sampling and emits the signal of #GestureControlSystem::samplingTaskStopped
     */
    virtual void _samplingCompleted();
    /**
     * @brief _handleSamplingError is the callback function when sampling failed.
     *
     * It informs the failing of sampling and emits the signal of #GestureControlSystem::samplingTaskStopped
     */
    virtual void _handleSamplingError(const SAMPLING_ERROR &e);
    /**
     * @brief _recognize identifies gestures from the sample image via #GestureAnalyst:analyze and makes action using #CommandInputter::input .
     * @param image : the sample image
     */
    virtual void _recognize(const cv::Mat &image, const cv::Point &tracked_point);
    virtual void _handleControllingError(const CONTROLLING_ERROR &e);

private:
    Settings *_settings;

    cv::Rect _roi;
    cv::Rect _cursor_roi;

    cv::VideoCapture *_camera;
    size_t _camera_fps = CAMERA_FPS;

};

#endif // GESTURECONTROLSYSTEM_H
