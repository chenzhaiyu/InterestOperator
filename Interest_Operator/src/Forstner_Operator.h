#pragma once

// Forstner����ͷ�ļ�

#ifndef FORSTNER_OPERATOR_H
#define FORSTNER_OPERATOR_H
#endif

#include <stdio.h>
#include <math.h>
#include "mymath.h"
#include "opencv2/core/core.hpp"  
#include "opencv2/highgui/highgui.hpp"  
#include "opencv2/imgproc/imgproc.hpp"

# define Forstner_Window_Size 5				// ���ڳߴ�
# define Forstner_Threshold_q 0.9			// Բ��q��ֵ
# define Forstner_Threshold_w 7000			// Ȩֵw��ֵ
# define Forstner_Refined_Window_Size 11	// ��ѡ���ڳߴ�

using namespace cv;
using namespace std;


class Forstner_Operator
{

public:
	Forstner_Operator();
	~Forstner_Operator();

	void extract(const Mat&); // ����ȡ����
	void draw(Mat&);		  // ����ƺ���

private:

	int if_any_point_extracted;  // �����������ʶ����ȱʡΪ0������ǰ���е����������Ϊ1
	int window_j_present;		 // ��ǰ���ڵ��кţ��������Ŵ��ڽ��б���
	int window_i_present;		 // ��ǰ���ڵ��кţ��������Ŵ��ڽ��б���
	int window_j_before;		 // ��һ�δ��ڵ��к�
	int window_i_before;		 // ��һ�δ��ڵ��к�

	Mat gradient_u;				 // �з�����ݶ�
	Mat gradient_v;				 // �з�����ݶ�
	Mat image_Q;				 // ����q�����ɵ�Mat��
	Mat image_W;				 // ����w�����ɵ�Mat��
	Mat max_q_in_window;		 // ��ǰ����������qֵ
	Mat max_w_in_window;		 // ��ǰ����������wֵ
	Mat N;						 // Э�������

	vector<CvPoint> points_of_interest;  // ���ڴ���Ȥ�������

};

