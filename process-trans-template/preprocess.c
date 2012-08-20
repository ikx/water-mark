#include "highgui.h"
#include "cv.h"
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#define PARSE_BG_SIZE 10
#define MAX_X 1000
#define MAX_Y 1000

/*
 1 channel version
 raw tempate size < 1000*1000;
 */


void parse_bg(uchar* data, double* bg);/* parse template's background color, 1 channel */
void real_area(const IplImage* tmpl_gray, IplImage* mask_gray, double bg, CvPoint* vertex);/* real template area */
void split(const char* full, char* path, char* file, char spliter);
char* make_name(char* new_name, char* path, char* prefix, char* file);

int main(int argc, char** argv){
	if(argc != 2){
		printf("USAGE: preprocess FILE_PATH/FILE_NAME\n");
		exit(1);
	}

	IplImage *template = NULL;
	IplImage *tmpl_gray = NULL;
	IplImage *mask_gray = NULL;

	const char* full = argv[1];
	if(!(template = cvLoadImage(full, 1))){
		printf("Cannot load image %s.\n", full);
		exit(1);
	}

	/* file names */
	char path[strlen(full)];
	char file[strlen(full)];

	split(full, path, file, '/');

	char tmpl_gray_name[strlen(full)+11];
	make_name(tmpl_gray_name, path, "real_gray_", file);
	puts(tmpl_gray_name);

	char masked_tmpl_name[strlen(full)+8];
	make_name(masked_tmpl_name, path, "masked_", file);
	puts(masked_tmpl_name);

	/* create binary copy of template */
	tmpl_gray = cvCreateImage(cvGetSize(template), 8, 1);
	mask_gray = cvCreateImage(cvGetSize(template), 8, 1);
	cvCvtColor(template, tmpl_gray, CV_BGR2GRAY);
	cvCvtColor(template, mask_gray, CV_BGR2GRAY);
	uchar* tdata = (uchar* )tmpl_gray->imageData;

	printf("raw template file \"%s\" loaded.\n", argv[1]);

	/* parse background */
	double bg;
	parse_bg(tdata, &bg);

	/* find real area of template from gray copy */
	CvPoint vertex[2] = {MAX_X,MAX_Y, 0,0};
	real_area(tmpl_gray, mask_gray, bg, vertex);
	CvSize realSize = cvSize(vertex[1].x - vertex[0].x, vertex[1].y - vertex[0].y);
	CvRect real_roi = cvRect(vertex->x, vertex->y, realSize.width, realSize.height);
	cvSetImageROI(tmpl_gray, real_roi);
	cvSetImageROI(mask_gray, real_roi);
	cvSaveImage(tmpl_gray_name,tmpl_gray,0);
	cvSaveImage(masked_tmpl_name,mask_gray,0);

	/* show results */
	cvNamedWindow("t_gray", 1);
	cvNamedWindow("m_gray", 1);
	cvShowImage("t_gray", tmpl_gray);
	cvShowImage("m_gray", mask_gray);
	cvWaitKey(0);
	cvDestroyWindow("t_gray");
	cvDestroyWindow("m_gray");

	/* free mem */
	cvReleaseImage(&template);
	cvReleaseImage(&tmpl_gray);
	cvReleaseImage(&mask_gray);

	return 0;
}

void parse_bg(uchar* data, double* bg){
	printf("parsing background color...\n");
	int i;
	double gray=0;
	for(i=0;i<PARSE_BG_SIZE;i++){
		gray += data[i];
	}
	*bg = gray / 10;
}

void real_area(const IplImage* t_gray, IplImage* m_gray, double bg, CvPoint* vertex){
	int step,width,height;
	int i,j;
	double gray;

	step    = t_gray->widthStep;
	width   = t_gray->width;
	height  = t_gray->height;
	uchar* tdata = (uchar*)t_gray->imageData;
	uchar* mdata = (uchar*)m_gray->imageData;

	for(i=0;i<height;i++)
		for(j=0;j<width;j++){
			gray = tdata[i*step + j];
			if(fabs(gray - bg) > 25){
				if(j<vertex->x)
					vertex->x = j;
				if(i<vertex->y)
					vertex->y = i;
				if(j>(vertex+1)->x)
					(vertex+1)->x = j;
				if(i>(vertex+1)->y)
					(vertex+1)->y = i;
				mdata[i*step + j] = 255;
			}else{
				mdata[i*step + j] = 0;
			}
		}
}

/* common tools */
void split(const char* full, char* path, char* file, char spliter){
	char* mark = strrchr(full, spliter);
	if(!mark){
		printf("USAGE: `path-to-file/file-name` !\n");
		exit(1);
	}
	int pos = (int)(mark - full);
	strncpy(path, full, ++pos);
	path[pos] = '\0';
	strcpy(file,++mark);
}

char* make_name(char* new_name, char* path, char* prefix, char* file){
	new_name[0] = '\0';
	strcat(new_name, path);
	strcat(new_name, prefix);
	strcat(new_name, file);
}
