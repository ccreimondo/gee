#pragma once

#include <opencv2\opencv.hpp>
#include <iostream>
#include <cmath>

using namespace std;
using namespace cv;


Mat doRBML(Mat train_a , Mat train_b );
Mat Regularization(Mat sigama);
