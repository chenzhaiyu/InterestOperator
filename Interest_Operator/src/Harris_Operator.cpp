#include "Harris_Operator.h"

// Harris����cpp�ļ�

// ��ʼ��Harris����ʵ��
Harris_Operator::Harris_Operator()
{
	// �������������Ժ�ʵ��ͳһ���ߣ����Ƽ���ֵ
	// ȱʡΪ0��Ĭ��Ϊû����ȡ����Ȥ��
	if_any_point_extracted = 0;		
}

Harris_Operator::~Harris_Operator()
{

}

// ����ȡ����
void Harris_Operator::extract(const Mat& image_input)
{
	// �����ܶԻҶ�ͼ��Ĵ���
	CV_Assert(image_input.depth() == CV_8U);

	// ������˹�ˣ���X��Y����ĸ�˹�����
	Mat GausiankernelX = getGaussianKernel(Harris_Window_Size, 0.5, CV_32FC1);
	Mat GausiankernelY = getGaussianKernel(Harris_Window_Size, 0.5, CV_32FC1);
	Gausiankernel = GausiankernelX * GausiankernelY.t();

	// ����x��y������ݶȾ���
	gx = Mat(Harris_Window_Size, Harris_Window_Size, CV_16SC1, Scalar(0)); // �ݶȿ���Ϊ��������С��-128��Signed Char��short����
	gy = Mat(Harris_Window_Size, Harris_Window_Size, CV_16SC1, Scalar(0)); // TODO: �ݾ����ʼֵ���������ܶ�����Ч����Ӱ��
	Gx = Mat(Harris_Window_Size, Harris_Window_Size, CV_32F, Scalar(0));   // ������˹�˲����ݶȾ���
	Gy = Mat(Harris_Window_Size, Harris_Window_Size, CV_32F, Scalar(0));

	Intensity = Mat(Harris_Window_Size, Harris_Window_Size, CV_32F, Scalar(0));   // ǿ��ֵ����
	max_in_window = Mat(image_input.rows, image_input.cols, CV_32F, Scalar(0));	  // ��ǰ���ڵ����ǿ��ֵ

	// �Բ�ͬ�Ĵ��ڽ��б���
	for (int j = 0; j < image_input.rows - 2 - Harris_Window_Size; j += Harris_Window_Size - 1)
	{
		for (int i = 0; i < (image_input.cols - 2 - Harris_Window_Size); i += Harris_Window_Size - 1)
		{
			static int j_buffer = 0;
			static int i_buffer = 0;

			// �Ե�ǰ���ڵ�ÿ����Ԫ���б���
			for (int n = 0; n < Harris_Window_Size; n++)
			{
				for (int m = 0; m < Harris_Window_Size; m++)
				{
					gx.at<short>(n, m) = image_input.ptr<uchar>(j + n)[i + m + 1] - image_input.ptr<uchar>(j + n)[i + m];
					gy.at<short>(n, m) = image_input.ptr<uchar>(j + n + 1)[i + m] - image_input.ptr<uchar>(j + n)[i + m];
				}
			}
			// �����С�����е�gx��gy�ֱ���и�˹�˲����ò����������
			// GaussianBlur(gx, Gx, Size(3, 3), 0.9, 0.9);
			// GaussianBlur(gy, Gy, Size(3, 3), 0.9, 0.9);

			// ����gxgy������˷�
			Mat _GxGx = gx.mul(gx);
			Mat _GyGy = gy.mul(gy);
			Mat _GxGy = gx.mul(gy);

			Mat __GxGx;
			Mat __GxGy;
			Mat __GyGy;

			_GxGx.convertTo(__GxGx,CV_32F);
			_GxGy.convertTo(__GxGy, CV_32F);
			_GyGy.convertTo(__GyGy, CV_32F);


			// �����Ŷ�M���ٽ����˲����������det(M)����Ϊ0����0û������
			GaussianBlur(__GxGx, GxGx, Size(3, 3), 0.1, 0.1);
			GaussianBlur(__GyGy, GyGy, Size(3, 3), 0.1, 0.1);
			GaussianBlur(__GxGy, GxGy, Size(3, 3), 0.1, 0.1);
			
			
			// ����Ĵ��������ؽ�������ֵ���㣬����д�������⣬det(M)��Ϊ0��ǿ��ֵ��Ϊ����û������
			// �������С����ѭ����Ϊ�˼��㵱ǰ������ǿ��ֵIntensity
			for (int n = 0; n < Harris_Window_Size - 1; n++)
			{
				for (int m = 0; m < Harris_Window_Size - 1; m++)
				{
					float det_M = GxGx.at<float>(n, m) * GyGy.at<float>(n, m) + GxGy.at<float>(n, m) * GxGy.at<float>(n, m);
					float k_tr_2_M = GxGx.at<float>(n, m) + GyGy.at<float>(n, m);
					Mat Covariance(2, 2, CV_32F, Scalar(0));
					Mat eigenValue;
					Mat eigenVector;

					/*
					Covariance.at<float>(0, 0) = GxGx.at<short>(n, m) * GxGx.at<short>(n, m);
					Covariance.at<float>(0, 1) = GxGy.at<short>(n, m) * GxGy.at<short>(n, m);
					Covariance.at<float>(1, 0) = GxGy.at<short>(n, m) * GxGy.at<short>(n, m);
					Covariance.at<float>(1, 1) = GyGy.at<short>(n, m) * GyGy.at<short>(n, m);
					*/

					//printf("%f\n", Covariance.at<float>(0, 0));
					//printf("%f\n", Covariance.at<float>(0, 1));
					//printf("%f\n", Covariance.at<float>(1, 0));
					//printf("%f\n", Covariance.at<float>(1, 1));

					// ��������ֵ������������ֻ��Ϊ�˺�Forstner�򻯺��ʽ�Ӽ���Ľ�����бȽϣ����ֽ�����
					eigen(Covariance, eigenValue, eigenVector);
					
					// �ֱ�Ϊ��������ֵ
					float lambda_1 = eigenValue.at<float>(0, 0);
					float lambda_2 = eigenValue.at<float>(1, 0);
					
					k_tr_2_M = 0.04 * pow(k_tr_2_M, 2);
					Intensity.at<float>(n, m) = det_M + k_tr_2_M;

					// ������С�������ǿ��ֵ����Ÿ���
					if (Intensity.at<float>(n, m) > max_in_window.at<float>(j, i) && Intensity.at<float>(n, m) > 100000)
					{
						// �������ǿ��ֵ
						max_in_window.at<float>(j, i) = Intensity.at<float>(n, m);
						if (i_buffer == i && j_buffer == j && if_any_point_extracted != 0)
						{
							points_of_interest.pop_back();
						}
						points_of_interest.push_back(cvPoint(i + m, j + n));
						if_any_point_extracted = 1;
					}
					// ���´�������
					i_buffer = i;
					j_buffer = j;
				}
			}
			
			
		}
	}
}

// ����ƺ���
void Harris_Operator::draw(Mat& Result)
{
	for (int i = 0; i < points_of_interest.size(); i++)
		cv::circle(Result, points_of_interest[i], 4, cv::Scalar(255, 255, 100));
}