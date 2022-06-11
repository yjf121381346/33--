#include "armor.h"

int main()
{
	VideoCapture capture("D:\\װ�װ������Ƶ.avi");

	Kalman KF; //��ʼ���������˲���
	
	while (true)
	{
		Mat frame;
		capture >> frame;
		Armor arm(frame);

		arm.setWidth(0.001175, 30, 0.28); //��ʼ�����ؿ�ȣ����������ȣ�����ͷ����
		arm.BinaryImage(Armor::redLight);
		arm.findLight();
		arm.fixLight();
		arm.distanceMeasure();
		arm.setPredictCenter(KF.predict(arm.getCenter()));
		arm.drawLight();
		arm.show();
	}

	/*Mat frame = imread("D:\\װ�װ����ͼƬ.jpg");
	Armor arm(frame);

	arm.setWidth(0.001175, 30, 0.28);
	arm.BinaryImage(Armor::blueLight);
	arm.findLight();
	arm.fixLight();
	arm.drawLight();
	arm.distanceMeasure();
	arm.show();*/
}