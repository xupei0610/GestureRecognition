#ifndef SETTINGVIEW_H
#define SETTINGVIEW_H
/**
 * @file
 * @author Pei Xu, xupei0610 at gmail.com
 * @brief The SettingView.h file contains a singleton class that provides the GUI of the setting window.
 */

#include <QDialog>
#include <QLabel>
#include <QSlider>
#include <QSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QWidget>
#include <QTabWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLineEdit>
#include <QInputDialog>
#include <QMessageBox>
#include <QSizePolicy>
#include <QFont>
#include <QPixmap>
#include <QString>
#include <QCloseEvent>
#include <QDebug>

#include "Singleton.h"
#include "Settings.h"

/**
 *@brief The SettingView class provides the GUI of the setting dialog.
 *
 * The setting window provides options to change most parameters of the system.\n
 * This is a singleton class. Use #SettingView::getInstance() to get the instance of this class.
 */
class SettingView final : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief SETTING_VIEW_PAGE is the index of the tab page of the setting window.
     *
     * @see #SettingView::activatePage
     */
    enum SETTING_VIEW_PAGE
    {
        PAGE_GENERAL = 0,   //<! first page, general settings for the hand detector
        PAGE_BACKGROUND = 1, //<! settings for the background subtractor
        PAGE_GESTURE_LIST = 2, //<! settings for the gesture list
        PAGE_KEYMAP = 3 //<! setings for the keymap scheme
    };

    /**
     * @brief getInstance returns the singleton instance of this class.
     * @return the singleton instance of #SettingView class
     *
     * @see #Singleton
     */
    static SettingView *getInstance();

    /**
     * @brief activatePage sets the current default page of the setting window.
     * @param page : the default page to display
     *
     * @see #SettingView::SETTING_VIEW_PAGE
     */
    void activatePage(const SETTING_VIEW_PAGE &page);
    /**
     * @brief syncSettings synchronizes the settings to #Settings .
     *
     * This function will be automatically called when the window is closed.\n
     * It should be called manually if the settings is changed inside the program.
     *
     * The change of label list is real time and has no need to call this function.
     */
    void syncSettings();

signals:
    /**
     * @brief changeSkinColorLowerRange is the signal of the new lower bound for the skin color filter.
     * @param h : hue of HSV color
     * @param s : saturation of HSV color
     * @param v : value of HSV color
     *
     * @see #HandDetector::setSkinColorFilterLowerBound
     * @see #SettingView::changeSkinColorUpperBound
     */
    void changeSkinColorLowerBound(const int &h, const int &s, const int &v);
    /**
     * @brief changeSkinColorUpperRange is the signal of the new uppwer bound for the skin color filter.
     * @param h : hue of HSV color
     * @param s : saturation of HSV color
     * @param v : value of HSV color
     *
     * @see #HandDetector::setSkinColorFilterUpperBound
     * @see #SettingView::changeSkinColorLowerBound
     */
    void changeSkinColorUpperBound(const int &h, const int &s, const int &v);
    /**
     * @brief changeRoiRange is the signal to indicate the new region of interesting area.
     *
     * @param start_x : the left bound of the ROI in percent
     * @param end_x : the right bound of the ROI in percent
     * @param start_y : the top bound of the ROI in percent
     * @param end_y : the bottom bound of the ROI in percent
     *
     */
    void changeRoiRange(const int &start_x, const int &end_x, const int &start_y, const int &end_y);
    /**
     * @brief changeDetectionArea is the signal of the new value of the minimum area of detecting contour.
     * @param area : the amount of the minimum contour area who be considered as a hand contour
     *
     * @see #HandDetector::setDetectionArea
     */
    void changeDetectionArea(const int &area);
    /**
     * @brief changeMorphology is the signal of performing morphological transformation or not.
     * @param perform_morphology : the flag to perform morphology or not
     *
     * @see #HandDetector::setMorphology
     */
    void changeMorphology(const bool &perform_morphology);
    /**
     * @brief backgroundSettingRequest is the signal of the request for setting background image for the background subtractor.
     *
     * @see #HandDetector::_bg_subtractor
     * @see #HandDetector::setBackgroundImage
     */
    void backgroundSettingRequest();
    /**
     * @brief backgroundClearingRequest is the signal of the request for clearing background image for the background subtractor.
     *
     * @see #HandDetector::_bg_subtractor
     * @see #HandDetector::clearBackgroundImage
     */
    void backgroundClearingRequest();
    /**
     * @brief changeLabelList functions is the signal of the change in gesture label list.
     * @see #TrackingView::reloadLabelList
     */
    void changeLabelList();

public slots:
    /**
     * @brief setToCurrentSettings initializes the system using current settings.
     */
    void setToCurrentSettings();
    /**
     * @brief setToDefaultSettings initializes the system using default settings.
     */
    void setToDefaultSettings();
    /**
     * @brief enableBackgroundSetting enables setting image for the background subtractor.
     *        Use this func when the camera is ready.
     */
    void enableBackgroundSetting();
    /**
     * @brief disableBackgroundSetting disables setting image for the background subtractor.
     *        Use this func when the camera is closed.
     */
    void disableBackgroundSetting();
    /**
     * @brief setBackgroundImage displays the given image as the background image used by the background subtractor.
     * @param img : the image shown in the setting window as the background image used by the background subtractor
     *
     * @see #HandDetector::_bg_subtractor
     * @see #HandDetector::backgroundImageSet
     */
    void setBackgroundImage(const QPixmap &img);
    /**
     * @brief clearBackgroundImage clears the shown background image.
     *        Use this func when the brackground subtraction is turned off.
     *
     * @see #HandDetector::_bg_subtractor
     * @see #HandDetector::backgroundImageCleared
     */
    void clearBackgroundImage();

private slots:
    void _uiSldHue1ValueChanged(const int &val);
    void _uiSldHue2ValueChanged(const int &val);
    void _uiSldSaturation1ValueChanged(const int &val);
    void _uiSldSaturation2ValueChanged(const int &val);
    void _uiSldValue1ValueChanged(const int &val);
    void _uiSldValue2ValueChanged(const int &val);
    void _uiSldHorizon1ValueChanged(const int &val);
    void _uiSldHorizon2ValueChanged(const int &val);
    void _uiSldVertical1ValueChanged(const int &val);
    void _uiSldVertical2ValueChanged(const int &val);
    void _uiBoxDetectionAreaValueChanged(const int &val);
    void _uiBoxMorphologyToggled(const bool &checked);
    void _backgroundSettingRequest();
    void _backgroundClearingRequest();
    void _gestureListAdd();
    void _gestureListRemove();
    void _gestureEdit(QListWidgetItem *item);

private:
    explicit SettingView(QWidget *parent=0);
    SettingView(const SettingView &) = delete;
    SettingView &operator=(const SettingView &) = delete;
    static SettingView *createInstance();

    inline void _setupUi();
    inline void _makeConnections();

    inline void _emitSkinColorRangeChangeSignal();
    inline void _emitRoiRangeChangeSignal();
    void closeEvent(QCloseEvent *e) override;

    Settings *_settings;

    QTabWidget *_ui_tab_widget;

    QLabel      *_ui_lbl_hue_range;
    QSlider     *_ui_sld_hue1;
    QSlider     *_ui_sld_hue2;
    QLabel      *_ui_lbl_saturation_range;
    QSlider     *_ui_sld_saturation1;
    QSlider     *_ui_sld_saturation2;
    QLabel      *_ui_lbl_value_range;
    QSlider     *_ui_sld_value1;
    QSlider     *_ui_sld_value2;
    QLabel      *_ui_lbl_horizon_range;
    QSlider     *_ui_sld_horizon1;
    QSlider     *_ui_sld_horizon2;
    QLabel      *_ui_lbl_vertical_range;
    QSlider     *_ui_sld_vertical1;
    QSlider     *_ui_sld_vertical2;
    QSpinBox    *_ui_box_detection_area;
    QCheckBox   *_ui_box_morphology;
    QPushButton *_ui_btn_general_reset;
    QPushButton *_ui_btn_general_default;

    QLabel      *_ui_lbl_background;
    QPushButton *_ui_btn_bg_set;
    QPushButton *_ui_btn_bg_clear;

    QListWidget *_ui_lst_gesture;
    QPushButton *_ui_btn_gesture_add;
    QPushButton *_ui_btn_gesture_remove;

    bool _flag_change_hue = false;
    bool _flag_change_saturation = false;
    bool _flag_change_value = false;
    bool _flag_change_roi_x = false;
    bool _flag_change_roi_y = false;
    bool _flag_change_detection_area = false;
    bool _flag_change_morphology = false;

};

#endif // SETTINGVIEW_H
