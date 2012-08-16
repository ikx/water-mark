#include "highgui.h"
#include "cv.h"
#include <stdio.h>

int main(int argc, char** argv){
	int i;
	if(argc != 3){
		printf("Error 1: 2 arguments expected, %d given.\n",argc-1);
		return 0;
	}
	IplImage* source = cvLoadImage(argv[1],-1);
	IplImage* src_gray = cvCreateImage(cvGetSize(source), 8, 1);
	cvCvtColor(source, src_gray, CV_BGR2GRAY);
	IplImage* tmpl   = cvLoadImage(argv[2],-1);
	int ww = src_gray->width - tmpl->width + 1;
	int hh = src_gray->height - tmpl->height + 1;
	IplImage *result = cvCreateImage(cvSize(ww,hh),IPL_DEPTH_32F, 1);//source->nChannels);

	cvMatchTemplate(src_gray, tmpl, result, CV_TM_SQDIFF);

	CvPoint minLoc;
	CvPoint maxLoc;
	double minVal;
	double maxVal;
	cvMinMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, 0);
	cvRectangle(src_gray, minLoc, cvPoint(minLoc.x+tmpl->width,minLoc.y+tmpl->height),cvScalar(0,0,255,1),1,8,0);

	cvNamedWindow("match",CV_WINDOW_AUTOSIZE);
	cvShowImage("match",src_gray);
	cvWaitKey(0);
	cvReleaseImage(&source);
	cvReleaseImage(&src_gray);
	//cvReleaseImage(&tmpl);
	//cvReleaseImage(&result);
	cvDestroyWindow("match");
}
