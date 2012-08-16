#include "highgui.h"
#include "cv.h"
#include <stdio.h>
#include <time.h>
#include <math.h>
#define MAX_X 1000
#define MAX_Y 1000
/*3 channels version
 *raw tempate size < 1000*1000;
 * */


void parseBGR(uchar* data, CvScalar* bg);/*parse template's background color to BGR*/
void realArea(const IplImage* template, CvScalar bg, CvPoint* vertex);/*real template area*/
char filterBG(CvScalar background, CvScalar test, int deta);/*tested point is background or not*/
void openImage();/**/

int main(int argc, char** argv){
	CvScalar bg;
	CvPoint vertex[2] = {MAX_X,MAX_Y, 0,0};
	clock_t start, end;
	IplImage *template = NULL;//, *real = NULL;
	start = clock();
	if(argc != 2){
		printf("Wrong arg!\n");
		exit(1);
	}
	if(!(template = cvLoadImage(argv[1], -1))){
		printf("Cannot load image %s.\n", argv[1]);
		exit(1);
	}
	uchar* tdata = (uchar* )template->imageData;
	printf("raw template file \"%s\" loaded.\n", argv[1]);

	parseBGR(tdata, &bg);
	printf("Background-color: BGR(%.f, %.f, %.f)\n", bg.val[0], bg.val[1], bg.val[2]);

	realArea(template, bg, vertex);
	printf("Real template area: (%d,%d)~(%d,%d)\n", vertex[0].x,vertex[0].y,vertex[1].x,vertex[1].y);

	CvSize realSize = cvSize(vertex[1].x - vertex[0].x, vertex[1].y - vertex[0].y);
	//real = cvCreateImage(realSize, IPL_DEPTH_8U, 3);
	cvSetImageROI(template, cvRect(vertex->x, vertex->y, realSize.width, realSize.height));
	char* realName = "../imgs/tmpl/real.jpg";
	cvSaveImage(realName, template, 0);

	end = clock();
	printf("Costs %ld ms.\n",(end));
	cvReleaseImage(&template);
	//cvReleaseImage(&real);
	return 0;
}

void parseBGR(uchar* data, CvScalar* bg){
	printf("parsing background color...\n");
	int i;
	double b,g,r;
	b=g=r=0;
	for(i=0;i<5;i++){
		b += data[i*3];
		g += data[i*3+1];
		r += data[i*3+2];
	}
	*bg = cvScalar(b/5., g/5., r/5., 0);
}
void realArea(const IplImage* template, CvScalar bg, CvPoint* vertex){
	int step,width,height,channels;
	int i,j;
	int b,g,r;

	step    = template->widthStep;
	width   = template->width;
	height  = template->height;
	channels=template->nChannels;
	uchar* tdata = (uchar*)template->imageData;

	for(i=0;i<height;i++)
		for(j=0;j<width;j++){
			b = tdata[i*step + j*channels];
			g = tdata[i*step + j*channels + 1];
			r = tdata[i*step + j*channels + 2];
			CvScalar tmp = cvScalar(b,g,r,0);
			if(filterBG(bg, tmp, 25) == 0){
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
