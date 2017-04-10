#ifndef COMMANDINPUTTER_H
#define COMMANDINPUTTER_H
/**
 * @file
 * @author Pei Xu, xupei0610 at gmail.com
 * @brief The CommandInputter.h file contains an implementation of the command inputter class who makes commands to the computer based on the recognition result obtained by gesture analyst classes.
 */
#include "CommandInputterInterface.h"
#include "global.h"

#include <vector>
#include <unordered_map>
#include <QTimer>
#include <QString>
#include <QSettings>
#include <QFileInfo>
#include <QList>
#include <QStringList>
#include <QDebug>
#include <opencv2/opencv.hpp>

#if defined(__APPLE__)
#include <ApplicationServices/ApplicationServices.h>
#elif defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
#define _WIN_
#include <windows.h>
#else
#define _X11_
#include <X11.h>
#endif

#ifndef ACTION_RESPONSE_INTERVAL
/**
 * @breif ACTION_RESPONSE_INTERVAL is the interval, in ms, to accept the next key/mouse command.
 */
#define ACTION_RESPONSE_INTERVAL 20*1000/CAMERA_FPS
#endif
#ifndef TRACKING_LOST_INTERVAL
/**
 * @breif TRACKING_LOST_INTERVAL is the interval, in ms , how long the system considers it loses tracking.
 */
#define TRACKING_LOST_INTERVAL 40*1000/CAMERA_FPS
#endif
#ifndef ACTION_SENSITIVITY
/**
 * @breif ACTION_SENSITIVITY is the threshold that how many frames an action must keep in a period before it is accepted.
 */
#define ACTION_SENSITIVITY 10
#endif
#ifndef ACTION_COUNT_PERIOD
/**
 * @breif ACTION_COUNT_PERIOD is the period, in ms, during which we count the frequency of each control signal and then make command to the computer based on the most frequent control signal.
 */
#define ACTION_COUNT_PERIOD 15*1000/CAMERA_FPS
#endif

/**
 * @brief The CommandInputter class is an implementation of the command inputter class who makes commands to the computer based on the recognition result obtained by gesture analyst classes.
 *
 * This class estimates the cursor position using the Kalman filter, and makes commands only based on the analysis result obtained by gesture analyst.
 *
 * It responds to the control signal via #CommandInputter::input every #CommandInputter::_action_interval in order to prevent noise signals.\n
 * For the mouse event, it counts the frequency of each gesture during a period of time, #CommandInputter::_action_count_period, and then makes commands based on the most frequent gesture (#CommandInputter::_action_sensitivity).\n
 * As for the keyboard event, it responds immediately, if the action interval is over, in order to improve performance.\n
 *
 * @see #CommandInputterInterface
 * @see #GestureAnalystInterface
 * @see #CommandInputter::_lost_tracking_timer
 * @see #CommandInputter::_update_action_count_timer
 */
class CommandInputter : public CommandInputterInterface
{
    Q_INTERFACES(CommandInputterInterface)

public:
    typedef int LabelId;

    explicit CommandInputter();
    ~CommandInputter();

    /**
     * @brief load loads the specific keymap file.
     * @param keymap_file : the specific keymap file
     * @retval true : if successfully load the file
     * @retval false : if unable to open the file or failed to parse the file
     */
    bool load(const QString &keymap_file) override;
    /**
     * @brief input inputs a command to computer according to the label index.
     *
     * The function searches the command, corresponding to the given `label_index`, from keymap file loaded by #CommandInputter::load and then make a command to the computer.
     * It should emit a signal, #CommandInputterInterface::commandMade, to inform what command it made.
     *
     * @param label_index : the index of the label with which a command is associated
     * @param tracked_pos_x : the horizontal position of the traked point which is used for control the cursor
     * @param tracked_pos_y : the vertical position of the traked point which is used for control the cursor
     *
     * @see #HandDetector::tracked_point
     */
    void input(const int &label_index,
               const float &tracked_pos_x,
               const float &tracked_pos_y) override;


    /**
     * @brief labels returns the list of labels (gestures).
     * @return the list of labels (gestures)
     */
    const QStringList labels() override;

    /**
     * @brief MOUSE_KEYBOARD_ACTION represents the key codes for each keyboard event and a set of customized mouse event codes.
     */
    enum MOUSE_KEYBOARD_ACTION
    {
        MOUSE_MOVE          = 0xFF,
        MOUSE_LEFT_CLICK    = 0xFE,
        MOUSE_RIGHT_CLICK   = 0xFD,
        MOUSE_DRAG          = 0xFC,
        MOUSE_DOUBLE_LEFT_CLICK = 0xFB,
#ifdef __APPLE__
        KEY_A              = 0x00,
        KEY_S              = 0x01,
        KEY_D              = 0x02,
        KEY_F              = 0x03,
        KEY_H              = 0x04,
        KEY_G              = 0x05,
        KEY_Z              = 0x06,
        KEY_X              = 0x07,
        KEY_C              = 0x08,
        KEY_V              = 0x09,
        KEY_B              = 0x0B,
        KEY_Q              = 0x0C,
        KEY_W              = 0x0D,
        KEY_E              = 0x0E,
        KEY_R              = 0x0F,
        KEY_Y              = 0x10,
        KEY_T              = 0x11,
        KEY_1              = 0x12,
        KEY_2              = 0x13,
        KEY_3              = 0x14,
        KEY_4              = 0x15,
        KEY_6              = 0x16,
        KEY_5              = 0x17,
        KEY_Equal          = 0x18,
        KEY_9              = 0x19,
        KEY_7              = 0x1A,
        KEY_Minus          = 0x1B,
        KEY_8              = 0x1C,
        KEY_0              = 0x1D,
        KEY_RightBracket   = 0x1E,
        KEY_O              = 0x1F,
        KEY_U              = 0x20,
        KEY_LeftBracket    = 0x21,
        KEY_I              = 0x22,
        KEY_P              = 0x23,
        KEY_L              = 0x25,
        KEY_J              = 0x26,
        KEY_Quote          = 0x27,
        KEY_K              = 0x28,
        KEY_Semicolon      = 0x29,
        KEY_Backslash      = 0x2A,
        KEY_Comma          = 0x2B,
        KEY_Slash          = 0x2C,
        KEY_N              = 0x2D,
        KEY_M              = 0x2E,
        KEY_Period         = 0x2F,
        KEY_Grave          = 0x32,
        KEY_KeypadDecimal  = 0x41,
        KEY_KeypadMultiply = 0x43,
        KEY_KeypadPlus     = 0x45,
        KEY_KeypadClear    = 0x47,
        KEY_KeypadDivide   = 0x4B,
        KEY_KeypadEnter    = 0x4C,
        KEY_KeypadMinus    = 0x4E,
        KEY_KeypadEquals   = 0x51,
        KEY_Keypad0        = 0x52,
        KEY_Keypad1        = 0x53,
        KEY_Keypad2        = 0x54,
        KEY_Keypad3        = 0x55,
        KEY_Keypad4        = 0x56,
        KEY_Keypad5        = 0x57,
        KEY_Keypad6        = 0x58,
        KEY_Keypad7        = 0x59,
        KEY_Keypad8        = 0x5B,
        KEY_Keypad9        = 0x5C,
        KEY_Return         = 0x24,
        KEY_Tab            = 0x30,
        KEY_Space          = 0x31,
        KEY_Delete         = 0x33,
        KEY_Escape         = 0x35,
        KEY_Command        = 0x37,
        KEY_Shift          = 0x38,
        KEY_CapsLock       = 0x39,
        KEY_Option         = 0x3A,
        KEY_Control        = 0x3B,
        KEY_RightShift     = 0x3C,
        KEY_RightOption    = 0x3D,
        KEY_RightControl   = 0x3E,
        KEY_Function       = 0x3F,
        KEY_F17            = 0x40,
        KEY_VolumeUp       = 0x48,
        KEY_VolumeDown     = 0x49,
        KEY_Mute           = 0x4A,
        KEY_F18            = 0x4F,
        KEY_F19            = 0x50,
        KEY_F20            = 0x5A,
        KEY_F5             = 0x60,
        KEY_F6             = 0x61,
        KEY_F7             = 0x62,
        KEY_F3             = 0x63,
        KEY_F8             = 0x64,
        KEY_F9             = 0x65,
        KEY_F11            = 0x67,
        KEY_F13            = 0x69,
        KEY_F16            = 0x6A,
        KEY_F14            = 0x6B,
        KEY_F10            = 0x6D,
        KEY_F12            = 0x6F,
        KEY_F15            = 0x71,
        KEY_Help           = 0x72,
        KEY_Home           = 0x73,
        KEY_PageUp         = 0x74,
        KEY_ForwardDelete  = 0x75,
        KEY_F4             = 0x76,
        KEY_End            = 0x77,
        KEY_F2             = 0x78,
        KEY_PageDown       = 0x79,
        KEY_F1             = 0x7A,
        KEY_Left           = 0x7B,
        KEY_Right          = 0x7C,
        KEY_Down           = 0x7D,
        KEY_Up             = 0x7E
#endif
#ifdef _WIN_
        // TODO WIN
#endif
#ifdef _X11_
        // TODO X11
#endif
    };

    /**
     * @brief EVENT_NAME2CODE_MAP is the map from key event's name to its code.
     */
    const std::map<QString, MOUSE_KEYBOARD_ACTION> EVENT_NAME2CODE_MAP =
    {
        {"MOUSE_MOVE", MOUSE_MOVE},
        {"MOUSE_LEFT_CLICK", MOUSE_LEFT_CLICK},
        {"MOUSE_RIGHT_CLICK", MOUSE_RIGHT_CLICK},
        {"MOUSE_DRAG", MOUSE_DRAG},
        {"MOUSE_DOUBLE_LEFT_CLICK", MOUSE_DOUBLE_LEFT_CLICK},
        {"KEY_A", KEY_A},
        {"KEY_S", KEY_S},
        {"KEY_D", KEY_D},
        {"KEY_F", KEY_F},
        {"KEY_H", KEY_H},
        {"KEY_G", KEY_G},
        {"KEY_Z", KEY_Z},
        {"KEY_X", KEY_X},
        {"KEY_C", KEY_C},
        {"KEY_V", KEY_V},
        {"KEY_B", KEY_B},
        {"KEY_Q", KEY_Q},
        {"KEY_W", KEY_W},
        {"KEY_E", KEY_E},
        {"KEY_R", KEY_R},
        {"KEY_Y", KEY_Y},
        {"KEY_T", KEY_T},
        {"KEY_1", KEY_1},
        {"KEY_2", KEY_2},
        {"KEY_3", KEY_3},
        {"KEY_4", KEY_4},
        {"KEY_6", KEY_6},
        {"KEY_5", KEY_5},
        {"KEY_Equal", KEY_Equal},
        {"KEY_9", KEY_9},
        {"KEY_7", KEY_7},
        {"KEY_Minus", KEY_Minus},
        {"KEY_8", KEY_8},
        {"KEY_0", KEY_0},
        {"KEY_RightBracket", KEY_RightBracket},
        {"KEY_O", KEY_O},
        {"KEY_U", KEY_U},
        {"KEY_LeftBracket", KEY_LeftBracket},
        {"KEY_I", KEY_I},
        {"KEY_P", KEY_P},
        {"KEY_L", KEY_L},
        {"KEY_J", KEY_J},
        {"KEY_Quote", KEY_Quote},
        {"KEY_K", KEY_K},
        {"KEY_Semicolon", KEY_Semicolon},
        {"KEY_Backslash", KEY_Backslash},
        {"KEY_Comma", KEY_Comma},
        {"KEY_Slash", KEY_Slash},
        {"KEY_N", KEY_N},
        {"KEY_M", KEY_M},
        {"KEY_Period", KEY_Period},
        {"KEY_Grave", KEY_Grave},
        {"KEY_KEYpadDecimal",  KEY_KeypadDecimal},
        {"KEY_KEYpadMultiply", KEY_KeypadMultiply},
        {"KEY_KEYpadPlus",     KEY_KeypadPlus},
        {"KEY_KEYpadClear", KEY_KeypadClear},
        {"KEY_KEYpadDivide", KEY_KeypadDivide},
        {"KEY_KEYpadEnter", KEY_KeypadEnter},
        {"KEY_KEYpadMinus", KEY_KeypadMinus},
        {"KEY_KEYpadEquals", KEY_KeypadEquals},
        {"KEY_KEYpad0", KEY_Keypad0},
        {"KEY_KEYpad1", KEY_Keypad1},
        {"KEY_KEYpad2", KEY_Keypad2},
        {"KEY_KEYpad3", KEY_Keypad3},
        {"KEY_KEYpad4", KEY_Keypad4},
        {"KEY_KEYpad5", KEY_Keypad5},
        {"KEY_KEYpad6", KEY_Keypad6},
        {"KEY_KEYpad7", KEY_Keypad7},
        {"KEY_KEYpad8", KEY_Keypad8},
        {"KEY_KEYpad9", KEY_Keypad9},
        {"KEY_Return", KEY_Return},
        {"KEY_Tab", KEY_Tab},
        {"KEY_Space", KEY_Space},
        {"KEY_Delete", KEY_Delete},
        {"KEY_Escape", KEY_Escape},
        {"KEY_Command", KEY_Command},
        {"KEY_Shift", KEY_Shift},
        {"KEY_CapsLock", KEY_CapsLock},
        {"KEY_Option", KEY_Option},
        {"KEY_Control", KEY_Control},
        {"KEY_RightShift", KEY_RightShift},
        {"KEY_RightOption", KEY_RightOption},
        {"KEY_RightControl", KEY_RightControl},
        {"KEY_Function", KEY_Function},
        {"KEY_F17", KEY_F17},
        {"KEY_VolumeUp", KEY_VolumeUp},
        {"KEY_VolumeDown", KEY_VolumeDown},
        {"KEY_Mute", KEY_Mute},
        {"KEY_F18", KEY_F18},
        {"KEY_F19", KEY_F19},
        {"KEY_F20", KEY_F20},
        {"KEY_F5", KEY_F5},
        {"KEY_F6", KEY_F6},
        {"KEY_F7", KEY_F7},
        {"KEY_F3", KEY_F3},
        {"KEY_F8", KEY_F8},
        {"KEY_F9", KEY_F9},
        {"KEY_F11", KEY_F11},
        {"KEY_F13", KEY_F13},
        {"KEY_F16", KEY_F16},
        {"KEY_F14", KEY_F14},
        {"KEY_F10", KEY_F10},
        {"KEY_F12", KEY_F12},
        {"KEY_F15", KEY_F15},
        {"KEY_Help", KEY_Help},
        {"KEY_Home", KEY_Home},
        {"KEY_PageUp", KEY_PageUp},
        {"KEY_ForwardDelete", KEY_ForwardDelete},
        {"KEY_F4", KEY_F4},
        {"KEY_End", KEY_End},
        {"KEY_F2", KEY_F2},
        {"KEY_PageDown", KEY_PageDown},
        {"KEY_F1", KEY_F1},
        {"KEY_Left", KEY_Left},
        {"KEY_Right", KEY_Right},
        {"KEY_Down", KEY_Down},
        {"KEY_Up", KEY_Up}
    };

    /**
     * @brief _keymap is the map for the label index to the mouse/keyboard action.
     *
     * This is a reference to #CommandInputter::_keymap . It depends on the keymap file it loads.
     */
    const std::unordered_map<int, std::vector<MOUSE_KEYBOARD_ACTION> > &keymap;

    /**
     * @brief makeMouseAction sends mouse event commands to the computer.
     * @param action : code of the mouse event
     * @param cursor_pos : the position where the event will happen. It usually is obtined by the Kalman filter
     *
     * @see #CommandInputter:estimateCursorPos
     */
    void makeMouseAction(const MOUSE_KEYBOARD_ACTION &action, const cv::Point &cursor_pos);
    /**
     * @brief makeKeyboardAction sends keyboard event commands to the computer.
     *
     * The key sequence will be picked out from #CommandInputter::keymap according the given label id.
     *
     * @param label_id : the label id of the control signal (gesture)
     */
    void makeKeyboardAction(const int &label_id);
    /**
     * @brief estimateCursorPos estimates the position of cursor via the Kalman filter using the new given position of the tracked point.
     * @param tracked_pos_x : the horizontal position of the tracked point.
     * @param tracked_pos_y : the vertical position of the tracked point.
     */
    cv::Point estimateCursorPos(const int & tracked_pos_x, const int & tracked_pos_y);


public slots:
    /**
     * @brief keyRelease releases the held keys.
     */
    void keyRelease();
    /**
     * @brief mouseRelease releases the held mouse button (usually the left button for drag action).
     */
    void mouseRelease();
    /**
     * @brief clearActionCount clears the count of control signals.
     *
     * @see #CommandInputter::_lost_tracking_interval
     */
    void clearActionCount();

protected:
    /**
     * @brief _action_timer is the timer of action interval.
     *
     * An action will be responded to if the interval is long enough.
     *
     * @see #CommandInputter::_action_interval
     */
    QTimer *_action_timer;
    /**
     * @brief _update_frame_count_timer is the timer to keep the count of control signals fresh.
     *
     * @see #CommandInputter::_action_frame_count
     * @see #CommandInputter::_action_sequence
     */
    QTimer *_update_action_count_timer;
    /**
     * @brief _lost_tracking_timer is the timer to count how long no tracked point is detected.
     *
     * The Kalman filter will be reinitialized if it is considered that the tracking is lost.
     *
     * @see #CommandInputter::_lost_tracking_interval
     */
    QTimer *_lost_tracking_timer;

    /**
     * @brief _action_interval is the interval, in ms, to accept the next key/mouse command
     *
     * @see #ACTION_RESPONSE_INTERVAL
     * @see #CommandInputter::_action_timer
     */
    const size_t _action_interval = ACTION_RESPONSE_INTERVAL;
    /**
     * @brief _lost_tracking_interval is the interval, in ms , how long the system considers it loses tracking.
     *
     * All parameters will be reinitialized when tracking is considered lost.
     *
     * @see #TRACKING_LOST_INTERVAL
     * @see #CommandInputter::_lost_tracking_timer
     * @see #CommandInputter::actionRelease
     * @see #CommandInputter::clearActionCount
     * @see #CommandInputter::_initKalmanFilter
     */
    const size_t _lost_tracking_interval = TRACKING_LOST_INTERVAL;
    /**
     * @brief _action_sensitivity is the threshold that how many frames an action must keep in a period before it is accepted.
     *
     * @see #ACTION_SENSITIVITY
     */
    const size_t _action_sensitivity = ACTION_SENSITIVITY;
    /**
     * @brief _action_count_period is the period, in ms, during which we count the frequency of each control signal and then make command to the computer based on the most frequent control signal.
     *
     * @see #ACTION_COUNT_PERIOD
     * @see #CommandInputter::_update_action_count_timer
     */
    const size_t _action_count_period = ACTION_COUNT_PERIOD;

    /**
     * @brief _action_frame_count is a counter of the control signals during a #CommandInputter:_action_count_period
     *
     * We, in fact, only count the mouse event in order to improve the performance of responding to keyboard events.
     */
    std::map<MOUSE_KEYBOARD_ACTION, size_t> _action_frame_count;
    /**
     * @brief _action_sequence is a sequence of the control signals during a #CommandInputter:_action_count_period .
     *
     * We, in fact, only count the mouse event in order to improve the performance of responding to keyboard events.
     */
    std::list<MOUSE_KEYBOARD_ACTION> _action_sequence;

    /**
     * @brief _mouse_drag_has_released is the flag whether the mouse drag action has released.
     *
     * Differing from other mouse actions, the drag action must be held until some other control signal has been received and has been confirmed to be effective.
     */
    bool _mouse_drag_has_released;
    /**
     * @brief _last_drag_action_pos records the position of the last drag action such that we know where to release the drag action.
     */
    std::pair<int, int> _last_drag_action_pos;

#ifdef __APPLE__
    /**
     * @brief _last_keyboard_events records the keyboard events made last time.
     *
     * Similarly to the mouse drag action, the keyboard events must be held until new control signals have been received and confirmed.
     *
     * This is for Mac OS API.
     */
    std::vector<CGEventRef> _last_keyboard_events;
#endif
#ifdef _WIN_
    // TODO WIN
#endif
#ifdef _X11_
    // TODO X11
#endif
    /**
     * @brief _last_keyboard_command is the command of keyboard events made last time.
     *
     * We use it to check if the new received command is a new command or not.
     */
     int _last_keyboard_command;

    /**
     * @brief _kalman_filter is the Kalman filter used to keep cursor stable.
     */
    cv::KalmanFilter *_kalman_filter;

    /**
     * @brief _keymap_settings is the instance of parsing the currently used keymap file.
     */
    QSettings *_keymap_settings;

    /**
     * @brief _labels is the gesture list.
     *
     * Its value depends on the keymap file it loads.
     */
    QStringList _labels;
    /**
     * @brief _shortcuts is the string name of each defined shortcut sequence.
     */
    QStringList _shortcuts;
    /**
     * @brief _keymap is the map for the label index to the keyboard action.
     *
     * It depends on the keymap file it loads.
     */
    std::unordered_map<LabelId, std::vector<MOUSE_KEYBOARD_ACTION> > _keymap;
    /**
     * @brief _mouse_map is the map for the label index to mouse action.
     *
     * It depends on the keymap file it loads.
     */
    QList<LabelId> _mouse_map;

    /**
     * @brief _countAction counts the control signals
     *
     * We, in fact, only count the mouse action.
     *
     * @param action : the current control signal
     *
     * @see #CommandInputter::_action_frame_count
     * @see #CommandInputter::_action_sequence
     * @see #CommandInputter::_update_action_count_timer
     */
    void _countAction(const MOUSE_KEYBOARD_ACTION &action);

protected slots:
    /**
     * @brief _initKalmanFilter initializes the Kalman filter.
     * @see #CommandInputter::_lost_tracking_interval
     */
    void _initKalmanFilter();

    /**
     * @brief _freshActionCount keeps the count of control signals fresh.
     *
     * @see #CommandInputter::_action_frame_count
     * @see #CommandInputter::_action_sequence
     * @see #CommandInputter::_update_action_count_timer
     */
    void _freshActionCount();

private:
    inline void _mouseMove(const int &cursor_x, const int &cursor_y);
    inline void _mouseDrag(const int &cursor_x, const int &cursor_y);
    inline void _mouseLeftClick(const int &cursor_x, const int &cursor_y);
    inline void _mouseRightClick(const int &cursor_x, const int &cursor_y);
    inline void _mouseDoubleClick(const int &cursor_x, const int &cursor_y);
};

#endif // COMMANDINPUTTER_H
