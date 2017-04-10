#ifndef SETTINGS_H
#define SETTINGS_H

/**
 * @file
 * @author Pei Xu, xupei0610 at gmail.com
 * @brief The Settings.h file contains a singleton class that provides persistent application settings.
 */

#include <QObject>
#include <QSettings>
#include <QDebug>

#include "global.h"
#include "Singleton.h"

/**
 * @brief APPLICATION_VERSION is the current version of the system.
 */
#define APPLICATION_VERSION "1.0"
/**
 * @brief SETTING_SCOPE is the vaild scope of the setting file.
 */
#define SETTING_SCOPE QSettings::UserScope
/**
 * @brief SETTING_ORGANIZATION_NAME is the name of developer.
 */
#define SETTING_ORGANIZATION_NAME "PeiXu"
/**
 * @brief SETTING_ORGANIZATION_NAME is the name of the system.
 */
#define SETTING_APPLICATION_NAME "CNNGestureControlSystem"

/**
 * @brief The Settings class is a singleton class that provides persistent application settings.
 *
 * This is a singleton class. Use #Settings::getInstance() to get the instance of this class.
 */
class Settings final : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief getInstance returns the singleton instance of this class.
     * @return the singleton instance of this class
     */
    static Settings *getInstance();
    ~Settings();

    /**
     * @brief roi_start_x is the current left bound, in percent, of the region of interesting at the image frame captured by the camera.
     */
    const int &roi_start_x;
    /**
     * @brief setRoiStartX sets the left bound of the region of interesting at the image frace captured by the camera.
     * @param x : new value of the left bound. In percent.
     */
    void setRoiStartX(const int &x);
    /**
     * @brief roi_end_x is the current right bound, in percent, of the region of interesting at the image frame captured by the camera.
     */
    const int &roi_end_x;
    /**
     * @brief setRoiStartX sets the right bound of the region of interesting at the image frace captured by the camera.
     * @param x : new value of the right bound. In percent.
     */
    void setRoiEndX(const int &x);
    /**
     * @brief roi_start_y is the current top bound, in percent, of the region of interesting at the image frame captured by the camera.
     */
    const int &roi_start_y;
    /**
     * @brief setRoiStartY sets the top bound of the region of interesting at the image frace captured by the camera.
     * @param y : new value of the top bound. In percent.
     */
    void setRoiStartY(const int &y);
    /**
     * @brief roi_end_y is the current bottom bound, in percent, of the region of interesting at the image frame captured by the camera.
     */
    const int &roi_end_y;
    /**
     * @brief setRoiEndY sets the bottom bound of the region of interesting at the image frace captured by the camera.
     * @param y : new value of the bottom bound. In percent.
     */
    void setRoiEndY(const int &y);
    /**
     * @brief skin_color_min_H is the current lower bound of skin color filter for hue in HSV color space.
     */
    const int &skin_color_min_H;
    /**
     * @brief setSkinColorMinH sets the lower bound of skin color filter for hue in HSV color space.
     * @param h : new lower bound of hue in HSV color space
     */
    void setSkinColorMinH(const int &h);
    /**
     * @brief skin_color_min_S is the current lower bound of skin color filter for saturation in HSV color space.
     */
    const int &skin_color_min_S;
    /**
     * @brief setSkinColorMinS sets the lower bound of skin color filter for saturation in HSV color space.
     * @param s : new lower bound of saturation in HSV color space
     */
    void setSkinColorMinS(const int &s);
    /**
     * @brief skin_color_min_V is the current lower bound of skin color filter for value in HSV color space.
     */
    const int &skin_color_min_V;
    /**
     * @brief setSkinColorMinV sets the lower bound of skin color filter for value in HSV color space.
     * @param v : new lower bound of value in HSV color space
     */
    void setSkinColorMinV(const int &v);
    /**
     * @brief skin_color_max_H is the current upper bound of skin color filter for hue in HSV color space.
     */
    const int &skin_color_max_H;
    /**
     * @brief setSkinColorMaxH sets the lower bound of skin color filter for hue in HSV color space.
     * @param h : new upper bound of hue in HSV color space
     */
    void setSkinColorMaxH(const int &h);
    /**
     * @brief skin_color_max_S is the current upper bound of skin color filter for saturation in HSV color space.
     */
    const int &skin_color_max_S;
    /**
     * @brief setSkinColorMaxH sets the lower bound of skin color filter for saturation in HSV color space.
     * @param s : new upper bound of saturation in HSV color space
     */
    void setSkinColorMaxS(const int &s);
    /**
     * @brief skin_color_max_V is the current upper bound of skin color filter for value in HSV color space.
     */
    const int &skin_color_max_V;
    /**
     * @brief setSkinColorMaxH sets the lower bound of skin color filter for value in HSV color space.
     * @param v : new upper bound of value in HSV color space
     */
    void setSkinColorMaxV(const int &v);
    /**
     * @brief skin_detection_area is the current minimum area of contour who is considered the contour of hand.
     */
    const int &skin_detection_area;
    /**
     * @brief setSkinDetectionArea sets the minimum area of a hand contour who is considered as a hand.
     * @param area : new minimum area
     */
    void setSkinDetectionArea(const int &area);
    /**
     * @brief skin_morphology is the flag if the morphological transformation is performed or not.
     */
    const bool &skin_morphology;
    /**
     * @brief setSkinMorphology sets the hand detector to perform the morphological transformation or not.
     * @param perform_morphology : the flag to perform the morphological transformation or not
     */
    void setSkinMorphology(const bool &perform_morphology);
    /**
     * @brief sampling_amount_per_time is the amount of samples collected once.
     */
    const int &sampling_amount_per_time;
    /**
     * @brief setSamplingAmountPerTime sets how many samples will be collected once.
     * @param amount : the number of samples that will be collected once
     */
    void setSamplingAmountPerTime(const int &amount);
    /**
     * @brief sampling_interval is the minimum interval between two sampling actions.
     */
    const int &sampling_interval;
    /**
     * @brief setSamplingInterval sets the minimum interval between two sampling actions.
     * @param ms : the interval in milliseconds.
     */
    void setSamplingInterval(const int &ms);
    /**
     * @brief gesture_selected is the index of the gesture who is selected by default.
     *
     * It is used to keep UI fresh.
     */
    const int &gesture_selected;
    /**
     * @brief setSelectedGesture sets
     *
     * It is used to keep UI fresh.
     *
     * @param index : the index of the gesture who is selected by default in UI
     */
    void setSelectedGesture(const int &index);
    /**
     * @brief sample_storage_path is the default storage path of the samples.
     *
     * It should be the storage path of the samples collected last time so that to keep UI fresh.
     */
    const QString &sample_storage_path;
    /**
     * @brief setSampleStoragePath sets the default storage path of samples.
     *
     * It should be called using the storage path of the samples collected last time so that to keep UI fresh.
     *
     * @param path : the default path of folder to store samples
     */
    void setSampleStoragePath(const QString &path);
    /**
     * @brief cnn_model_file is the default file path of the model file.
     *
     * It should be the file path of the model file used last time so that to keep UI fresh.
     */
    const QString &cnn_model_file;
    /**
     * @brief setCnnModelFile sets the default model file.
     *
     * It should be called using the file path of the model file used last time so that to keep UI fresh.
     *
     * @param file : the default path of the model file
     */
    void setCnnModelFile(const QString &file);
    /**
     * @brief cnn_model_file is the default file path of the keymap file.
     *
     * It should be the file path of the keymap file used last time so that to keep UI fresh.
     */
    const QString &keymap_file;
    /**
     * @brief setKeymapFile sets the default keymap file.
     *
     * It should be called using the file path of the keymap file used last time so that to keep UI fresh.
     *
     * @param file : the default path of the model file
     */
    void setKeymapFile(const QString &file);
    /**
     * @brief gesture_list is the list of gestures.
     */
    const QStringList &gesture_list;
    /**
     * @brief filterGestureName filters the given gesture name to make it valid.
     * @param name : the given name of the gesture
     * @return the gesture name after filtering
     */
    QString filterGestureName(const QString &name);
    /**
     * @brief modifyGestureName modifies the name of a specific gesture.
     * @param index : the index of gesture whose name will be modified
     * @param new_name : the new name of the speicific gesture, filtered by #Settings::filterGestureName firstly
     * @retval true : if modified successfully.
     * @retval false : if another gesture already uses the specific name
     */
    bool modifyGestureName(const int &index, const QString &new_name);
    /**
     * @brief addGestrue adds a new gesture using the specific name.
     * @param gesture_name : the name of the new gesture
     * @return : the index of the added gesture, or the index of the gestrue whose name is the same with the specific name
     */
    int addGestrue(const QString &gesture_name);
    /**
     * @brief removeGesture removes a gesture from the gesture list.
     * @param index : the index of gesture who will be removed.
     */
    void removeGesture(const int &index);

    /**
     * @brief version is the current version of the setting file.
     */
    const QString &version;

private:
    Settings(QObject * parent = 0);
    static Settings * createInstance();
    Settings(const Settings &) = delete;
    Settings &operator=(const Settings &) = delete;

    QSettings * _settings = new QSettings(QSettings::IniFormat,
                                          SETTING_SCOPE,
                                          SETTING_ORGANIZATION_NAME,
                                          SETTING_APPLICATION_NAME);
    int _roi_start_x;
    int _roi_end_x;
    int _roi_start_y;
    int _roi_end_y;
    int _skin_color_min_H;
    int _skin_color_min_S;
    int _skin_color_min_V;
    int _skin_color_max_H;
    int _skin_color_max_S;
    int _skin_color_max_V;
    int _skin_detection_area;
    bool _skin_morpology;
    int _sampling_amount_per_time;
    int _sampling_interval;
    int _gesture_selected;
    QString _sample_storage_path;
    QString _cnn_model_file;
    QString _keymap_file;
    QStringList _gesture_list;
    QString _version;

};

#endif // SETTINGS_H
