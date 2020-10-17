/*
*┌────────────────────────────────────────────────┐
*│　描    述：根据原理编写程序，实现高斯模糊													   │
*│　作    者：牟峙桦|BestBlade																	   │
*│　版    本：2.0																			   	   │
*│　创建时间：2020.07.11																		   │
*└────────────────────────────────────────────────┘
*/

#include <iostream>
#include<opencv2/core/core.hpp>  
#include<opencv2/highgui/highgui.hpp>  
#include"opencv2/imgproc/imgproc.hpp"
#include <stdio.h>
#include <math.h>																				/*	exp()函数	*/
# define pi 3.1414926

using namespace std;
using namespace cv;


Mat mygaussianBlur(Mat img, Size temp, float sigma) {
	if (img.rows < temp.height || img.cols < temp.width) {
		cerr << "the size of template is not match the original picture" << endl;
	}

	if (temp.height % 2  == 0 || temp.width % 2 == 0) {
		cerr << "template is not 2N+1 size" << endl;
	}

	Mat res(img.rows, img.cols, img.type());
	int row_start = temp.height >> 1;
	int col_start = temp.width >> 1;
	float weight = 2 * pi * sigma * sigma;

	//生成高斯模板
	vector<vector<float> > mygaussianTemplate(temp.height, vector<float>(temp.width));
	float wij_sum = 0;
	for (int i = 0; i < temp.height; i++) {
		for (int j = 0; j < temp.width; j++) {
			int wi = i - row_start;
			int wj = j - col_start;
			mygaussianTemplate[i][j] = weight * exp(-(wi * wi + wj * wj) / (2 * (sigma * sigma)));
			wij_sum += mygaussianTemplate[i][j];
		}
	}

	for (int i = 0; i < temp.height; i++) {
		for (int j = 0; j < temp.width; j++) {
			mygaussianTemplate[i][j] /= wij_sum;
		}
	}
	//单通道图像
	if (res.channels() == 1) {
		for (int x = row_start; x < res.rows - row_start; x++) {
			for (int y = col_start; y < res.cols - col_start; y++) {
				float val = 0;

				for (int i = 0; i < temp.height; i++) {
					for (int j = 0; j < temp.width; j++) {
						val += img.at<uchar>(x - row_start + i, y - col_start + j) * mygaussianTemplate[i][j];
					}
				}

				res.at<uchar>(x, y) = uchar(val);
			}
		}
	}
	//三通道图像
	else {
		for (int c = 0; c < res.channels(); c++) {
			for (int x = row_start; x < res.rows - row_start; x++) {
				for (int y = col_start; y < res.cols - col_start; y++) {
					float val = 0;
					for (int i = 0; i < temp.height; i++) {
						for (int j = 0; j < temp.width; j++) {
							val += img.at<Vec3b>(x - temp.height / 2 + i, y - temp.width / 2 + j)[c] * mygaussianTemplate[i][j];
						}
					}

					res.at<Vec3b>(x, y)[c] = uchar(val);
				}
			}
		}
	}
	return res;
}

int main()
{
	Mat img = imread("C://Users//Chrysanthemum//Desktop//1.png",0);
	Mat res;
	Size temp(9, 9);
	double sigma = 1.4;
	
	res = mygaussianBlur(img,temp,sigma);										/*	自定义高斯模糊函数	*/

	imshow("原图", img);
	imshow("3x3,sigma = 1,平滑结果图", res);

	Mat result_pic_opencv_blur;
	GaussianBlur(img, result_pic_opencv_blur, temp, sigma, BORDER_DEFAULT);				/*	OPENCV自带高斯模糊	*/
	imshow("OPENCV GaussianBlur()结果图", result_pic_opencv_blur);

	waitKey(0);
	//return 0;
}
