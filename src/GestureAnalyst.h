#ifndef GESTUREANALYST_H
#define GESTUREANALYST_H

/**
 * @file
 * @author Pei Xu, xupei0610 at gmail.com
 * @brief The GestureAnalyst.h file contains an implementation of the gesture analyst class who performs classification on the given image based on CNN and MNIST network structure.
 */
#include "GestureAnalystInterface.h"
#include "global.h"

#include <QObject>
#include <QDebug>
#include <caffe/caffe.hpp>

#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/text_format.h>
#include <QFile>
#include <QTemporaryFile>
#include <QDebug>

/**
 * @brief The GestureAnalyst class is an implementation of the gesture analyst based on CNN and MNIST network structure.
 */
class GestureAnalyst : public GestureAnalystInterface
{
    Q_INTERFACES(GestureAnalystInterface)
public:
    explicit GestureAnalyst();

    /**
     * @brief load loads model file.
     * @param model_file : the path of the model file
     * @param network_structure : the structure of the CNN defined by string
     * @return the number of labels the classifier defiend
     */
    int load(const QString &model_file, const QString &network_structure);
    /**
     * @brief analyze recognizes gestures from the given image.
     * @param img : a sample image containing hand/gesture
     * @param get_N : the number of prediction results that will be returned
     * @return N best prediction results where N is defined by the argument `get_N`
     */
    std::vector<Prediction> analyze(const cv::Mat &img, const int &get_N);

protected:
    /**
     * @brief _net is the instance of the current convolution neural network.
     */
    caffe::shared_ptr<caffe::Net<float> > _net;
    /**
     * @brief _input_geometry is the size of the input image defined by the network.
     */
    cv::Size _input_geometry;
    /**
     * @brief _num_of_channels is the number of channels of input image.
     */
    const int _num_of_channels = 1;

};

#endif // GESTUREANALYST_H
