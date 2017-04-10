#include "GestureAnalyst.h"

GestureAnalyst::GestureAnalyst()
{
    caffe::Caffe::set_mode(caffe::Caffe::CAFFE_WORK_MODE);
}

int GestureAnalyst::load(const QString &model_file, const QString &network_structure)
{
    std::string network_file("/Users/XP//Documents/Src/HandRecognitionML/data/sample64/lenet.prototxt");
    _net.reset(new caffe::Net<float>(network_file, caffe::TEST));



    //    caffe::NetParameter param;

    //    caffe::ReadProtoFromTextFile(param_file, &param);

    //    int fd = open(filename, O_RDONLY);
    //      CHECK_NE(fd, -1) << "File not found: " << filename;
    //      FileInputStream* input = new FileInputStream(fd);
    //      bool success = google::protobuf::TextFormat::Parse(input, proto);
    //      delete input;
    //      close(fd);

    //    param.mutable_state()->set_phase(caffe::TEST);
    //    param.mutable_state()->set_level(0);




    _net->CopyTrainedLayersFrom(model_file.toStdString());
    if (_net->num_inputs() != 1)
        //        return ERROR_INPUT_LAYER_NUM;
        return -1;
    if (_net->num_outputs() != 1)
        //        return ERROR_OUTPUT_LAYER_NUM;
        return -2;
    caffe::Blob<float> * input_layer = _net->input_blobs()[0];
    if (input_layer->channels() != _num_of_channels)
        //        return ERROR_IMAGE_CHANNEL;
        return -3;

    _input_geometry = cv::Size(input_layer->width(), input_layer->height());
    if (_input_geometry.width != SAMPLE_SIZE_WIDTH || _input_geometry.height != SAMPLE_SIZE_HEIGHT)
        //        return ERROR_IMAGE_SIZE;
        return -4;

    return _net->output_blobs()[0]->channels();
}

std::vector<GestureAnalyst::Prediction> GestureAnalyst::analyze(const cv::Mat &img, const int &get_N)
{
    // Refresh input layer
    _net->input_blobs()[0]->Reshape(1, _num_of_channels, _input_geometry.height, _input_geometry.width);
    _net->Reshape();
    // Map each input at the input layer to a cv::Mat variable, input_channels,
    // so that we can give input by directly modifying the Mat variable
    std::vector<cv::Mat> input_channels;
    caffe::Blob<float> * input_layer = _net->input_blobs()[0];
    float * data = input_layer->mutable_cpu_data();
    for (int i = 0; i < input_layer->channels(); ++i)
    {
        cv::Mat channel(input_layer->height(), input_layer->width(), CV_32FC1, data);
        input_channels.push_back(channel);
        data += input_layer->height() * input_layer->width();
    }

    // convert input image into suitable format so that it is consistent with the input layer
    // 1 channel graysacle or 3 channels BGR
    cv::Mat tar;
    if (img.channels() == _num_of_channels)    // ==  1 or 3
        img.copyTo(tar);
    else
    {
        if (img.channels() == 3 && _num_of_channels == 1)
            cv::cvtColor(img, tar, cv::COLOR_BGR2GRAY);
        else if (img.channels() == 1 && _num_of_channels == 3)
            cv::cvtColor(img, tar, cv::COLOR_GRAY2BGR);
        else //if (img.channels() == 4)
        {
            if (_num_of_channels == 1)
                cv::cvtColor(img, tar, cv::COLOR_BGRA2GRAY);
            else
                cv::cvtColor(img, tar, cv::COLOR_BGRA2BGR);
        }
    }
    // resize should be done outsie the function
//    // resize input image to keep it consistent with the input layer
//    if (tar.size() != _input_geometry)
//        tar = SampleCollector::resizeSample(tar);

    if (_num_of_channels == 3)
        tar.convertTo(tar, CV_32FC3);
    else
        tar.convertTo(tar, CV_32FC1);

    // No mean image used now for this application
    //     // subtract the mean image from the input image
    //    cv::subtract(tar, _mean, tar);

    // write data to the input channels mapped to the input layer already
    cv::split(tar, input_channels);

    // run the network
    float loss;
    _net->Forward(&loss);

    // pick up the output results
    caffe::Blob<float> * output_layer = _net->output_blobs()[0];
    std::vector<Prediction> prob;
    for (int i = output_layer->channels(); --i > -1;)
        prob.push_back(Prediction(i, *(output_layer->cpu_data()+i)));

    // get the best N results
    std::partial_sort(prob.begin(), prob.begin()+(get_N<prob.size()?get_N:prob.size()), prob.end(),
                      [](const Prediction &lhs,
                      const Prediction &rhs) -> bool
    {
        return lhs.prob > rhs.prob;
    });
    std::vector<Prediction> max_N;
    for (int i = 0; i < get_N; ++i)
        max_N.push_back(prob[i]);

    return max_N;
}
