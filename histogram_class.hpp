#ifndef histogram_HPP
#define histogram_HPP

#include <opencv2/highgui.hpp>

//ヒストグラムの情報を入れるクラス
class Hist {
public:
	//輝度値の数
	int r_hist[256], g_hist[256], b_hist[256];

	//ヒストグラムの開始と終了
	//RGBの順番
	int start[3], end[3];

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

#endif //histogram_HPP