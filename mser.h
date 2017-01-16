#pragma once
#include "opencv.h"
std::vector<cv::Rect> mserGetPlate(cv::Mat srcImage);
std::vector<cv::Rect> combineRegion(std::vector<cv::Rect> vRctSrc);
bool checkCombine(cv::Rect rct1, cv::Rect rct2, int nFlag);
void combine(cv::Rect &rct1, cv::Rect &rct2);


/*
int delta; //! delta, in the code, it compares (size_{i}-size_{i-delta})/size_{i-delta}
int maxArea; //! prune the area which bigger than maxArea
int minArea; //! prune the area which smaller than minArea
float maxVariation; //! prune the area have simliar size to its children
float minDiversity; //! trace back to cut off mser with diversity < min_diversity

The next few params for MSER of color image :
int maxEvolution; //! for color image, the evolution steps
double areaThreshold; //! the area threshold to cause re-initialize
double minMargin; //! ignore too small margin
int edgeBlurSize; //! the aperture size for edge blur
*/