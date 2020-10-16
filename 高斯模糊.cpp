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

using namespace std;
using namespace cv;

Mat mygaussianBlur(Mat img, Size temp, double sigma) {
	if (img.rows < temp.height || img.cols < temp.width) {
		cerr << "the size of template is not match the original picture" << endl;
	}
	if (temp.height >> 1 || temp.width >> 1) {
		cerr << "template is not 2N+1 size" << endl;
	}
	Mat res(img.rows, img.cols, img.type());
	int row_start = temp.height >> 1;
	int col_start = temp.width >> 1;

	if (res.channels() == 1) {
		for (int x = row_start; x < res.rows - row_start; x++) {
			for (int y = col_start; y < res.cols - col_start; y++) {
				//计算权重总和
				double wij_sum = 0;
				for (int p = x - row_start; p <= x + row_start; p++) {
					for (int q = y - col_start; q <= y + col_start; q++) {
						int wi = x - p;
						int wj = y - q;

						double wij = exp(-(wi * wi + wj * wj) / 2 * (sigma * sigma));
						wij_sum += wij;
					}
				}

				float val = 0;
				for (int p = x - row_start; p <= x + row_start; p++) {
					for (int q = y - col_start; q <= y + col_start; q++) {
						int wi = x - p;
						int wj = y - q;

						double wij = exp(-(wi * wi + wj * wj) / 2 * (sigma * sigma)) / wij_sum;

						val += img.at<uchar>(p, q) * wij;
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
					//计算权重总和
					double wij_sum = 0;
					for (int p = x - row_start; p <= x + row_start; p++) {
						for (int q = y - col_start; q <= y + col_start; q++) {
							int wi = x - p;
							int wj = y - q;

							double wij = exp(-(wi * wi + wj * wj) / 2 * (sigma * sigma));
							wij_sum += wij;
						}
					}

					float val = 0;
					for (int p = x - row_start; p <= x + row_start; p++) {
						for (int q = y - col_start; q <= y + col_start; q++) {
							int wi = x - p;
							int wj = y - q;

							double wij = exp(-(wi * wi + wj * wj) / 2 * (sigma * sigma)) / wij_sum;

							val += img.at<Vec3b>(p, q)[c] * wij;
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
	Mat origin_pic = imread("C://Users//Chrysanthemum//Desktop//1.png");
	Mat result_pic;
	Size ksize(3, 3);
	double sigma = 1;

	Mat res = mygaussianBlur(origin_pic,ksize,sigma);										/*	自定义高斯模糊函数	*/

	imshow("原图", origin_pic);
	imshow("3x3,sigma = 1,平滑结果图", res);

	Mat result_pic_opencv_blur;
	GaussianBlur(origin_pic, result_pic_opencv_blur, ksize, sigma, BORDER_DEFAULT);				/*	OPENCV自带高斯模糊	*/
	imshow("OPENCV GaussianBlur()结果图", result_pic_opencv_blur);

	waitKey(0);
	//return 0;
}
