#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <math.h>
#include "highgui.h"
#include "cv.h"

#define SPLITH 300

void show_img(IplImage* img);
int main(){
	IplImage* src = cvLoadImage("/home/rainy/project/OpenCV/water-mark/mat-match-mask/imgs/7772-XTm8D.jpg", -1);
	IplImage* gray_src = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U,1);
	cvCvtColor(src, gray_src, CV_BGR2GRAY);
	int img_width, img_height, wins, img_step;
	int i,j,k;
	clock_t start, end;
	img_width = gray_src->width;
	img_height= gray_src->height;
	img_step  = gray_src->widthStep;
	wins  = ceil((double)img_height / SPLITH);
	uchar *gdata = (uchar *)gray_src->imageData;

	start = clock();
	pthread_t thrds[wins];
	for(i=0; i<wins; i++){
		CvPoint roi_lt = {0, SPLITH*i};
		printf("roi_lt: (%d, %d)\n", roi_lt.x, roi_lt.y);
		int roi_h = (i == wins - 1) ? (img_height - SPLITH * (wins - 1)): SPLITH;
		printf("roi_h: %d\n", roi_h);
		int roi_w = img_width;
		CvPoint roi_rb = {roi_w, roi_lt.y+roi_h};
		CvRect roi= cvRect(roi_lt.x, roi_lt.y, roi_w, roi_h);
		cvSetImageROI(gray_src, roi);
		printf("ROI img size: %d*%d\n", roi.width, roi.height);
		for(j=0;j<roi.height;j++)
			for(k=0;k<roi.width;k++)
				printf("*\n");
				//printf("gray value: %d", gdata[j*img_step+k]);
		//show_img(src);

		//cvRectangle(src, roi_lt, roi_rb, cvScalar(0,0,255,1), 1, 8, 0);
	}
	end = clock();
	printf("costs time: %.f\n", (double)(end - start));
	//show_img(src);

	/*
	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			if(i==0 || i==height-1)
				if(j==0 || j==width-1)
					printf("+");
				else
					printf("--");
			else
				if(j==0 || j==width-1)
					printf("|");
				else
					printf("  ");
		}
		printf("\n");
	}
	*/

	//printf("Image size: %d*%d\n", width, height);
	//printf("wins: %d\n", wins);

	//show_img(src);
	cvReleaseImage(&src);
}
void show_img(IplImage* img){
	cvNamedWindow("Debug", 1);
	cvShowImage("Debug", img);
	cvWaitKey(0);
	cvDestroyWindow("Debug");
}
