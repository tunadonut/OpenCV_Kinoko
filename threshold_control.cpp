#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#pragma comment(lib, "opencv_world300d.lib")

//ヒストグラムの情報を入れるクラス
class Hist {
public:
	//輝度値の数
	int r_hist[256], g_hist[256], b_hist[256];

	//ヒストグラムの最大値
	int r_hist_max, g_hist_max, b_hist_max;

	//正規化した輝度値を入れる配列
	float r_histf[256], g_histf[256], b_histf[256];

	//輝度値の確率分布
	double r_hist_pd[256], g_hist_pd[256], b_hist_pd[256];

	//輝度値の最大・最小
	int r_max, g_max, b_max;
	int r_min, g_min, b_min;

	//閾値t
	int rt, gt, bt;

	//黒クラス、白クラス
	double r_sigma[2], g_sigma[2], b_sigma[2];

	Hist(){
		r_hist_max = 0;
		g_hist_max = 0;
		b_hist_max = 0;

		r_max = 0;
		g_max = 0;
		b_max = 0;

		r_min = 255;
		g_min = 255;
		b_min = 255;

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

//分散を計算する関数
//cal_V(ヒストグラム, 下限, 上限)
double cal_V(int hist[256], int min, int max){
	double ave = 0.0;
	double ex2 = 0;
	double result;

	for (int i = min; i < max; i++){
		ave += hist[i];
		ex2 += std::pow(hist[i], 2.0);
	}

	int n = max - min;

	ave = ave / n;
	ex2 = ex2 / n;
	result = ex2 - ave;

	return result;
}

//平均を求める
double cal_average(int hist[256], int min, int max){
	double result = 0.0;

	for (int i = min; i < max; i++){
		result += hist[i];
	}

	result = result / (max - min);
	return result;
}

//大津の二値化
//binarization_otsu(調べるヒストグラム, min, max)
int binarization_otsu(int hist[256], double sig[2], int min, int max){

	//クラス範囲
	/*omega1 : min から t まで
	　omega2 : t から max まで*/
	int omega1, omega2;

	//全体平均
	double mt = cal_average(hist, 0, 256);
	//黒クラス、白クラス平均
	double m1, m2;

	//閾値
	int threshold;

	//白黒クラス内分散変数
	double sigma1, sigma2;

	//クラス内分散
	double sigma_w = 100000000000.0;

	//クラス間分散
	double sigma_b = 0.0;

	//分離度S
	double s, tmp;
	s = 0.0;

	/*大津の二値化を行う。*/
	for (int t = min + 1; t < max; t++){
		//分散を計算
		sigma1 = cal_V(hist, min, t);
		sigma2 = cal_V(hist, t, max);

		//クラス内分散を計算
		omega1 = t - min;
		omega2 = max - t;
		sigma_w = omega1 * sigma1 + omega2 * sigma2;
		sigma_w = sigma_w / (omega1 + omega2);

		//各平均値を求める
		//黒クラス　m1
		m1 = cal_average(hist, min, t);
		//白クラス　m2
		m2 = cal_average(hist, t, max);

		//クラス間分散
		sigma_b = omega1 * std::pow(m1 - mt, 2.0) + omega2 * std::pow(m2 - mt, 2.0);
		sigma_b = sigma_b / (omega1 + omega2);

		tmp = sigma_b / sigma_w;

		if (tmp > s){
			s = tmp;
			threshold = t;
			sig[0] = sigma1;
			sig[1] = sigma2;
		}
	}
	std::cout << "分離度S = " << s << std::endl;
	std::cout << "チャンネル閾値t = " << threshold << std::endl;

	return threshold;
}

/*==========================================================================================================================================*/
/*メイン関数*/
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

	//大津の二値化から閾値を求める
	hist.rt = binarization_otsu(hist.r_hist, hist.r_sigma, hist.r_min, hist.r_max);
	hist.gt = binarization_otsu(hist.g_hist, hist.g_sigma, hist.g_min, hist.g_max);
	hist.bt = binarization_otsu(hist.b_hist, hist.b_sigma, hist.b_min, hist.b_max);

	for (int i = 0; i < 2; i++){
		std::cout << "hist.r_sigma" << i << " = " << hist.r_sigma[i] << std::endl;
		std::cout << "hist.g_sigma" << i << " = " << hist.g_sigma[i] << std::endl;
		std::cout << "hist.b_sigma" << i << " = " << hist.b_sigma[i] << std::endl;
	}

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