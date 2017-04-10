#include "SampleCollector.h"

const cv::Size SampleCollector::sample_image_size(SAMPLE_SIZE_WIDTH,SAMPLE_SIZE_HEIGHT);
const char *SampleCollector::image_format = SAMPLING_IMAGE_FORMAT;

SampleCollector::SampleCollector(QObject *parent) :
    QObject(parent),
    storage_path(_storage_path),
    _sampling_timer(new QTimer),
    _settings(Settings::getInstance()),
    _storage_dir(nullptr)
{
    _sampling_timer->setSingleShot(true);
}

SampleCollector::~SampleCollector()
{
    if (_storage_dir != nullptr)
        delete _storage_dir;
}

cv::Mat SampleCollector::resizeSample(const cv::Mat &sample)
{
    if (sample.empty() || (sample.rows == sample_image_size.height && sample.cols == sample_image_size.width))
        return cv::Mat(sample);

    cv::Mat result(sample_image_size, CV_8UC1);
    float scalex = (float)sample_image_size.width/sample.cols;
    float scaley = (float)sample_image_size.height/sample.rows;
    if (scalex == scaley)
    {
        cv::resize(sample, result, sample_image_size, 0, 0, cv::INTER_LINEAR);
        return result;
    }

    result.setTo(cv::Scalar(0));
    int x = 0, y = 0;
    cv::Size size;
    if (scalex < scaley)
    {
        size.width = sample_image_size.width;
        size.height = std::ceil(sample.rows*scalex);
        y = (sample_image_size.height-size.height)/2;
    }
    else
    {
        size.height = sample_image_size.height;
        size.width  = std::ceil(sample.cols*scaley);
        x = (sample_image_size.width-size.width)/2;
    }
    cv::Mat tmp;
    cv::resize(sample, tmp, size);
    tmp.copyTo(result(cv::Rect(x,y,size.width,size.height)));

    return result;
}

bool SampleCollector::setStoragePath(const QString &sample_folder, const QString &label_name)
{
    auto dir = QDir(sample_folder);
    if (_storage_dir == nullptr)
        _storage_dir = new QDir(dir.filePath(label_name));
    else
        _storage_dir->setPath(dir.filePath(label_name));
    _storage_path = _storage_dir->absolutePath();
    if (dir.isRoot())
        return false;
    if (dir.exists() && (dir.exists(label_name) || dir.mkdir(label_name)))
        return true;
    return false;
}

bool SampleCollector::sample(const cv::Mat &image)
{
    if (image.empty())
        return false;
    QPixmap img = ImgConvertor::cvMat2QPixmap(resizeSample(image));
    QString file_name = QString::number(qrand());
    while (true)
    {
        if (_storage_dir->exists(file_name))
            file_name = QString::number(qrand());
        else
        {
            if (img.save(_storage_dir->filePath(file_name), image_format))
            {
                _sampling_timer->start(_settings->sampling_interval);
                return true;
            }
            return false;
        }
    }
    return true;
}

bool SampleCollector::deny()
{
    return _sampling_timer->isActive();
}
