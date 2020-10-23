/*
*┌────────────────────────────────────────────────┐
*│　描    述：根据原理编写程序，实现高斯模糊													   │
*│　作    者：牟峙桦|BestBlade																	   │
*│　版    本：3.0																			   	   │
*│　创建时间：2020.10.23																		   │
*└────────────────────────────────────────────────┘
*/

#include <iostream>
#include<opencv2/core/core.hpp>  
#include<opencv2/highgui/highgui.hpp>  
#include"opencv2/imgproc/imgproc.hpp"
#include <stdio.h>
#include <math.h>																				/*	exp()函数	*/
# define pi 3.1415926535898

using namespace std;
using namespace cv;


Mat mygaussianBlur(Mat img, Size temp, float sigma) {
	if ((temp.height % 2) == 0 || (temp.width % 2) == 0) {
		cerr << "the size of template is not 2M+1*2N+1" << endl;
	}
	vector<vector<float> >gaussiantemplate(temp.height, vector<float>(temp.width));
	float wij_sum = 0;
	int cx = temp.height / 2;
	int cy = temp.width / 2;
	for (int i = 0; i < gaussiantemplate.size(); i++) {
		for (int j = 0; j < gaussiantemplate[i].size(); j++) {
			float wij = exp(-((i - cx) * (i - cx) + (j - cy) * (j - cy)) / (2 * sigma * sigma)) / (2 * pi * sigma * sigma);
			wij_sum += wij;
			gaussiantemplate[i][j] = wij;
		}
	}
	for (int i = 0; i < gaussiantemplate.size(); i++) {
		for (int j = 0; j < gaussiantemplate[i].size(); j++) {
			gaussiantemplate[i][j] /= wij_sum;
		}
	}
	Mat dst(img.size(), img.type());
	for (int x = cx; x < img.rows - cx; x++) {
		for (int y = cy; y < img.cols - cy; y++) {
			float val = 0;
			for (int i = 0; i < gaussiantemplate.size(); i++) {
				for (int j = 0; j < gaussiantemplate[i].size(); j++) {
					val += img.at<uchar>(x - cx + i, y - cy + j) * gaussiantemplate[i][j];
				}
			}
			dst.at<uchar>(x, y) = uchar(val);
		}
	}
	return dst;
}

Mat minusPic(Mat img1, Mat img2) {
	Mat res(img1.size(), CV_8UC1);
	for (int x = 0; x < img1.rows; x++) {
		for (int y = 0; y < img1.cols; y++) {
			res.at<uchar>(x, y) = abs(img1.at<uchar>(x, y) - img2.at<uchar>(x, y));
		}
	}
	return res;
}
int main()
{
	Mat img = imread("C://Users//Chrysanthemum//Desktop//1.png",0);
	Mat res;
	Size temp(9, 9);
	double sigma = 5;
	
	res = mygaussianBlur(img,temp,sigma);										/*	自定义高斯模糊函数	*/

	imshow("原图", img);
	imshow("9*9,sigma = 5,平滑结果图", res);

	Mat result_pic_opencv_blur;
	GaussianBlur(img, result_pic_opencv_blur, temp, sigma);						/*	OPENCV自带高斯模糊	*/
	imshow("OPENCV GaussianBlur()结果图", result_pic_opencv_blur);

	//计算两图片差值
	Mat cha = minusPic(result_pic_opencv_blur , res);
	imshow("差值", cha);
	waitKey(0);
}
