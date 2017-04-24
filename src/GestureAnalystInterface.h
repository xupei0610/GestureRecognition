#ifndef GESTUREANALYSTINTERFACE_H
#define GESTUREANALYSTINTERFACE_H
/**
 * @file
 * @author Pei Xu, xupei0610 at gmail.com
 * @brief The GestureAnalystInterface.h file contains the interface of GestureAnalyst class who analyzing hand image and recognize gestures from the image.
 */

#include <vector>
#include <QObject>
#include <opencv2/opencv.hpp>

/**
 * @brief The GestureAnalystInterface class provides an interface of the analyst class who can recognize gestures.
 */
class GestureAnalystInterface : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Prediction represents a prediction result
     */
    struct Prediction
    {
        /**
         * @brief label_id is the index of the label to which the target is assigned.
         */
        int label_id;
        /**
         * @brief prob is the probability by which the target belongs to the label.
         */
        double prob;
        Prediction(const int &label_id, const double &prob) : label_id(label_id), prob(prob) {}
    };

    /**
     * @brief load loads model file.
     * @param model_file : the path of the model file
     * @param network_structure : the structure of the CNN defined by string
     * @return the number of labels the classifier defiend
     */
    virtual int load(const QString& model_file,
                     const QString& network_structure) = 0;
    /**
     * @brief analyze recognizes gestures from the given image.
     * @param img : a sample image containing hand/gesture
     * @param get_N : the number of prediction results that will be returned
     * @return N best prediction results where N is defined by the argument `get_N`
     */
    virtual std::vector<Prediction> analyze(const cv::Mat &img, const int& get_N=1) = 0;

};
Q_DECLARE_INTERFACE(GestureAnalystInterface,"PeiXu.GestureAnalystInterface/1.0")
#endif // GESTUREANALYSTINTERFACE_H
