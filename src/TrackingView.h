#ifndef TRACKINGVIEW_H
#define TRACKINGVIEW_H
/**
 * @file
 * @author Pei Xu, xupei0610 at gmail.com
 * @brief The TrackingView.h file contains a singleton class that provides the GUI of the tracking window.
 */

#include <QDialog>
#include <QLabel>
#include <QPlainTextEdit>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QTextCursor>
#include <QFileDialog>
#include <QFileInfo>
#include <QString>
#include <QCloseEvent>
#include <QDebug>

#include "Settings.h"
#include "SettingView.h"
#include "MonitorView.h"

/**
 * @brief The TrackingView class provides the GUI of the tracking window.
 *
 * The tracking window is used for the sampling and control tasks.\n
 * This is a singleton class. Use #TrackingView::getInstance() to get the instance of this class.
 */
class TrackingView final : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief WORK_MODE represents the current work mode of the window.
     *
     * @see #TrackingView::setWorkMode
     */
    enum WORK_MODE
    {
        MODE_SAMPLING,   //!< sampling mode
        MODE_CONTROLLING //!< controlling mode
    };
    /**
     * @brief WORK_STATUS represents the current work status.
     */
    enum WORK_STATUS
    {
        STATUS_WAITING_CAMERA, //!< camera does not starts
        STATUS_IDLE,           //!< work does not starts but camera starts
        STATUS_WORKING,        //!< work is in progress
    };
    /**
     * @brief work_mode is current work mode.
     *
     * @see #TrackingView::setWorkMode
     */
    const WORK_MODE &work_mode;
    /**
     * @brief getInstance returns the singleton instance of this class.
     * @return the singleton instance of #TrackingView class
     *
     * @see #Singleton
     */
    static TrackingView *getInstance();
    /**
     * @brief setWorkMode sets the window to the specific work mode.
     * @param mode : work mode
     *
     * @see #TrackingView::WORK_MODE
     */
    void setWorkMode(const WORK_MODE &mode);
    /**
     * @brief updateVideoFrame displays the given image on the video widget.
     *
     * The given image will be sacled according to the size of the widget to display the video.
     *
     * @param img : image of the current video frame
     */
    void updateVideoFrame(const QPixmap &img);
    /**
     * @brief getVideoFrameWidth returns the width of the label showing the video frame.
     * @return the width, in pixel, of the label showing the video frame
     * @see #TrackingView::getVideoFrameHeight
     */
    int getVideoFrameWidth();
    /**
     * @brief getVideoFrameHeight returns the height of the label showing the video frame.
     * @return the height, in pixel, of the label showing the video frame
     * @see #TrackingView::getVideoFrameWidth
     */
    int getVideoFrameHeight();
    /**
     * @brief appendText adds text on the text panel in a new line.
     * @param text : text that will be added onto the text panel
     *
     * @see #TrackingView::updateText
     * @see #TrackingView::clearText
     * @see #TrackingView::setText
     */
    void appendText(const QString &text);
    /**
     * @brief updateText updates the content of the last line of the text panel.
     * @param new_text : new text that will be shown on the last line of the text panel
     *
     * @see #TrackingView::appendText
     * @see #TrackingView::clearText
     * @see #TrackingView::setText
     */
    void updateText(const QString &new_text);
    /**
     * @brief clearText clears the text panel.
     *
     * @see #TrackingView::appendText
     * @see #TrackingView::updateText
     * @see #TrackingView::setText
     */
    void clearText();
    /**
     * @brief setText sets the content of the text panel.
     *
     * It combines #TrackingView::clearText and #TrackingView::appendText together but should be more efficient perhaps.
     *
     * @param text : text that will be shown on the text panel
     * @see #TrackingView::clearText
     */
    void setText(const QString &text);

signals:
    /**
     * @brief cameraRequest is the signal of the request for launching the camera.
     *
     * @see #TrackingView::cameraStarted
     */
    void cameraRequest();
    /**
     * @brief samplingTaskStartRequest is the signal of the request for starting sampling task.
     * @param label_index : index of the label into which samples will be added
     * @param folder_path : path of the folder containing the sample
     *
     * @see #TrackingView::samplingStarted
     * @see #TrackingView::samplingTaskStopRequest
     */
    void samplingTaskStartRequest(const int &label_index, const QString &folder_path);
    /**
     * @brief samplingTaskStopRequest is the signal of the request for stopping sampling task.
     *
     * @see #TrackingView::samplingStopped
     * @see #TrackingView::samplingTaskStartRequest
     */
    void samplingTaskStopRequest();
    /**
     * @brief controllingTaskStartRequest is the signal of the request for starting controlling task.
     * @param model_file : file path of the model file
     * @param keymap_file : file path of the keymap file
     *
     * @see #TrackingView::controllingStarted
     * @see #TrackingView::controllingTaskStopRequest
     */
    void controllingTaskStartRequest(const QString &model_file, const QString &keymap_file);
    /**
     * @brief controllingTaskStopRequest is the signal of the request for stopping controlling task.
     *
     * @see #TrackingView::controllingStopped
     * @see #TrackingView::controllingTaskStartRequest
     */
    void controllingTaskStopRequest();
    /**
     * @brief monitorWindowRequest is the signal of the request for the monitor window.
     */
    void monitorWindowRequest();
    /**
     * @brief settingWindowRequest is the signal of the request for the setting window.
     */
    void settingWindowRequest();
    /**
     * @brief settingWindowGestureListRequest is the signal of the request for the gesture list page of the setting window.
     */
    void settingWindowGestureListRequest();

public slots:
    /**
     * @brief reloadLabelList is the callback after the gesture lable list is changed
     *
     * It updates the gesture label list using data from #Settings::gesture_list .
     * @see #SettingView::changeLabelList
     */
    void reloadLabelList();
    /**
     * @brief cameraStarted is the callback when the camera is started.
     *
     * It updates the text on the start button to `control` or `sample`.
     *
     * @see #TrackingView::cameraRequest
     */
    void cameraStarted();
    /**
     * @brief cameraReleased is the callback when the camera is closed.
     *
     * It sets the work status to waiting for camera and update UI.
     *
     * @see #TrackingView::controllingTaskStopped
     * @see #TrackingView::samplingTaskStopped
     */
    void cameraReleased();
    /**
     * @brief samplingTaskStarted is the callback after sampling task starts.
     *
     * It changes the work status and update UI.
     *
     * @see #TrackingView::samplingTaskStartRequest
     * @see #TrackingView::controllingTaskStarted
     */
    void samplingTaskStarted();
    /**
     * @brief controllingTaskStarted is the callback after sampling task starts.
     *
     * It changes the work status and update UI.
     *
     * @see #TrackingView::controllingTaskStartRequest
     * @see #TrackingView::samplingTaskStarted
     */
    void controllingTaskStarted();
    /**
     * @brief samplingSTasktopped is the callback after the sampling task stops
     *
     * @see #TrackingView::samplingTaskStopRequest
     * @see #TrackingView::controllingTaskStopped
     */
    void samplingTaskStopped();
    /**
     * @brief controllingTaskStopped is the callback after the controlling task stops
     *
     * @see #TrackingView::controllingTaskStopRequest
     * @see #TrackingView::samplingTaskStopped
     */
    void controllingTaskStopped();

private slots:
    void _uiBtnStartReleased();
    void _uiBtnChooseSampleDirReleased();
    void _uiBtnChooseModelFileReleased();
    void _uiBtnChooseKeymapFileReleased();
    void _uiTxtSampleDirChanged(const QString &folder_path);
    void _uiTxtModelFileChanged(const QString &file_path);
    void _uiTxtKeymapFileChanged(const QString &file_path);

private:
    explicit TrackingView(QWidget *parent = 0);
    TrackingView(const TrackingView &) = delete;
    TrackingView &operator=(const TrackingView &) = delete;
    static TrackingView *createInstance();

    inline void _setupUi();
    inline void _makeConnections();
    inline void _loadSettings();

    WORK_STATUS _work_status;
    WORK_MODE _work_mode;

    Settings *_settings;

    QPushButton *_ui_btn_start;
    QPushButton *_ui_btn_monitor;
    QPushButton *_ui_btn_setting;
    QLabel      *_ui_lbl_video;
    QPlainTextEdit *_ui_txt_panel;

    QPushButton *_ui_btn_edit_gesture_list;
    QPushButton *_ui_btn_choose_sample_folder;
    QComboBox   *_ui_box_gesture_list;
    QLineEdit   *_ui_txt_sample_dir;
    QWidget *_ui_sample_form;

    QPushButton *_ui_btn_choose_model_file;
    QPushButton *_ui_btn_choose_keymap_file;
    QLineEdit   *_ui_txt_model_file;
    QLineEdit   *_ui_txt_keymap_file;
    QWidget *_ui_control_form;

};

#endif // TRACKINGVIEW_H
