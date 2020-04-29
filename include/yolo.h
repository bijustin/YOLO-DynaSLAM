#include <iostream>
#include <opencv2/dnn.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <opencv2/imgcodecs.hpp>
#include <fstream>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
using namespace cv;
using namespace dnn;

namespace yolov3 {
    class yolov3Segment {
        private:
            float confThreshold = 0.5; // Confidence threshold
            float nmsThreshold = 0.4;  // Non-maximum suppression threshold
            int inpWidth = 640;  // Width of network's input image
            int inpHeight = 480; // Height of network's input image
            vector<string> classes;
            Net net;

        public:
            yolov3Segment();
            ~yolov3Segment();
            cv::Mat Segmentation(cv::Mat &image);

            // Remove the bounding boxes with low confidence using non-maxima suppression
            cv::Mat postprocess(Mat& frame, const vector<Mat>& out);
            // Get the names of the output layers
            vector<String> getOutputsNames(const Net& net);

    };
}