#pragma once

// Moravec����ͷ�ļ�

#ifndef MORAVEC_OPERATOR_H
#define MORAVEC_OPERATOR_H
#endif

#include <stdio.h>
#include <math.h>
#include "mymath.h"
#include "opencv2/core/core.hpp"  
#include "opencv2/highgui/highgui.hpp"  
#include "opencv2/imgproc/imgproc.hpp"

# define Moravec_Interest_Window_Size 5		// ��ȡ���ڳߴ�
# define Moravec_Filter_Window_Size 21		// ��ѡ���ڳߴ�
# define Moravec_Threshold 20				// ��Ȥֵ��ֵ

using namespace cv;
using namespace std;

class Moravec_Operator
{

public:
	Moravec_Operator();
	~Moravec_Operator();

	void extract(const Mat&);	// ����ȡ����
	void draw(Mat&);			// ����ƺ���


private:

	Mat image_input;			// �����ͼ��
	Mat max_in_window;			// �����������Ȥֵ

	int if_any_point_extracted; // �����������ʶ����ȱʡΪ0������ǰ���е����������Ϊ1

	int window_j_present;		// ��ǰ���ڵ��кţ��������Ŵ��ڽ��б���
	int window_i_present;		// ��ǰ���ڵ��кţ��������Ŵ��ڽ��б���
	int window_j_before;		// ��һ�δ��ڵ��к�
	int window_i_before;		// ��һ�δ��ڵ��к�

	// 4�������Ϸֱ������ݶ�ֵ
	int interest_value_1;	
	int interest_value_2;
	int interest_value_3;
	int interest_value_4;

	vector<CvPoint> points_of_interest; // ���ڴ���Ȥ�������

};

