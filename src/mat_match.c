//#include "matchlib.h"
#include "lib/showBigImg.h"
#include "highgui.h"
#include "cv.h"
#include <stdlib.h>
#include <stdio.h>

/*
 * USAGE: ./mat_match source_file template_file(real_gray);
 *
 * */
int main(int argc, char** argv){
	if(argc != 3){
		printf("USAGE: show_big_image PATH_TO_IMAGE_FILE\n");
		exit(1);
	}
	IplImage* src  = cvLoadImage(argv[1], -1);
	IplImage* tmp = cvLoadImage(argv[2], -1);//use real_gray as template;
	if(!src || !tmp){
		printf("Failed to load image.\n");
		exit(1);
	}

	IplImage* src_gray = cvCreateImage(cvGetSize(src), 8, 1);
	cvCvtColor(src, src_gray, CV_BGR2GRAY);
	//show(src_gray);

	cvReleaseImage(&src);
	cvReleaseImage(&tmp);
	cvReleaseImage(&src_gray);
}
