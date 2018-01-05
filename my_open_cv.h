#ifndef MY_OPEN_CV_H
#define MY_OPEN_CV_H

#include "common.h"

/* Load an image, then show */
void load_and_show_image(char* file_name) {

	/* Load an image */
	Mat src = imread(file_name, IMREAD_COLOR);
	if (src.empty()) {
		cout << "Not image!" << endl;
		return;
	}

	/* Showing image */
	namedWindow("Initial Image", WINDOW_AUTOSIZE);
	imshow("Initial Image", src);

	waitKey(0);
}

/* Load images (2), then show */
void load_and_show_images(char* file_name, char* file_name2) {

	/* Load images */
	Mat src = imread(file_name, IMREAD_COLOR);
	Mat src2 = imread(file_name2, IMREAD_COLOR);

	if (src.empty()) {
		cout << "Not find first image!" << endl;
		return;
	}

	if (src2.empty()) {
		cout << "Not find second image!" << endl;
		return;
	}

	/* Showing images */
	namedWindow("Initial Image", WINDOW_AUTOSIZE);
	imshow("Initial Image", src);

	namedWindow("Final Image", WINDOW_AUTOSIZE);
	imshow("Final Image", src2);

	waitKey(0);
}


/* Load images (3), then show */
void load_and_show_images_3(char* file_name, char* file_name2, char* file_name3) {

	/* Load images */
	Mat src = imread(file_name, IMREAD_COLOR);
	Mat src2 = imread(file_name2, IMREAD_COLOR);
	Mat src3 = imread(file_name3, IMREAD_COLOR);

	if (src.empty()) {
		cout << "Not find first image!" << endl;
		return;
	}

	if (src2.empty()) {
		cout << "Not find second image!" << endl;
		return;
	}

	if (src3.empty()) {
		cout << "Not find second image!" << endl;
		return;
	}

	/* Showing images */
	namedWindow("Initial Image", WINDOW_AUTOSIZE);
	imshow("Initial Image", src);

	namedWindow("Noise Image", WINDOW_AUTOSIZE);
	imshow("Noise Image", src2);

	namedWindow("Filter median", WINDOW_AUTOSIZE);
	imshow("Filter median", src3);

	waitKey(0);
}

void translate(int x_ofset, int y_ofset) {

	/* Load an image */
	Mat src = imread("../images/tiger.bmp", IMREAD_COLOR);
	if (src.empty()) {
		cout << "Not image!" << endl;
		return;
	}

	/* Three points initial (src) and final (dst) */
	Point2f srcTri[3];
	Point2f dstTri[3];
	Mat translate_mat(2, 3, CV_32FC1);
	Mat translate_dst;

	/* Set the dst (resulting) image the same type and size as src */
	translate_dst = Mat::zeros(src.rows, src.cols, src.type());

	/* Set your 3 points to calculate the  Affine Transform */
	srcTri[0] = Point2f(0, 0);
	srcTri[1] = Point2f(src.cols - 1, 0);
	srcTri[2] = Point2f(0, src.rows - 1);

	dstTri[0] = Point2f(x_ofset, y_ofset);
	dstTri[1] = Point2f(src.cols - 1 + x_ofset, y_ofset);
	dstTri[2] = Point2f(x_ofset, src.rows + y_ofset);

	/* Get the Affine Transform */
	translate_mat = getAffineTransform(srcTri, dstTri);

	/* Applying the Affine Transform just found to the src image */
	warpAffine(src, translate_dst, translate_mat, translate_dst.size());

	/* Showing images */
	namedWindow("Initial Image", WINDOW_AUTOSIZE);
	imshow("Initial Image", src);

	namedWindow("Translate Image", WINDOW_AUTOSIZE);
	imshow("Translate Image", translate_dst);

	/* Save the image and wait untill user exits the program */
	imwrite("../images/translation/tiger_trans.bmp", translate_dst);
	waitKey(0);
}


void rotate(double angle) {

	/* Load an image */
	Mat src = imread("../images/tiger.bmp", IMREAD_COLOR);
	if (src.empty()) {
		cout << "Not image!" << endl;
		return;
	}

	Point2f srcTri[3];
	Point2f dstTri[3];

	Mat rot_mat(2, 3, CV_32FC1);
	Mat warp_mat(2, 3, CV_32FC1);
	Mat warp_dst, warp_rotate_dst;

	/// Set the dst image the same type and size as src
	warp_dst = Mat::zeros(src.rows, src.cols, src.type());

	/// Set your 3 points to calculate the  Affine Transform
	srcTri[0] = Point2f(0, 0);
	srcTri[1] = Point2f(src.cols - 1, 0);
	srcTri[2] = Point2f(0, src.rows - 1);

	dstTri[0] = Point2f(0, 0);
	dstTri[1] = Point2f(src.cols - 1, 0);
	dstTri[2] = Point2f(0, src.rows - 1);

	/// Get the Affine Transform
	warp_mat = getAffineTransform(srcTri, dstTri);

	/// Apply the Affine Transform just found to the src image
	warpAffine(src, warp_dst, warp_mat, warp_dst.size());

	/** Rotating the image after Warp */

	/// Compute a rotation matrix with respect to the center of the image
	Point center = Point(warp_dst.cols / 2, warp_dst.rows / 2);
	double scale = 1.0;

	/// Get the rotation matrix with the specifications above
	rot_mat = getRotationMatrix2D(center, angle, scale);

	/// Rotate the warped image
	warpAffine(warp_dst, warp_rotate_dst, rot_mat, warp_dst.size());

	/* Display results */
	namedWindow("Initial Image", WINDOW_AUTOSIZE);
	imshow("Initial Image", src);

	namedWindow("Rotate Image", WINDOW_AUTOSIZE);
	imshow("Rotate Image", warp_rotate_dst);

	/* Save the image and wait untill user exits the program */
	imwrite("../images/rotation/tiger_rot.bmp", warp_rotate_dst);
	waitKey(0);
}

void scaling(double scale) {

	/* Load an image */
	Mat src = imread("../images/tiger.bmp", IMREAD_COLOR);
	if (src.empty()) {
		cout << "Not image!" << endl;
		return;
	}

	Point2f srcTri[3];
	Point2f dstTri[3];

	Mat rot_mat(2, 3, CV_32FC1);
	Mat warp_mat(2, 3, CV_32FC1);
	Mat warp_dst, warp_rotate_dst;

	/// Set the dst image the same type and size as src
	warp_dst = Mat::zeros(src.rows * scale , src.cols * scale , src.type());

	/// Set your 3 points to calculate the  Affine Transform
	srcTri[0] = Point2f(0, 0);
	srcTri[1] = Point2f(src.cols - 1, 0);
	srcTri[2] = Point2f(0, src.rows - 1);

	dstTri[0] = Point2f(0, 0);
	dstTri[1] = Point2f(src.cols - 1, 0);
	dstTri[2] = Point2f(0, src.rows - 1);

	/// Get the Affine Transform
	warp_mat = getAffineTransform(srcTri, dstTri);

	/// Apply the Affine Transform just found to the src image
	warpAffine(src, warp_dst, warp_mat, warp_dst.size());

	/** Rotating the image after Warp */

	/// Compute a rotation matrix with respect to the center of the image
	Point center = Point(warp_dst.cols / 2, warp_dst.rows / 2);
	double angle = 0.0;

	/// Get the rotation matrix with the specifications above
	rot_mat = getRotationMatrix2D(center, angle, scale);

	/// Rotate the warped image
	warpAffine(warp_dst, warp_rotate_dst, rot_mat, warp_dst.size());

	/* Display results */
	namedWindow("Initial Image", WINDOW_AUTOSIZE);
	imshow("Initial Image", src);

	namedWindow("Scale Image", WINDOW_AUTOSIZE);
	imshow("Scale Image", warp_rotate_dst);

	/* Save the image and wait untill user exits the program */
	imwrite("../images/scaling/tiger_scal.bmp", warp_rotate_dst);
	waitKey(0);
}

/* Convert a RGB image to grayscale */
void gray_scale() {

	/* Load an image */
	Mat img_src = imread("../images/tiger.bmp", IMREAD_COLOR);
	Mat img_gray;
	if (img_src.empty()) {
		cout << "Not image!" << endl;
		return;
	}

	/* Convert to grayscale and apply Histogram Equalization */
	cvtColor(img_src, img_gray, CV_BGR2GRAY);

	/* Display results */
	namedWindow("Initial Image", CV_WINDOW_AUTOSIZE);
	imshow("Initial Image", img_src);

	namedWindow("Gray scale Image", CV_WINDOW_AUTOSIZE);
	imshow("Gray scale Image", img_gray);

	/* Save the image and wait untill user exits the program */
	imwrite("../images/gray_scale/tiger_gray.bmp", img_gray);
	waitKey(0);
}

/* Apply Histogram Equalization */
void equalization() {

	/* Load an image */
	Mat img_src = imread("../images/tiger.bmp", IMREAD_COLOR);
	Mat img_gray, img_equaliz;

	if (img_src.empty()) {
		cout << "Not image!" << endl;
		return;
	}

	/* Convert to grayscale and apply Histogram Equalization */
	cvtColor(img_src, img_gray, CV_BGR2GRAY);
	equalizeHist(img_gray, img_equaliz);

	/* Display results */
	namedWindow("Initial Image", CV_WINDOW_AUTOSIZE);
	imshow("Initial Image", img_src);

	namedWindow("Gray scale Image", CV_WINDOW_AUTOSIZE);
	imshow("Gray scale Image", img_gray);

	namedWindow("Equalizate Image", CV_WINDOW_AUTOSIZE);
	imshow("Equalizate Image", img_equaliz);

	/* Save the image and wait untill user exits the program */
	imwrite("../images/equalization/tiger_eq.bmp", img_equaliz);
	waitKey(0);
}

/* Apply Convolution */
void convolution(int kernel_size) {

	/* Load an image */
	Mat img_src = imread("../images/tiger.bmp", IMREAD_COLOR);
	Mat img_conv;

	if (img_src.empty()) {
		cout << "Not image!" << endl;
		return;
	}

	Mat kernel;
	Point anchor = Point(-1, -1);
	double delta = 0.0;
	int ddepth = -1;

	/* Update kernel size for a normalized box filter */
	kernel = Mat::ones(kernel_size, kernel_size, CV_32F) / (float)(kernel_size * kernel_size);

	/* Apply filter (convolution) */
	filter2D(img_src, img_conv, ddepth, kernel, anchor, delta, BORDER_DEFAULT);

	/* Display results */
	namedWindow("Initial Image", CV_WINDOW_AUTOSIZE);
	imshow("Initial Image", img_src);

	namedWindow("Convolution Matrix", CV_WINDOW_AUTOSIZE);
	imshow("Convolution Matrix", img_conv);

	/* Save the image and wait untill user exits the program */
	imwrite("../images/convolution/tiger_conv.bmp", img_conv);
	waitKey(0);
}

void fast_fourier_transform() {

	/* Load an image */
	Mat img_src = imread("../images/woman.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	//Mat img_conv;

	if (img_src.empty()) {
		cout << "Not image!" << endl;
		return;
	}

	Mat padded;                            //expand input image to optimal size
	int m = getOptimalDFTSize(img_src.rows);
	int n = getOptimalDFTSize(img_src.cols); // on the border add zero values
	copyMakeBorder(img_src, padded, 0, m - img_src.rows, 0, n - img_src.cols, BORDER_CONSTANT, Scalar::all(0));

	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
	Mat complexI;
	merge(planes, 2, complexI);         // Add to the expanded another plane with zeros

	dft(complexI, complexI);            // this way the result may fit in the source matrix

	// compute the magnitude and switch to logarithmic scale
	// => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
	split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
	magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
	Mat magI = planes[0];

	magI += Scalar::all(1);                    // switch to logarithmic scale
	log(magI, magI);

	// crop the spectrum, if it has an odd number of rows or columns
	magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

	// rearrange the quadrants of Fourier image  so that the origin is at the image center
	int cx = magI.cols / 2;
	int cy = magI.rows / 2;

	Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
	Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
	Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
	Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

	Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
	q2.copyTo(q1);
	tmp.copyTo(q2);

	normalize(magI, magI, 0, 1, CV_MINMAX); // Transform the matrix with float values into a
	// viewable image form (float between values 0 and 1).

	imshow("Input Image", img_src);    // Show the result
	imshow("Fourier Transform", magI);
	waitKey(0);

}

#endif