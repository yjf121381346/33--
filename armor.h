#pragma once

#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

//卡尔曼滤波器
class Kalman
{
private:
	KalmanFilter KF;

	//测量矩阵
	Mat measurement;
public:
	//初始化KalmanFilter
	Kalman();

	//用当前中心点预测，并返回预测点
	Point predict(Point p);
};

//装甲板
class Armor
{
private:
	//摄像头获取的图像
	Mat image;
	//二值图
	Mat binaryImage;

	//所有的灯条
	vector<RotatedRect> allLight;

	//左右灯条
	RotatedRect leftLight;
	RotatedRect rightLight;

	//所有匹配好的左右灯条，两两存入
	vector<RotatedRect> allFixedLight;

	//装甲板中心点
	Point center;
	//预测值
	Point predictCenter;

	//像素宽度
	double pixelWidth;

	//装甲板宽度
	double armorWidth;

	//相机焦距
	double focus;

	//估计距离(cm)
	double distance;

public:
	//灯条颜色
	enum { redLight = 1, blueLight = 2 };

	//构造函数，对image初始化
	Armor(Mat & m);

	//将画面二值化
	bool BinaryImage(int color);

	//找到所有可能的灯条存入allLight
	bool findLight();

	//灯条匹配
	bool fixLight();

	//对矩形x坐标排序
	friend bool cmp(RotatedRect & rect1, RotatedRect & rect2);

	//画出灯条矩形和装甲板中心点
	void drawLight();

	//显示画面
	void show();

	//设置两个宽度
	void setWidth(double p, double a, double f);

	//测距
	void distanceMeasure();

	//返回中心点
	Point getCenter();

	//设置预测点
	void setPredictCenter(Point p);
};