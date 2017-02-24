#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv){
    VideoCapture cap;
    if(!cap.open(0)){
        return 0;
    }
    for(;;){
        Mat frame;
        cap >> frame;
        if(frame.empty()){
            break;
        }
        imshow("Video",frame);
        if(waitKey(10)==27){
            break;
        }
    }
    return 0;
}
