#include <iostream>
#include <string>
#include <ostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#pragma comment(lib, "opencv_world300d.lib")

static const int BLACK = 0;

class Kinoko {
public:
	int x[2];
	int y[2];

	Kinoko(){
		//���W������̂Ő��̐������Ȃ�
		x[0] = -1;
		x[1] = -1;
		y[0] = -1;
		y[1] = -1;
	}

	int get_width(){
		return x[1] - x[0];
	}
};

void find_kinoko(cv::Mat canny_img, std::vector<Kinoko> &obj){

	for (int y = 0; y < canny_img.rows; y++){
		//�s���ڂ�����V�������
		Kinoko kinoko;
		obj.push_back(kinoko);
		for (int x = 0; x < canny_img.cols; x++){
			int idx = y * canny_img.cols + x;

			if (canny_img.data[idx] == BLACK && obj.back().x[0] == -1){
				obj.back().x[0] = x;
			}
			else  if(canny_img.data[idx] == BLACK && obj.back().x[0] != -1){
				obj.back().x[1] = x;
				Kinoko kinoko;
				obj.push_back(kinoko);
			}
		}
	}
}
int main(int argc, char *argy[]){
	//8, 43
	cv::Mat src_img = cv::imread("C:\\file\\pos\\t_kinoko\\ (8).jpg");
	
	if (src_img.empty()){
		std::cout << "cannot open file" << std::endl;
		return -1;
	}

	for (int i = 0; i < 2; i++){
		//cv::GaussianBlur(src_img, src_img, cv::Size(5, 5), 10, 10);
		//�I�[�v�j���O
		cv::erode(src_img, src_img, cv::Mat(), cv::Point(-1, -1), 2);
		cv::dilate(src_img, src_img, cv::Mat(), cv::Point(-1, -1), 2);
		cv::GaussianBlur(src_img, src_img, cv::Size(9, 7), 10, 10);
	}
	cv::namedWindow("image", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
	cv::imshow("image", src_img);
	cv::waitKey(0);

	cv::Mat channels[3];
	cv::split(src_img, channels);
	for (int i = 0; i < 3; i++){
		cv::Canny(channels[i], channels[i], 60, 180, 3);
	}
	cv::Mat canny_img;
	cv::bitwise_or(channels[0], channels[1], canny_img);
	cv::bitwise_or(channels[2], canny_img, canny_img);
	canny_img = ~canny_img;

	std::vector<Kinoko> obj;
	find_kinoko(canny_img, obj);
	/*���s�������ƁB
	if(canny_img.data[idx] == BLACK && obj.back().x[0] == -1)}else{}�Ə����Ă��܂������A
	���ꂾ��else�ɓ������Ƃ���canny_img.data[idx] != BLACK������Ă��܂��̂�else�͂����Ə������ƁB
	get_width()���g���Ƃ���x[1]�ɒl�������Ă��Ȃ��ƂƂ�ł��Ȃ������ɂȂ�̂�if���Ń`�F�b�N����B
	�ł��|���̂ŁA���get_width()�����ǂ��Ă����B*/
	for (int i = 0; i < obj.size(); i++){
		if (obj[i].x[1] != -1){
			std::cout << i << "�Ԗڂ̕� = " << obj[i].get_width() << std::endl;
		}
	}
	cv::imshow("image", canny_img);
	cv::waitKey(0);

	cv::imwrite("C:\\file\\result\\kinoko1\\43.jpg", canny_img);
	return 0;
}