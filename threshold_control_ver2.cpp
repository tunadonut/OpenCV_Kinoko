#include <iostream>
#include <sstream>
#include <string>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "histogram_class.hpp"
#include "func_histogram.hpp"
#include "otsu_binarization.hpp"

#pragma comment(lib, "opencv_world340.lib")

int main(int argc, char *argy[]){
	int num;
	std::cout << "ファイル番号 : ";
	std::cin >> num;
	std::ostringstream ss;
	ss << num;

	std::string fn = "C:\\file\\pos\\t_kinoko\\(" + ss.str() + ").jpg";
	std::cout << fn << std::endl;
	//C:\file\pos\t_kinoko
	//fn = "C:\\file\\pos\\anime\\samidare.jpg";
	cv::Mat src_img = cv::imread(fn);
	cv::Mat out_img = cv::imread(fn);

	//cannyフィルタの閾値用変数
	int rgb_canny_threshold[3];

	//ファイルが開けたがチェック
	if (src_img.empty()){
		std::cout << "開けませんでした" << std::endl;
		return -1;
	}

	//前処理
	for (int i = 0; i < 5; i++) {
		cv::erode(out_img, out_img, cv::Mat(), cv::Point(-1, -1), 2);
		cv::dilate(out_img, out_img, cv::Mat(), cv::Point(-1, -1), 2);
		if (i % 2 == 0) {
			cv::GaussianBlur(out_img, out_img, cv::Size(3, 3), 10, 10);
		}
	}
	//test
	//DiscriminantAnalysis(src_img, src_img.rows, src_img.cols);

	Hist hist;

	make_hist(out_img, hist);

	//ヒストグラムの最大値を求める
	find_max_hist(hist);

	/*ヒストグラムの最大値表示*/
	printer(hist.rgb_hist_max, "ヒストグラムの最大値表示");

	//最大値で正規化
	hist_normalize(hist);

	//確率分布を作成
	make_pd(hist, (src_img.cols * src_img.rows));

	//ヒストグラムを描く
	cv::Mat hist_img = cv::Mat(cv::Size(276, 340), CV_8UC3, cv::Scalar(255, 255, 255));
	draw_histogram(hist_img, hist);

	//輝度値の最大最小値を求める
	find_max_min(hist);
	printer(hist.rgb_max, "輝度値の最大");
	printer(hist.rgb_min, "輝度値の最小");

	//大津の二値化
	for (int i = 0; i < 3; i++){
		hist.rgb_threshold[i] = otsu_binari(hist.rgb_hist[i], hist.rgb_min[i], hist.rgb_max[i]);
	}
	printer(hist.rgb_threshold, "閾値RGB");

	//平均クラス間エッジ強度
	cal_edge_strength(hist, hist.edge_stren);
	printer(hist.edge_stren, "平均クラス間エッジ強度 RGB順");

	cv::Mat channels[3];
	cv::split(out_img, channels);
	/*
	エッジ検出感度
	上げるとより細かくエッジを検出する。
	下げるとより大きなエッジを検出する。
	*/
	double sensitivity = 2.0;
	for (int i = 0; i < 3; i++){
		int t1 = (int)(hist.edge_stren[i] / sensitivity);
		int t3 = (int)(hist.edge_stren[i] / sensitivity + hist.edge_stren[i]);
		if (t3 > 256){
			t3 = 255;
		}
		cv::Canny(channels[i], channels[i], t1, t3, 3);
	}

	cv::Mat canny_img;
	cv::bitwise_or(channels[0], channels[1], canny_img);
	cv::bitwise_or(channels[2], canny_img, canny_img);
	canny_img = ~canny_img;
	cv::erode(canny_img, canny_img, cv::Mat(), cv::Point(-1, -1), 1);
	cv::namedWindow("histogram");
	cv::namedWindow("src_img");
	cv::namedWindow("out_img");
	cv::namedWindow("canny_img");
	cv::imshow("histogram", hist_img);
	cv::imshow("src_img", src_img);
	cv::imshow("out_img", out_img);
	cv::imshow("canny_img", canny_img);
	cv::waitKey(0);

	cv::imwrite("C:\\file\\result\\threshold_control\\59番前処理あり.jpg", hist_img);

	return 0;
}