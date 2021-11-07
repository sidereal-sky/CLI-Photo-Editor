// Moroiu Alexandra
// helper functions

#ifndef _FILE_UTILITIES_H_
#define _FILE_UTILITIES_H_

// image structure
typedef struct {
	char type[3];
	int max, size;
	int height, width;
	int **mat;
} image;

void set(int *pixel, int val, int poz);

int get(int pixel, int poz);

int min(int val1, int val2);

int max(int val1, int val2);

void swap(int *val1, int *val2);

void reset(int new_w,
		   int new_h,
		   int *min_x,
		   int *min_y,
		   int *max_x,
		   int *max_y);

int check_nr(char *pixel, int size);

int check_dim(image img, int *value, int count);

int check_ang(int angle);

image *read_img(FILE *in, image *img);

void print_img(FILE *out, image img, int ok);

void free_mem(image img);

#endif
