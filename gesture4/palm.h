

#ifndef PALM_H_  
#define PALM_H_ 
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <math.h>
#include<vector>
#include<stdint.h>
#include <stdio.h>
#include <winsock2.h>
#include"palm.h"
#include"define.h"
using namespace cv;
using namespace std;

pair<Point, double> DetectInCircles(vector<Point> contour, Mat src);
void Calculate_Moment(Mat src_gray);
double matchShape(Mat src, vector<Point> contour);
double Similar(double *hu1, double *hu2);
void socket_com(char sendline[], int length);
#endif