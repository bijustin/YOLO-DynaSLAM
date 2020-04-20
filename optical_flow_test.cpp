#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <opencv2/imgcodecs.hpp>
#include <fstream>
#include <chrono>
#include <thread>
#include <unistd.h>

using namespace cv;
using namespace std;

void LoadImages(const string &strFile, vector<string> &vstrImageFilenames,
                vector<double> &vTimestamps);

int main(int argc, char **argv)
{
    /*
    const string about =
        "This sample demonstrates Lucas-Kanade Optical Flow calculation.\n"
        "The example file can be downloaded from:\n"
        "  https://www.bogotobogo.com/python/OpenCV_Python/images/mean_shift_tracking/slow_traffic_small.mp4";
    const string keys =
        "{ h help  |      | print this help message }"
        "{ @image1 |      | image1 for compare }"
        "{ @image2 |      | image2 for compare }";
    CommandLineParser parser(argc, argv, keys);
    parser.about(about);
    if (parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }
    string filename1 = samples::findFile(parser.get<string>("@image1"));
    Mat im1 = imread(filename1);
    string filename2 = samples::findFile(parser.get<string>("@image2"));
    Mat im2 = imread(filename2);
    */

    // Retrieve paths to images
    vector<string> vstrImageFilenames;
    vector<double> vTimestamps;
    string strFile = string(argv[1])+"/rgb.txt";
    LoadImages(strFile, vstrImageFilenames, vTimestamps);
    int nImages = vstrImageFilenames.size();

    // Create some random colors
    vector<Scalar> colors;
    RNG rng;
    for(int i = 0; i < 100; i++)
    {
        int r = rng.uniform(0, 256);
        int g = rng.uniform(0, 256);
        int b = rng.uniform(0, 256);
        colors.push_back(Scalar(r,g,b));
    }

    cv::Mat im1, im2;
    for (int ni = 0; ni < nImages-1; ni++) {
        // Read image from file
        im1 = cv::imread(string(argv[1])+"/"+vstrImageFilenames[ni],CV_LOAD_IMAGE_UNCHANGED);
        double tframe = vTimestamps[ni];
        im2 = cv::imread(string(argv[1])+"/"+vstrImageFilenames[ni+1],CV_LOAD_IMAGE_UNCHANGED);
        
        if(im1.empty())
        {
            cerr << endl << "Failed to load image at: "
                 << string(argv[1]) << "/" << vstrImageFilenames[ni] << endl;
            return 1;
        }
        if(im2.empty())
        {
            cerr << endl << "Failed to load image at: "
                 << string(argv[1]) << "/" << vstrImageFilenames[ni+1] << endl;
            return 1;
        }
/*
#ifdef COMPILEDWITHC11
        std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
#else
        std::chrono::monotonic_clock::time_point t1 = std::chrono::monotonic_clock::now();
#endif
*/
        Mat im1_gray;
        vector<Point2f> p0, p1;
        // Take first frame and find corners in it
        cvtColor(im1, im1_gray, COLOR_BGR2GRAY);
        goodFeaturesToTrack(im1_gray, p0, 100, 0.3, 7, Mat(), 7, false, 0.04);
        // Create a mask image for drawing purposes
        Mat mask = Mat::zeros(im1.size(), im1.type());
        //while(true){
            Mat im2_gray;
            if (im2.empty())
                break;
            cvtColor(im2, im2_gray, COLOR_BGR2GRAY);
            // calculate optical flow
            vector<uchar> status;
            vector<float> err;
            TermCriteria criteria = TermCriteria((TermCriteria::COUNT) + (TermCriteria::EPS), 10, 0.03);
            calcOpticalFlowPyrLK(im1_gray, im2_gray, p0, p1, status, err, Size(15,15), 2, criteria);
            vector<Point2f> good_new;
            for(uint i = 0; i < p0.size(); i++)
            {
                // Select good points
                if(status[i] == 1) {
                    good_new.push_back(p1[i]);
                    // draw the tracks
                    line(mask,p1[i], p0[i], colors[i], 2);
                    circle(im2, p1[i], 5, colors[i], -1);
                }
            }
            Mat img;
            add(im2, mask, img);
            imshow("Frame", img);
            //imwrite("/result/"+to_string(ni)+".png", img);
            cout << ni << ", " << img.size() << endl;
            //int keyboard = waitKey(30);
            //if (keyboard == 'q' || keyboard == 27)
            //    break;
            // Now update the previous frame and previous points
            im1_gray = im2_gray.clone();
            p0 = good_new;
        //}
        //sleep(0.1);
        waitKey(100);
    }
    int keyboard = waitKey(30);
    return 0;
}

void LoadImages(const string &strFile, vector<string> &vstrImageFilenames, vector<double> &vTimestamps)
{
    ifstream f;
    f.open(strFile.c_str());

    // skip first three lines
    string s0;
    getline(f,s0);
    getline(f,s0);
    getline(f,s0);

    while(!f.eof())
    {
        string s;
        getline(f,s);
        if(!s.empty())
        {
            stringstream ss;
            ss << s;
            double t;
            string sRGB;
            ss >> t;
            vTimestamps.push_back(t);
            ss >> sRGB;
            vstrImageFilenames.push_back(sRGB);
        }
    }
}