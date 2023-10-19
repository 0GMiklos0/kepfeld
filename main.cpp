#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

int szures() {
	Mat img = imread("kep.png");
	imshow("kep", img);
	waitKey(0);
	return 0;
}

int circles() {
	Mat img = imread("go2.png", IMREAD_GRAYSCALE), edges;
	Mat gray;
	cvtColor(img, gray, COLOR_BGR2GRAY);

	//GaussianBlur(img, img, Size(5, 5),1.0);
	vector<Vec3f> circles;
	//HoughCircles(gray, circles, HOUGH_GRADIENT, 2, 10, 80, 50, 22, 28);
	HoughCircles(gray, circles, HOUGH_GRADIENT_ALT, 2, 10, 80, 0.5, 22, 28);

	medianBlur(gray, gray, 3);
	for (Vec3b c : circles) {
		if (gray.at<uchar>(c[1],c[0]) < 80)
			circle(img, Point(c[0], c[1]), c[2], Scalar(0, 0, 255), 2);
		else if (gray.at<uchar>(c[1],c[0])>160)
			circle(img, Point(c[0], c[1]), c[2], Scalar(255, 0, 0), 2);

	}

	imshow("korok", img);
	waitKey();

}

int canny() {
	Mat img = imread("go2.png", IMREAD_GRAYSCALE), edges;
	cv::Canny(img, edges, 60, 90);

	Mat dx, dy, d, d2;
	Sobel(img, dx, CV_32F, 1, 0);
	Sobel(img, dy, CV_32F, 0, 1);

	cv::sqrt(dx.mul(dx) + dy.mul(dy), d);
	convertScaleAbs(d, d2);
	imshow("sobel gradiens", dy / 256);

	imshow("CANNY", edges);
	waitKey();
}

int elkereses3() {
	Mat img = imread("edges_noise.png", IMREAD_GRAYSCALE);
	//GaussianBlur(img, img, Size(5, 5),1.0);
	medianBlur(img, img, 3);
	imshow("img", img);
	Mat d, d2;
	cv::Laplacian(img, d, CV_16S, 3); //a 3 a gradiens számításhoz használt kernel
	convertScaleAbs(d, d);
	imshow("gradient", d);
	threshold(d, d2, 30, 255, THRESH_BINARY);
	imshow("edges", d2);
	waitKey();
}

int elkereses2() {
	Mat img = imread("go2.png", IMREAD_GRAYSCALE);
	Mat dx, dy, d1, d2;
	Sobel(img, dx, CV_32F, 1, 0);
	Sobel(img, dy, CV_32F, 0, 1);
	cv::sqrt(dx.mul(dx) + dy.mul(dy), d1);
	convertScaleAbs(d1, d2);
	imshow("sobel gradiens", dy / 256);
	cout << threshold(d2, d2, 30, 255, THRESH_OTSU);
	imshow("edges", d2);
	waitKey();
}

int elkereses() {
	
	Mat img = imread("go2.png",IMREAD_GRAYSCALE);
	Mat G1 = Mat::zeros(img.size(), CV_16S); //short
	Mat G2 = Mat::zeros(img.size(), CV_16S);
	
	for (int i = 0; i < img.rows-1; i++) {
		for (int j = 0; j < img.cols-1; j++) {
			G1.at<short>(i, j) = img.at<uchar>(i, j) - img.at<uchar>(i + 1, j+1);
			G2.at<short>(i, j) = img.at<uchar>(i+1, j) - img.at<uchar>(i , j + 1);
		}
	}
	Mat G = cv::abs(G1) + cv::abs(G2), dest;
	convertScaleAbs(G, dest);
	threshold(dest, dest, 30, 255, THRESH_OTSU);
	imshow("dest", dest);
	
	waitKey();

	return 0;
}

int main() {
	circles();
}
