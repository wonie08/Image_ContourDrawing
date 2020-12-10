#define _CRT_SECURE_NO_WARNINGS

#include "opencv2/opencv.hpp"
#include <iostream>
#include <direct.h> // getcwd
#include <vector>
#include <Windows.h>

using namespace cv;
using namespace std;

#ifndef _MAX
#define _MAX 260 // maxinum string length for file path
#endif


int main(void)
{
	// get current directory where the main.cpp file is located
	// https://shaeod.tistory.com/324
	char strBuffer[_MAX] = { 0, };
	char *current_directory = NULL;
	current_directory = _getcwd(strBuffer, _MAX);
	cout << "Current Work Path: " << current_directory << endl;

	// get Dataset directory
	char Dataset_path[_MAX] = { 0, };
	strcpy(Dataset_path, current_directory);
	strcat(Dataset_path, "\\Dataset\\*.*");
	cout << "Dataset_path: " << Dataset_path << endl;

	// get Dataset_result directory
	char Dataset_result_path[_MAX] = { 0, };
	strcpy(Dataset_result_path, current_directory);
	strcat(Dataset_result_path, "\\Dataset_result\\");
	cout << "Dataset_result_path: " << Dataset_result_path << endl << endl;
	
	// load a list of all files in the Dataset_path
	// https://bitwizx.tistory.com/26
	vector<string> file_names;
	glob(Dataset_path, file_names, true);
	//cout << "Dataset_path: " << Dataset_path << endl;


	// process ImageContourDrawing by taking out files one by one
	// https://anishdubey.com/find-contour-images-opencv
	for (int i = 0; i < file_names.size(); i++)
	{
		string file_name = file_names[i];
		cout << "Dataset " << i + 1 << ": " << file_name << endl;

		Mat src = imread(file_name, IMREAD_COLOR); // image file

		if (src.empty()) {
			cerr << "Image load failed!" << endl;
			return -1;
		}

		Mat gray;
		cvtColor(src, gray, COLOR_BGR2GRAY);
		Canny(gray, gray, 100, 200); // low critical value 100, high critical value 200

		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;

		RNG rng(12345); // Random Number Generator
		/*
			findContours: Extract contour from binary image
			RETR_TREE: Contour Extraction Mode
			CHAIN_APPROX_SIMPLE: Contour Approximation Method
		*/
		findContours(gray, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
		Mat drawing = Mat::zeros(gray.size(), CV_8UC3);

		for (int i = 0; i < contours.size(); i++)
		{
			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
		}

		// set the storage path for processed image
		// separate 'path' and 'file name'
		// https://jinha-world.blogspot.com/2009/04/c-stl-string-path.html
		string path, file;
		int nFind = file_name.rfind("\\") + 1;
		path = file_name.substr(0, nFind);
		file = file_name.substr(nFind, file_name.length() - nFind);
		String storage_path = String(Dataset_result_path) + String(file);

		imwrite(storage_path, drawing);
		cout << "Dataset" << i + 1 << " Completed processing and storage" << endl;

	}

	return 0;

}