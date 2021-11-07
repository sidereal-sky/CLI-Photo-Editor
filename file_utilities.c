// Moroiu Alexandra

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "file_utilities.h"

// set value for the poz-th byte
void set(int *pixel, int val, int poz)
{
	unsigned char *p = (unsigned char *)pixel;
	p[poz] = val;
}

// extract value of poz-th pixel
int get(int pixel, int poz)
{
	unsigned char *p = (unsigned char *)&pixel;
	return p[poz];
}

int min(int val1, int val2)
{
	if (val1 > val2)
		return val2;
	return val1;
}

int max(int val1, int val2)
{
	if (val1 < val2)
		return val2;
	return val1;
}

void swap(int *val1, int *val2)
{
	int aux = *val1;
	*val1 = *val2;
	*val2 = aux;
}

// set/reset image dimesnions
void reset(int new_w, int new_h, int *min_x, int *min_y, int *max_x, int *max_y)
{
	*min_x = 0;
	*min_y = 0;
	*max_x = new_w;
	*max_y = new_h;
}

// check if parameters are numerical
int check_nr(char *pixel, int size)
{
	for (int i = 0; i < size; i++)
		if ((pixel[i] - '0') > 9)
			return 0;
	return 1;
}

// check selection coordinates
int check_dim(image img, int *value, int count)
{
	for (int i = 0; i < count; i++)
		if (value[i] < 0)
			return 0;
	if (max(value[0], value[2]) > img.width ||
		max(value[1], value[3]) > img.height)
		return 0;
	if (value[0] == value[2] || value[1] == value[3])
		return 0;
	return 1;
}

// check rotation angle
int check_ang(int angle)
{
	if (abs(angle) % 90 == 0 && abs(angle) < 360)
		return 1;
	return 0;
}

// read image from file
image *read_img(FILE *in, image *img)
{
	// P1 P2 P4 and P5 images
	img->size = 1;

	fscanf(in, "%s", img->type);

	// P3 and P6 images
	if ((img->type[1] - '0') % 3 == 0)
		img->size = 3;

	// dimensions
	fscanf(in, "%d %d", &img->width, &img->height);
	// maximum value of a pixel
	fscanf(in, "%d\n", &img->max);

	int temp;

	img->mat = malloc(img->height * sizeof(int *));
	for (int i = 0; i < img->height; i++)
		img->mat[i] = malloc(img->width * sizeof(int));

	for (int i = 0; i < img->height; i++) {
		for (int j = 0; j < img->width; j++) {
			for (int k = 0; k < img->size; k++) {
				if (img->type[1] > '3')
					// binary format
					fread(&temp, sizeof(char), 1, in);
				else
					// ascii format
					fscanf(in, "%d", &temp);

				set(&img->mat[i][j], temp, k);
			}
		}
	}

	return img;
}

// save image to file
void print_img(FILE *out, image img, int ok)
{
	// ok = 0 => ascii
	// ok = 1 => binary

	if (img.type[1] < '4' && ok == 1)
		// switch to binary format
		img.type[1] = img.type[1] + 3;

	if (img.type[1] > '3' && ok == 0)
		// switch to ascii format
		img.type[1] = img.type[1] - 3;

	// image type
	fprintf(out, "%s\n", img.type);

	fprintf(out, "%d %d\n", img.width, img.height);

	fprintf(out, "%d\n", img.max);

	int temp;

	for (int i = 0; i < img.height; i++) {
		for (int j = 0; j < img.width; j++) {
			for (int k = 0; k < img.size; k++) {
				temp = get(img.mat[i][j], k);
				if (ok == 0)
					fprintf(out, "%d ", temp);
				else
					fwrite(&temp, sizeof(char), 1, out);
			}
		}
		if (ok == 0)
			fprintf(out, "\n");
	}
}

// free memory
void free_mem(image img)
{
	for (int i = 0; i < img.height; i++)
		free(img.mat[i]);
	free(img.mat);
}
