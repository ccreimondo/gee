#include "GetFeature.h"

GetFeature::GetFeature() {
	//初始化pca
    FileStorage fs_pca("/media/reimondo/HDD/Workspace/Projects/gee/src/PCA.xml",
                       FileStorage::READ);
	fs_pca["mean"] >> pca.mean;
	fs_pca["eigenvalues"] >> pca.eigenvalues;
	fs_pca["eigenvectors"] >> pca.eigenvectors;
	fs_pca.release();
}

/************************************************************************
*功能描述：创建掩膜数组，方便对行人图片每个条带特征提取。
*参数：
*	img：行人图片
*返回：掩膜数组，分别对应从上到下6个水平条带。
**************************************************************************/
Mat* GetFeature::creatMask(const Mat img)const {
	Mat *mask = new Mat[6];

	for (int idx = 0; idx < 6; ++idx) {
		//mask元素初始化为0
		mask[idx] = Mat::zeros(img.rows, img.cols, CV_8UC1);
		//设置ROI
		for (int r = int(img.rows / 6)*idx; r < int(img.rows / 6)*(idx + 1); ++r) {
			for (int c = 0; c < img.cols; ++c) {
				mask[idx].at<uchar>(r, c) = 1;
			}
		}
	}

	//将由于取整操作而未能遍历到的像素点添加到第六个ROI中
	for (int r = int(img.rows / 6) * 6; r < img.rows; ++r) {
		for (int c = 0; c < img.cols; ++c) {
			mask[5].at<uchar>(r, c) = 1;
		}
	}

	return mask;
}

/************************************************************************
*功能描述：获取行人图片的RGB直方图特征，图片被平均分为6个水平条带，分别提取每个水平带各通道的直方图。
*参数：
*	img：行人图片
*	mask：方便提取六个水平条带特征的ROI掩膜
*返回：分别返回每个条带的RGB三个通道的直方图特征，共18个特征向量，每个特征向量维数为16。
**************************************************************************/
Mat* GetFeature::getRGBfeature(const Mat img, Mat* mask)const {
	Mat *feature = new Mat[18];

	//分割成3个单通道图像 ( R, G 和 B )
	Mat* rgb_planes = new Mat[3];
	split(img, rgb_planes);

	/// 设定bin数目 
	int histSize = 16;

	/// 设定取值范围 ( B,G,R)
	float range[] = { 0, 255 };
	const float* histRange = { range };

	bool uniform = true; bool accumulate = false;

	/// 分别计算六个条带RGB各通道直方图:
	for (int idx = 0; idx < 6; ++idx) {
		calcHist(&rgb_planes[0], 1, 0, mask[idx], feature[idx * 3 + 0], 1, &histSize, &histRange, uniform, accumulate);
		calcHist(&rgb_planes[1], 1, 0, mask[idx], feature[idx * 3 + 1], 1, &histSize, &histRange, uniform, accumulate);
		calcHist(&rgb_planes[2], 1, 0, mask[idx], feature[idx * 3 + 2], 1, &histSize, &histRange, uniform, accumulate);
	}

	return feature;
}

/************************************************************************
*功能描述：获取行人图片的HSV直方图特征，图片被平均分为6个水平条带，分别提取每个水平带各通道的直方图。
*参数：
*	img：行人图片
*返回：分别返回每个条带的HSV三个通道的直方图特征，共18个特征向量，每个特征向量维数为16。
**************************************************************************/
Mat* GetFeature::getHSVfeature(const Mat img, Mat* mask)const {
	Mat *feature = new Mat[18];
	Mat hsvImg;

	//转换色彩空间：BGR->HSV
	cvtColor(img, hsvImg, CV_BGR2HSV);

	//分割成3个单通道图像 
	Mat* hsv_planes = new Mat[3];
	split(hsvImg, hsv_planes);

	// 设定bin数目 
	int histSize = 16;

	// 设定取值范围
	float h_ranges[] = { 0, 180 };
	float s_ranges[] = { 0, 255 };
	float v_ranges[] = { 0, 255 };

	const float* histRanges[] = { h_ranges, s_ranges,v_ranges };

	bool uniform = true; bool accumulate = false;

	// 分别计算六个条带HSV各通道直方图:
	for (int idx = 0; idx < 6; ++idx) {
		calcHist(&hsv_planes[0], 1, 0, mask[idx], feature[idx * 3 + 0], 1, &histSize, &histRanges[0], uniform, accumulate);
		calcHist(&hsv_planes[1], 1, 0, mask[idx], feature[idx * 3 + 1], 1, &histSize, &histRanges[1], uniform, accumulate);
		calcHist(&hsv_planes[2], 1, 0, mask[idx], feature[idx * 3 + 2], 1, &histSize, &histRanges[2], uniform, accumulate);
	}

	return feature;
}

/************************************************************************
*功能描述：获取行人图片的YCrCb直方图特征，图片被平均分为6个水平条带，分别提取每个水平带各通道的直方图。
*参数：
*	img：行人图片
*	mask：方便提取六个水平条带特征的ROI掩膜
*返回：分别返回每个条带的YCrCb三个通道的直方图特征，共18个特征向量，每个特征向量维数为16。
**************************************************************************/
Mat* GetFeature::getYCbCrfeature(const Mat img, Mat* mask)const {
	Mat *feature = new Mat[18];
	Mat YCrCbImg;

	//转换色彩空间：BGR->YCrCb
	cvtColor(img, YCrCbImg, CV_BGR2YCrCb);

	//分割成3个单通道图像 
	Mat* YCrCb_planes = new Mat[3];
	split(YCrCbImg, YCrCb_planes);

	// 设定bin数目 
	int histSize = 16;

	// 设定取值范围
	float Y_ranges[] = { 0, 255 };
	float Cr_ranges[] = { 0, 255 };
	float Cb_ranges[] = { 0, 255 };

	const float* histRanges[] = { Y_ranges, Cr_ranges,Cb_ranges };

	bool uniform = true; bool accumulate = false;

	// 分别计算六个条带YCrCb各通道直方图:
	for (int idx = 0; idx < 6; ++idx) {
		calcHist(&YCrCb_planes[0], 1, 0, mask[idx], feature[idx * 3 + 0], 1, &histSize, &histRanges[0], uniform, accumulate);
		calcHist(&YCrCb_planes[1], 1, 0, mask[idx], feature[idx * 3 + 1], 1, &histSize, &histRanges[1], uniform, accumulate);
		calcHist(&YCrCb_planes[2], 1, 0, mask[idx], feature[idx * 3 + 2], 1, &histSize, &histRanges[2], uniform, accumulate);
	}

	return feature;
}

/************************************************************************
*功能描述：获取行人图片的LAB直方图特征，图片被平均分为6个水平条带，分别提取每个水平带各通道的直方图。
*参数：
*	img：行人图片
*	mask：方便提取六个水平条带特征的ROI掩膜
*返回：分别返回每个条带的LAB三个通道的直方图特征，共18个特征向量，每个特征向量维数为16。
**************************************************************************/
Mat* GetFeature::getLabfeature(const Mat img, Mat* mask)const {
	Mat *feature = new Mat[18];
	Mat LabImg;

	//转换色彩空间：BGR->Lab
	cvtColor(img, LabImg, CV_BGR2Lab);

	//分割成3个单通道图像 
	Mat* Lab_planes = new Mat[3];
	split(LabImg, Lab_planes);

	// 设定bin数目 
	int histSize = 16;

	// 设定取值范围
	float L_ranges[] = { 0, 255 };
	float a_ranges[] = { 0, 255 };
	float b_ranges[] = { 0, 255 };

	const float* histRanges[] = { L_ranges, a_ranges,b_ranges };

	bool uniform = true; bool accumulate = false;

	// 分别计算六个条带Lab各通道直方图:
	for (int idx = 0; idx < 6; ++idx) {
		calcHist(&Lab_planes[0], 1, 0, mask[idx], feature[idx * 3 + 0], 1, &histSize, &histRanges[0], uniform, accumulate);
		calcHist(&Lab_planes[1], 1, 0, mask[idx], feature[idx * 3 + 1], 1, &histSize, &histRanges[1], uniform, accumulate);
		calcHist(&Lab_planes[2], 1, 0, mask[idx], feature[idx * 3 + 2], 1, &histSize, &histRanges[2], uniform, accumulate);
	}

	return feature;
}

/************************************************************************
*功能描述：将一维向量src复制到dst的num序号后
*参数：
*	src：源向量
dst：目的向量
num：复制到的位置序号
*返回：无
**************************************************************************/
void GetFeature::cpyMat(Mat src, Mat dst, int& num)const {
	for (int idx = 0; idx < src.rows; idx++) {
		dst.at<float>(num++) = src.at<float>(idx);
	}
}

/************************************************************************
*功能描述：PCA降维
*参数：
*	feature：需要降维的特征矩阵
*返回：PCA降维后结果
**************************************************************************/
Mat GetFeature::doPCA(Mat feature)const {
	return pca.project(feature);
}

/************************************************************************
*功能描述：获取行人图片的特征向量，并进行降维
*参数：
*	img：行人图片
*返回：分别返回每个水平各颜色模型带各通道的直方图特征，6个条带，4种特征，每种特征3个通道，每个通道dims=16,共6*4*3*16=1152维。
**************************************************************************/
Mat GetFeature::getFeature(Mat img)const {

	Mat *mask = creatMask(img);

	//提取特征
	Mat *rgbFeature = getRGBfeature(img, mask);
	Mat *hsvFeature = getHSVfeature(img, mask);
	Mat *ycbcrFeature = getYCbCrfeature(img, mask);
	Mat *labFeature = getLabfeature(img, mask);

	//创建特征向量
	Mat feature = Mat(1152, 1, CV_32FC1);

	//将提取的直方图特征形成特征向量
	int count = 0;
	for (int stripe = 0; stripe < 6; ++stripe) {
		cpyMat(rgbFeature[stripe * 3 + 0], feature, count);
		cpyMat(rgbFeature[stripe * 3 + 1], feature, count);
		cpyMat(rgbFeature[stripe * 3 + 2], feature, count);
		cpyMat(hsvFeature[stripe * 3 + 0], feature, count);
		cpyMat(hsvFeature[stripe * 3 + 1], feature, count);
		cpyMat(hsvFeature[stripe * 3 + 2], feature, count);
		cpyMat(ycbcrFeature[stripe * 3 + 0], feature, count);
		cpyMat(ycbcrFeature[stripe * 3 + 1], feature, count);
		cpyMat(ycbcrFeature[stripe * 3 + 2], feature, count);
		cpyMat(labFeature[stripe * 3 + 0], feature, count);
		cpyMat(labFeature[stripe * 3 + 1], feature, count);
		cpyMat(labFeature[stripe * 3 + 2], feature, count);
	}
	Mat pcaFeature = doPCA(feature);

	delete[] mask;
	delete[] rgbFeature;
	delete[] hsvFeature;
	delete[] ycbcrFeature;
	delete[] labFeature;

	return pcaFeature;
}
