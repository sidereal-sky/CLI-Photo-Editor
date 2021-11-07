Introduction:\
	&nbsp;&nbsp; -- this is a CLI photo editor implemented in C\
	&nbsp;&nbsp; -- it allows the user to perform operations on PPM and PGM images such as:\
	&nbsp;&nbsp; LOAD, SELECT, ROTATE, GRAYSCALE etc.\
	&nbsp;&nbsp; -- it's very user friendly for it handles multiple possible errors the user
	may make

Structure:\
	&nbsp;&nbsp; -- file_utilities takes care of the image structure and helper functions\
	&nbsp;&nbsp; such as: min(), max(), read_img(), free_mem() etc.\
	&nbsp;&nbsp; -- operations deals precisely with what the user wants to do with their\
	&nbsp;&nbsp; image: rotate(), crop(), grayscale() etc.

File_Utilities:

	1. set() and get():
		-- each int value is split into its 4 bytes, which optimizes memory
		usage especially when it comes to storing color images: the 1st byte
		is associated to the RED value, the 2nd to GREEN and the 3rd to BLUE;
		-- set's role is to assign respective values to the bytes and get's
		to exract them when needed

	2. reset():
		-- sets/resets the maximum and minimum coordinates of the image

	3. check_nr(), check_dim() si check_ang():
		-- check_nr() checks whether the elements of a string are numbers
		in case the user introduces wrong coordinates when selecting the
		image
		-- check_dim() ensures that the selection coordinates aren't negative
		nor outside the image's bounds
		-- check_ang() checks whether the rotation angle is one of the
		following: {0, +-90, +-180, +-270, +-360}

	4. read_img() si print_img():
		-- read_img() takes the image as input: type, dimensions, maximum
		pixel value, pixel matrix and returns it so that it can be loaded
		later on
		-- print_img() writes the image in a file, in ascii or binary format

Operations:
	
	1. load():
		-- check if the file was loaded successfully
		-- makes sure to free the memory of a previous image if need be

	2. selection():
		-- if the user selects "ALL", then set the minimum and maximum
		coordinates to the entire image
		-- if the selection is not "ALL", then read the coordinates one by
		one and validate them
		-- furthermore, check if the selection is square in case of a possible
		"ROTATE"

	3. rotate_sel(), rotate_all() si rotate():
		-- rotate():
			~ do nothing for 0 and 360 angles
			~ rotate the image once for 270 and -90 angles
			~ rotate the image twice for +-180 angles
			~ rotate the image 3 times 90 and -270 angles
		-- free the memory occupied by the original image

	4. crop():
		-- check whether the image changes
		-- if it does, save the cropped image and free the memory occupied by
		the orginal image

	5. grayscale() si sepia():
		-- extract the pixel's RGB values and modify them using the proper
		formulas
		-- the grayscale effect only uses 1 color byte

	6. save():
		-- before saving an image check if it was loaded correctly
		-- check the save option: ascii or binary

Final Notes:\
	&nbsp;&nbsp; -- the program executes until it meets the "EXIT" command\
	&nbsp;&nbsp; -- all operations are conditioned by "check_load", which tells the\
	&nbsp;&nbsp; user whether they have an image to perform operations on\
	&nbsp;&nbsp; -- "min_x" is initialized with -1 to indicate that no image has been\
	&nbsp;&nbsp; loaded successfully\
	&nbsp;&nbsp; -- sel_x[10] = "-1" indicates that no selection has been made yet
