//This class is mainly for extract color histogram featureby following 
//the detailed settings:
//
//Each person image is represented by a 1152 - dimensional feature vector.
//Specifically, we divided the person image into six horizontal stripes.
//For each stripe, the RGB, YCbCr, HSV ,Lab color features  were extracted
//and represented as a histogram feature.
//We applied PCA to project the feature representation into a 
//100 - dimensional feature vector to remove the redundancy of the 
//high - dimensional feature space.
//
#pragma once
#ifndef GETFEATURE_H
#define GETFEATURE_H

#include <opencv2/opencv.hpp>

using namespace cv;

class GetFeature {
private:
	PCA pca;	//use to PCA

private:
	//creat mask to divided the person image into six horizontal stripes.
	Mat* creatMask(const Mat img)const;

	//For each stripe,extract RGB,HSV,YCbCr,Lab color feature.
	Mat* getRGBfeature(const Mat img, Mat* mask)const;
	Mat* getHSVfeature(const Mat img, Mat* mask)const;
	Mat* getYCbCrfeature(const Mat img, Mat* mask)const;
	Mat* getLabfeature(const Mat img, Mat* mask)const;

	//copy src to dst
	void cpyMat(Mat src, Mat dst, int& num)const;

	//do PCA 
	Mat doPCA(Mat feature)const;

public:
	GetFeature();
	~GetFeature(){}

	//extract feature vector of person image
	Mat getFeature(const Mat img)const;
};
#endif // GETFEATURE_H
