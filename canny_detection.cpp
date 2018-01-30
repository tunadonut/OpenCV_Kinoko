#include <iostream>
#include <string>
#include <ostream>
#include <vector>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#pragma comment(lib, "opencv_world300d.lib")

static const int BLACK = 0;

class Kinoko {
public:
	int x[2];
	int y[2];
	int length;

	Kinoko(){
		//座標が入るので正の数を入れない
		x[0] = -1;
		x[1] = -1;
		y[0] = -1;
		y[1] = -1;
		length = 0;
	}

	int get_width(){
		return x[1] - x[0];
	}

	int get_coordinateX(int n, int cols, int y){
		return x[n] - (y * cols);
	}
};

void find_kinoko(cv::Mat canny_img, std::vector<Kinoko> &obj){

	for (int y = 0; y < canny_img.rows; y++){
		//行が移ったら新しく作る
		Kinoko kinoko;
		obj.push_back(kinoko);
		for (int x = 0; x < canny_img.cols; x++){
			int idx = y * canny_img.cols + x;

			if (canny_img.data[idx] == BLACK && obj.back().x[0] == -1){
				obj.back().x[0] = idx;
				//エッジの太さを検出しないようにする
				x += 3;
			}
			else  if(canny_img.data[idx] == BLACK && obj.back().x[0] != -1){
				obj.back().x[1] = idx;
				x += 3;
				Kinoko kinoko;
				obj.push_back(kinoko);
			}
		}
	}
}

void search_length(cv::Mat &img, Kinoko kinoko){

}

void search_black(cv::Mat &img, std::vector<Kinoko> &kinoko){
	
	for (int idx = 0; idx < (img.cols * img.rows); idx++){
		if (img.data[idx] == BLACK) {
			Kinoko obj;
			kinoko.push_back(obj);
			search_length(img, kinoko.back());
		}
	}
}

//横幅を書く（たぶん使わない）
void write_line(cv::Mat &img, Kinoko kinoko){
	int y = (int)(kinoko.x[0] / img.cols);
	int x0 = kinoko.get_coordinateX(0, img.cols, y);
	int x1 = kinoko.get_coordinateX(1, img.cols, y);
	std::cout << "y = " << y << std::endl;
	std::cout << "x[0] = " << x0 << std::endl;
	std::cout << "x[1] = " << x1 << std::endl;
	//x[0],x[1]の値はidxなのでyで割る必要がある。
	cv::line(img, cv::Point(x0, y), cv::Point(x1, y), cv::Scalar(0, 0, 200), 3, 4);
}


int main(int argc, char *argy[]){
	//8, 43
	std::string fn = "C:\\file\\pos\\t_kinoko\\ (8).jpg";
	cv::Mat src_img = cv::imread(fn);
	cv::Mat original = cv::imread(fn);
	if (src_img.empty()){
		std::cout << "cannot open file" << std::endl;
		return -1;
	}

	for (int i = 0; i < 2; i++){
		//cv::GaussianBlur(src_img, src_img, cv::Size(5, 5), 10, 10);
		//オープニング
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
	cv::erode(canny_img, canny_img, cv::Mat(), cv::Point(-1, -1), 10);
	cv::dilate(canny_img, canny_img, cv::Mat(), cv::Point(-1, -1), 5);

	std::vector<Kinoko> obj;
	int max_idx = 0;
	int max_width = 0;
	//find_kinoko(canny_img, obj);
	/*失敗したこと。
	if(canny_img.data[idx] == BLACK && obj.back().x[0] == -1)}else{}と書いてしまったが、
	これだとelseに入ったときにcanny_img.data[idx] != BLACKを取ってしまうのでelseはちゃんと書くこと。
	get_width()を使うときはx[1]に値が入っていないととんでもない数字になるのでif文でチェックする。
	でも怖いので、後でget_width()を改良しておく。*/
	/*
	for (int i = 0; i < obj.size(); i++){
		if (obj[i].x[1] != -1){
			//std::cout << i << "番目の幅 = " << obj[i].get_width() << std::endl;
			if (obj[i].get_width() > max_width) {
				max_width = obj[i].get_width();
				max_idx = i;
			}
		}
	}
	*/
	//std::cout << "max_idx = " << max_idx << std::endl;
	//std::cout << "max_width = " << max_width << std::endl;

	//write_line(original, obj[max_idx]);

	cv::namedWindow("color", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
	cv::imshow("image", canny_img);
	cv::imshow("color", original);
	cv::waitKey(0);

	cv::imwrite("C:\\file\\result\\kinoko1\\43.jpg", canny_img);
	return 0;
}