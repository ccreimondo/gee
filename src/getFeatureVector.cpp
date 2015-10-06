#pragma once

#include "getFeatureVector.h"


/************************************************************************
*����������������Ĥ���飬���������ͼƬÿ������������ȡ��
*������
*	img������ͼƬ
*���أ���Ĥ���飬�ֱ��Ӧ���ϵ���6��ˮƽ������
**************************************************************************/
Mat* CreatMask(const Mat img) {
	Mat *mask = new Mat[6];

	for (int idx = 0; idx < 6; ++idx) {
		//maskԪ�س�ʼ��Ϊ0
		mask[idx] = Mat::zeros(img.rows, img.cols, CV_8UC1);
		//����ROI
		for (int r = int(img.rows / 6)*idx; r < int(img.rows / 6)*(idx + 1); ++r) {
			for (int c = 0; c < img.cols; ++c) {
				mask[idx].at<uchar>(r, c) = 1;
			}
		}
	}

	//������ȡ��������δ�ܱ����������ص���ӵ�������ROI��
	for (int r = int(img.rows / 6) * 6; r < img.rows; ++r) {
		for (int c = 0; c < img.cols; ++c) {
			mask[5].at<uchar>(r, c) = 1;
		}
	}

	return mask;
}

/************************************************************************
*������������ȡ����ͼƬ��RGBֱ��ͼ������ͼƬ��ƽ����Ϊ6��ˮƽ�������ֱ���ȡÿ��ˮƽ����ͨ����ֱ��ͼ��
*������
*	img������ͼƬ
*	mask��������ȡ����ˮƽ����������ROI��Ĥ
*���أ��ֱ𷵻�ÿ��������RGB����ͨ����ֱ��ͼ��������18������������ÿ����������ά��Ϊ16��
**************************************************************************/
Mat* GetRGBFeature(const Mat img, Mat* mask) {
	Mat *feature = new Mat[18];

	//�ָ��3����ͨ��ͼ�� ( R, G �� B )
	Mat* rgb_planes = new Mat[3];
	split(img, rgb_planes);

	/// �趨bin��Ŀ 
	int histSize = 16;

	/// �趨ȡֵ��Χ ( B,G,R)
	float range[] = { 0, 255 };
	const float* histRange = { range };

	bool uniform = true; bool accumulate = false;

	/// �ֱ������������RGB��ͨ��ֱ��ͼ:
	for (int idx = 0; idx < 6; ++idx) {
		calcHist(&rgb_planes[0], 1, 0, mask[idx], feature[idx * 3 + 0], 1, &histSize, &histRange, uniform, accumulate);
		calcHist(&rgb_planes[1], 1, 0, mask[idx], feature[idx * 3 + 1], 1, &histSize, &histRange, uniform, accumulate);
		calcHist(&rgb_planes[2], 1, 0, mask[idx], feature[idx * 3 + 2], 1, &histSize, &histRange, uniform, accumulate);
	}

	return feature;
}

/************************************************************************
*������������ȡ����ͼƬ��HSVֱ��ͼ������ͼƬ��ƽ����Ϊ6��ˮƽ�������ֱ���ȡÿ��ˮƽ����ͨ����ֱ��ͼ��
*������
*	img������ͼƬ
*���أ��ֱ𷵻�ÿ��������HSV����ͨ����ֱ��ͼ��������18������������ÿ����������ά��Ϊ16��
**************************************************************************/
Mat* GetHSVFeature(const Mat img, Mat* mask) {
	Mat *feature = new Mat[18];
	Mat hsvImg;

	//ת��ɫ�ʿռ䣺BGR->HSV
	cvtColor(img, hsvImg, CV_BGR2HSV);

	//�ָ��3����ͨ��ͼ�� 
	Mat* hsv_planes = new Mat[3];
	split(hsvImg, hsv_planes);

	// �趨bin��Ŀ 
	int histSize = 16;

	// �趨ȡֵ��Χ
	float h_ranges[] = { 0, 180 };
	float s_ranges[] = { 0, 255 };
	float v_ranges[] = { 0, 255 };

	const float* histRanges[] = { h_ranges, s_ranges,v_ranges };

	bool uniform = true; bool accumulate = false;

	// �ֱ������������HSV��ͨ��ֱ��ͼ:
	for (int idx = 0; idx < 6; ++idx) {
		calcHist(&hsv_planes[0], 1, 0, mask[idx], feature[idx * 3 + 0], 1, &histSize, &histRanges[0], uniform, accumulate);
		calcHist(&hsv_planes[1], 1, 0, mask[idx], feature[idx * 3 + 1], 1, &histSize, &histRanges[1], uniform, accumulate);
		calcHist(&hsv_planes[2], 1, 0, mask[idx], feature[idx * 3 + 2], 1, &histSize, &histRanges[2], uniform, accumulate);
	}

	return feature;
}

/************************************************************************
*������������ȡ����ͼƬ��YCrCbֱ��ͼ������ͼƬ��ƽ����Ϊ6��ˮƽ�������ֱ���ȡÿ��ˮƽ����ͨ����ֱ��ͼ��
*������
*	img������ͼƬ
*	mask��������ȡ����ˮƽ����������ROI��Ĥ
*���أ��ֱ𷵻�ÿ��������YCrCb����ͨ����ֱ��ͼ��������18������������ÿ����������ά��Ϊ16��
**************************************************************************/
Mat* GetYCbCrFeature(const Mat img, Mat* mask) {
	Mat *feature = new Mat[18];
	Mat YCrCbImg;

	//ת��ɫ�ʿռ䣺BGR->YCrCb
	cvtColor(img, YCrCbImg, CV_BGR2YCrCb);

	//�ָ��3����ͨ��ͼ�� 
	Mat* YCrCb_planes = new Mat[3];
	split(YCrCbImg, YCrCb_planes);

	// �趨bin��Ŀ 
	int histSize = 16;

	// �趨ȡֵ��Χ
	float Y_ranges[] = { 0, 255 };
	float Cr_ranges[] = { 0, 255 };
	float Cb_ranges[] = { 0, 255 };

	const float* histRanges[] = { Y_ranges, Cr_ranges,Cb_ranges };

	bool uniform = true; bool accumulate = false;

	// �ֱ������������YCrCb��ͨ��ֱ��ͼ:
	for (int idx = 0; idx < 6; ++idx) {
		calcHist(&YCrCb_planes[0], 1, 0, mask[idx], feature[idx * 3 + 0], 1, &histSize, &histRanges[0], uniform, accumulate);
		calcHist(&YCrCb_planes[1], 1, 0, mask[idx], feature[idx * 3 + 1], 1, &histSize, &histRanges[1], uniform, accumulate);
		calcHist(&YCrCb_planes[2], 1, 0, mask[idx], feature[idx * 3 + 2], 1, &histSize, &histRanges[2], uniform, accumulate);
	}

	return feature;
}

/************************************************************************
*������������ȡ����ͼƬ��LABֱ��ͼ������ͼƬ��ƽ����Ϊ6��ˮƽ�������ֱ���ȡÿ��ˮƽ����ͨ����ֱ��ͼ��
*������
*	img������ͼƬ
*	mask��������ȡ����ˮƽ����������ROI��Ĥ
*���أ��ֱ𷵻�ÿ��������LAB����ͨ����ֱ��ͼ��������18������������ÿ����������ά��Ϊ16��
**************************************************************************/
Mat* GetLabFeature(const Mat img, Mat* mask) {
	Mat *feature = new Mat[18];
	Mat LabImg;

	//ת��ɫ�ʿռ䣺BGR->Lab
	cvtColor(img, LabImg, CV_BGR2Lab);

	//�ָ��3����ͨ��ͼ�� 
	Mat* Lab_planes = new Mat[3];
	split(LabImg, Lab_planes);

	// �趨bin��Ŀ 
	int histSize = 16;

	// �趨ȡֵ��Χ
	float L_ranges[] = { 0, 255 };
	float a_ranges[] = { 0, 255 };
	float b_ranges[] = { 0, 255 };

	const float* histRanges[] = { L_ranges, a_ranges,b_ranges };

	bool uniform = true; bool accumulate = false;

	// �ֱ������������Lab��ͨ��ֱ��ͼ:
	for (int idx = 0; idx < 6; ++idx) {
		calcHist(&Lab_planes[0], 1, 0, mask[idx], feature[idx * 3 + 0], 1, &histSize, &histRanges[0], uniform, accumulate);
		calcHist(&Lab_planes[1], 1, 0, mask[idx], feature[idx * 3 + 1], 1, &histSize, &histRanges[1], uniform, accumulate);
		calcHist(&Lab_planes[2], 1, 0, mask[idx], feature[idx * 3 + 2], 1, &histSize, &histRanges[2], uniform, accumulate);
	}

	return feature;
}

/************************************************************************
*������������һά����src���Ƶ�dst��num��ź�
*������
*	src��Դ����
dst��Ŀ������
num�����Ƶ���λ�����
*���أ���
**************************************************************************/
void Matcpy(Mat src, Mat dst, int& num) {
	for (int idx = 0; idx < src.rows; idx++) {
		dst.at<float>(num++) = src.at<float>(idx);
	}
}

/************************************************************************
*������������ȡ����ͼƬ������������ͼƬ��ƽ����Ϊ6��ˮƽ�������ֱ���ȡÿ��ˮƽ����ɫģ�ʹ���ͨ����ֱ��ͼ��
*������
*	img������ͼƬ
*���أ��ֱ𷵻�ÿ��ˮƽ����ɫģ�ʹ���ͨ����ֱ��ͼ������6��������4��������ÿ������3��ͨ����ÿ��ͨ��dims=16,��6*4*3*16=1152ά��
**************************************************************************/
Mat GetFeatureVector(Mat img) {

	Mat *mask = CreatMask(img);

	//��ȡ����
	Mat *rgbFeature = GetRGBFeature(img, mask);
	Mat *hsvFeature = GetHSVFeature(img, mask);
	Mat *ycbcrFeature = GetYCbCrFeature(img, mask);
	Mat *labFeature = GetLabFeature(img, mask);

	//������������
	Mat feature = Mat(1152, 1, CV_32FC1);

	//����ȡ��ֱ��ͼ�����γ���������
	int count = 0;
	for (int stripe = 0; stripe < 6; ++stripe) {
		Matcpy(rgbFeature[stripe * 3 + 0], feature, count);
		Matcpy(rgbFeature[stripe * 3 + 1], feature, count);
		Matcpy(rgbFeature[stripe * 3 + 2], feature, count);
		Matcpy(hsvFeature[stripe * 3 + 0], feature, count);
		Matcpy(hsvFeature[stripe * 3 + 1], feature, count);
		Matcpy(hsvFeature[stripe * 3 + 2], feature, count);
		Matcpy(ycbcrFeature[stripe * 3 + 0], feature, count);
		Matcpy(ycbcrFeature[stripe * 3 + 1], feature, count);
		Matcpy(ycbcrFeature[stripe * 3 + 2], feature, count);
		Matcpy(labFeature[stripe * 3 + 0], feature, count);
		Matcpy(labFeature[stripe * 3 + 1], feature, count);
		Matcpy(labFeature[stripe * 3 + 2], feature, count);
	}

	return feature;

}
