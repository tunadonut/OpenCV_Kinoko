#ifndef func_histogram_HPP
#define func_histogram_HPP

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "histogram_class.hpp"

//輝度値を数える
void make_hist(cv::Mat src_img, Hist &hist){
	int idx = 0;
	int r, g, b;
	for (int y = 0; y < src_img.rows; y++){
		for (int x = 0; x < src_img.cols; x++){
			idx = (y * src_img.cols + x) * src_img.channels();
			b = src_img.data[idx];
			g = src_img.data[idx + 1];
			r = src_img.data[idx + 2];
			hist.b_hist[b]++;
			hist.g_hist[g]++;
			hist.r_hist[r]++;
		}
	}
}

//ヒストグラムの最大値を求める
void find_max_hist(Hist &hist){

	for (int i = 0; i < 256; i++){
		if (hist.r_hist[i] > hist.r_hist_max) hist.r_hist_max = hist.r_hist[i];
		if (hist.g_hist[i] > hist.g_hist_max) hist.g_hist_max = hist.g_hist[i];
		if (hist.b_hist[i] > hist.b_hist_max) hist.b_hist_max = hist.b_hist[i];
	}
}

//最大値で正規化する
void hist_normalize(Hist &hist){
	for (int i = 0; i < 256; i++){
		hist.r_histf[i] = hist.r_hist[i] / (float)hist.r_hist_max;
		hist.g_histf[i] = hist.g_hist[i] / (float)hist.g_hist_max;
		hist.b_histf[i] = hist.b_hist[i] / (float)hist.b_hist_max;
	}
}

//ヒストグラムを描画
void draw_histogram(cv::Mat &hist_img, Hist hist){
	int h = 100;
	for (int i = 0; i < 256; i++){
		cv::rectangle(hist_img, cv::Point(10 + i, 110), cv::Point(10 + i, 110 - (int)(hist.r_histf[i] * h)), cv::Scalar(0, 0, 255), 1, 8, 0);
		cv::rectangle(hist_img, cv::Point(10 + i, 220), cv::Point(10 + i, 220 - (int)(hist.g_histf[i] * h)), cv::Scalar(0, 255, 0), 1, 8, 0);
		cv::rectangle(hist_img, cv::Point(10 + i, 330), cv::Point(10 + i, 330 - (int)(hist.b_histf[i] * h)), cv::Scalar(255, 0, 0), 1, 8, 0);
	}
}

//確率分布作成
void make_pd(Hist &hist, int img_size){
	double size = (double)img_size;
	for (int i = 0; i < 256; i++){
		hist.r_hist_pd[i] = hist.r_hist[i] / size;
		hist.g_hist_pd[i] = hist.g_hist[i] / size;
		hist.b_hist_pd[i] = hist.b_hist[i] / size;
	}
}

//輝度値の最大最小値を求める関数
void find_max_min(Hist &hist){
	for (int i = 0; i < 256; i++){
		//最大値
		if (hist.r_hist[i] != 0 && i > hist.r_max) hist.r_max = i;
		if (hist.g_hist[i] != 0 && i > hist.g_max) hist.g_max = i;
		if (hist.b_hist[i] != 0 && i > hist.b_max) hist.b_max = i;
		//最小値
		if (hist.r_hist[i] != 0 && i < hist.r_min) hist.r_min = i;
		if (hist.g_hist[i] != 0 && i < hist.g_min) hist.g_min = i;
		if (hist.b_hist[i] != 0 && i < hist.b_min) hist.b_min = i;
	}
}

#endif //func_histogram_HPP