#pragma once

#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

//�������˲���
class Kalman
{
private:
	KalmanFilter KF;

	//��������
	Mat measurement;
public:
	//��ʼ��KalmanFilter
	Kalman();

	//�õ�ǰ���ĵ�Ԥ�⣬������Ԥ���
	Point predict(Point p);
};

//װ�װ�
class Armor
{
private:
	//����ͷ��ȡ��ͼ��
	Mat image;
	//��ֵͼ
	Mat binaryImage;

	//���еĵ���
	vector<RotatedRect> allLight;

	//���ҵ���
	RotatedRect leftLight;
	RotatedRect rightLight;

	//����ƥ��õ����ҵ�������������
	vector<RotatedRect> allFixedLight;

	//װ�װ����ĵ�
	Point center;
	//Ԥ��ֵ
	Point predictCenter;

	//���ؿ��
	double pixelWidth;

	//װ�װ���
	double armorWidth;

	//�������
	double focus;

	//���ƾ���(cm)
	double distance;

public:
	//������ɫ
	enum { redLight = 1, blueLight = 2 };

	//���캯������image��ʼ��
	Armor(Mat & m);

	//�������ֵ��
	bool BinaryImage(int color);

	//�ҵ����п��ܵĵ�������allLight
	bool findLight();

	//����ƥ��
	bool fixLight();

	//�Ծ���x��������
	friend bool cmp(RotatedRect & rect1, RotatedRect & rect2);

	//�����������κ�װ�װ����ĵ�
	void drawLight();

	//��ʾ����
	void show();

	//�����������
	void setWidth(double p, double a, double f);

	//���
	void distanceMeasure();

	//�������ĵ�
	Point getCenter();

	//����Ԥ���
	void setPredictCenter(Point p);
};