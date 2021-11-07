// Moroiu Alexandra

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "file_utilities.h"
#include "operations.h"

// LOAD image
void load(char *file,
		  image *img,
		  int *check_load,
		  int *min_x,
		  int *min_y,
		  int *max_x,
		  int *max_y)
{
	FILE *in = fopen(file, "r");
	// check if file exists
	if (!in) {
		printf("Failed to load %s\n", file);
		*check_load = 0;
	} else {
		// check if an image was already loaded
		if (*check_load && *min_x != -1)
			free_mem(*img);
		img = read_img(in, img);
		printf("Loaded %s\n", file);
		// set maximum coordinates
		reset(img->width, img->height, min_x, min_y, max_x, max_y);
		fclose(in);
	}
}

// SELECT
void selection(char *sel_x,
			   image img,
			   int *min_x,
			   int *min_y,
			   int *max_x,
			   int *max_y,
			   int *check_sel,
			   int check_load)
{
	int value[5], count = 1, valid = 0;
	char coord[50];

	if (!strcmp(sel_x, "ALL") && check_load) {
		reset(img.width, img.height, min_x, min_y, max_x, max_y);
		printf("Selected ALL\n");

	} else if (strcmp(sel_x, "ALL")) {
		int size = strlen(sel_x);

		// check 1st coordinate
		if (check_nr(sel_x, size))
			valid++;
		// convert to integer
		value[0] = atoi(sel_x);

		// read the other coordinates
		fgets(coord, 50, stdin);
		char *pixel;

		// extract coordoninates
		pixel = strtok(coord, " ");
		while (pixel && count < 5) {
			size = strlen(pixel);
			if (check_nr(pixel, size))
				valid++;
			value[count++] = atoi(pixel);
			pixel = strtok(NULL, " ");
		}

		if (check_load) {
			// check the number of coordinates
			// and if they're all numerical
			if (count == 4 && valid == 4) {
				if (check_dim(img, value, count)) {
					// modify max and min coordinates
					*min_x = min(value[0], value[2]);
					*max_x = max(value[0], value[2]);
					*min_y = min(value[1], value[3]);
					*max_y = max(value[1], value[3]);
					// check if matrix is square
					if ((*max_y - *min_y) == (*max_x - *min_x))
						*check_sel = 1;
					else
						*check_sel = 0;
					printf("Selected %d %d %d %d\n", *min_x, *min_y, *max_x,
						   *max_y);
				} else {
					printf("Invalid set of coordinates\n");
				}
			} else {
				printf("Invalid command\n");
			}
		}
	}
}

// rotate portion of image 90deg to the left
int **rotate_sel(image *img, int x1, int y1, int x2, int y2)
{
	for (int i = y1; i < (y2 + y1) / 2; i++) {
		for (int j = x1 + i - y1; j < x2 - 1 - i + y1; j++) {
			int temp = img->mat[i][j];

			// top left pixels
			img->mat[i][j] = img->mat[j + y1 - x1][y2 - 1 - i + x1];

			// bottom right pixels
			img->mat[j + y1 - x1][y2 - 1 - i + x1] =
			img->mat[y2 - 1 - i + y1][x2 - 1 - j + x1];

			// top right pixels
			img->mat[y2 - 1 - i + y1][x2 - 1 - j + x1] =
			img->mat[x2 - 1 - j + y1][i + x1 - y1];

			// bottom left pixels
			img->mat[x2 - 1 - j + y1][i + x1 - y1] = temp;
		}
	}
	return img->mat;
}

// rotate entire image 90deg to the left
int **rotate_all(image *img)
{
	int **copy;

	copy = malloc(img->width * sizeof(int *));
	for (int i = 0; i < img->width; i++)
		copy[i] = malloc(img->height * sizeof(int));

	// initialise new matrix
	for (int i = img->width - 1; i >= 0; i--)
		for (int j = 0; j < img->height; j++)
			copy[img->width - 1 - i][j] = img->mat[j][i];

	// free memory occupied by the old matrix
	free_mem(*img);

	return copy;
}

// ROTATE
void rotate(char *sel_x,
			image *img,
			int angle,
			int check_sel,
			int *min_x,
			int *min_y,
			int *max_x,
			int *max_y)
{
	int times = -1;

	if (abs(angle) == 360 || angle == 0) {
		// nothing to modify
		printf("Rotated %d\n", angle);
		times = 0;
	} else if (angle == 90 || angle == -270) {
		times = 3;
	} else if (angle == 180 || angle == -180) {
		times = 2;
	} else if (angle == 270 || angle == -90) {
		times = 1;
	}

	// modify image
	if (times) {
		// check rotation angle
		if (check_ang(angle)) {
			printf("Rotated %d\n", angle);

			if (!strcmp(sel_x, "ALL") ||
				(*min_y == 0 && *min_x == 0 && *max_x == img->width &&
				 *max_y == img->height)) {
				// rotate the entire image
				for (int k = 0; k < times; k++) {
					img->mat = rotate_all(img);
					swap(&img->width, &img->height);
				}

				reset(img->width, img->height, min_x, min_y, max_x, max_y);

			} else {
				// rotate a portion of the image
				// validate the selection
				if (check_sel) {
					for (int k = 0; k < times; k++)
						img->mat =
							rotate_sel(img, *min_x, *min_y, *max_x, *max_y);
				} else {
					printf("The selection must be square\n");
				}
			}
		} else {
			printf("Unsupported rotation angle\n");
		}
	}
}

// CROP
image crop(image *img, int *min_x, int *min_y, int *max_x, int *max_y)
{
	printf("Image cropped\n");

	// the image doesn't change
	if (*min_x == *max_x - img->width && *max_x == img->width + *min_x &&
		*min_y == *max_y - img->height && *max_y == img->height + *min_y)
		return *img;

	image img2;

	strcpy(img2.type, img->type);
	img2.max = img->max;
	img2.size = img->size;
	img2.height = *max_y - *min_y;
	img2.width = *max_x - *min_x;

	img2.mat = malloc(img2.height * sizeof(int *));
	for (int i = 0; i < img2.height; i++)
		img2.mat[i] = malloc(img2.width * sizeof(int));

	// initialise new matrix
	for (int i = *min_y; i < *max_y; i++)
		for (int j = *min_x; j < *max_x; j++)
			img2.mat[i - *min_y][j - *min_x] = img->mat[i][j];

	// reset image dimensions
	reset(img2.width, img2.height, min_x, min_y, max_x, max_y);

	free_mem(*img);

	return img2;
}

// GRAYSCALE filter
int **grayscale(image img, int x1, int y1, int x2, int y2)
{
	for (int i = y1; i < y2; i++) {
		for (int j = x1; j < x2; j++) {
			int red = get(img.mat[i][j], 0);
			int green = get(img.mat[i][j], 1);
			int blue = get(img.mat[i][j], 2);
			// new R, G, B values
			int new = (red + blue + green) / 3;

			for (int k = 0; k < img.size; k++)
				set(&img.mat[i][j], new, k);
		}
	}
	printf("Grayscale filter applied\n");

	return img.mat;
}

// SEPIA filter
int **sepia(image img, int x1, int y1, int x2, int y2)
{
	for (int i = y1; i < y2; i++) {
		for (int j = x1; j < x2; j++) {
			double red = get(img.mat[i][j], 0);
			double green = get(img.mat[i][j], 1);
			double blue = get(img.mat[i][j], 2);

			// new red value
			double new_r = 0.393 * red + 0.769 * green + 0.189 * blue + 0.5;
			// new green value
			double new_g = 0.349 * red + 0.686 * green + 0.168 * blue + 0.5;
			//new blue value
			double new_b = 0.272 * red + 0.534 * green + 0.131 * blue + 0.5;

			set(&img.mat[i][j], min(img.max, new_r), 0);
			set(&img.mat[i][j], min(img.max, new_g), 1);
			set(&img.mat[i][j], min(img.max, new_b), 2);
		}
	}
	printf("Sepia filter applied\n");

	return img.mat;
}

// SAVE
void save(char *file, image img, int check_load)
{
	char option[10];
	fgets(option, 10, stdin);

	// check if the image was loaded correctly
	if (check_load) {
		// check save option
		if (!strcmp(option, " ascii\n")) {
			// ascii format
			FILE *out = fopen(file, "wt");
			print_img(out, img, 0);
			fclose(out);
		} else {
			// binary format
			FILE *out = fopen(file, "w");
			print_img(out, img, 1);
			fclose(out);
		}
		printf("Saved %s\n", file);
	} else {
		printf("No image loaded\n");
	}
}
