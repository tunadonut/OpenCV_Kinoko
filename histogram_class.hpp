#ifndef histogram_HPP
#define histogram_HPP

#include <opencv2/highgui.hpp>

//�q�X�g�O�����̏�������N���X
class Hist {
public:
	//�P�x�l�̐�
	int rgb_hist[3][256];

	//�q�X�g�O�����̊J�n�ƏI��
	//RGB�̏���
	int start[3], end[3];

	//�q�X�g�O�����̍ő�l
	int rgb_hist_max[3];

	//���K�������P�x�l������z��
	float rgb_histf[3][256];

	//�P�x�l�̊m�����z
	double rgb_hist_pd[3][256];

	//�P�x�l�̍ő�E�ŏ�
	int rgb_max[3];
	int rgb_min[3];

	//臒lt
	//RGB�̏���
	int rgb_threshold[3];

	//���N���X�A���N���X
	double r_sigma[2], g_sigma[2], b_sigma[2];

	//�G�b�W���x�̊��Ғl
	//RGB�̏���
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