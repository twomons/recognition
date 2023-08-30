#include <iostream>
#include <opencv2/opencv.hpp>


void detectArmor(cv::Mat frame) {
    //图像转到HSV空间
    cv::Mat hsvFrame;
    cv::cvtColor(frame, hsvFrame, cv::COLOR_BGR2HSV);
    //创建橙色掩模
    cv::Mat orangeMask;
    cv::inRange(hsvFrame, cv::Scalar(0, 70, 50), cv::Scalar(20, 255, 255), orangeMask);
    //形态学操作去除小色块
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    cv::morphologyEx(orangeMask, orangeMask, cv::MORPH_OPEN, kernel);
    //依次找到轮廓
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(orangeMask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    //对每一个轮廓进行遍历，绘制最小矩形和计算角度
    for (const auto& contour : contours) {
        double area = cv::contourArea(contour);
        if (area > 100 && area < 10000) {
            cv::RotatedRect rotatedRect = cv::minAreaRect(contour);
            float angle = rotatedRect.angle;
	    //角度修正
            if (angle < -90.0)
                angle += 180.0;
            else if (angle > 90.0)
                angle -= 180.0;

            std::string orientation;
            if (angle < 0)
                orientation = std::to_string(int(angle));

            else
                orientation = std::to_string(int(angle));

            cv::Rect boundingRect = cv::boundingRect(contour);
            cv::putText(frame, orientation, cv::Point(boundingRect.x, boundingRect.y - 10),
                        cv::FONT_HERSHEY_SIMPLEX, 0.9, cv::Scalar(0, 255, 0), 2);
            cv::rectangle(frame, boundingRect, cv::Scalar(0, 255, 0), 2);
        }
    }

    cv::imshow("Armor Detection", frame);
}


int main() {
    // 打开视频文件
    cv::VideoCapture video("/home/laplace/桌面/proj/3.mp4");
    
    // 检查视频是否成功打开
    if (!video.isOpened()) {
        std::cout << "无法打开视频文件" << std::endl;
        return -1;
    }
    
    // 获取视频帧率
    double frameRate = video.get(cv::CAP_PROP_FPS);
    
    
    // 读取并显示视频帧
    cv::Mat frame;
    while (video.read(frame)) {
    	// 指定缩小的目标大小
    	int targetWidth = 800;  // 目标宽度
    	int targetHeight = frame.rows * targetWidth / frame.cols;  // 根据宽高比计算目标高度
    
    	// 等比例缩小图像
    	cv::Mat resizedImage;
    	cv::resize(frame, resizedImage, cv::Size(targetWidth, targetHeight));

        if (resizedImage.empty())
            break;
        detectArmor(resizedImage);
        // 等待帧率时间
        int delay = 1000 / frameRate;
       
        // 按下ESC键退出循环
        if (cv::waitKey(delay) == 27)
            break;
    }
    
    // 释放视频对象和关闭窗口
    video.release();
    cv::destroyAllWindows();
    
    return 0;
}
