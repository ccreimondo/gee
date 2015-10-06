//#include "readPersonImg.h"
#include "getFeatureVector.h"
#include "RBML.h"
#include <iostream>

using namespace std;


//int main() {
//
//	std::vector<Mat> feature_a = ReadPersonImg("d:\\cam_a");
//	std::vector<Mat> feature_b = ReadPersonImg("d:\\cam_b");
//	Mat cam_a = GetTrainSet(feature_a);
//	Mat cam_b = GetTrainSet(feature_b);
//	std::cout << feature_a.size() << std::endl;
//	std::cout << cam_a.rows<<" "<< cam_a.cols<< std::endl;
//	std::cout << feature_b.size() << std::endl;
//	std::cout << cam_b.rows << " " << cam_b.cols << std::endl;
//
//	//std::cout << cam_a << std::endl;
//	//std::cout << cam_b << std::endl;
//
//	//存储数据
//	FileStorage fs("d:\\camera.xml", FileStorage::WRITE);
//	fs << "cam_a" << cam_a << "cam_b" << cam_b;
//	fs.release();
//
//	return 0;
//}
//
//int main() {
//	//读取训练集
//	FileStorage fs_cam("d:\\camera.xml", FileStorage::READ);
//	Mat trainSet_a, trainSet_b;
//	fs_cam["cam_a"] >> trainSet_a;
//	fs_cam["cam_b"] >> trainSet_b;
//	fs_cam.release();
//	cout << trainSet_a.rows << " " << trainSet_b.cols << endl;
//	cout << trainSet_b.rows << " " << trainSet_b.cols << endl;
//
//	//合并训练集
//	Mat trainSet(trainSet_a.rows, trainSet_a.cols + trainSet_b.cols, CV_32FC1);
//	Mat desttemp = trainSet.colRange(0, trainSet_a.cols);
//	trainSet_a.copyTo(desttemp);
//	desttemp = trainSet.colRange(trainSet_a.cols, trainSet_a.cols + trainSet_b.cols);
//	trainSet_b.copyTo(desttemp);
//	cout << trainSet.rows << " " << trainSet.cols << endl;
//	//std::cout << trainSet << std::endl;
//
//	//PCA降维
//	//int maxComponents = 100;//保留最大的主成分数
//	////构造pca数据结构
//	//PCA pca(trainSet, Mat(), CV_PCA_DATA_AS_COL, maxComponents);
//	//std::cout << pca.eigenvalues << std::endl;
//
//	////存储pca
//	//FileStorage fs_pca("d:\\PCA.xml", FileStorage::WRITE);
//	//fs_pca << "mean" << pca.mean << "eigenvalues" << pca.eigenvalues<<"eigenvectors"<<pca.eigenvectors;
//	//fs_pca.release();
//
//	//读取PCA
//	FileStorage fs_pca("d:\\PCA.xml", FileStorage::READ);
//	PCA pca;
//	fs_pca["mean"] >> pca.mean;
//	fs_pca["eigenvalues"] >> pca.eigenvalues;
//	fs_pca["eigenvectors"] >> pca.eigenvectors;
//	fs_pca.release();
//
//	Mat pcaTrainSet_a = pca.project(trainSet_a);
//	cout << pcaTrainSet_a.rows << " " << pcaTrainSet_a.cols << endl;
//	Mat pcaTrainSet_b = pca.project(trainSet_b);
//	cout << pcaTrainSet_b.rows << " " << pcaTrainSet_b.cols << endl;
//	Mat M = doRBML(pcaTrainSet_a, pcaTrainSet_b);
//	//存储M
//	FileStorage fs_M("d:\\M.xml", FileStorage::WRITE);
//	fs_M << "M" << M;
//	fs_M.release();
//	cout << M << endl;
//	return 0;
//}
float calcSimilarity(Mat x, Mat y, Mat M) {
	Mat m=(x - y).t()*M*(x - y);
	cout << m << endl;
	return m.at<float>(0,0);
}

//int main() {
//	读取PCA
//	FileStorage fs_pca("d:\\PCA.xml", FileStorage::READ);
//	PCA pca;
//	fs_pca["mean"] >> pca.mean;
//	fs_pca["eigenvalues"] >> pca.eigenvalues;
//	fs_pca["eigenvectors"] >> pca.eigenvectors;
//	fs_pca.release();
//
//	读取M
//	FileStorage fs_M("d:\\M.xml", FileStorage::READ);
//	Mat M;
//	fs_M["M"] >> M;
//	fs_M.release();
//
//	读取训练集
//	FileStorage fs_cam("d:\\camera.xml", FileStorage::READ);
//	Mat trainSet_a, trainSet_b;
//	fs_cam["cam_a"] >> trainSet_a;
//	fs_cam["cam_b"] >> trainSet_b;
//	fs_cam.release();
//
//	Mat pcaTrainSet_a = pca.project(trainSet_a);
//	cout << pcaTrainSet_a.rows << " " << pcaTrainSet_a.cols << endl;
//	Mat pcaTrainSet_b = pca.project(trainSet_b);
//	cout << pcaTrainSet_b.rows << " " << pcaTrainSet_b.cols << endl;
//
//	for (int i = 0; i < 50; ++i) {
//		calcSimilarity(pcaTrainSet_a.col(9), pcaTrainSet_b.col(i), M);
//	}
//
//	return 0;
//}






