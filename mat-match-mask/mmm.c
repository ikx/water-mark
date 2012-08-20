//#include "lib/cts.h"
#include "highgui.h"
#include "cv.h"
#include <stdlib.h>
#include <stdio.h>

<<<<<<< HEAD
#define MAX_WM_NUMS 4
#define THRESHOLD 25

=======
>>>>>>> c1eae1c2a134f1674bcbe6742e0a92d926806c8c
void show_img(IplImage* img);//debug tool
void init_mask(IplImage* mask);//
void my_cvAvg(IplImage* src, IplImage* tpl, IplImage* dest, IplImage* mask, double* avg);
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
<<<<<<< HEAD

=======
>>>>>>> c1eae1c2a134f1674bcbe6742e0a92d926806c8c
	IplImage* gray_src = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	IplImage* sub_res  = cvCreateImage(cvGetSize(real_tpl), IPL_DEPTH_8U, 1);
	cvCvtColor(src, gray_src, CV_BGR2GRAY);
	CvSize src_size = cvGetSize(gray_src);
	CvSize tpl_size = cvGetSize(real_tpl);
	int h_i,w_j;
	int gs_step, rt_step, mt_step, sub_step;
	gs_step = gray_src->widthStep;
	rt_step = real_tpl->widthStep;
	mt_step = mask_tpl->widthStep;
	sub_step= sub_res->widthStep;
	uchar* gsdata = (uchar*)gray_src->imageData;
	uchar* mtdata = (uchar*)mask_tpl->imageData;
	uchar* rtdata = (uchar*)real_tpl->imageData;
	uchar* subdata= (uchar*)sub_res->imageData;

	CvPoint match_region = {src_size.width - tpl_size.width, src_size.height - tpl_size.height};
	double min = 256;
	CvPoint min_pos = {0,0};
	for(h_i = 0; h_i < match_region.y; h_i++){
		//cvSetZero(sub_res);
		for(w_j = 0; w_j < match_region.x; w_j++){
			CvRect match_roi = cvRect(w_j, h_i, tpl_size.width, tpl_size.height);
			cvSetImageROI(gray_src, match_roi);
			double avg = 0;
			my_cvAvg(gray_src, real_tpl, sub_res, mask_tpl, &avg);
			if(avg < min){
				min=avg;
				min_pos = cvPoint(w_j,h_i);
			}
<<<<<<< HEAD
			//printf("pos: (%d, %d)\n",h_i,w_j);
			//printf("avg: %f\n", avg);
			//printf("===========\n");
=======
			printf("pos: (%d, %d)\n",h_i,w_j);
			printf("avg: %f\n", avg);
			printf("===========\n");
>>>>>>> c1eae1c2a134f1674bcbe6742e0a92d926806c8c
		}
	}

	printf("size: %d\n", src->height);
	printf("MIN: %f\n",min);
	printf("min pos: (%d, %d)\n", min_pos.x, min_pos.y);

	cvRectangle(src, min_pos, cvPoint(min_pos.x+tpl_size.width,min_pos.y+tpl_size.height),cvScalar(0,0,255,1),1,8,0);

	cvNamedWindow("match",CV_WINDOW_AUTOSIZE);
	cvShowImage("match",src);
	cvWaitKey(0);
	cvDestroyWindow("match");

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
