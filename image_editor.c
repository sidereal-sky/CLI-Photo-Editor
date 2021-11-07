// Moroiu Alexandra

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "file_utilities.h"
#include "operations.h"

int main(void)
{
	image img;
	char oper[10], contin[10], file[100], sel_x[10] = "-1";
	int check_load = 0, check_sel, angle, min_x = -1, max_x, min_y, max_y;
	// read operation
	scanf("%s", oper);

	while (strcmp(oper, "EXIT")) {
		if (!strcmp(oper, "LOAD")) {
			check_load = 1;
			angle = 0;
			scanf("%s", file);
			load(file, &img, &check_load, &min_x, &min_y, &max_x, &max_y);

		} else if (!strcmp(oper, "SELECT")) {
			scanf("%s", sel_x);
			selection(sel_x, img, &min_x, &min_y, &max_x, &max_y, &check_sel,
					  check_load);
			if (check_load == 0)
				printf("No image loaded\n");

		} else if (!strcmp(oper, "ROTATE")) {
			scanf("%d", &angle);
			if (check_load)
				rotate(sel_x, &img, angle, check_sel, &min_x, &min_y, &max_x,
					   &max_y);
			else
				printf("No image loaded\n");

		} else if (!strcmp(oper, "CROP")) {
			if (check_load)
				img = crop(&img, &min_x, &min_y, &max_x, &max_y);
			else
				printf("No image loaded\n");

		} else if (!strcmp(oper, "GRAYSCALE")) {
			if (check_load) {
				// check if image is color
				if ((img.type[1] - '0') % 3 == 0)
					img.mat = grayscale(img, min_x, min_y, max_x, max_y);
				else
					printf("Grayscale filter not available\n");
			} else {
				printf("No image loaded\n");
			}

		} else if (!strcmp(oper, "SEPIA")) {
			if (check_load) {
				// check if image is color
				if ((img.type[1] - '0') % 3 == 0)
					img.mat = sepia(img, min_x, min_y, max_x, max_y);
				else
					printf("Sepia filter not available\n");
			} else {
				printf("No image loaded\n");
			}

		} else if (!strcmp(oper, "SAVE")) {
			scanf("%s", file);
			save(file, img, check_load);

		} else {
			fgets(contin, 10, stdin);
			printf("Invalid command\n");
		}
		scanf("%s", oper);
	}

	if (check_load == 0)
		printf("No image loaded\n");
	if (min_x != -1)
		free_mem(img);

	return 0;
}
