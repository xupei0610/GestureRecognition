#ifndef SAMPLECOLLECTOR_H
#define SAMPLECOLLECTOR_H
/**
 * @file
 * @author Pei Xu, xupei0610 at gmail.com
 * @brief The SampleCollector.h file contains the class of conducting sampling and processing sample image.
 */
#include <QObject>
#include <QDir>
#include <QString>
#include <QTimer>

#include <opencv2/opencv.hpp>

#include "global.h"
#include "ImgConvertor.h"
#include "Settings.h"

/**
 * @brief The SampleCollector class is the controller of sampling who also provides some static methods to process sample image.
 *
 * **ATTENTION**:
 *  This function is not thread-safe.
 *
 * @see #SampleCollector::sample
 */
class SampleCollector : public QObject
{
    Q_OBJECT
public:
//    /**
//     * @brief Size is a simple structure of size
//     */
//    struct Size
//    {
//        /**
//         * @brief w is the property of width
//         *
//         * The default value without intialization is -1.
//         */
//        int w;
//        /**
//         * @brief h is the property of height
//         *
//         * The default value without intialization is -1.
//         */
//        int h;
//        Size() : w(-1), h(-1) {}
//        Size(const int &w, const int &h) : w(w), h(h) {}
//        /**
//         * @brief operator = is a copy assignment from cv::Size
//         * @param s : a cv::Size object
//         */
//        void operator=(const cv::Size &s)
//        {
//            w = s.width;
//            h = s.height;
//        }
//        /**
//         * @brief operator = is a copy assignment from another Size object.
//         * @param p : another Size object
//         */
//        void operator=(const Size &s)
//        {
//            w = s.width;
//            h = s.height;
//        }
//    };

    /**
     * @brief storage_path is the path to store the following sample images.
     *
     * @see #SampleCollector::setStoragePath
     */
    const QString &storage_path;
    /**
     * @brief image_format is the format using which to store the sample image.
     */
    const static char *image_format;

    /**
     * @brief sample_size is the size of a sample image
     *
     * @see #SampleCollector::Size
     */
    const static cv::Size sample_image_size;

    explicit SampleCollector(QObject *parent = 0);
    ~SampleCollector();

    /**
     * @brief resizeSample resizes the given sample image with certain size.
     *
     * @param sample : the sample image
     * @return the name image after resizing.
     *
     * @see #SampleCollector::sample
     */
    virtual cv::Mat resizeSample(const cv::Mat &sample);

    /**
     * @brief setStoragePath sets the path to store the next sample images.
     *
     * The sample images will be stored at `sample_folder/label_name/`.
     *
     * If no directory named `label_name` exists in `sample_folder`, a new directory will be made.
     *
     * **ATTENTION**:
     *  This function is not thread-safe. And, usually, we do not hope to reset the storage path during sampling.
     *
     * @param sample_folder : path of folder to contain samples
     * @param label_name : label to which the following sample images belong
     * @retval true : successfully set the storage path
     * @retval false : failed. This may appear if
     *  - `sample_folder` does not exist or is not a directory
     *  - `label_name` has beened used as a file at the path `sample_folder`
     *  - failed to make directory with name `label_name` at the path `sample_folder`
     *
     * @see #SampleCollector::storage_path
     * @see #SampleCollector::resizeSample
     * @see #SampleCollector::sample
     */
    virtual bool setStoragePath(const QString &sample_folder, const QString &label_name);

    /**
     * @brief sample does a sampling at #SampleCollector::storage_path using the given image.
     *
     * **Attention**:
     *
     *  - use #SampleCollector::setStoragePath firstly before a batch of sampling
     *  - this function would call #SampleCollector::resizeSample automatically so that there is no need to do resizing explicitly.
     *
     * @param image : the sample image
     * @retval true : successfully stored an sample image
     * @retval false : something fatal happened. This usually is caused by empty image, invaild storage path or the lack of permission to write file at the storage path.
     *
     * @see #SampleCollector::setStoragePath
     * @see #SampleCollector::resizeSample
     */
    virtual bool sample(const cv::Mat &image);

    /**
     * @brief deny indicates that if the collector temporarility accepts sample or not.
     * @retval true : if the collector has preprared to accepts a new sample.
     * @retval false : otherwise
     */
    virtual bool deny();

protected:
    /**
     * @brief _sampling_timer is the timer of sampling interval
     */
    QTimer *_sampling_timer;
    /**
     * @brief _settings is an instance of #Settings .
     */
    Settings *_settings;

private:
    QString _storage_path;
    QDir *_storage_dir;

};

#endif // SAMPLECOLLECTOR_H
