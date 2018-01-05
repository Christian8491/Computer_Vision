#ifndef INITIALIZE_H
#define INITIALIZE_H

#include "common.h"
#include "my_open_cv.h"

void initialize() {
	/*

	string input;

	DIR *dir;
	dirent *ent;
	int cont = -1;
	vector<string> files;
	printf("Images, select a number:\n");
	if ((dir = opendir("../images")) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			if (cont > 0)
				printf("%d. %s\n", cont, ent->d_name);
			files.push_back(ent->d_name);
			cont++;
		}
		closedir(dir);
	}
	else {
		perror("");
		return;
	}

	files.erase(files.begin());
	files.erase(files.begin());

	int m = 0;
	while (m < 1 || m > 10){
		printf("\nPut the number of the image to processing:\t");
		scanf("%d", &m);
	}

	input = files[m - 1];

	ifstream instanceFiles;
	instanceFiles.open("instances\\" + input);

	if (instanceFiles.fail()) {
		cerr << "ERROR: Can not open input data file: " << endl;
		return;
	}

	cout << endl;

	*/

	int n;
	printf("Selected a transformation:\n");
	printf("1. Translation \n2. Rotation \n3. Scaling \n");
	printf("4. Gray Scale \n5. Equalization \n6. Convolution\n");
	printf("7. Show Image \n8. Filter median\n9. Fourier Transformation\n");
	printf("\n\nSelect a number: ");
	cin >> n;

	while (n < 1 || n > 9)  {
		printf("\nSelect a valid number: \n");
		cin >> n;
	}

	int x_offset, y_offset, size_kernel, size_noise;
	double angle, scale;

	if (n == 1) {
		printf("\nSelect offset in x: \n");
		cin >> x_offset;
		printf("\nSelect offset in y: \n");
		cin >> y_offset;
		translate(x_offset, y_offset);
	}

	else if (n == 2) {
		printf("\nSelect an angle: \n");
		cin >> angle;
		rotate(angle);
	}

	else if (n == 3) {
		printf("\nSelect a value for the scaling: \n");
		cin >> scale;
		scaling(scale);
	}

	else if (n == 4) gray_scale();

	else if (n == 5) equalization();

	else if (n == 6) {
		printf("\nSelect an size for the kernel matrix: {3, 5, 7, 9} \n");
		cin >> size_kernel;
		convolution(size_kernel);
	}

	else if (n == 7) open_image();

	else if (n == 8) {
		printf("\nSelect a value for the percentage of noise: {10, 15, 20} \n");
		cin >> size_noise;
		noise(size_noise);
	}

	else if (n == 9) fast_fourier_transform();
}


#endif