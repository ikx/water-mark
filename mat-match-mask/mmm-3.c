#include "lib/showBigImg.h"
#include "highgui.h"
#include "cv.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAX_WM_NUMS 4
#define THRESHOLD 25
#define HEIGHT 300

/* structs */
typedef struct{
	CvPoint lt_pos;//left top point.
	double avg;//avg value of AbsDiff matrix.
} LTAvg;

/* global vars */
LTAvg match_points[MAX_WM_NUMS];
int mp_ptr = 0;

/* tools */
void show_img(IplImage* img);//debug tool, show image while developing.
void init_mask(IplImage* mask);//init mask image as 0 vs 255, not used here.
void array_push();


/* core methods */
void my_cvAvg(IplImage* src, IplImage* tpl, IplImage* dest, IplImage* mask, double* avg);//cv's cvAvg does not work for me.

/* main fun */
int main(int argc, char** argv){
	if(argc != 4){
		printf("USAGE: ./mmm `PATH-TO/real_gray_tmpl_X.jpg PATH-TO/masked_tmpl_X.jpg PATH-TO/source.jpg`\n");
		exit(1);
	}
	IplImage* src = cvLoadImage(argv[3], -1);
	IplImage* real_tpl = cvLoadImage(argv[1], -1);
	IplImage* mask_tpl = cvLoadImage(argv[2], -1);
	//init_mask(mask_tpl);
	if(!src || !real_tpl || !mask_tpl){
		printf("Failed to load image.\n");
		exit(1);
	}
	IplImage* gray_src = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	IplImage* sub_res  = cvCreateImage(cvGetSize(real_tpl), IPL_DEPTH_8U, 1);
	cvCvtColor(src, gray_src, CV_BGR2GRAY);
	CvSize src_size = cvGetSize(gray_src);
	CvSize tpl_size = cvGetSize(real_tpl);
	CvPoint region  = cvPoint(src_size.width - tpl_size.width, src_size.height - tpl_size.height);
	int h_i, w_j;
	for(h_i=0;h_i<region.y;h_i++){
		for(w_j=0;w_j<region.x;w_j++){
		    CvRect match_roi = cvRect(w_j, h_i, tpl_size.width, tpl_size.height);
		    cvSetImageROI(gray_src, match_roi);
			double avg = 0.0;
			my_cvAvg(gray_src, real_tpl, sub_res, mask_tpl, &avg);
			//printf("avg: %.f\n", avg);
			if(mp_ptr >= MAX_WM_NUMS){
				printf("water mark number overflow\n");
				exit(-1);
			}
			if(avg < THRESHOLD){
				match_points[mp_ptr].lt_pos = cvPoint(w_j, h_i);
				match_points[mp_ptr].avg      = avg;
				mp_ptr++;
			}
		}
	}
	printf("mp_ptr: %d\n", mp_ptr);
	int i;
	for(i=0;i<mp_ptr;i++){
		CvPoint lt = match_points[i].lt_pos;
		printf("pos: (%d,%d)\n", lt.x, lt.y);
		CvPoint rb = cvPoint(lt.x+tpl_size.width, lt.y+tpl_size.height);
		cvRectangle(src, lt, rb, cvScalar(0,0,255,1), 1, 8, 0);
	}
	
	//cvResetImageROI(gray_src);
	show(src);
	/*
	printf("size: %d\n", src->height);
	printf("MIN: %f\n",min);
	printf("min pos: (%d, %d)\n", min_pos.x, min_pos.y);

	cvRectangle(src, min_pos, cvPoint(min_pos.x+tpl_size.width,min_pos.y+tpl_size.height),cvScalar(0,0,255,1),1,8,0);

	cvNamedWindow("match",CV_WINDOW_AUTOSIZE);
	cvShowImage("match",src);
	cvWaitKey(0);
	cvDestroyWindow("match");

	uchar* gsdata = (uchar*)gray_src->imageData;
	uchar* mtdata = (uchar*)mask_tpl->imageData;
	uchar* rtdata = (uchar*)real_tpl->imageData;
	uchar* subdata= (uchar*)sub_res->imageData;

	*/

	cvReleaseImage(&src);
	cvReleaseImage(&real_tpl);
	cvReleaseImage(&mask_tpl);
	cvReleaseImage(&gray_src);
	cvReleaseImage(&sub_res);
}
void show_img(IplImage* img){
	cvNamedWindow("Debug", 1);
	cvShowImage("Debug", img);
	cvWaitKey(0);
	cvDestroyWindow("Debug");
}
void init_mask(IplImage* mask){
	int i,j;
	int w,h,step;
	w = mask->width;
	h = mask->height;
	step = mask->widthStep;
	uchar* mdata = (uchar*)mask->imageData;
	for(i = 0; i<h; i++)
		for(j = 0; j<w; j++)
			if(mdata[i*step+j] < 128){
				mdata[i*step+j] = 0;
			}else{
				mdata[i*step+j] = 255;
			}
}
void my_cvAvg(IplImage* src, IplImage* tpl, IplImage* dest, IplImage* mask, double* avg){
	int i,j,count;
	double sum = 0;
	CvSize tpl_size = cvGetSize(tpl);
	cvAbsDiff(src, tpl, dest);

	int dst_step = dest->widthStep;
	uchar* dstdata = (uchar*)dest->imageData;
	uchar* mdata   = (uchar*)mask->imageData;

	count = 0;
	for(i=0;i<tpl_size.height;i++){
		for(j=0;j<tpl_size.width;j++){
			if(mdata[i*dst_step+j] > 128){
				sum += dstdata[i*dst_step+j];
				count++;
			}
		}
	}
	*avg = sum / count;

}
