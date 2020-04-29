#include <iostream>
#include <opencv2/dnn.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core.hpp>
#include <fstream>
#include "yolo.h"
#include <vector>

using namespace cv;
using namespace dnn;
using namespace std;

namespace yolov3 {

yolov3Segment::yolov3Segment() {
    // Load names of classes
    string classesFile = "src/yolo/coco.names";
    ifstream ifs(classesFile.c_str());
    string line;
    while (getline(ifs, line)) classes.push_back(line);
    
    // Give the configuration and weight files for the model
    String modelConfiguration = "src/yolo/yolov3.cfg";
    String modelWeights = "src/yolo/yolov3.weights";

    // Load the network
    net = readNetFromDarknet(modelConfiguration, modelWeights);
    net.setPreferableBackend(DNN_BACKEND_OPENCV);
    net.setPreferableTarget(DNN_TARGET_CPU);
}


cv::Mat yolov3Segment::Segmentation(cv::Mat &image) {
    cv::Mat blob;
    // Create a 4D blob from a frame.
    blobFromImage(image, blob, 1/255.0, cvSize(this->inpWidth, this->inpHeight), Scalar(0,0,0), true, false);
    //Sets the input to the network
    this->net.setInput(blob);
    // Runs the forward pass to get output of the output layers
    vector<Mat> outs;
    this->net.forward(outs, this->getOutputsNames(this->net));

    int dilation_size = 15;
    cv::Mat kernel = getStructuringElement(cv::MORPH_ELLIPSE,
                                        cv::Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                        cv::Point( dilation_size, dilation_size ) );
    cv::Mat mask = cv::Mat::ones(480,640,CV_8U);
    cv::Mat maskyolo = this->postprocess(image, outs);
    cv::Mat maskyolodil = maskyolo.clone();
    cv::dilate(maskyolo, maskyolodil, kernel);
    mask = mask - maskyolodil;
    return mask;
}


// Remove the bounding boxes with low confidence using non-maxima suppression
cv::Mat yolov3Segment::postprocess(Mat& frame, const vector<Mat>& outs)
{
    vector<int> classIds;
    vector<float> confidences;
    vector<Rect> boxes;
    
    for (size_t i = 0; i < outs.size(); ++i)
    {
        // Scan through all the bounding boxes output from the network and keep only the
        // ones with high confidence scores. Assign the box's class label as the class
        // with the highest score for the box.
        float* data = (float*)outs[i].data;
        for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
        {
            Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
            Point classIdPoint;
            double confidence;
            // Get the value and location of the maximum score
            minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
            if (confidence > this->confThreshold)
            {
                int centerX = (int)(data[0] * frame.cols);
                int centerY = (int)(data[1] * frame.rows);
                int width = (int)(data[2] * frame.cols);
                int height = (int)(data[3] * frame.rows);
                int left = centerX - width / 2;
                int top = centerY - height / 2;
                
                classIds.push_back(classIdPoint.x);
                confidences.push_back((float)confidence);
                boxes.push_back(Rect(left, top, width, height));
            }
        }
    }
    
    cv::Mat mask = cv::Mat::zeros(480,640,CV_8U);
    // Perform non maximum suppression to eliminate redundant overlapping boxes with
    // lower confidences
    vector<int> indices;
    NMSBoxes(boxes, confidences, this->confThreshold, this->nmsThreshold, indices);
    for (size_t i = 0; i < indices.size(); ++i)
    {
        int idx = indices[i];
        Rect box = boxes[idx];
        //drawPred(classIds[idx], confidences[idx], box.x, box.y,
        //         box.x + box.width, box.y + box.height, frame);
        string c = this->classes[classIds[idx]];
        if (c == "person" || c == "car" || c == "bicycle" || c == "motorcycle" || c == "bus" || c == "truck") {
            for (int x = max(0, box.x + box.width / 4); x < box.x + 3*box.width/4 && x < 640; ++x)
                for (int y = max(0, box.y); y < box.y + box.height && y < 480; ++y)
                    mask.at<uchar>(y, x) = 1;
        }
    }

    return mask;
}

// Get the names of the output layers
vector<String> yolov3Segment::getOutputsNames(const Net& net)
{
    static vector<String> names;
    if (names.empty())
    {
        //Get the indices of the output layers, i.e. the layers with unconnected outputs
        vector<int> outLayers = this->net.getUnconnectedOutLayers();
        
        //get the names of all the layers in the network
        vector<String> layersNames = this->net.getLayerNames();
        
        // Get the names of the output layers in names
        names.resize(outLayers.size());
        for (size_t i = 0; i < outLayers.size(); ++i)
        names[i] = layersNames[outLayers[i] - 1];
    }
    return names;
}

}