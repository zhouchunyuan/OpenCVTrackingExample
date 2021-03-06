//https://www.learnopencv.com/object-tracking-using-opencv-cpp-python/
#include "opencv2/opencv.hpp"
#include "opencv2/tracking.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
//#include "opencv2/ocl.hpp"

using namespace cv;
using namespace std;

// Convert to string
//#define SSTR( x ) static_cast< std::ostringstream & >( ( std::ostringstream() << std::dec << x ) ).str()

int main(int argc, char** argv)
{
    // List of tracker types in OpenCV 3.4.1
    string trackerTypes[8] = { "BOOSTING", "MIL", "KCF", "TLD","MEDIANFLOW", "GOTURN", "MOSSE", "CSRT" };
    // vector <string> trackerTypes(types, std::end(types));

    // Create a tracker
    string trackerType = trackerTypes[7];

    Ptr<Tracker> tracker;

#if (CV_MINOR_VERSION < 3)
    {
        tracker = Tracker::create(trackerType);
    }
#else
    {
        if (trackerType == "BOOSTING")
            tracker = TrackerBoosting::create();
        if (trackerType == "MIL")
            tracker = TrackerMIL::create();
        if (trackerType == "KCF")
            tracker = TrackerKCF::create();
        if (trackerType == "TLD")
            tracker = TrackerTLD::create();
        if (trackerType == "MEDIANFLOW")
            tracker = TrackerMedianFlow::create();
        if (trackerType == "GOTURN")
            tracker = TrackerGOTURN::create();
        if (trackerType == "MOSSE")
            tracker = TrackerMOSSE::create();
        if (trackerType == "CSRT")
            tracker = TrackerCSRT::create();
    }
#endif
    // Read video
    VideoCapture video(1);

    // Exit if video is not opened
    if (!video.isOpened())
    {
        cout << "Could not read video file" << endl;
        return 1;
    }

    // Read first frame 
    Mat frame;
    Mat outImg;//for display
    bool ok = video.read(frame);
    cv::resize(frame, outImg, cv::Size(), 0.3, 0.3);
    // Define initial bounding box 
    Rect2d bbox(287, 23, 86, 320);



    //string wndName = "Tracking";
    //namedWindow(wndName, cv::WINDOW_AUTOSIZE);
    //imshow(wndName, frame);
    // Uncomment the line below to select a different bounding box 
    bbox = selectROI("selectROI",outImg, false);
    cv::destroyWindow("selectROI");
    // Display bounding box. 
    rectangle(outImg, bbox, Scalar(255, 0, 0), 2, 1);
    tracker->init(outImg, bbox);

    while (video.read(frame))
    {
        cv::resize(frame, outImg, cv::Size(), 0.3, 0.3);
        // Start timer
        double timer = (double)getTickCount();

        // Update the tracking result
        bool ok = tracker->update(outImg, bbox);

        // Calculate Frames per second (FPS)
        float fps = getTickFrequency() / ((double)getTickCount() - timer);

        if (ok)
        {
            // Tracking success : Draw the tracked object
            rectangle(outImg, bbox, Scalar(255, 0, 0), 2, 1);
        }
        else
        {
            // Tracking failure detected.
            putText(outImg, "Tracking failure detected", Point(100, 80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 255), 2);
        }

        // Display tracker type on frame
        putText(outImg, trackerType + " Tracker", Point(100, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);

        // Display FPS on frame
        putText(outImg, "FPS : " + to_string(int(fps)), Point(100, 50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);

        // Display frame.
        
        
        imshow("Tracking", outImg);

        // Exit if ESC pressed.
        int k = waitKey(1);
        if (k == 27)
        {
            break;
        }

    }
}