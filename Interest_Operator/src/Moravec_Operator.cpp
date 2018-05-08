#include "Moravec_Operator.h"

// Moravec����cpp�ļ�

// ��ʼ��Moravec����ʵ��
Moravec_Operator::Moravec_Operator()
{
	if_any_point_extracted = 0;
	window_j_before = 0;
	window_i_before = 0;
	window_j_present = 0;
	window_i_present = 0;
}


Moravec_Operator::~Moravec_Operator()
{

}

// ����ȡ����
void Moravec_Operator::extract(const Mat& image_input)
{
	max_in_window = Mat(image_input.rows, image_input.cols, CV_8UC1, Scalar(0)); // �����Ǹ��ӣ����Ҳ��8λ������255����Ȥֵ���޷�����max_in_window��
	CV_Assert(image_input.depth() == CV_8U);							         // �����ܶ�CV_8Uͼ��Ĵ������������׳��쳣

	// �����ض��н��б�������һ�к����һ�в����м���
	for (int j = 2; j < image_input.rows - 2; ++j)                               
	{
		for (int i = 2; i < (image_input.cols - 2); ++i)				       
		{
			// ��4��������ݶȳ�ʼ��Ϊ0
			interest_value_1 = 0;
			interest_value_2 = 0;
			interest_value_3 = 0;
			interest_value_4 = 0;

			// 4���������ݶ�ֵ����
			for (int w = -int(Moravec_Interest_Window_Size / 2); w <= int(Moravec_Interest_Window_Size / 2) - 1; w++)
			{
				interest_value_1 += pow(image_input.ptr<uchar>(j + w)[i] - image_input.ptr<uchar>(j + w + 1)[i], 2);			 // ��ֱ����
				interest_value_2 += pow(image_input.ptr<uchar>(j)[i + w] - image_input.ptr<uchar>(j)[i + w + 1], 2);			 // ˮƽ����
				interest_value_3 += pow(image_input.ptr<uchar>(j + w)[i + w] - image_input.ptr<uchar>(j + w + 1)[i + w + 1], 2); // ���µ�����
				interest_value_4 += pow(image_input.ptr<uchar>(j + w)[i + w] - image_input.ptr<uchar>(j + w + 1)[i + w - 1], 2); // ���ϵ�����
			}
			// 4���������ݶ�ֵ��Сֵ
			int interest_value_min = min(interest_value_1, interest_value_2, interest_value_3, interest_value_4) / 100; // ����100��Ϊ�˲�����Ȥֵ����255�������Ͳ��øĶ�max_in_window������
			
			// ��ǰ���ڵ�������ţ��Դ���������������ţ�e.g.1��2��3��4��
			window_j_present = j / Moravec_Filter_Window_Size;
			window_i_present = i / Moravec_Filter_Window_Size;

			if (interest_value_min > Moravec_Threshold && interest_value_min > max_in_window.at<uchar>(window_j_present, window_i_present))
			{
				// ֱ�����������������жϽ������ƾֲ����
				max_in_window.at<uchar>(window_j_present, window_i_present) = interest_value_min;
				// ���������ڵ�ǰ���������и�����Ȥֵ�ĵ�������������޳�ǰ��ĵ�
				if (window_i_before == window_i_present && window_j_before == window_j_present && if_any_point_extracted != 0)
				{
					points_of_interest.pop_back();
				}
				// �����������ĵ�ǰ�����������
				points_of_interest.push_back(cvPoint(i, j));
				if_any_point_extracted = 1;
			}
			// Ϊ���ж��ǲ��ǻ���ͬһ�����ӣ�ͨ���Ƚϸ��������¾�ֵ
			window_i_before = window_i_present;
			window_j_before = window_j_present;
		}
	}
}

// ����ƺ���
void Moravec_Operator::draw(Mat& Result)
{
	for (int i = 0; i < points_of_interest.size(); i++)
		cv::circle(Result, points_of_interest[i], 4, cv::Scalar(255, 255, 100));
}