#include "Harris_Operator.h"
#include "Moravec_Operator.h"
#include "Forstner_Operator.h"

// ����Moravec���ӣ�Forstner���ӣ�Harris���ӵ�������

int main(int argc, char* argv[])
{
	char imageName[] = "data/image1.jpg";
				
	Mat image;		// �Ҷ�ͼ������ʵ����ȡ����
	Mat colorImage; // ��ɫͼ��������ʾ��ȡЧ��

	// ����ͼ��
	image = imread(imageName, CV_8U);	
	colorImage = imread(imageName, CV_LOAD_IMAGE_UNCHANGED);
	
	if (image.empty())                     
	{
		printf("Could not open or find the image. \n");
		return -1;
	}

	Mat result1 = colorImage.clone();
	Mat result2 = colorImage.clone();
	Mat result3 = colorImage.clone();

	// Moravec��Ȥ�����ӵ���
	Moravec_Operator moravec_operator;
	moravec_operator.extract(image);
	moravec_operator.draw(result1);

	// Forstner��Ȥ�����ӵ���
	Forstner_Operator forstner_operator;
	forstner_operator.extract(image);
	forstner_operator.draw(result2);

	// Harris��Ȥ�����ӵ���
	Harris_Operator harris_operator;
	harris_operator.extract(image);
	harris_operator.draw(result3);

	// ��Ȥ�������
	namedWindow("Moravec��Ȥ����ȡ���", WINDOW_AUTOSIZE);     
	imshow("Moravec��Ȥ����ȡ���", result1);

	namedWindow("Forstner��Ȥ����ȡ���", WINDOW_AUTOSIZE);      
	imshow("Forstner��Ȥ����ȡ���", result2);

	namedWindow("Harris��Ȥ����ȡ���", WINDOW_AUTOSIZE);     
	imshow("Harris��Ȥ����ȡ���", result3);

	waitKey(0);
	return 0;
}