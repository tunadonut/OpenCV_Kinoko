#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#pragma comment(lib, "opencv_world300d.lib")

//�q�X�g�O�����̏�������N���X
class Hist {
public:
	int r_hist[256], g_hist[256], b_hist[256];
	int r_max, g_max, b_max;

	//���K�������P�x�l������z��
	float r_histf[256], g_histf[256], b_histf[256];

	//�P�x�l�̊m�����z
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
		if (hist.r_hist[i] > hist.r_max) hist.r_max = hist.r_hist[i];
		if (hist.g_hist[i] > hist.g_max) hist.g_max = hist.g_hist[i];
		if (hist.b_hist[i] > hist.b_max) hist.b_max = hist.b_hist[i];
	}
}

//�ő�l�Ő��K������
void hist_normalize(Hist &hist){
	for (int i = 0; i < 256; i++){
		hist.r_histf[i] = hist.r_hist[i] / (float)hist.r_max;
		hist.g_histf[i] = hist.g_hist[i] / (float)hist.g_max;
		hist.b_histf[i] = hist.b_hist[i] / (float)hist.b_max;
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
/*���C���֐�*/
int main(int argc, char *argy[]){

	cv::Mat src_img = cv::imread("C:\\file\\pos\\t_kinoko\\ (17).jpg");

	//�t�@�C�����J�������`�F�b�N
	if (src_img.empty()){
		std::cout << "�J���܂���ł���" << std::endl;
	}

	cv::Mat channels[3];
	cv::split(src_img, channels);

	Hist hist;

	//�P�x�l�𐔂���
	make_hist(src_img, hist);

	//�q�X�g�O�����̍ő�l�����߂�
	find_max_hist(hist);

	/*�ő�l�\��*/
	std::cout << "r_hist_max = " << hist.r_max << std::endl;
	std::cout << "g_hist_max = " << hist.g_max << std::endl;
	std::cout << "b_hist_max = " << hist.b_max << std::endl;

	//�ő�l�Ő��K��
	hist_normalize(hist);

	//�m�����z���쐬
	make_pd(hist, (src_img.cols * src_img.rows));
	//�q�X�g�O������`��
	cv::Mat hist_img = cv::Mat(cv::Size(276, 340), CV_8UC3, cv::Scalar(255, 255, 255));
	draw_histogram(hist_img, hist);

	cv::namedWindow("histogram");
	cv::namedWindow("src_img");
	cv::imshow("histogram", hist_img);
	cv::imshow("src_img", src_img);
	cv::waitKey(0);


	return 0;
}