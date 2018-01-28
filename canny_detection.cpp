#include <iostream>
#include <string>
#include <ostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#pragma comment(lib, "opencv_world300d.lib")


int main(int argc, char *argy[]){
	//8, 43
	cv::Mat src_img = cv::imread("C:\\file\\pos\\t_kinoko\\humika.jpg");
	
	if (src_img.empty()){
		std::cout << "cannot open file" << std::endl;
		return -1;
	}

	for (int i = 0; i < 2; i++){
		//cv::GaussianBlur(src_img, src_img, cv::Size(5, 5), 10, 10);
		//オープニング
		cv::erode(src_img, src_img, cv::Mat(), cv::Point(-1, -1), 2);
		cv::dilate(src_img, src_img, cv::Mat(), cv::Point(-1, -1), 2);
		cv::GaussianBlur(src_img, src_img, cv::Size(9, 7), 10, 10);
	}
	cv::namedWindow("image", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
	cv::imshow("image", src_img);
	cv::waitKey(0);

	cv::Mat channels[3];
	cv::split(src_img, channels);
	for (int i = 0; i < 3; i++){
		cv::Canny(channels[i], channels[i], 60, 180, 3);
	}
	cv::Mat canny_img;
	cv::bitwise_or(channels[0], channels[1], canny_img);
	cv::bitwise_or(channels[2], canny_img, canny_img);
	canny_img = ~canny_img;

	//キノコを囲む
	cv::imshow("image", canny_img);
	cv::waitKey(0);

	cv::imwrite("C:\\file\\result\\kinoko1\\43.jpg", canny_img);
	return 0;
}