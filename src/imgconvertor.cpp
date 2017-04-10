#include "ImgConvertor.h"

cv::Mat ImgConvertor::qImage2CvMat(const QImage &input_image)
{
    auto input_format = input_image.format();
    if (input_format == QImage::Format_ARGB32 || input_format == QImage::Format_ARGB32_Premultiplied)
    {
        return cv::Mat(input_image.height(),
                       input_image.width(),
                       CV_8UC4,
                       const_cast<uchar*>(input_image.bits()),
                       static_cast<size_t>(input_image.bytesPerLine()));
    }
    else if (input_format == QImage::Format_Indexed8)
    {
        return cv::Mat(input_image.height(),
                       input_image.width(),
                       CV_8UC4,
                       const_cast<uchar*>(input_image.bits()),
                       static_cast<size_t>(input_image.bytesPerLine()));
    }
    else if (input_format == QImage::Format_RGB32 || input_format == QImage::Format_RGB888)
    {
        QImage temp;
        if (input_format == QImage::Format_RGB888)
            temp = input_image.convertToFormat(QImage::Format_RGB32).rgbSwapped();
        else
            temp = input_image.rgbSwapped();
        return cv::Mat(temp.height(),
                       temp.width(),
                       CV_8UC3,
                       const_cast<uchar*>(temp.bits()),
                       static_cast<size_t>(temp.bytesPerLine()));
    }

    qWarning() << "ImgConvertor() -- Unsupported QImage Input Format:" << input_image.format();
    return cv::Mat();
}

QImage ImgConvertor::cvMat2QImage(const cv::Mat& input_mat)
{
    auto input_format = input_mat.type();
    if (input_format == CV_8UC4)
    {
        return QImage(input_mat.data,
                      input_mat.cols,
                      input_mat.rows,
                      static_cast<int>(input_mat.step),
                      QImage::Format_ARGB32);
    }
    else if (input_format == CV_8UC3)
    {
        return QImage(input_mat.data,
                      input_mat.cols,
                      input_mat.rows,
                      static_cast<int>(input_mat.step),
                      QImage::Format_RGB888).rgbSwapped();
    }
    else if (input_format == CV_8UC1)
    {
#if QT_VERSION >= QT_VERSION_CHECK(5,5,0)
        return QImage(input_mat.data,
                      input_mat.cols,
                      input_mat.rows,
                      static_cast<int>(input_mat.step),
                      QImage::Format_Grayscale8);
#else
        QVector<QRgb> colorTable;
        if (colorTable.isEmpty())
        {
            colorTable.resize(256);
            for (int i = 0; i < 256; ++i)
                colorTable[i] = qRgb(i,i,i);
        }
        QImage temp(input_mat.data,
                    input_mat.cols,
                    input_mat.rows,
                    static_cast<int>(input_mat.step),
                    QImage::Format_Indexed8);
        temp.setColorTable(colorTable);
        return temp;
#endif
    }
    qWarning() << "ImageConvertor() -- Unsupported Opencv Input Type:" << input_mat.type();
    return QImage();
}

