#ifndef MAINVIEW_H
#define MAINVIEW_H
/**
 * @file
 * @author Pei Xu, xupei0610 at gmail.com
 * @brief The MainView.h file contains the class that provides the GUI of the main window.
 */
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QCloseEvent>

#include "Singleton.h"
#include "SettingView.h"
#include "TrackingView.h"

/**
 * @brief The MainView class provides the GUI of the main window.
 */
class MainView final: public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief getInstance returns the singleton instance of this class.
     * @return the singleton instance of #MainView class
     *
     * @see #Singleton
     */
    static MainView *getInstance();
public slots:
    /**
     * @brief setStatusBar sets the status bar using the given message.
     * @param msg : the message that will be displayed on the status bar
     */
    void setStatusBar(const QString &msg);
    /**
     * @brief changeWorkStatusToNothing changes the current work status to idle.
     *
     * @see #MainView::changeWorkStatusToControlling
     * @see #MainView::changeWorkStatusToSampling
     */
    void changeWorkStatusToNothing();
    /**
     * @brief changeWorkStatusToControlling changes the current work status to controlling.
     *
     * @see #MainView::changeWorkStatusToNothing
     * @see #MainView::changeWorkStatusToSampling
     */
    void changeWorkStatusToControlling();
    /**
     * @brief changeWorkStatusToSampling changes the current work status to sampling.
     *
     * @see #MainView::changeWorkStatusToNothing
     * @see #MainView::changeWorkStatusToControlling
     */
    void changeWorkStatusToSampling();
signals:
    /**
     * @brief mainViewClosing is the signal that indicates the main GUI is closing.
     */
    void mainViewClosing();
    /**
     * @brief controllingTaskStartRequest is the signal to request for controlling task.
     */
    void controllingWindowRequest();
    /**
     * @brief samplingTaskStartRequest is the signal to request for sampling task.
     */
    void samplingWindowRequest();
    /**
     * @brief settingWindowRequest is the signal to request the setting window.
     */
    void settingWindowRequest();

private:
    explicit MainView(QWidget *parent = 0);
    MainView(const MainView &) = delete;
    MainView &operator=(const MainView &) = delete;
    static MainView *createInstance();

    inline void _setupUi();
    inline void _makeConnections();

    void closeEvent(QCloseEvent *e) override;

    QPushButton *_ui_btn_controlling;
    QPushButton *_ui_btn_sampling;
    QPushButton *_ui_btn_setting;
    QLabel *_ui_status_bar;


};

#endif // MAINVIEW_H
