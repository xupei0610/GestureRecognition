#ifndef IMGCONVERTOR_H
#define IMGCONVERTOR_H
/**
 * @file
 * @author Pei Xu, xupei0610 at gmail.com
 * @brief The ImgConvertor.h file contains the convertor between Qt image and opencv mat.
 */
#include <QImage>
#include <QPixmap>
#include <QDebug>
#include <opencv2/opencv.hpp>

/**
 * @brief The ImgConvertor class provides a convertor between Qt image and opencv mat.
 */
class ImgConvertor {
public:
    /**
     * @brief qImage2CvMat is a converter from QImage to cv::Mat
     * @param input_image : an image in the format of QImage
     * @return the image in the format of cv::Mat
     */
    static cv::Mat qImage2CvMat(const QImage &input_image);
    /**
     * @brief qPixmap2CvMat is a converter from QPixmap to cv::Mat
     * @param input_image : an image in the format of QPixmap
     * @return the image in the format of cv::Mat
     */
    static inline cv::Mat qPixmap2CvMat(const QPixmap &input_image)
    {
        return qImage2CvMat(input_image.toImage());
    }
    /**
     * @brief cvMat2QImage is a converter from cv::Mat to QImage
     * @param input_image : an image in the format of cv::Mat
     * @return the image in the format of QImage
     */
    static QImage cvMat2QImage(const cv::Mat & input_mat);
    /**
     * @brief cvMat2QPixmap is a converter from QPixmap to cv::Mat
     * @param input_image : an image in the format of cv::Mat
     * @return the image in the format of QPixmap
     */
    static inline QPixmap cvMat2QPixmap(const cv::Mat & input_mat)
    {
        return QPixmap::fromImage(cvMat2QImage(input_mat));
    }
};

#endif // IMGCONVERTOR_H
