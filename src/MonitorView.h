#ifndef MONITORVIEW_H
#define MONITORVIEW_H

/**
 * @file
 * @author Pei Xu, xupei0610 at gmail.com
 * @brief The MonitorView.h file contains a singleton class that provides the GUI of the monitor window.
 */

#include <QDialog>
#include <QLabel>
#include <QGridLayout>
#include <QCloseEvent>

#include "Singleton.h"

/**
 * @brief The MonitorView class provides the GUI of the monitor window
 *
 * The monitor window will displays three images as the monitor. \n
 * The three images will be displayed on the left-top, right-top and left-bottom region of the window.\n
 * The right-bottom region is used to display text information.
 *
 * This is a singleton class. Use #Monitor::getInstance() to get the instance of this class.
 */
class MonitorView final : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief getInstance returns the singleton instance of this class.
     * @return the singleton instance of #MonitorView class
     *
     * @see #Singleton
     */
    static MonitorView *getInstance();
    /**
     * @brief updateMonitorImage3 displays three images on the monitor window.
     * @param image1 : left-top corner image
     * @param image2 : right-top corner image
     * @param image3 : left-bottom corner image
     */
    void updateMonitorImage3(const QPixmap &image1, const QPixmap &image2, const QPixmap &image3);
    /**
     * @brief updateMonitorImage2 displays only two images on the monitor window.
     *
     * The region for the 3rd image (left-bottom corner) will be cleared.
     *
     * @param image1 : left-top corner image
     * @param image2 : right-top corner image
     */
    void updateMonitorImage2(const QPixmap &image1, const QPixmap &image2);
    /**
     * @brief setmsg shows the given text on the message region.
     * @param text : text that will be shown
     */
    void setMsg(const QString &text);

private:
    explicit MonitorView(QWidget *parent = 0);
    MonitorView(const MonitorView &) = delete;
    MonitorView &operator=(const MonitorView &) = delete;
    static MonitorView *createInstance();

    void closeEvent(QCloseEvent *e) override;

    QLabel * _ui_lbl_image1;
    QLabel * _ui_lbl_image2;
    QLabel * _ui_lbl_image3;
    QLabel * _ui_lbl_text;

};

#endif // MONITORVIEW_H
