// main.cpp : Defines the entry point for the console application.
//
#define DEBUG_FLAG 0 // 1: CMDL_TEST, 2: IMREAD_TEST, 3: KERNEL_WATCH

#include "stdafx.h"
#include "simplecv.h"
#include "argh.h"

using namespace std;
using namespace cv; 
using namespace argh;

static void showHelp(string);

int main(int argc, char* argv[])
{
	auto cmdl = argh::parser(argc, argv);

#if DEBUG_FLAG == 1
	cout << '\n' << cmdl[0] << '\n' << cmdl[1] << '\n' << cmdl[2] << endl;
#endif

	if (argc < 3)
	{
		showHelp(argv[0]);
		return -1;
	}
	
	try {		

#if DEBUG_FLAG == 2
		Mat srcImg = imread("C:\\Users\\LaughingBlue\\Pictures\\12-6-2017 02-36 PM Office Lens.jpg", 0);
		//cvtColor(srcImg, srcImg, COLOR_BGR2GRAY);
		imshow(".\\hqdefault.jpg", srcImg);
		GaussianBlur(addNoise_saltPepper(srcImg, 0.05, 0.05), deNoisingImg, Size(5, 5), 0.8, 0.8);
#endif

		Mat srcImg = imread(cmdl[1], 0);
		Mat noiseImg = addNoise_saltPepper(srcImg, 0.05, 0.05);
		Mat deNoisingImg = DIY_GaussianBlur(noiseImg, 5, 0.8);

		if (cmdl[{ "-s", "--show" }])
		{
			imshow("[noise image]", noiseImg);
			imshow("[After GaussianBlur]", deNoisingImg);
		}
		else if (cmdl[{ "-w", "--write" }])
		{
			imwrite(cmdl[1] + "_addedNoise.jpg", noiseImg);
			imwrite(cmdl[1] + "_deNoising.jpg", deNoisingImg);
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