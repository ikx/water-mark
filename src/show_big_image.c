#include "lib/showBigImg.h"
int main(int argc, char** argv){
	int imgw, imgh;
	int windows;
	int i;

	if(argc != 2){
		printf("USAGE: show_big_image PATH_TO_IMAGE_FILE\n");
		exit(1);
	}
	IplImage* img = cvLoadImage(argv[1], -1);
	if(!img){
		printf("Failed to load image \"%s\"\n", argv[1]);
		exit(1);
	}
	show(img);
	cvReleaseImage(&img);
}
