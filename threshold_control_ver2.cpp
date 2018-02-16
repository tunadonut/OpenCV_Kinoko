#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "histogram_class.hpp"
#include "func_histogram.hpp"
#include "otsu_binarization.hpp"

#pragma comment(lib, "opencv_world300d.lib")

int main(int argc, char *argy[]){

	cv::Mat src_img = cv::imread("C:\\file\\pos\\t_kinoko\\ (8).jpg");
	cv::Mat out_img;

	//ファイルが開けたがチェック
	if (src_img.empty()){
		std::cout << "開けませんでした" << std::endl;
	}

	//前処理
	cv::erode(src_img, out_img, cv::Mat(), cv::Point(-1, -1), 2);
	cv::dilate(out_img, out_img, cv::Mat(), cv::Point(-1, -1), 2);
	cv::GaussianBlur(out_img, out_img, cv::Size(9, 7), 10, 10);

	//test
	//DiscriminantAnalysis(src_img, src_img.rows, src_img.cols);

	//チャンネル分け
	cv::Mat channels[3];
	cv::split(out_img, channels);

	Hist hist;

	//輝度値を数える
	make_hist(out_img, hist);

	//ヒストグラムの最大値を求める
	find_max_hist(hist);

	/*ヒストグラムの最大値表示*/
	std::cout << "ヒストグラムの最大値" << std::endl;
	std::cout << "r_hist_max = " << hist.r_hist_max << std::endl;
	std::cout << "g_hist_max = " << hist.g_hist_max << std::endl;
	std::cout << "b_hist_max = " << hist.b_hist_max << std::endl;

	//最大値で正規化
	hist_normalize(hist);

	//確率分布を作成
	make_pd(hist, (src_img.cols * src_img.rows));

	//ヒストグラムを描く
	cv::Mat hist_img = cv::Mat(cv::Size(276, 340), CV_8UC3, cv::Scalar(255, 255, 255));
	draw_histogram(hist_img, hist);

	//輝度値の最大最小値を求める
	find_max_min(hist);
	std::cout << "輝度値の最大、最小" << std::endl;
	std::cout << "r_max = " << hist.r_max << "  r_min = " << hist.r_min << std::endl;
	std::cout << "g_max = " << hist.g_max << "  g_min = " << hist.g_min << std::endl;
	std::cout << "b_max = " << hist.b_max << "  b_min = " << hist.b_min << std::endl;

	//大津の二値化
	hist.rt = otsu_binari(hist.r_hist, hist.r_min, hist.r_max);
	hist.gt = otsu_binari(hist.g_hist, hist.g_min, hist.g_max);
	hist.bt = otsu_binari(hist.b_hist, hist.b_min, hist.b_max);

	std::cout << "hist.rt = " << hist.rt << std::endl;
	std::cout << "hist.gt = " << hist.gt << std::endl;
	std::cout << "hist.bt = " << hist.bt << std::endl;

	cv::namedWindow("histogram");
	cv::namedWindow("src_img");
	cv::namedWindow("out_img");
	cv::imshow("histogram", hist_img);
	cv::imshow("src_img", src_img);
	cv::imshow("out_img", out_img);
	cv::waitKey(0);

	cv::imwrite("C:\\file\\result\\threshold_control\\?番処理あり.jpg", hist_img);
	return 0;
}