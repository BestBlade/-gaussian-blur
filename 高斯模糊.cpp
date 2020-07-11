/*
*┌────────────────────────────────────────────────┐
*│　描    述：根据原理编写程序，实现高斯模糊													   │
*│　作    者：牟峙桦|BestBlade																	   │
*│　版    本：1.0																			   	   │
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

int Gaussian_blur(Mat origin_pic, Mat& result_pic, Size ksize, double sigma)
{
	int kw = ksize.width;
	int kh = ksize.height;

	result_pic = origin_pic.clone();

	int row = origin_pic.rows;
	int col = origin_pic.cols;
																								/*	简单判定图片参数是否正确	*/
	if ((row < kw) && (col < kh))
	{
		return -1;																				/*	图片尺寸过小	*/
	}
	else
		if ((kw % 2 == 0) || (kh % 2 == 0))
		{
			return -2;																			/*	平滑窗口尺寸错误	*/
		}
		else
		{
			int row_start = kw / 2;
			int col_start = kh / 2;

			for (int i = row_start; i < row - row_start; i++)
			{
				for (int j = col_start; j < col - col_start; j++)								/*	循环图像第(i,j)个像素	*/
				{
					double channel_r = { 0 };
					double channel_g = { 0 };
					double channel_b = { 0 };
					double wij_sum	 = { 0 };													/*	定义权重总和	*/

					for (int p = i - row_start; p <= i + row_start; p++)
					{
						for (int q = j - col_start; q <= j + col_start; q++)					/*	在第(i,j)像素周围的ksize(weight,height)个像素中计算权重总和	*/
						{
							int wi = p - i;														/*	计算当前像素对中心点(i,j)的相对坐标，下面同理	*/
							int wj = q - j;

							double wij = exp(-(wi * wi + wj * wj) / (2 * sigma * sigma));		/*	计算原始高斯权重	*/

							wij_sum += wij;														/*	得到权重总和	*/
						}		
					}

					for (int p = i - row_start; p <= i + row_start; p++)
					{
						for (int q = j - col_start; q <= j + col_start; q++)					/*	在第(i,j)像素周围的ksize(weight,height)个像素中计算总和	*/
						{
							int wi = p - i;														
							int wj = q - j;

							double w_ij = exp(-(wi * wi + wj * wj) / (2 * sigma * sigma)) / wij_sum;	/*	用每个点的权重除以权重总和以归一化	*/

							Vec3b pq = origin_pic.at<Vec3b>(p, q);
							channel_r += pq[2] * w_ij;											/*	各通道乘上高斯矩阵权重	*/
							channel_g += pq[1] * w_ij;
							channel_b += pq[0] * w_ij;
						}
					}
					result_pic.at<Vec3b>(i, j)[0] = saturate_cast<uchar>(channel_b);			/*	『saturate_cast』函数可以保护数据不溢出	*/
					result_pic.at<Vec3b>(i, j)[1] = saturate_cast<uchar>(channel_g);			/*	重组	*/
					result_pic.at<Vec3b>(i, j)[2] = saturate_cast<uchar>(channel_r);
				}
			}
			return 0;
		}
}

int main()
{


	Mat origin_pic = imread("C:/Users/Chrysanthemum/Desktop/客户端开发.JPG");
	Mat result_pic;
	Size ksize(3, 3);
	double sigma = 1;

	Gaussian_blur(origin_pic, result_pic, ksize, sigma);										/*	自定义高斯模糊函数	*/

	imshow("原图", origin_pic);
	imshow("3x3,sigma = 1,平滑结果图", result_pic);

	Mat result_pic_opencv_blur;
	GaussianBlur(origin_pic, result_pic_opencv_blur, ksize, sigma, BORDER_DEFAULT);				/*	OPENCV自带高斯模糊	*/
	imshow("OPENCV GaussianBlur()结果图", result_pic_opencv_blur);

	waitKey(0);
	//return 0;
}
