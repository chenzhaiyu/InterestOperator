#pragma once

// Harris����ͷ�ļ�

#ifndef HARRIS_OPERATOR_H
#define HARRIS_OPERATOR_H
#endif

#include <stdio.h>
#include <math.h>
#include "mymath.h"
#include "opencv2/core/core.hpp"  
#include"opencv2/highgui/highgui.hpp"  
#include"opencv2/imgproc/imgproc.hpp"

# define Harris_Window_Size 21  // ���ڳߴ�

using namespace cv;
using namespace std;

class Harris_Operator
{

public:
	Harris_Operator();
	~Harris_Operator();

	void extract(const Mat&); // ����ȡ����
	void draw(Mat&);		  // ����ƺ���

private:

	int if_any_point_extracted; // �����������ʶ����ȱʡΪ0������ǰ���е����������Ϊ1

	vector<CvPoint> points_of_interest; // ���ڴ���Ȥ�������

	Mat image_input;	// �����ͼ��
	Mat max_in_window;  // ��ǰ�����������Ȥֵ
	Mat gx;				// x�����ݶȣ�ԭʼ��
	Mat gy;				// y�����ݶȣ�ԭʼ��
	Mat Gx;				// x�����ݶȣ�������˹�˲���
	Mat Gy;				// y�����ݶȣ�������˹�˲���
	Mat GxGy;			// Э����GxGy
	Mat GxGx;			// Э����GxGx
	Mat GyGy;			// Э����GyGy
	Mat Intensity;		// ǿ��ֵ
	Mat Gausiankernel;  // ��˹��

};