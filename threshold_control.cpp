#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#pragma comment(lib, "opencv_world300d.lib")

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


//�P�x�l�𐔂���
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

//�q�X�g�O�����̍ő�l�����߂�
void find_max_hist(Hist &hist){

	for (int i = 0; i < 256; i++){
		if (hist.r_hist[i] > hist.r_hist_max) hist.r_hist_max = hist.r_hist[i];
		if (hist.g_hist[i] > hist.g_hist_max) hist.g_hist_max = hist.g_hist[i];
		if (hist.b_hist[i] > hist.b_hist_max) hist.b_hist_max = hist.b_hist[i];
	}
}

//�ő�l�Ő��K������
void hist_normalize(Hist &hist){
	for (int i = 0; i < 256; i++){
		hist.r_histf[i] = hist.r_hist[i] / (float)hist.r_hist_max;
		hist.g_histf[i] = hist.g_hist[i] / (float)hist.g_hist_max;
		hist.b_histf[i] = hist.b_hist[i] / (float)hist.b_hist_max;
	}
}

//�q�X�g�O������`��
void draw_histogram(cv::Mat &hist_img, Hist hist){
	int h = 100;
	for (int i = 0; i < 256; i++){
		cv::rectangle(hist_img, cv::Point(10 + i, 110), cv::Point(10 + i, 110 - (int)(hist.r_histf[i] * h)), cv::Scalar(0, 0, 255), 1, 8, 0);
		cv::rectangle(hist_img, cv::Point(10 + i, 220), cv::Point(10 + i, 220 - (int)(hist.g_histf[i] * h)), cv::Scalar(0, 255, 0), 1, 8, 0);
		cv::rectangle(hist_img, cv::Point(10 + i, 330), cv::Point(10 + i, 330 - (int)(hist.b_histf[i] * h)), cv::Scalar(255, 0, 0), 1, 8, 0);
	}
}

//�m�����z�쐬
void make_pd(Hist &hist, int img_size){
	double size = (double)img_size;
	for (int i = 0; i < 256; i++){
		hist.r_hist_pd[i] = hist.r_hist[i] / size;
		hist.g_hist_pd[i] = hist.g_hist[i] / size;
		hist.b_hist_pd[i] = hist.b_hist[i] / size;
	}
}

//�P�x�l�̍ő�ŏ��l�����߂�֐�
void find_max_min(Hist &hist){
	for (int i = 0; i < 256; i++){
		//�ő�l
		if (hist.r_hist[i] != 0 && i > hist.r_max) hist.r_max = i;
		if (hist.g_hist[i] != 0 && i > hist.g_max) hist.g_max = i;
		if (hist.b_hist[i] != 0 && i > hist.b_max) hist.b_max = i;
		//�ŏ��l
		if (hist.r_hist[i] != 0 && i < hist.r_min) hist.r_min = i;
		if (hist.g_hist[i] != 0 && i < hist.g_min) hist.g_min = i;
		if (hist.b_hist[i] != 0 && i < hist.b_min) hist.b_min = i;
	}
}

//���U���v�Z����֐�
//cal_V(�q�X�g�O����, ����, ���)
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

//���ς����߂�
double cal_average(int hist[256], int min, int max){
	double result = 0.0;

	for (int i = min; i < max; i++){
		result += hist[i];
	}

	result = result / (max - min);
	return result;
}

//��Â̓�l��
//binarization_otsu(���ׂ�q�X�g�O����, min, max)
int binarization_otsu(int hist[256], double sig[2], int min, int max){

	//�N���X�͈�
	/*omega1 : min ���� t �܂�
	�@omega2 : t ���� max �܂�*/
	int omega1, omega2;

	//�S�̕���
	double mt = cal_average(hist, 0, 256);
	//���N���X�A���N���X����
	double m1, m2;

	//臒l
	int threshold;

	//�����N���X�����U�ϐ�
	double sigma1, sigma2;

	//�N���X�����U
	double sigma_w = 100000000000.0;

	//�N���X�ԕ��U
	double sigma_b = 0.0;

	//�����xS
	double s, tmp;
	s = 0.0;

	/*��Â̓�l�����s���B*/
	for (int t = min + 1; t < max; t++){
		//���U���v�Z
		sigma1 = cal_V(hist, min, t);
		sigma2 = cal_V(hist, t, max);

		//�N���X�����U���v�Z
		omega1 = t - min;
		omega2 = max - t;
		sigma_w = omega1 * sigma1 + omega2 * sigma2;
		sigma_w = sigma_w / (omega1 + omega2);

		//�e���ϒl�����߂�
		//���N���X�@m1
		m1 = cal_average(hist, min, t);
		//���N���X�@m2
		m2 = cal_average(hist, t, max);

		//�N���X�ԕ��U
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
	std::cout << "�����xS = " << s << std::endl;
	std::cout << "�`�����l��臒lt = " << threshold << std::endl;

	return threshold;
}

/*==========================================================================================================================================*/
/*���C���֐�*/
int main(int argc, char *argy[]){

	cv::Mat src_img = cv::imread("C:\\file\\pos\\t_kinoko\\ (8).jpg");
	cv::Mat out_img;

	//�t�@�C�����J�������`�F�b�N
	if (src_img.empty()){
		std::cout << "�J���܂���ł���" << std::endl;
	}

	//�O����
	cv::erode(src_img, out_img, cv::Mat(), cv::Point(-1, -1), 2);
	cv::dilate(out_img, out_img, cv::Mat(), cv::Point(-1, -1), 2);
	cv::GaussianBlur(out_img, out_img, cv::Size(9, 7), 10, 10);

	//�`�����l������
	cv::Mat channels[3];
	cv::split(out_img, channels);

	Hist hist;

	//�P�x�l�𐔂���
	make_hist(out_img, hist);

	//�q�X�g�O�����̍ő�l�����߂�
	find_max_hist(hist);

	/*�q�X�g�O�����̍ő�l�\��*/
	std::cout << "�q�X�g�O�����̍ő�l" << std::endl;
	std::cout << "r_hist_max = " << hist.r_hist_max << std::endl;
	std::cout << "g_hist_max = " << hist.g_hist_max << std::endl;
	std::cout << "b_hist_max = " << hist.b_hist_max << std::endl;

	//�ő�l�Ő��K��
	hist_normalize(hist);

	//�m�����z���쐬
	make_pd(hist, (src_img.cols * src_img.rows));

	//�q�X�g�O������`��
	cv::Mat hist_img = cv::Mat(cv::Size(276, 340), CV_8UC3, cv::Scalar(255, 255, 255));
	draw_histogram(hist_img, hist);

	//�P�x�l�̍ő�ŏ��l�����߂�
	find_max_min(hist);
	std::cout << "�P�x�l�̍ő�A�ŏ�" << std::endl;
	std::cout << "r_max = " << hist.r_max << "  r_min = " << hist.r_min << std::endl;
	std::cout << "g_max = " << hist.g_max << "  g_min = " << hist.g_min << std::endl;
	std::cout << "b_max = " << hist.b_max << "  b_min = " << hist.b_min << std::endl;

	//��Â̓�l������臒l�����߂�
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

	cv::imwrite("C:\\file\\result\\threshold_control\\?�ԏ�������.jpg", hist_img);
	return 0;
}