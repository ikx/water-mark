#include "highgui.h"
#include "cv.h"
#include <stdio.h>
#include <time.h>
#include <math.h>
#define MAX_X 1000
#define MAX_Y 1000
/*1 channel version
 *raw tempate size < 1000*1000;
 * */


void parseBGR(uchar* data, double* bg);/*parse template's background color to BGR*/
void realArea(const IplImage* template, double bg, CvPoint* vertex);/*real template area*/
char filterBG(CvScalar background, CvScalar test, int deta);/*tested point is background or not*/
void openImage();/**/

int main(int argc, char** argv){
	double bg;
	CvPoint vertex[2] = {MAX_X,MAX_Y, 0,0};
	clock_t start, end;
	IplImage *template = NULL, *t_gray = NULL;//*real = NULL;
	start = clock();
	if(argc != 2){
		printf("Wrong arg!\n");
		exit(1);
	}
	if(!(template = cvLoadImage(argv[1], 1))){
		printf("Cannot load image %s.\n", argv[1]);
		exit(1);
	}
	t_gray = cvCreateImage(cvGetSize(template), 8, 1);
	cvCvtColor(template, t_gray, CV_BGR2GRAY);
	char* tmpl_gray = "../imgs/tmpl/template_gray.jpg";
	cvSaveImage(tmpl_gray, t_gray, 0);
	uchar* tdata = (uchar* )t_gray->imageData;
	printf("raw template file \"%s\" loaded.\n", argv[1]);
	/*
	cvNamedWindow("t_gray", 1);
	cvShowImage("t_gray", t_gray);
	cvWaitKey(0);
	cvDestroyWindow("t_gray");
	*/

	parseBGR(tdata, &bg);
	printf("Background-color: Gray(%.f)\n", bg);

	realArea(t_gray, bg, vertex);
	printf("Real template area: (%d,%d)~(%d,%d)\n", vertex[0].x,vertex[0].y,vertex[1].x,vertex[1].y);

	CvSize realSize = cvSize(vertex[1].x - vertex[0].x, vertex[1].y - vertex[0].y);
	//real = cvCreateImage(realSize, IPL_DEPTH_8U, 3);
	cvSetImageROI(t_gray, cvRect(vertex->x, vertex->y, realSize.width, realSize.height));
	char* realName = "../imgs/tmpl/real_gray.jpg";
	cvSaveImage(realName, t_gray, 0);
	/*

	*/
	end = clock();
	printf("Costs %ld ms.\n",(end));
	cvReleaseImage(&template);
	cvReleaseImage(&t_gray);
	//cvReleaseImage(&real);
	return 0;
}

void parseBGR(uchar* data, double* bg){
	printf("parsing background color...\n");
	int i;
	double gray=0;
	for(i=0;i<10;i++){
		gray += data[i];
	}
	*bg = gray / 10;
}
void realArea(const IplImage* t_gray, double bg, CvPoint* vertex){
	int step,width,height;
	int i,j;
	double gray;

	step    = t_gray->widthStep;
	width   = t_gray->width;
	height  = t_gray->height;
	uchar* tdata = (uchar*)t_gray->imageData;

	for(i=0;i<height;i++)
		for(j=0;j<width;j++){
			gray = tdata[i*step + j];
			//if(filterBG(bg, tmp, 25) == 0){
			if(fabs(gray - bg) > 25){
				if(j<vertex->x)
					vertex->x = j;
				if(i<vertex->y)
					vertex->y = i;
				if(j>(vertex+1)->x)
					(vertex+1)->x = j;
				if(i>(vertex+1)->y)
					(vertex+1)->y = i;
			}
		}
}
char filterBG(CvScalar background, CvScalar test, int deta){
	int i;
	char flag = 1;
	for(i=0;i<3;i++)
		if(fabs(background.val[i] - test.val[i]) > deta){
			flag = 0;
			break;
		}
	return flag;
}
