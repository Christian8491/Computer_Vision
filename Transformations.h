#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

/* Call to many test samples */

#include "Common.h"
#include "bitmap_image.h"
#include "my_open_cv.h"
#include "rgb.h"
#include <omp.h>

#define N 5004
#define M 5000
#define P 2
#define RGB_ 3
#define KERNEL_SIZE 9 // matrix of 3 x 3 x 3 (x, y , color)

/* Load a file */
bitmap_image load_image(char* file_name) {

	bitmap_image image(file_name);

	if (!image) {
		printf("Error - Failed to open '%s'\n", file_name);
		system("pause");
		exit(EXIT_FAILURE);
	}

	return image;
}

/* Openg a single file .bmp and show its info */
void open_image() {

	bitmap_image image = load_image("../images/tiger.bmp");

	image.print_info();

	/* Load an show image - openCV */
	load_and_show_image("../images/tiger.bmp");

}

/* Copy a bmp file */
void copy_file() {

	bitmap_image image = load_image("../images/tiger.bmp");

	/* Load an show image - openCV */
	load_and_show_image("../images/tiger.bmp");

	image.save_image("../images/tiger_copy.bmp");
}


/* Count the number of pixels that have a red channel value of 111 and larger */
void count_red_colors(const int red) {

	bitmap_image image = load_image("../images/tiger.bmp");

	unsigned int total_number_of_pixels = 0;
	const unsigned int height = image.height();
	const unsigned int width = image.width();

	rgb color;

	for (size_t y = 0; y < height; ++y) {
		for (size_t x = 0; x < width; ++x) {
			image.get_pixel(x, y, color);

			if (color.red >= red) total_number_of_pixels++;
		}
	}

	printf("Number of pixels with red >= %d: %d\n", red, total_number_of_pixels);

	/* Load an show image - openCV */
	load_and_show_image("../images/tiger.bmp");

}


/* Create a padding in contourns */
void padding(const int pad_x, const int pad_y) {

	bitmap_image image = load_image("../images/tiger.bmp");

	const unsigned int height = image.height();
	const unsigned int width = image.width();
	const unsigned int pad_bottom = height - pad_x;
	const unsigned int pad_right = width - pad_x;

	rgb color;

	for (size_t y = 0; y < height; ++y) {
		for (size_t x = 0; x < width; ++x) {
			image.get_pixel(x, y, color);
			if (x < pad_x || x > pad_right || y < pad_y || y > pad_bottom) image.set_pixel(x, y, 0, 0, 0);
		}
	}

	image.save_image("../images/padding/tiger_padding.bmp");

	/* Load an show images (original and padding) - openCV */
	load_and_show_images("../images/tiger.bmp", "../images/padding/tiger_padding.bmp");
	
}

/* Ecualization: Save an image in binary (black an white color) */
void to_gray_scale() {

	bitmap_image image = load_image("../images/tiger.bmp");

	const unsigned int height = image.height();
	const unsigned int width  = image.width();
	const unsigned int middle = 255 / 2;

	rgb colour;

	for (size_t y = 0; y < height; ++y) {
		for (size_t x = 0; x < width; ++x) {
			image.get_pixel(x, y, colour);
			image.set_pixel(x, y, colour.red, colour.blue, colour.green - 250);
		}
	}

	image.save_image("../images/gray_scale/tiger_binary.bmp");

	/* Load an show images (original and gray scale) - openCV */
	load_and_show_images("../images/tiger.bmp", "../images/gray_scale/tiger_binary.bmp");

}

/* Convolution */
void test_convolution() {
	
	bitmap_image image = load_image("../images/tiger.bmp");

	float input[N][N];
	float output[N][N];

#   pragma omp for schedule(runtime)
	for (int i = P; i < M + P; i++) {
		for (int j = P; j < M + P; j++) {
			for (int m = i - P; m <= i + P; m++) {
				for (int n = j - P; n <= j + P; n++) {
					output[i][j] += input[m][n] * input[i + P - m][j + P - n];
				}
			}
		}
	}
}

/* Usefuls functions to apply filter of the median after some noises */
unsigned char find_median(vector<unsigned char>& canal) {
	
	unsigned char temp;
	
	for (size_t i = 0; i < KERNEL_SIZE - 1; i++) { 
		for (size_t j = 0; j < KERNEL_SIZE - i - 1; j++) {
			if (canal[j] > canal[j + 1]) {
				temp = canal[j];
				canal[j] = canal[j + 1];
				canal[j + 1] = temp; 
			} 
		} 
	}
	
	return canal[KERNEL_SIZE / 2];
}

vector<unsigned char> divide_in_canals(vector<unsigned char> kernel) {

	vector<unsigned char> medians(3);
	vector<unsigned char> red_canal(KERNEL_SIZE);
	vector<unsigned char> green_canal(KERNEL_SIZE);
	vector<unsigned char> blue_canal(KERNEL_SIZE);
	
	for (int i = 0; i < KERNEL_SIZE; i++) {
		blue_canal[i]  = kernel[RGB_ * i + 0];
		green_canal[i] = kernel[RGB_ * i + 1];
		red_canal[i]   = kernel[RGB_ * i + 2];
	}

	medians[0] = find_median(blue_canal);
	medians[1] = find_median(green_canal);
	medians[2] = find_median(red_canal);

	return medians;
}

bitmap_image filter_median(bitmap_image& image) {
	
	bitmap_image filter_image = image;

	const unsigned int height = image.height() - 1;
	const unsigned int width = image.width() - 1;

	rgb color;
	vector<unsigned char> kernel(KERNEL_SIZE * RGB_);
	vector<unsigned char> medians(RGB_);

	for (size_t y = 1; y < height; ++y) {
		for (size_t x = 1; x < width; ++x) {
					/* First row */
					image.get_pixel(x - 1, y - 1, color);
					kernel[RGB_ * 0 + 0] = color.blue;
					kernel[RGB_ * 0 + 1] = color.green;
					kernel[RGB_ * 0 + 2] = color.red;

					image.get_pixel(x, y - 1, color);
					kernel[RGB_ * 1 + 0] = color.blue;
					kernel[RGB_ * 1 + 1] = color.green;
					kernel[RGB_ * 1 + 2] = color.red;

					image.get_pixel(x + 1, y - 1, color);
					kernel[RGB_ * 2 + 0] = color.blue;
					kernel[RGB_ * 2 + 1] = color.green;
					kernel[RGB_ * 2 + 2] = color.red;

					/* Second row */
					image.get_pixel(x - 1, y, color);
					kernel[RGB_ * 3 + 0] = color.blue;
					kernel[RGB_ * 3 + 1] = color.green;
					kernel[RGB_ * 3 + 2] = color.red;

					image.get_pixel(x, y, color);
					kernel[RGB_ * 4 + 0] = color.blue;
					kernel[RGB_ * 4 + 1] = color.green;
					kernel[RGB_ * 4 + 2] = color.red;

					image.get_pixel(x + 1, y, color);
					kernel[RGB_ * 5 + 0] = color.blue;
					kernel[RGB_ * 5 + 1] = color.green;
					kernel[RGB_ * 5 + 2] = color.red;

					/* Third row */
					image.get_pixel(x - 1, y + 1, color);
					kernel[RGB_ * 6 + 0] = color.blue;
					kernel[RGB_ * 6 + 1] = color.green;
					kernel[RGB_ * 6 + 2] = color.red;

					image.get_pixel(x, y + 1, color);
					kernel[RGB_ * 7 + 0] = color.blue;
					kernel[RGB_ * 7 + 1] = color.green;
					kernel[RGB_ * 7 + 2] = color.red;

					image.get_pixel(x + 1, y + 1, color);
					kernel[RGB_ * 8 + 0] = color.blue;
					kernel[RGB_ * 8 + 1] = color.green;
					kernel[RGB_ * 8 + 2] = color.red;

					medians = divide_in_canals(kernel);
					filter_image.set_pixel(x, y, medians[2], medians[1], medians[0]);
		}
	}

	return filter_image;

}

bool is_white(int random) {
	if (random % 2 == 0) return 1;
	else return 0;
}

void noise(int percentage) {

	bitmap_image image = load_image("../images/tiger.bmp");

	const unsigned int height = image.height();
	const unsigned int width = image.width();

	int total_pixels = (height * width * percentage) / 100;

	int rand_x, rand_y, rand_none;

	rgb color_white(255, 255, 255);
	rgb color_black(0, 0, 0);

	for (int i = 0; i < total_pixels; ++i) {
		default_random_engine generator(time(0));
		uniform_int_distribution<int> distribution(0, width);
		rand_x = rand() % (width - 3);
		rand_y = rand() % (height - 3);
		if (is_white(rand_x)) image.set_pixel(rand_x + 1, rand_y + 1, color_white);
		else image.set_pixel(rand_x, rand_y, color_black);
	}

	image.save_image("../images/noises/tiger_noise.bmp");

	image = filter_median(image);

	image.save_image("../images/filter_median/tiger_median.bmp");

	/* Load an show images (original, noise imagen and image with filter median)*/
	load_and_show_images_3("../images/tiger.bmp", "../images/noises/tiger_noise.bmp", "../images/filter_median/tiger_median.bmp");
}


#endif  // TRANSFORMATIONS_H