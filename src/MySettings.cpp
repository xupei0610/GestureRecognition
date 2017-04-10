#include "Settings.h"

Settings * Settings::getInstance()
{
    return Singleton<Settings>::instance(Settings::createInstance);
}

Settings * Settings::createInstance()
{
    return new Settings();
}

Settings::Settings(QObject * parent) :
    QObject(parent),
    roi_start_x(_roi_start_x),
    roi_end_x(_roi_end_x),
    roi_start_y(_roi_start_y),
    roi_end_y(_roi_end_y),
    skin_color_min_H(_skin_color_min_H),
    skin_color_min_S(_skin_color_min_S),
    skin_color_min_V(_skin_color_min_V),
    skin_color_max_H(_skin_color_max_H),
    skin_color_max_S(_skin_color_max_S),
    skin_color_max_V(_skin_color_max_V),
    skin_detection_area(_skin_detection_area),
    skin_morphology(_skin_morpology),
    sampling_amount_per_time(_sampling_amount_per_time),
    sampling_interval(_sampling_interval),
    gesture_selected(_gesture_selected),
    sample_storage_path(_sample_storage_path),
    cnn_model_file(_cnn_model_file),
    keymap_file(_keymap_file),
    gesture_list(_gesture_list),
    version(_version)
{
    _roi_start_x = _settings->value("roi-start-x", DEFAULT_ROI_START_X).toInt();
    _roi_end_x = _settings->value("roi-end-x", DEFAULT_ROI_END_X).toInt();
    _roi_start_y = _settings->value("roi-start-y", DEFAULT_ROI_START_Y).toInt();
    _roi_end_y = _settings->value("roi-end-y", DEFAULT_ROI_END_Y).toInt();

    _skin_color_min_H = _settings->value("skin-color-min-H", DEFAULT_SKIN_COLOR_MIN_H).toInt();
    _skin_color_min_S = _settings->value("skin-color-min-S", DEFAULT_SKIN_COLOR_MIN_S).toInt();
    _skin_color_min_V = _settings->value("skin-color-min-V", DEFAULT_SKIN_COLOR_MIN_V).toInt();
    _skin_color_max_H = _settings->value("skin-color-max-H", DEFAULT_SKIN_COLOR_MAX_H).toInt();
    _skin_color_max_S = _settings->value("skin-color-max-S", DEFAULT_SKIN_COLOR_MAX_S).toInt();
    _skin_color_max_V = _settings->value("skin-color-max-V", DEFAULT_SKIN_COLOR_MAX_V).toInt();
    _skin_detection_area = _settings->value("skin-detection-area", DEFAULT_SKIN_DETECTION_AREA).toInt();
    _skin_morpology = _settings->value("skin-morphology", DEFAULT_SKIN_MORPHOLOGY).toBool();

    _sampling_amount_per_time = _settings->value("sampling-amount-per-time", DEFAULT_SAMPLING_AMOUNT_PER_TIME).toInt();
    _sampling_interval = _settings->value("sampling-interval", DEFAULT_SAMPLING_INTERVAL).toInt();
    _gesture_selected = _settings->value("gesture-selected", 0).toInt();
    _sample_storage_path = _settings->value("sample-storage-path").toString();

    _cnn_model_file = _settings->value("cnn-model-file").toString();
    _keymap_file = _settings->value("keymap-file").toString();

    _gesture_list = _settings->value("gesture-list").toString().split(SETTING_STRING_DELIMITER, QString::SkipEmptyParts);

    _version = _settings->value("version", APPLICATION_VERSION).toString();
}

Settings::~Settings()
{
    delete _settings;
}

void Settings::setRoiStartX(const int &x)
{
    _roi_start_x = x;
    _settings->setValue("roi-start-x", x);
}

void Settings::setRoiEndX(const int &x)
{
    _roi_end_x = x;
    _settings->setValue("roi-end-x", x);
}

void Settings::setRoiStartY(const int &y)
{
    _roi_start_y = y;
    _settings->setValue("roi-start-y", y);
}

void Settings::setRoiEndY(const int &y)
{
    _roi_end_y = y;
    _settings->setValue("roi-end-y", y);
}

void Settings::setSkinColorMinH(const int &h)
{
    _skin_color_min_H = h;
    _settings->setValue("skin-color-min-H", h);
}

void Settings::setSkinColorMinS(const int &s)
{
    _skin_color_min_H = s;
    _settings->setValue("skin-color-min-S", s);
}

void Settings::setSkinColorMinV(const int &v)
{
    _skin_color_min_V = v;
    _settings->setValue("skin-color-min-V", v);
}

void Settings::setSkinColorMaxH(const int &h)
{
    _skin_color_max_H = h;
    _settings->setValue("skin-color-max-H", h);
}

void Settings::setSkinColorMaxS(const int &s)
{
    _skin_color_max_S = s;
    _settings->setValue("skin-color-max-S", s);
}

void Settings::setSkinColorMaxV(const int &v)
{
    _skin_color_max_V = v;
    _settings->setValue("skin-color-max-V", v);
}

void Settings::setSkinDetectionArea(const int &area)
{
    _skin_detection_area = area;
    _settings->setValue("skin-detection-area", area);
}

void Settings::setSkinMorphology(const bool &perform_morphology)
{
    _skin_morpology = perform_morphology;
    _settings->setValue("skin-morphology", perform_morphology);
}

void Settings::setSamplingAmountPerTime(const int &amount)
{
    _sampling_amount_per_time = amount;
    _settings->setValue("sampling-amount-per-time", amount);
}

void Settings::setSamplingInterval(const int &ms)
{
    _sampling_interval = ms;
    _settings->setValue("sampling-interval", ms);
}

void Settings::setSelectedGesture(const int &index)
{
    if (index < 0)
        return;
    _gesture_selected = index;
    _settings->setValue("gesture-selected", index);
}

void Settings::setSampleStoragePath(const QString &path)
{
    _sample_storage_path = path;
    _settings->setValue("gesture-selected", path);
}

void Settings::setCnnModelFile(const QString &file)
{
    _cnn_model_file = file;
    _settings->setValue("cnn-model-file", file);
}

void Settings::setKeymapFile(const QString &file)
{
    _keymap_file = file;
    _settings->setValue("keymap-file", file);
}

QString Settings::filterGestureName(const QString & text)
{
    return text.trimmed().replace(' ', '-').replace(SETTING_STRING_DELIMITER, "-");
}

bool Settings::modifyGestureName(const int &index, const QString &new_name)
{
    if (_gesture_list.size() > index && index > -1)
    {
        if (_gesture_list.contains(new_name))
            return false;
        _gesture_list[index] = new_name;
        _settings->setValue("gesture-list", _gesture_list.join(SETTING_STRING_DELIMITER));
        return true;
    }
    return false;
}

int Settings::addGestrue(const QString &gesture_name)
{
    if (!gesture_name.isEmpty())
    {
        if (_gesture_list.contains(gesture_name))
            return _gesture_list.lastIndexOf(gesture_name);
        _gesture_list.append(gesture_name);
        _settings->setValue("gesture-list", _gesture_list.join(SETTING_STRING_DELIMITER));
    }
    return _gesture_list.size() - 1;
}

void Settings::removeGesture(const int &index)
{
    if (_gesture_list.size() > index && index > -1)
    {
        _gesture_list.removeAt(index);
        _settings->setValue("gesture-list", _gesture_list.join(SETTING_STRING_DELIMITER));
        if (index == _gesture_selected)
        {
            _gesture_selected = 0;
            _settings->setValue("gesture-selected", 0);
        }
        else if (index > _gesture_selected)
        {
            _gesture_selected -= 1;
            _settings->setValue("gesture-selected", _gesture_selected);
        }
    }
}
