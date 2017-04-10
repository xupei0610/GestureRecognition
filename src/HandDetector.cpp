#include "HandDetector.h"

const cv::Scalar HandDetector::COLOR_BLACK(cv::Scalar(0,0,0,255));
const cv::Scalar HandDetector::COLOR_WHITE(cv::Scalar(255,255,255,255));
const cv::Scalar HandDetector::COLOR_GRAY(cv::Scalar(127, 127, 127, 255));
const cv::Scalar HandDetector::COLOR_RED(cv::Scalar(0,0,255,255));
const cv::Scalar HandDetector::COLOR_GREEN(cv::Scalar(0,255,0,255));
const cv::Scalar HandDetector::COLOR_BLUE(cv::Scalar(255,0,0,255));

HandDetector::HandDetector(QObject *parent) :
    QObject(parent),
    interesting_img(_interesting_img),
    filtered_img(_filtered_img),
    convexity_img(_convexity_img),
    extracted_img(_extracted_img),
    background_img(_background_img),
    skin_color_lower_bound(_skin_color_lower_bound),
    skin_color_upper_bound(_skin_color_upper_bound),
    morphology(_morphology),
    detection_area(_detection_area),
    tracked_point(_tracked_point),
    fingers(_fingers),
    hand_center(_hand_center),
    _bg_subtractor(cv::createBackgroundSubtractorMOG2(1, 16, false)),
    _has_set_bg(false),
    _skin_color_lower_bound(cv::Scalar(DEFAULT_SKIN_COLOR_MIN_H, DEFAULT_SKIN_COLOR_MIN_S, DEFAULT_SKIN_COLOR_MIN_V)),
    _skin_color_upper_bound(cv::Scalar(DEFAULT_SKIN_COLOR_MAX_H, DEFAULT_SKIN_COLOR_MAX_S, DEFAULT_SKIN_COLOR_MAX_V)),
    _gaussian_size(cv::Size(7,7)),
    _gaussian_variance(0.8),
    _morphology(DEFAULT_SKIN_MORPHOLOGY),
    _morphology_kernel(cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(9, 9))),
    _detection_area(DEFAULT_SKIN_DETECTION_AREA)
{}

bool HandDetector::detect(const cv::Mat &input_img)
{
    input_img.copyTo(_interesting_img);
    _imagePreprocessing();
    return _fingerExtraction();
}


void HandDetector::setMorphology(const bool &perform_morphology)
{
    _morphology = perform_morphology;
}

void HandDetector::setSkinColorFilterLowerBound(const int &H, const int &S, const int &V)
{
    _skin_color_lower_bound = cv::Scalar(H, S, V);
}

void HandDetector::setSkinColorFilterUpperBound(const int &H, const int &S, const int &V)
{
    _skin_color_upper_bound = cv::Scalar(H, S, V);
}

void HandDetector::setDetectionArea(const int &area)
{
    _detection_area = area;
}

void HandDetector::setBackgroundImage()
{
    if (_interesting_img.empty())
        clearBackgroundImage();
    return;
    _bg_subtractor->apply(_interesting_img, _bg, 1);
    _has_set_bg = true;
    _interesting_img.copyTo(_background_img);
    emit backgroundImageSet();
}

void HandDetector::clearBackgroundImage()
{
    _has_set_bg = false;
    emit backgroundImageCleared();
}

void HandDetector::_imagePreprocessing()
{
    // background subtractor
    if (_has_set_bg == true)
    {
        _bg_subtractor->apply(_interesting_img, _bg, 0);
        _interesting_img.copyTo(_filtered_img, _bg);
    }
    else
        _interesting_img.copyTo(_filtered_img);

    // skin color filter
    cv::cvtColor(_filtered_img, _filtered_img, cv::COLOR_RGB2HSV);
    cv::inRange(_filtered_img, _skin_color_lower_bound, _skin_color_upper_bound, _filtered_img);
    // smooth
    cv::GaussianBlur(_filtered_img, _filtered_img, _gaussian_size, _gaussian_variance);
    // thresholding
    cv::threshold(_filtered_img, _filtered_img, 10, 255, cv::THRESH_BINARY);
    // morphological transformation
    if (_morphology)
    {
        cv::morphologyEx(_filtered_img, _filtered_img, cv::MORPH_OPEN, _morphology_kernel);
        cv::morphologyEx(_filtered_img, filtered_img, cv::MORPH_CLOSE, _morphology_kernel);
    }
}

bool HandDetector::_fingerExtraction()
{
    std::vector<std::vector<cv::Point> > contours;
    double area, largest_area = 0, thresh = 0.9*_filtered_img.rows*_filtered_img.cols;
    _convexity_img = cv::Mat(_filtered_img.rows, _filtered_img.cols, CV_8UC3);
    _convexity_img.setTo(HandDetector::COLOR_WHITE);
    _extracted_img.release();
    _fingers.clear();
    _tracked_point.x = -1;
    _tracked_point.y = -1;

    // contour extraction
    cv::findContours(_filtered_img, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    int indx = -1;
    for (int i = static_cast<int>(contours.size()); --i > -1;)
    {
        area = cv::contourArea(contours[i]);
        if (area > _detection_area && area > largest_area)
        {
            largest_area = area;
            indx = i;
        }
    }
    // fail if no contour extraction
    if (indx == -1 || largest_area > thresh)
        return false;

    std::vector<cv::Point> contour;
    std::vector<std::vector<int> > hulls(1);
    std::vector<cv::Vec4i> defects;
    double dist1, dist2, angle;
    bool flag1, flag2;

    cv::Rect hand_bound = cv::boundingRect(contours[indx]);
    _filtered_img(hand_bound).copyTo(_extracted_img);

    // approximate contour region using polygon
    cv::approxPolyDP(contours[indx], contour, 10.0, true);
    // extract convexity defects
    cv::convexHull(contour, hulls[0], false);
    cv::convexityDefects(contour, hulls[0], defects);
    // estimate finger tops

    for (const auto &d : defects)
    {
        // discard those whose start and end points are too far or too close
        dist1 = _squaredEuclidDist<cv::Point, cv::Point>(contour[d[0]], contour[d[2]]);
        if (dist1 < 100 || dist1 > 30000)
            continue;
        dist2 = _squaredEuclidDist<cv::Point, cv::Point>(contour[d[1]], contour[d[2]]);
        if (dist2 < 100 || dist2 > 30000)
            continue;
        // discard those from which the angle formed is twoo small or big
        angle = std::acos(
                    double(
                        (contour[d[0]].x - contour[d[2]].x)*(contour[d[1]].x - contour[d[2]].x)
                +(contour[d[0]].y - contour[d[2]].y)*(contour[d[1]].y - contour[d[2]].y)
                ) / std::sqrt(dist1 * dist2)
                );
        if (angle < 0.2618 || angle > 2.3562) // 15 degree, 135 degree
            continue;
        // keep those start or end points who are not too far from those who have been kept
        flag1 = true; flag2 = true;
        for (const auto &p : _fingers)
        {
            if (flag1 && _squaredEuclidDist<cv::Point, cv::Point>(contour[d[0]], p) < 900)
                flag1 = false;
            if (flag2)
            {
                if (_squaredEuclidDist(contour[d[1]], p) < 900)
                    flag2 = false;

            }
            else if (flag1 == false)
                break;
        }
        if (flag1)
        {
            if (flag2)
            {
                if (_squaredEuclidDist<cv::Point, cv::Point>(contour[d[1]], contour[d[0]]) < 1000)
                {
                    if (contour[d[0]].y < contour[d[1]].y)
                        _fingers.push_back(contour[d[0]]);
                    else
                        _fingers.push_back(contour[d[1]]);
                }
                else
                {
                    _fingers.push_back(contour[d[0]]);
                    _fingers.push_back(contour[d[1]]);
                }
            }
            else
                _fingers.push_back(contour[d[0]]);
        }
        else if (flag2)
            _fingers.push_back(contour[d[1]]);
    }

    // estimate hand center via gravity
    auto mom = cv::moments(contour, true);
    _hand_center.x = mom.m10/mom.m00;
    _hand_center.y = mom.m01/mom.m00;

    // we always use the top most point as the track point
    //    if (_fingers.empty())
    _tracked_point = *std::min_element(
                contour.begin(), contour.end(),
                [](const cv::Point &lhs, const cv::Point &rhs) -> bool
    {
        return lhs.y < rhs.y;
    });
    //    else
    //        _tracked_point = *std::min_element(
    //                    _fingers.begin(), _fingers.end(),
    //                    [](const Point &lhs, const Point &rhs) -> bool
    //        {
    //            return lhs.y < rhs.y;
    //        });

    cv::drawContours(_convexity_img, contours, indx, HandDetector::COLOR_GRAY, -1);
    for (const auto & p : _fingers)
    {
        cv::circle(_convexity_img, p, 10, HandDetector::COLOR_RED, 3);
        cv::line(_convexity_img, p, _hand_center, HandDetector::COLOR_BLUE, 3);
    }
    cv::rectangle(_convexity_img, hand_bound, HandDetector::COLOR_GREEN, 2);
    cv::circle(_convexity_img, _hand_center, 10, HandDetector::COLOR_RED, -1);

    return true;
}

template <typename T1, typename T2>
double HandDetector::_squaredEuclidDist(const T1 &p1, const T2 &p2) const
{
    return (p1.x - p2.x)*(p1.x-p2.x) + (p1.y - p2.y)*(p1.y-p2.y);
}

