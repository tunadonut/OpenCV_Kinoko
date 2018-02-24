#ifndef otsu_binarization_HPP
#define otsu_binarization_HPP

#include <cmath>

void cal_ave(int hist[256], int start, int end, double &m, int &omega){
	double ave = 0.0;
	double sum = 0.0;
	omega = 0;
	for (int i = start; i < end; i++){
		//‹P“x’l‚Ì‡Œv
		sum += hist[i] * i;
		//‰æ‘f”‚Ì‡Œv
		omega += hist[i];
	}

	ave = sum / (double)omega;
	m = ave;
	//std::cout << "m = " << m << std::endl;
}

int otsu_binari(int hist[256], int start, int end){
	int omega1, omega2;
	int threshold = 0;
	double m1, m2;
	double reS;
	double max_reS = 0.0;

	for (int t = 0; t < 256; t++){
		cal_ave(hist, 0, t, m1, omega1);
		cal_ave(hist, t, 256, m2, omega2);

		reS = omega1 * omega2 * std::pow(m1 - m2, 2.0);

		if (reS > max_reS) {
			max_reS = reS;
			threshold = t;
		}
	}

	return threshold;
}

#endif //otsu_binarization_HPP