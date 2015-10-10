#pragma once
#include <iostream>
#include <string>
#include "browsedir.h"
#include "getfeature.h"

std::vector<Mat> ReadPersonImg(const char* directory);
Mat GetTrainSet(std::vector<Mat> feature);
