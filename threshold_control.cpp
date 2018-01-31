#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#pragma comment(lib, "opencv_world300d.lib")

//ヒストグラムの情報を入れるクラス
class Hist {
public:
	int r_hist[256], g_hist[256], b_hist[256];
	int r_max, g_max, b_max;

	//正規化した輝度値を入れる配列
	float r_histf[256], g_histf[256], b_histf[256];

	//輝度値の確率分布
	double r_hist_pd[256], g_hist_pd[256], b_hist_pd[256];

	Hist(){
		r_max = 0;
		g_max = 0;
		b_max = 0;

		for (int i = 0; i < 256; i++){
			r_hist[i] = 0;
			g_hist[i] = 0;
			b_hist[i] = 0;
		}
	}
};
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
		if (hist.r_hist[i] > hist.r_max) hist.r_max = hist.r_hist[i];
		if (hist.g_hist[i] > hist.g_max) hist.g_max = hist.g_hist[i];
		if (hist.b_hist[i] > hist.b_max) hist.b_max = hist.b_hist[i];
	}
}

//最大値で正規化する
void hist_normalize(Hist &hist){
	for (int i = 0; i < 256; i++){
		hist.r_histf[i] = hist.r_hist[i] / (float)hist.r_max;
		hist.g_histf[i] = hist.g_hist[i] / (float)hist.g_max;
		hist.b_histf[i] = hist.b_hist[i] / (float)hist.b_max;
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
/*メイン関数*/
int main(int argc, char *argy[]){

	cv::Mat src_img = cv::imread("C:\\file\\pos\\t_kinoko\\ (17).jpg");

	//ファイルが開けたがチェック
	if (src_img.empty()){
		std::cout << "開けませんでした" << std::endl;
	}

	cv::Mat channels[3];
	cv::split(src_img, channels);

	Hist hist;

	//輝度値を数える
	make_hist(src_img, hist);

	//ヒストグラムの最大値を求める
	find_max_hist(hist);

	/*最大値表示*/
	std::cout << "r_hist_max = " << hist.r_max << std::endl;
	std::cout << "g_hist_max = " << hist.g_max << std::endl;
	std::cout << "b_hist_max = " << hist.b_max << std::endl;

	//最大値で正規化
	hist_normalize(hist);

	//確率分布を作成
	make_pd(hist, (src_img.cols * src_img.rows));
	//ヒストグラムを描く
	cv::Mat hist_img = cv::Mat(cv::Size(276, 340), CV_8UC3, cv::Scalar(255, 255, 255));
	draw_histogram(hist_img, hist);

	cv::namedWindow("histogram");
	cv::namedWindow("src_img");
	cv::imshow("histogram", hist_img);
	cv::imshow("src_img", src_img);
	cv::waitKey(0);

	return 0;
}