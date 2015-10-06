#include "RBML.h"


Mat doRBML(Mat train_a, Mat train_b) {
	//分别计算正负样本的协方差矩阵∑1和∑0
	Mat sigma1 = Mat::zeros(train_a.rows, train_a.rows, CV_32FC1);
	Mat sigma0 = Mat::zeros(train_a.rows, train_a.rows, CV_32FC1);

	for (int i = 0; i < train_a.cols; ++i) {
		sigma1 += (train_a.col(i) - train_b.col(i))*(train_a.col(i) - train_b.col(i)).t();
	}
	for (int i = 0; i < train_a.cols; ++i) {
		for (int j = 0; j < train_b.cols; ++j) {
			if (i != j) {
				sigma0+= (train_a.col(i) - train_b.col(j))*(train_a.col(i) - train_b.col(j)).t();
			}
		}
	}

	//cout << sigma1 << endl;
	//cout << sigma0 << endl;

	Mat regSigma1 = Regularization(sigma1);
	Mat regSigma0 = Regularization(sigma0);

	return regSigma1.inv() - regSigma0.inv();
}

float accumulation(Mat m, int n) {
	float sum = 0;
	for (int i = 0; i < n; ++i) {
		sum += m.at<float>(i, 0);
	}
	return sum;
}

Mat Regularization(Mat sigama) {
	SVD svd(sigama, SVD::FULL_UV);
	Mat s = svd.w.clone();

	//求90%能量的参数c
	int c = 1;
	for (; c <= s.rows; ++c) {
		if (accumulation(s, c) / accumulation(s,s.rows)>0.9) {
			break;
		}
	}
	float a = s.at<float>(0, 0)*s.at<float>(c - 1, 0)*(c - 1) / (s.at<float>(0, 0) - s.at<float>(c - 1, 0));
	float b = (c*s.at<float>(c - 1, 0) - s.at<float>(0, 0)) / (s.at<float>(0, 0) - s.at<float>(c - 1, 0));
	float K = a / (c + b) - pow(a / (b + c), 0.9);
	for (int i = 1; i <=s.rows; ++i) {
		if (i <= c)
			s.at<float>(i - 1, 0) = pow(a / (i + b), 0.9) + K;
		else
			s.at<float>(i - 1, 0) = a / (i + b);
	}
	
	//构建对角阵w
	Mat w=Mat::zeros(s.rows, s.rows, CV_32FC1);
	for (int i = 0; i < s.rows; ++i) {
		w.at<float>(i, i) = s.at<float>(i, 0);
	}
	return svd.u*w*svd.vt;
}
