#ifndef COMMANDINPUTTERINTERFACE_H
#define COMMANDINPUTTERINTERFACE_H
/**
 * @file
 * @author Pei Xu, xupei0610 at gmail.com
 * @brief The CommandInputterInterface.h file contains the interface of the inputter class who makes commands to the computer based on the recognized gesture.
 */
#include <QObject>

/**
 * @brief The CommandInputterInterface class provides an interface of the inputter class who makes commands to the computer based on the recognized gesture.
 */
class CommandInputterInterface : public QObject
{
    Q_OBJECT
public:

    /**
     * @brief load loads the specific keymap file.
     * @param keymap_file : the specific keymap file
     * @retval true : if successfully load the file
     * @retval false : if unable to open the file or failed to parse the file
     */
    virtual bool load(const QString &keymap_file) = 0;
    /**
     * @brief input inputs a command to computer according to the label index.
     *
     * The function searches the command, corresponding to the given `label_index`, from keymap file loaded by #CommandInputter::load and then make a command to the computer.
     * It should emit a signal, #CommandInputterInterface::commandMade, to inform what command it made.
     *
     * The given position is a fractional number in range [0,1].
     *
     * @param label_index : the index of the label with which a command is associated
     * @param tracked_pos_x : the horizontal position of the traked point which is used for control the cursor
     * @param tracked_pos_y : the vertical position of the traked point which is used for control the cursor
     *
     */
    virtual void input(const int &label_index,
                       const float &tracked_pos_x,
                       const float &tracked_pos_y) = 0;

    /**
     * @brief labels returns the list of labels (gestures).
     * @return the list of labels (gestures)
     */
    virtual const QStringList labels() = 0;

signals:
    /**
     * @brief commandMade is the signal to indicate what command the inputter just made.
     * @param cmd : the name of the command just made
     */
    void commandMade(const QString& cmd);

};
Q_DECLARE_INTERFACE(CommandInputterInterface,"PeiXu.CommandInputterInterface/1.0")
#endif // COMMANDINPUTTERINTERFACE_H
