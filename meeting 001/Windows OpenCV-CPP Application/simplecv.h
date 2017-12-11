#pragma once

#include "stdafx.h"

using namespace std;
using namespace cv;

Mat addNoise_saltPepper(const Mat&, double, double);
Mat DIY_GaussianBlur(const Mat&, const int&, const double&);

Mat addNoise_saltPepper(const Mat& srcImg, double pecentOfSalt, double pecentOfPepper)
{
	if (pecentOfSalt > 1 || pecentOfSalt < 0 || pecentOfPepper>1 || pecentOfPepper < 0)
	{
		string errMsg = "Invalid pepper&salt percentage input.";
		throw errMsg;
	}

	RNG rand;
	Mat returnImg = srcImg;

	for (int count = 0, expectSalt = (returnImg.rows*returnImg.cols*pecentOfSalt); count < expectSalt; ++count)
		returnImg.at<uchar>(rand.uniform(0, returnImg.rows), rand.uniform(0, returnImg.cols)) = 0;

	for (int count = 0, expectPepper = (returnImg.rows*returnImg.cols*pecentOfSalt); count < expectPepper; ++count)
		returnImg.at<uchar>(rand.uniform(0, returnImg.rows), rand.uniform(0, returnImg.cols)) = 255;

	return returnImg;
}

void GetGaussianKernel(double kernel[][5], const int& size, const double& sigma)
{
	double r, s = 2.0 * pow(sigma, 2);  // Assigning deviation
	double sum = 0.0;
	for (int x = -2; x <= 2; x++)
	{
		for (int y = -2; y <= 2; y++)
		{
			r = sqrt(x*x + y*y);
			kernel[x + 2][y + 2] = (exp(-(r*r) / s)) / (CV_PI * s);
			sum += kernel[x + 2][y + 2];
#if DEBUG_FLAG == 3
			cout << kernel[x + 2][y + 2] << '\t';
#endif
		}
#if DEBUG_FLAG == 3
		cout << endl;
#endif
	}
	for (int i = 0; i < 5; ++i) // Loop to normalize the kernel
	{
		for (int j = 0; j < 5; ++j)
		{
			kernel[i][j] /= sum;
#if DEBUG_FLAG == 3
			cout << kernel[i][j] << '\t';
#endif
		}
#if DEBUG_FLAG == 3
		cout << endl;
#endif
	}
}

Mat DIY_GaussianBlur(const Mat& srcImg, const int& kernelSize, const double& sigma)
{
	double kernel[5][5] = {};
	GetGaussianKernel(kernel, kernelSize, sigma);
	Mat denoiseImg = srcImg.clone();

	for (int j = 2; j<srcImg.rows - 2; j++)
	{
		for (int i = 2; i<srcImg.cols - 2; i++)
		{
			int sum = 0;
			for (int m = j - 2; m<j + 3; m++)
			{
				for (int n = i - 2; n<i + 3; n++)
				{
					sum += denoiseImg.at<uchar>(m, n) * kernel[m - j + 2][n - i + 2];
				}
			}
			denoiseImg.at<uchar>(j, i) = sum;
		}
	}
	return denoiseImg;
}
