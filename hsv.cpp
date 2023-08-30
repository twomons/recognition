#include <opencv2/opencv.hpp>

cv::Mat frame;
cv::Mat hsvFrame;
cv::Mat mask;

int hMin = 0, sMin = 0, vMin = 0;
int hMax = 179, sMax = 255, vMax = 255;

void updateMask() {
    cv::inRange(hsvFrame, cv::Scalar(hMin, sMin, vMin), cv::Scalar(hMax, sMax, vMax), mask);
    cv::imshow("Mask", mask);
}

void onTrackbar(int, void*) {
    updateMask();
}

int main() {
    cv::VideoCapture cap("/home/laplace/桌面/proj/1.mp4");
    if (!cap.isOpened()) {
        std::cout << "Failed to open video file." << std::endl;
        return -1;
    }

    cv::namedWindow("Frame");
    cv::namedWindow("Mask");

    cv::createTrackbar("Hue Min", "Frame", &hMin, 179, onTrackbar);
    cv::createTrackbar("Hue Max", "Frame", &hMax, 179, onTrackbar);
    cv::createTrackbar("Saturation Min", "Frame", &sMin, 255, onTrackbar);
    cv::createTrackbar("Saturation Max", "Frame", &sMax, 255, onTrackbar);
    cv::createTrackbar("Value Min", "Frame", &vMin, 255, onTrackbar);
    cv::createTrackbar("Value Max", "Frame", &vMax, 255, onTrackbar);

    double fps = cap.get(cv::CAP_PROP_FPS);
    int delay = static_cast<int>(1000 / fps);

    while (true) {
        cap >> frame;
        if (frame.empty())
            break;

        cv::cvtColor(frame, hsvFrame, cv::COLOR_BGR2HSV);
        updateMask();

        cv::imshow("Frame", frame);

        char key = cv::waitKey(delay);
        if (key == 'q' || key == 'Q')
            break;
    }

    cv::destroyAllWindows();
    return 0;
}
