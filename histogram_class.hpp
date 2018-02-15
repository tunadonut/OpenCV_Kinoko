#ifndef histogram_HPP
#define histogram_HPP

#include <opencv2/highgui.hpp>

//�q�X�g�O�����̏�������N���X
class Hist {
public:
	//�P�x�l�̐�
	int r_hist[256], g_hist[256], b_hist[256];

	//�q�X�g�O�����̍ő�l
	int r_hist_max, g_hist_max, b_hist_max;

	//���K�������P�x�l������z��
	float r_histf[256], g_histf[256], b_histf[256];

	//�P�x�l�̊m�����z
	double r_hist_pd[256], g_hist_pd[256], b_hist_pd[256];

	//�P�x�l�̍ő�E�ŏ�
	int r_max, g_max, b_max;
	int r_min, g_min, b_min;

	//臒lt
	int rt, gt, bt;

	//���N���X�A���N���X
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