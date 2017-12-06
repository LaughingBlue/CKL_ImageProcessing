// main.cpp : Defines the entry point for the console application.
//
#define DEBUG_FLAG 0

#include "stdafx.h"
#include "argh.h"
#include <opencv2/opencv.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv; 
using namespace argh;

static void showHelp(string);
Mat addNoise_saltPepper(const Mat&, float, float);
Mat DIY_GaussianBlur(const Mat&, const int&, const float&);

int main(int argc, char* argv[])
{
	auto cmdl = argh::parser(argc, argv);

#if DEBUG_FLAG == 1
	cout << '\n' << cmdl[0] << '\n' << cmdl[1] << '\n' << cmdl[2] << endl;
	return 0;
#endif

	if (argc < 3)
	{
		showHelp(argv[0]);
		return -1;
	}
	
	try {		
		Mat srcImg = imread("C:\\Users\\LaughingBlue\\Pictures\\hqdefault.jpg", 0);
		Mat srcImg = imread(cmdl[2], 0);
		Mat noiseImg = addNoise_saltPepper(srcImg, 0.05, 0.05);
		Mat deNoisingImg = DIY_GaussianBlur(noiseImg, 5, 0.8);
#if DEBUG_FLAG == 2
		GaussianBlur(addNoise_saltPepper(srcImg, 0.05, 0.05), deNoisingImg, Size(5, 5), 0.8, 0.8);
#endif
		if (cmdl[{ "-s", "--show" }])
		{
			imshow("[noise image]", noiseImg);
			imshow("[After GaussianBlur]", deNoisingImg);
		}
		else if (cmdl[{ "-w", "--write" }])
		{
			imwrite(cmdl[2]+"_addedNoise.jpg", noiseImg);
			imwrite(cmdl[2] + "_deNoising.jpg", deNoisingImg);
		}
		else
		{
			showHelp(argv[0]);
		}
	}
	catch (string msg)
	{
		cerr << msg << endl;
	}	
	catch (cv::Exception & e)
	{
		cerr << e.msg << endl;
	}
	waitKey(0);
	return 0;
}

static void showHelp(string name)
{
	cerr << "Usage: " << name << " <option(s)> SourceImage"
		 << "Options:\n"
		 << "\t-h,--help\tShow this help message\n"
		 << "\t-s,--show \tNo output files Show in windows\n"
		 << "\t-w,--write \tWrite process image as files"
		 << std::endl;
}

Mat addNoise_saltPepper(const Mat& srcImg, float pecentOfSalt, float pecentOfPepper)
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

Mat GetGaussianKernel(const int& size, const float& sigma)
{
	Mat kernel;
	const double PI = 4.0*atan(1.0); //¶ê©P²vpi½á­È  
	int center = size / 2;
	double sum = 0;
	for (int i = 0; i<size; i++)
	{
		for (int j = 0; j<size; j++)
		{
			kernel.at<uchar>(i,j) = (1 / (2 * PI*sigma*sigma))*exp(-((i - center)*(i - center) + (j - center)*(j - center)) / (2 * sigma*sigma));
			sum += kernel.at<uchar>(i, j);
		}
	}

	for (int i = 0; i<size; i++)
	{
		for (int j = 0; j<size; j++)
		{
			kernel.at<uchar>(i, j) /= sum;
		}
	}
	return kernel;
}

Mat DIY_GaussianBlur(const Mat& srcImg, const int& kernelSize, const float& sigma)
{
	Mat kernel = GetGaussianKernel(kernelSize, sigma);
	Mat denoiseImg;

	for (int j = 2; j<srcImg.rows - 2; j++)
	{
		for (int i = 2; i<srcImg.cols - 2; i++)
		{
			int sum = 0;
			int index = 0;
			for (int m = j - 2; m<j + 3; m++)
			{
				for (int n = i - 2; n<i + 3; n++)
				{
					sum += denoiseImg.at<uchar>(m, n) * kernel.at<uchar>(index++);
				}
			}
			denoiseImg.at<uchar>(j, i) = sum;
		}
	}

	return denoiseImg;
}