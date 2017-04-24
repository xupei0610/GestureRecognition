#include "CommandInputter.h"

CommandInputter::CommandInputter():
    keymap(_keymap),
    _action_timer(new QTimer),
    _update_action_count_timer(new QTimer),
    _lost_tracking_timer(new QTimer),
    _mouse_drag_has_released(true),
    _last_keyboard_command(-1),
    _kalman_filter(new cv::KalmanFilter),
    _keymap_settings(nullptr)
{
    _action_timer->setSingleShot(true);
    _lost_tracking_timer->setSingleShot(true);

    connect(_update_action_count_timer, &QTimer::timeout, this, &CommandInputter::_freshActionCount);
    connect(_lost_tracking_timer, &QTimer::timeout, this, &CommandInputter::_initKalmanFilter);
    connect(_lost_tracking_timer, &QTimer::timeout, this, &CommandInputter::keyRelease);
    connect(_lost_tracking_timer, &QTimer::timeout, this, &CommandInputter::mouseRelease);
    connect(_lost_tracking_timer, &QTimer::timeout, this, &CommandInputter::clearActionCount);

}

CommandInputter::~CommandInputter()
{
    if (_kalman_filter != nullptr)
        delete _kalman_filter;
}

bool CommandInputter::load(const QString &keymap_file)
{
    if (!QFileInfo(keymap_file).isFile())
        return false;

    if (_keymap_settings != nullptr)
        delete _keymap_settings;

    _keymap_settings = new QSettings(keymap_file, QSettings::IniFormat);

    _labels = _keymap_settings->value("labels").toString().split(SETTING_STRING_DELIMITER);

    _shortcuts = _keymap_settings->value("key-shortcuts").toString().split(SETTING_STRING_DELIMITER);
    if (_shortcuts.size() != _labels.size())
    {
        delete _keymap_settings;
        return false;
    }

    mouseRelease();
    keyRelease();
    _action_frame_count.clear();
    clearActionCount();
    _initKalmanFilter();

    auto it_end = EVENT_NAME2CODE_MAP.end();
    int i = -1;
    for (const auto &s : _shortcuts)
    {
        i++;
        if (s.isEmpty())
            continue;
        std::vector<MOUSE_KEYBOARD_ACTION> tmp;
        for (const auto &c: s.split("+"))
        {
            if (EVENT_NAME2CODE_MAP.find(c) != it_end && EVENT_NAME2CODE_MAP.at(c) < 0xF0)
                tmp.push_back(EVENT_NAME2CODE_MAP.at(c));
        }
        if (!tmp.empty())
            _keymap[i] = std::move(tmp);
    }

    auto mouses = _keymap_settings->value("mouse-actions").toString().split(SETTING_STRING_DELIMITER);
    for (const auto &s : mouses)
    {
        _mouse_map.push_back(s.toInt());
    }

    _action_frame_count[MOUSE_LEFT_CLICK] = 0;
    _action_frame_count[MOUSE_RIGHT_CLICK] = 0;
    _action_frame_count[MOUSE_DRAG] = 0;
    _action_frame_count[MOUSE_DOUBLE_LEFT_CLICK] = 0;

    return true;
}

void CommandInputter::input(const int &label_index,
                            const float &tracked_pos_x,
                            const float &tracked_pos_y)
{
    _lost_tracking_timer->start(_lost_tracking_interval);

#if defined(__APPLE__)
    int screen_width = CGDisplayPixelsWide(CGMainDisplayID());
    int screen_height = CGDisplayPixelsHigh(CGMainDisplayID());
#elif defined(_WIN_)
    // TODO Screen geometry using windows
#else
    // TODO Screen geometry using X11
#endif
    auto cursor_pos = estimateCursorPos(tracked_pos_x*screen_width, tracked_pos_y*screen_height);
    int indx = _mouse_map.indexOf(label_index);

    if (indx > -1)
        makeMouseAction(static_cast<MOUSE_KEYBOARD_ACTION>(indx^0xFF), cursor_pos);
    else makeKeyboardAction(label_index);
}

const QStringList CommandInputter::labels()
{
    return _labels;
}

void CommandInputter::makeMouseAction(const MOUSE_KEYBOARD_ACTION &action, const cv::Point &cursor_pos)
{
    if ((action !=MOUSE_MOVE && !_action_timer->isActive() && _action_frame_count[action] > _action_sensitivity) ||
             (action == MOUSE_DRAG && _mouse_drag_has_released == false))
     {
         _action_timer->start(_action_interval);
         clearActionCount();
         if (action == MOUSE_DRAG)
         {
             _mouseDrag(cursor_pos.x, cursor_pos.y);
             _last_drag_action_pos.first = cursor_pos.x;
             _last_drag_action_pos.second = cursor_pos.y;
             _mouse_drag_has_released = false;
             return;
         }
         else
         {
             mouseRelease();
             if (action == MOUSE_LEFT_CLICK)
             {
                 _mouseLeftClick(cursor_pos.x, cursor_pos.y);
             }
             else if (action == MOUSE_RIGHT_CLICK)
             {
                 _mouseRightClick(cursor_pos.x, cursor_pos.y);
             }
             else if (action == MOUSE_DOUBLE_LEFT_CLICK)
             {
                 _mouseDoubleClick(cursor_pos.x, cursor_pos.y);
             }
             return;
         }
     }
     if (_action_timer->isActive())
         _countAction(MOUSE_MOVE);
     else
         _countAction(action);
     mouseRelease();
     _mouseMove(cursor_pos.x, cursor_pos.y);
}

void CommandInputter::makeKeyboardAction(const int &label_id)
{
    if (_action_timer->isActive())
        return;

    mouseRelease();
    if (label_id != _last_keyboard_command)
    {
        keyRelease();
        if (_keymap.find(label_id) == _keymap.end())
            return;
        for (const auto &c: _keymap.at(label_id))
        {
#ifdef __APPLE__
            _last_keyboard_events.push_back(CGEventCreateKeyboardEvent(NULL, (CGKeyCode)c, true));
#endif
#ifdef _WIN_
            // TODO WIN
#endif
#ifdef _X11_
            // TODO X11
#endif
        }
        _last_keyboard_command = label_id;
    }
    for (auto &k : _last_keyboard_events)
        CGEventPost(kCGSessionEventTap, k);
    emit commandMade(_shortcuts.at(label_id));
}

void CommandInputter::mouseRelease()
{
    if (_mouse_drag_has_released == false)
    {
#ifdef __APPLE__
        CGEventRef mouse_left_up = CGEventCreateMouseEvent(NULL,
                                                           kCGEventLeftMouseUp,
                                                           CGPointMake(_last_drag_action_pos.first,
                                                                       _last_drag_action_pos.second),
                                                           kCGMouseButtonLeft);
        CGEventPost(kCGHIDEventTap, mouse_left_up);
        CFRelease(mouse_left_up);
        _mouse_drag_has_released = true;
#endif
#ifdef _WIN_
        // TODO WIN
#endif
#ifdef _X11_
        // TODO X11
#endif
    }
}

void CommandInputter::keyRelease()
{
    if (_last_keyboard_command == -1)
        return;
    for (auto & k : _last_keyboard_events)
    {
#ifdef __APPLE__
        CGEventSetType(k, kCGEventKeyUp);
        CGEventPost(kCGSessionEventTap, k);
        CFRelease(k);
#endif
#ifdef _WIN_
        // TODO WIN
#endif
#ifdef _X11_
        // TODO X11
#endif
    }
    _last_keyboard_events.clear();
    _last_keyboard_command = -1;
}

void CommandInputter::clearActionCount()
{
    _update_action_count_timer->stop();
    _action_sequence.clear();
    for (auto & c: _action_frame_count)
        c.second = 0;
}

cv::Point CommandInputter::estimateCursorPos(const int &tracked_pos_x, const int &tracked_pos_y)
{
    _kalman_filter->predict();
    auto estimated = _kalman_filter->correct((cv::Mat_<float>(2,1) << tracked_pos_x, tracked_pos_y));
    return cv::Point(estimated.at<float>(0), estimated.at<float>(1));
}

void CommandInputter::_countAction(const MOUSE_KEYBOARD_ACTION &action)
{
    _action_frame_count[action]++;
    _action_sequence.push_back(action);
    if (!_update_action_count_timer->isActive())
        _update_action_count_timer->start(_action_count_period);
}

void CommandInputter::_initKalmanFilter()
{
    _kalman_filter->init(4, 2);
    _kalman_filter->statePre = cv::Mat::zeros(4,1,CV_32F);
    _kalman_filter->transitionMatrix = cv::Mat::eye(4,4,CV_32F);
    cv::setIdentity(_kalman_filter->measurementMatrix);
    cv::setIdentity(_kalman_filter->processNoiseCov, cv::Scalar::all(1e-4));
    cv::setIdentity(_kalman_filter->measurementNoiseCov, cv::Scalar::all(1e-3));
    cv::setIdentity(_kalman_filter->errorCovPost, cv::Scalar::all(0.2));
}

void CommandInputter::_freshActionCount()
{
    _action_sequence.pop_front();
    if (_action_frame_count[_action_sequence.front()]>0)
        _action_frame_count[_action_sequence.front()]--;
    if (_action_sequence.empty())
        _update_action_count_timer->stop();
}

void CommandInputter::_mouseMove(const int & cursor_x, const int & cursor_y)
{
#if defined(__APPLE__)
    CGEventRef mouse_move = CGEventCreateMouseEvent(NULL,
                                                    kCGEventMouseMoved,
                                                    CGPointMake(cursor_x, cursor_y),
                                                    kCGMouseButtonLeft);
    CGEventPost(kCGHIDEventTap, mouse_move);
    CFRelease(mouse_move);
#elif defined(_WIN_)
    // TODO MouseMoveEvent for windows
#else
    // TODO MouseMoveEvent using X11
#endif
}

void CommandInputter::_mouseLeftClick(const int & cursor_x, const int & cursor_y)
{
#if defined(__APPLE__)
    CGEventRef mouse_click = CGEventCreateMouseEvent(NULL,
                                                     kCGEventLeftMouseDown,
                                                     CGPointMake(cursor_x, cursor_y),
                                                     kCGMouseButtonLeft);
    CGEventPost(kCGHIDEventTap, mouse_click);
    CGEventSetType(mouse_click, kCGEventLeftMouseUp);
    CGEventPost(kCGHIDEventTap, mouse_click);
    CFRelease(mouse_click);
#elif defined(_WIN_)
    // TODO MouseLeftClickEvent for windows
#else
    // TODO MouseLeftClickEvent using X11
#endif
    emit commandMade("Mouse: Left Click");
}

void CommandInputter::_mouseRightClick(const int & cursor_x, const int & cursor_y)
{
#if defined(__APPLE__)
    CGEventRef mouse_click = CGEventCreateMouseEvent(NULL,
                                                     kCGEventRightMouseDown,
                                                     CGPointMake(cursor_x, cursor_y),
                                                     kCGMouseButtonRight);

    CGEventPost(kCGHIDEventTap, mouse_click);
    CGEventSetType(mouse_click, kCGEventRightMouseUp);
    CGEventPost(kCGHIDEventTap, mouse_click);
    CFRelease(mouse_click);
#elif defined(_WIN_)
    // TODO MouseRightClickEvent for windows
#else
    // TODO MouseRightClickEvent using X11
#endif
    emit commandMade("Mouse: Right Click");
}

void CommandInputter::_mouseDoubleClick(const int & cursor_x, const int & cursor_y)
{
#if defined(__APPLE__)
    CGEventRef mouse_click = CGEventCreateMouseEvent(NULL,
                                                     kCGEventLeftMouseDown,
                                                     CGPointMake(cursor_x, cursor_y),
                                                     kCGMouseButtonLeft);
    CGEventPost(kCGHIDEventTap, mouse_click);
    CGEventSetType(mouse_click, kCGEventLeftMouseUp);
    CGEventPost(kCGHIDEventTap, mouse_click);
    CGEventSetIntegerValueField(mouse_click, kCGMouseEventClickState, 2);
    CGEventSetType(mouse_click, kCGEventLeftMouseDown);
    CGEventPost(kCGHIDEventTap, mouse_click);
    CGEventSetType(mouse_click, kCGEventLeftMouseUp);
    CGEventPost(kCGHIDEventTap, mouse_click);
    CFRelease(mouse_click);
#elif defined(_WIN_)
    // TODO MouseDoubleLeftClickEvent for windows
#else
    // TODO MouseDoubleLeftClickEvent using X11
#endif
    emit commandMade("Mouse: Double Click");
}

void CommandInputter::_mouseDrag(const int & cursor_x, const int & cursor_y)
{
#if defined(__APPLE__)
    CGEventRef mouse_drag;
    if (_mouse_drag_has_released == true)
    {
        mouse_drag = CGEventCreateMouseEvent(NULL,
                                             kCGEventLeftMouseDown,
                                             CGPointMake(cursor_x, cursor_y),
                                             kCGMouseButtonLeft);
        emit commandMade("Mouse: Drag Begining");
    }
    else
    {
        mouse_drag = CGEventCreateMouseEvent(NULL,
                                             kCGEventMouseMoved,
                                             CGPointMake(cursor_x, cursor_y),
                                             kCGMouseButtonLeft);
    }
    CGEventPost(kCGHIDEventTap, mouse_drag);
    CFRelease(mouse_drag);

#elif defined(_WIN_)
    // TODO MouseDragEvent for windows
#else
    // TODO MouseDragEvent using X11
#endif
}
