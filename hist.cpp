#include <iostream>
#include <string>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>>

#pragma comment(lib, "opencv_world300d.lib")

int main(int argc, char *argy[]){
	cv::Mat src_img = cv::imread("C:\\file\\pos\\t_kinoko\\humika.jpg");
	if (src_img.empty()){
		std::cout << "開けない" << std::endl;
		return -1;
	}

	std::vector<cv::Mat> channels(3);
	cv::split(src_img, channels);
	int r, g, b;
	int r_hist[256], g_hist[256], b_hist[256];
	for (int i = 0; i < 256; i++){
		r_hist[i] = 0;
		g_hist[i] = 0;
		b_hist[i] = 0;
	}

	for (int y = 0; y < src_img.rows; y++){
		for (int x = 0; x < src_img.cols; x++){
			int idx = (y * src_img.cols + x) * src_img.channels();
			b = channels[0].at<uchar>(y, x);
			g = channels[1].at<uchar>(y, x);
			r = channels[2].at<uchar>(y, x);

			b_hist[b]++;
			g_hist[g]++;
			r_hist[r]++;
		}
	}

	//最大値を求める
	int r_hist_max = 0, g_hist_max = 0, b_hist_max = 0;
	for (int i = 0; i < 256; i++){
		if (r_hist[i] > r_hist_max) r_hist_max = r_hist[i];
		if (g_hist[i] > g_hist_max) g_hist_max = g_hist[i];
		if (b_hist[i] > g_hist_max) b_hist_max = b_hist[i];

		std::cout << "r_hist[" << i << "] = " << r_hist[i] << std::endl;
		std::cout << "g_hist[" << i << "] = " << g_hist[i] << std::endl;
		std::cout << "b_hist[" << i << "] = " << b_hist[i] << std::endl;

	}

	float r_histf[256], g_histf[256], b_histf[256];
	for (int i = 0; i < 256; i++){
		r_histf[i] = r_hist[i] / (float)r_hist_max;
		g_histf[i] = g_hist[i] / (float)g_hist_max;
		b_histf[i] = b_hist[i] / (float)b_hist_max;

		std::cout << "r_histf = " << r_histf[i] << std::endl;
		std::cout << "g_histf = " << r_histf[i] << std::endl;
		std::cout << "b_histf = " << r_histf[i] << std::endl;
	}

	cv::Mat hist_image = cv::Mat(cv::Size(276, 320), CV_8UC3, cv::Scalar(255, 255, 255));
	for (int i = 0; i < 256; i++){
		cv::line(hist_image, cv::Point(10 + i, 100), cv::Point(10 + i, 100 - (int)(r_histf[i] * 80)), cv::Scalar(0, 0, 255), 1, 8, 0);
		cv::line(hist_image, cv::Point(10 + i, 200), cv::Point(10 + i, 200 - (int)(g_histf[i] * 80)), cv::Scalar(0, 255, 0), 1, 8, 0);
		cv::line(hist_image, cv::Point(10 + i, 300), cv::Point(10 + i, 300 - (int)(b_histf[i] * 80)), cv::Scalar(255, 0, 0), 1, 8, 0);
	}
	cv::namedWindow("ヒストグラム");
	cv::imshow("ヒストグラム", hist_image);
	std::cout << "r_hist_max = " << r_hist_max << std::endl;
	std::cout << "g_hist_max = " << r_hist_max << std::endl;
	std::cout << "b_hist_max = " << r_hist_max << std::endl;
	cv::waitKey(0);
	return 0;
}