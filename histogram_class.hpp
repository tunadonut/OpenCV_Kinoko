#ifndef histogram_HPP
#define histogram_HPP

#include <opencv2/highgui.hpp>

//ヒストグラムの情報を入れるクラス
class Hist {
public:
	//輝度値の数
	int rgb_hist[3][256];

	//ヒストグラムの開始と終了
	//RGBの順番
	int start[3], end[3];

	//ヒストグラムの最大値
	int rgb_hist_max[3];

	//正規化した輝度値を入れる配列
	float rgb_histf[3][256];

	//輝度値の確率分布
	double rgb_hist_pd[3][256];

	//輝度値の最大・最小
	int rgb_max[3];
	int rgb_min[3];

	//閾値t
	//RGBの順番
	int rgb_threshold[3];

	//黒クラス、白クラス
	double r_sigma[2], g_sigma[2], b_sigma[2];

	//エッジ強度の期待値
	//RGBの順番
	int edge_stren[3];

	Hist(){

		for (int i = 0; i < 3; i++){
			rgb_hist_max[i] = 0;
			rgb_max[i] = 0;
			rgb_min[i] = 255;
		}

		for (int i = 0; i < 3; i++){
			for (int j = 0; j < 256; j++){
				rgb_hist[i][j] = 0;
			}
		}
	}
};

#endif //histogram_HPP