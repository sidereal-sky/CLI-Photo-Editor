// Moroiu Alexandra
// command functions

#ifndef _OPERATIONS_H_
#define _OPERATIONS_H_

void load(char *file,
		  image *img,
		  int *check_load,
		  int *min_x,
		  int *min_y,
		  int *max_x,
		  int *max_y);

void selection(char *x,
			   image img,
			   int *min_x,
			   int *min_y,
			   int *max_x,
			   int *max_y,
			   int *check_sel,
			   int check_load);

int **rotate_sel(image *img, int x1, int y1, int x2, int y2);

int **rotate_all(image *img);

void rotate(char *x,
			image *img,
			int angle,
			int check_sel,
			int *min_x,
			int *min_y,
			int *max_x,
			int *max_y);

image crop(image *img, int *min_x, int *min_y, int *max_x, int *max_y);

int **grayscale(image img, int x1, int y1, int x2, int y2);

int **sepia(image img, int x1, int y1, int x2, int y2);

void save(char *file, image img, int check_load);

#endif
