#include "cv.h"
#include "highgui.h"
#include <stdio.h>

int main(int argc, char** argv){
	IplImage* img;
	CvMat* imgMat;
	img = cvLoadImage(argv[1],-1);
	if(!img){
		printf("Failed to open image %s\n",argv[1]);
	}
	printf("Image %s opened.\n",argv[1]);
	if(img->depth > 8){
		printf("Image depth out of range.\n");
		return 0;
	}
	printf("Size: %d*%d\n",img->width,img->height);
	printf("Channels: %d\n",img->nChannels);
	printf("Step: %d\n",img->widthStep);
	int i,j;
	CvScalar s;
	uchar* data = (uchar* )img->imageData;
	for(i=0;i<img->height;i++){
		for(j=0;j<img->width;j++){
			//s=cvGet2D(img,i,j);
			printf("%d ",data[j*img->widthStep+i]);
		}
		printf("\n");
	}
	printf("\n");
	cvReleaseImage(&img);
	printf("Image %s released.\n",argv[1]);
}
