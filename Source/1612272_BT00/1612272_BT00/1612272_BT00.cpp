// 1612272_BT00.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "opencv2\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include <iostream>
using namespace std;
using namespace cv;
// Global variable
int brightnessValue = 0; // Luu gia tri brightness
int contrastValue = 1; // Luu gia tri contrast
// End
int ShowImage(char** argv)
{
	Mat image; // (1)
	image = imread(argv[1], IMREAD_COLOR); // (2)
	if (!image.data)
	{
		cout << "Khong the mo anh" << std::endl;
		return -1;
	}
	namedWindow("Display window", WINDOW_AUTOSIZE); // (3)
	imshow("Display window", image); // (4)
	waitKey(0);
	return 0;
}
int RGB2Grey(char* argv) 
{
	Mat input;
	input = imread(argv, IMREAD_COLOR);
	if (input.data == NULL)
	{
		cout << "Khong mo duoc anh" << endl;
		return -1;
	}
	// Lay dong va cot
	int row = input.rows;
	int col = input.cols;
	// Tao anh output
	Mat output(row, col, CV_8UC1);
	for (int i = 0; i < row; i++)
	{
		// Lay 2 con tro anh
		uchar* outputPointer = output.ptr<uchar>(i);
		uchar* inputPointer = input.ptr<uchar>(i);
		for (int j = 0; j < col; j++)
		{
			// Cong thuc chuyen doi Grey = 0.11B + 0.59G + 0.3R
			outputPointer[j] = uchar(inputPointer[0] * 0.11 + inputPointer[1] * 0.59 + inputPointer[2] * 0.3);
			// Tang con tro len 3
			inputPointer += 3;
		}
	}
	imwrite("output_RGB2Grey.png", output);
	namedWindow("Display window",WINDOW_AUTOSIZE);
	imshow("Display window", output);
	namedWindow("Original", WINDOW_AUTOSIZE);
	imshow("Original", input);
	waitKey(0);
	return 0;
}
void Brightness(int, void* argv)
{
	Mat input = imread((char*)argv, CV_LOAD_IMAGE_GRAYSCALE);
	if (input.data == NULL)
	{
		cout << "Khong the mo anh" << endl;
		return;
	}
	// Lay dong va cot
	int row = input.rows;
	int col = input.cols;
	// Tao anh output
	Mat output(row, col, CV_8UC1);
	for (int i = 0; i < row; i++)
	{
		// Lay con tro
		uchar* inputPointer = input.ptr<uchar>(i);
		uchar* outputPointer = output.ptr<uchar>(i);
		for (int j = 0; j < col; j++)
		{
			// Tang gia tri pixel len brightnessValue
			int temp = inputPointer[j] + brightnessValue;
			// Neu lon hon 255 thi gan bang 255 va nguoc lai
			outputPointer[j] = (temp >= 255) ? 255 : temp;
		}
	}
	imwrite("output_brightness.png", output);
	imshow("Display window", output);
}
void Contrast(int, void* argv)
{
	Mat input = imread((char*)argv, CV_LOAD_IMAGE_GRAYSCALE);
	if (input.data == NULL)
	{
		cout << "Khong the mo anh" << endl;
		return;
	}
	// Lay dong va cot
	int row = input.rows;
	int col = input.cols;
	// Tao anh output
	Mat output(row, col, CV_8UC1);
	for (int i = 0; i < row; i++)
	{
		// Lay con tro anh
		uchar* inputPointer = input.ptr<uchar>(i);
		uchar* outputPointer = output.ptr<uchar>(i);
		for (int j = 0; j < col; j++)
		{
			// Tang gia tri pixel contrastValue lan
			int temp = inputPointer[j] * contrastValue;
			// Neu lon hon 255 thi gan bang 255
			outputPointer[j] = (temp >= 255) ? 255 : temp;
		}
	}
	imwrite("output_contrast.png", output);
	imshow("Display window", output);
}
int main(int argc, char** argv)
{
	if (argc < 2)
	{
		cout << "Chuong trinh mo va hien thi anh" << endl;
		return -1;
	}
	else
	{
		switch (argc)
		{
		case 2:
			ShowImage(argv); 
			break;
		case 3:
			cout << "Dang chuyen ..." << endl;
			if(strcmp(argv[2],"RGB2Grey") == 0)
				RGB2Grey(argv[1]);
			else if (strcmp(argv[2], "Grey2RGB") == 0)
			{
				// Ta chỉ cần chỉ thị load ảnh màu vì chuyển từ Grey sang
				// RGB phải thực hiện lấy mẫu mới làm được
				Mat input = imread(argv[1], CV_LOAD_IMAGE_COLOR);
				imwrite("output_Grey2RGB.png", input);
				namedWindow("Original", WINDOW_AUTOSIZE);
				namedWindow("Display window", WINDOW_AUTOSIZE);
				imshow("Original", input);
				imshow("Display window", input);
				waitKey(0);
			}
			else
			{
				cout << "Ma lenh sai" << endl;
				break;
			}
			break;
		case 4:
			cout << "Dang chinh sua ..." << endl;
			if (strcmp(argv[2], "Brightness") == 0)
			{
				// Luu tham so
				int maxValue = atoi(argv[3]);
				// Tao window
				namedWindow("Display window",CV_GUI_EXPANDED);
				// Tao Trackbar
				createTrackbar("Brightness", "Display window", &brightnessValue, maxValue, Brightness, argv[1]);
				Brightness(brightnessValue, argv[1]);
				waitKey(0);
				break;
			}
			else if (strcmp(argv[2], "Contrast") == 0)
			{
				// Luu tham so
				int maxValue = atoi(argv[3]);
				// Tao window
				namedWindow("Display window", CV_GUI_EXPANDED);
				// Tao Trackbar
				createTrackbar("Contrast", "Display window", &contrastValue, maxValue, Contrast, argv[1]);
				Brightness(contrastValue, argv[1]);
				waitKey(0);
				break;
			}
			cout << "Ma lenh sai" << endl;
			break;
		default:
			cout << "Khong du tham so" << endl;
			break;
		}
	}	
	return 0;
}