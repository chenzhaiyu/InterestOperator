#include "Forstner_Operator.h"

// Forstner����cpp�ļ�

// ��ʼ��Forstner����ʵ��
Forstner_Operator::Forstner_Operator()
{
	if_any_point_extracted = 0;
	window_j_present = 0;
	window_i_present = 0;
	window_j_before = 0;
	window_i_before = 0;
}

Forstner_Operator::~Forstner_Operator()
{

}

// ����ȡ����
void Forstner_Operator::extract(const Mat& myImage)
{
	gradient_u = Mat(myImage.rows, myImage.cols, CV_16SC1, Scalar(0)); // ���з����ݶ�ͼ��
	gradient_v = Mat(myImage.rows, myImage.cols, CV_16SC1, Scalar(0)); // ���з����ݶ�ͼ��
	image_Q = Mat(myImage.rows, myImage.cols, CV_32FC1, Scalar(0));    // ������ͼ���Բ�ȣ�32������
	image_W = Mat(myImage.rows, myImage.cols, CV_32FC1, Scalar(0));    // ������ͼ���Ȩֵ��32������
	
	CV_Assert(myImage.depth() == CV_8U); // ������Ҷ�ͼ��

	max_q_in_window = Mat(myImage.rows, myImage.cols, CV_32FC1, Scalar(0)); // �浱ǰ������q�����ֵ
	max_w_in_window = Mat(myImage.rows, myImage.cols, CV_32FC1, Scalar(0)); // �浱ǰ������w�����ֵ

	// ����ÿ�����ش���Robert�ݶ�
	// ��һ�к����һ�в����м���
	for (int j = 0; j < myImage.rows - 1; ++j) 
	{
		for (int i = 0; i < (myImage.cols - 1); ++i)				      
		{
			gradient_u.at<short>(j, i) = myImage.ptr<uchar>(j + 1)[i + 1] - myImage.ptr<uchar>(j)[i];
			gradient_v.at<short>(j, i) = myImage.ptr<uchar>(j)[i + 1] - myImage.ptr<uchar>(j + 1)[i];
		}
	}

	// ��ÿ�ζԲ�ͬ���ڵı����н��Ƿ���ȡ����ı�ʶ������Ϊ0
	if_any_point_extracted = 0; 

	// ���㴰���лҶȵ�Э�������
	// ��������for�����������Ͻ����꣬���������
	for (int j = 0; j < myImage.rows - Forstner_Window_Size; j++)
	{
		for (int i = 0; i < (myImage.cols - Forstner_Window_Size); i++)
		{
			int sum_gradient_u_2 = 0;
			int sum_gradient_v_2 = 0;
			int sum_gradient_v_u = 0;
			float w = 0, q = 0;

			// ������ǰ��Ԫ�����ݶ�Э�������
			N = Mat(2, 2, CV_16SC1, Scalar(0)); 

			// ��������for����ÿ�������ڵ�����
			for (int n = 0; n < Forstner_Window_Size; n++)
			{
				for (int m = 0; m < Forstner_Window_Size; m++)
				{
					// Forstner�����ƺ�����ô��˾���
					sum_gradient_u_2 += gradient_u.ptr<short>(j + n)[i + m] * gradient_u.ptr<short>(j + n)[i + m];
					sum_gradient_v_2 += gradient_v.ptr<short>(j + n)[i + m] * gradient_v.ptr<short>(j + n)[i + m];
					sum_gradient_v_u += gradient_v.ptr<short>(j + n)[i + m] * gradient_u.ptr<short>(j + n)[i + m];
				}
			}
			// ��N����
			N.at<short>(0, 0) = sum_gradient_u_2;
			N.at<short>(0, 1) = sum_gradient_v_u;
			N.at<short>(1, 0) = sum_gradient_v_u;
			N.at<short>(1, 1) = sum_gradient_v_2;

			// ����Ȥֵq��w
			if ((N.at<short>(0, 0) + N.at<short>(1, 1)) != 0)
			{
				w = 1.0 * (N.at<short>(0, 0) * N.at<short>(1, 1) - N.at<short>(0, 1) * N.at<short>(1, 0)) / (N.at<short>(0, 0) + N.at<short>(1, 1));
				q = 4.0 * (N.at<short>(0, 0) * N.at<short>(1, 1) - N.at<short>(0, 1) * N.at<short>(1, 0)) / ((N.at<short>(0, 0) + N.at<short>(1, 1)) * (N.at<short>(0, 0) + N.at<short>(1, 1)));
			}

			// wȡ����ֵ
			// ����ȡ����Ȥֵ�����µ�Mat

			// �������������Ĵ���
			window_j_present = j / Forstner_Refined_Window_Size;
			window_i_present = i / Forstner_Refined_Window_Size;

			if (q > Forstner_Threshold_q && w > Forstner_Threshold_w && q > max_q_in_window.at<float>(window_j_present, window_i_present) && w > max_w_in_window.at<float>(window_j_present, window_i_present))
			{
				// ֱ������������к�ѡ���м�ֵ���ѡȡ
				// ���´��������ֵ��ֵ
				max_q_in_window.at<float>(window_j_present, window_i_present) = q;
				max_w_in_window.at<float>(window_j_present, window_i_present) = w;

				if (window_i_before == window_i_present && window_j_before == window_j_present && if_any_point_extracted != 0)
				{
					points_of_interest.pop_back();
				}

				points_of_interest.push_back(cvPoint(i + Forstner_Window_Size / 2, j + Forstner_Window_Size / 2));
				if_any_point_extracted = 1;
			}

			// ��q��w����Q��W��Mat��
			image_Q.at<float>(j + Forstner_Window_Size / 2, i + Forstner_Window_Size / 2) = q;
			image_W.at<float>(j + Forstner_Window_Size / 2, i + Forstner_Window_Size / 2) = w;

			// ���´�����Ż��棬�����ж���һ�α����ǲ��ǻ��ڵ�ǰ����
			window_i_before = window_i_present;
			window_j_before = window_j_present;
		}
	}
}

// ����ƺ���
void Forstner_Operator::draw(Mat& Result)
{
	for (int i = 0; i < points_of_interest.size(); i++)
		cv::circle(Result, points_of_interest[i], 4, cv::Scalar(255, 255, 100));
}