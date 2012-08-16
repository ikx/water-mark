#include "highgui.h"
#include "cv.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAXWIDTH 1000
#define MAXHEIGHT 700

void show(IplImage* img){
	int imgw, imgh;
	int windows;
	int i;

	if(!img){
		printf("Failed to load image \n");
		exit(1);
	}
	imgw = img->width;
	imgh = img->height;
	printf("image height: %d\n", imgh);
	windows = ceil((double)imgh / MAXHEIGHT );
	printf("windows: %d\n", windows);

	char winame[windows][7];
	for(i=0;i<windows;i++){
		sprintf(winame[i], "part%d", i+1);
		CvPoint roi_lt = cvPoint(0, MAXHEIGHT * i);
		int roi_h      = (i != windows - 1) ? MAXHEIGHT : (MAXHEIGHT * windows - imgh);
		int roi_w	   = imgw;
		CvRect roi     = cvRect(roi_lt.x, roi_lt.y, roi_w, roi_h);
		cvSetImageROI(img, roi);
		cvNamedWindow(winame[i], CV_WINDOW_AUTOSIZE);
		cvShowImage(winame[i],img);
	}
	cvWaitKey(0);
	for(i=0;i<windows;i++){
		printf("windows: %s\n", winame[i]);
		cvDestroyWindow(winame[i]);
	}
	cvResetImageROI(img);
	//cvReleaseImage(&img);
}
