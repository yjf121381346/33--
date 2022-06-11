#include "armor.h"

int main()
{
	VideoCapture capture("D:\\×°¼×°å²âÊÔÊÓÆµ.avi");

	Kalman KF; //³õÊ¼»¯¿¨¶ûÂüÂË²¨Æ÷
	
	while (true)
	{
		Mat frame;
		capture >> frame;
		Armor arm(frame);

		arm.setWidth(0.001175, 30, 0.28); //³õÊ¼»¯ÏñËØ¿í¶È£¬±»²âÎïÌå¿í¶È£¬ÉãÏñÍ·½¹¾à
		arm.BinaryImage(Armor::redLight);
		arm.findLight();
		arm.fixLight();
		arm.distanceMeasure();
		arm.setPredictCenter(KF.predict(arm.getCenter()));
		arm.drawLight();
		arm.show();
	}

	/*Mat frame = imread("D:\\×°¼×°å²âÊÔÍ¼Æ¬.jpg");
	Armor arm(frame);

	arm.setWidth(0.001175, 30, 0.28);
	arm.BinaryImage(Armor::blueLight);
	arm.findLight();
	arm.fixLight();
	arm.drawLight();
	arm.distanceMeasure();
	arm.show();*/
}