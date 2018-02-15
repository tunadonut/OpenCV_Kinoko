#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "histogram_class.hpp"
#include "func_histogram.hpp"

#pragma comment(lib, "opencv_world300d.lib")

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