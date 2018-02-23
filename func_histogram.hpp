#ifndef func_histogram_HPP
#define func_histogram_HPP

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "histogram_class.hpp"

//�\���p�֐�
void printer(int value[3], std::string str){
	std::cout << str << std::endl;
	for (int i = 0; i < 3; i++){
		std::cout << value[i] << std::endl;
	}
}

//�q�X�g�O�����̍쐬
void make_hist(cv::Mat src_img, Hist &hist){
	int idx = 0;//�s�N�Z���̏ꏊ������
	int r, g, b;
	for (int y = 0; y < src_img.rows; y++){
		for (int x = 0; x < src_img.cols; x++){
			idx = (y * src_img.cols + x) * src_img.channels();
			b = src_img.data[idx];
			g = src_img.data[idx + 1];
			r = src_img.data[idx + 2];
			
			hist.rgb_hist[0][r]++;
			hist.rgb_hist[1][g]++;
			hist.rgb_hist[2][b]++;
		}
	}
}

//�q�X�g�O�����̍ő�l�����߂�
void find_max_hist(Hist &hist){
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 256; j++){
			if (hist.rgb_hist[i][j] > hist.rgb_hist_max[i]) hist.rgb_hist_max[i] = hist.rgb_hist[i][j];
		}
	}
}

//�ő�l�Ő��K������
void hist_normalize(Hist &hist){
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 256; j++){
			hist.rgb_histf[i][j] = hist.rgb_hist[i][j] / (float)hist.rgb_hist_max[i];
		}
	}
}

//�q�X�g�O������`��
void draw_histogram(cv::Mat &hist_img, Hist hist){
	int h = 100;
	for (int i = 0; i < 256; i++){
		cv::rectangle(hist_img, cv::Point(10 + i, 110), cv::Point(10 + i, 110 - (int)(hist.rgb_histf[0][i] * h)), cv::Scalar(0, 0, 255), 1, 8, 0);
		cv::rectangle(hist_img, cv::Point(10 + i, 220), cv::Point(10 + i, 220 - (int)(hist.rgb_histf[1][i] * h)), cv::Scalar(0, 255, 0), 1, 8, 0);
		cv::rectangle(hist_img, cv::Point(10 + i, 330), cv::Point(10 + i, 330 - (int)(hist.rgb_histf[2][i] * h)), cv::Scalar(255, 0, 0), 1, 8, 0);
	}
}

//�m�����z�쐬
void make_pd(Hist &hist, int img_size){
	double size = (double)img_size;
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 256; j++){
			hist.rgb_hist_pd[i][j] = hist.rgb_hist[i][j] / size;
		}
	}
}

//�P�x�l�̍ő�ŏ��l�����߂�֐�
void find_max_min(Hist &hist){
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 256; j++){
			if (hist.rgb_hist[i][j] > 0 && j > hist.rgb_max[i]) hist.rgb_max[i] = j;
			if (hist.rgb_hist[i][j] > 0 && j < hist.rgb_min[i]) hist.rgb_min[i] = j;
		}
	}
}

//�q�X�g�O�����̊J�n�ƏI�������߂�֐�
void find_start_end(int hist[256], int &start, int &end){
	for (int i = 0; i < 256; i++){
		if (hist[i] != 0) start = i;
	}
	for (int i = 0; i < 256; i++){
		if (hist[255 - i] != 0) end = 255 - i;
	}
}

//�N���X�ԃG�b�W���x�̊��Ғl
void cal_edge_strength(Hist hist){
	double class1, class2;
	double sigma = 0.0;
	//��f��
	int omega[2];

	std::cout << "���σN���X�ԃG�b�W���x [0] = �ԁA[1] = �΁A[2] = �ł��B" << std::endl;

	for (int i = 0; i < 3; i++){
		class1 = 0.0;
		class2 = 0.0;
		omega[0] = 0;
		omega[1] = 0;
		
		//���N���X�̋P�x�l�̍��v�A��f���̍��v
		for (int j = 0; j < hist.rgb_threshold[i]; j++){
			class1 += (hist.rgb_hist[i][j] * j);
			omega[0] += hist.rgb_hist[i][j];
		}

		//���N���X�̋P�x�l�̍��v�A��f���̍��v
		for (int j = hist.rgb_threshold[i]; j < 256; j++){
			class2 += (hist.rgb_hist[i][j] * j);
			omega[1] += hist.rgb_hist[i][j];
		}
		//���N���X�̕���
		class1 = class1 / omega[0];
		//���N���X�̕���
		class2 = class2 / omega[1];
		//���σN���X�ԃG�b�W���x
		sigma = class1 - class2;
		if (sigma < 0){
			sigma = sigma * -1;
		}

		std::cout << "���σN���X�ԃG�b�W���x[" << i << "] = " << sigma << std::endl;
	}
}
#endif //func_histogram_HPP