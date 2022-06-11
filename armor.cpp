#include "armor.h"

inline bool judgePoint(Point left, Point right, int k)
{
	if (right.y / (k * (right.x - left.x) + left.y) < 0.8 ||
		right.y / (k * (right.x - left.x) + left.y) > 1.25)
		return false;
	else
		return true;
}

Armor::Armor(Mat & m)
{
	image = m;
}

bool Armor::BinaryImage(int color)
{
	vector<Mat> channels;
	split(image, channels); //ͨ������

	if (color == 1)
		binaryImage = channels.at(2);
	else if (color == 2)
		binaryImage = channels.at(0);
	else
	{
		cout << "Invalid color!\n";
		return false;
	}

	GaussianBlur(binaryImage, binaryImage, Size(5, 5), 0); //��˹�˲�
	threshold(binaryImage, binaryImage, 200, 255, THRESH_BINARY); //��ֵ��
	return true;
}	

bool Armor::findLight()
{
	vector<vector<Point>> contours;
	findContours(binaryImage, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	cout << "size of contours: " << contours.size() << endl;
	vector<vector<Point>>::iterator pr;
	for (pr = contours.begin(); pr != contours.end(); pr++)
	{
		double area = fabs(contourArea(*pr));
		if (area < 50) //���˹�С���
			continue;

		RotatedRect temp;
		temp = minAreaRect(*pr);

		float h = temp.size.height;
		float w = temp.size.width;

		if ((contourArea(*pr) / w * h) <= 0.85) //����ȹ���
			continue;

		float ratio = h > w ? h / w : w / h;
		if (ratio < 2 || ratio > 8) //����ȹ���
			continue;
			
		/*Point2f vertices[4];
		temp.points(vertices);
		for (int j = 0; j < 4; j++)
			line(image, vertices[j], vertices[(j + 1) % 4], Scalar(0, 0, 255), 1);*/

		//drawContours(image, contours, -1, Scalar(0, 0, 255));
		allLight.push_back(minAreaRect(*pr));
	}

	if (allLight.size() < 2)
	{
		cout << "Can't find more than two lights!\n";
		return false;
	}

	return true;
}

bool cmp(RotatedRect & rect1, RotatedRect & rect2)
{
	return rect1.center.x < rect2.center.x;
}

bool Armor::fixLight()
{
	sort(allLight.begin(), allLight.end(), cmp);
	cout << "size of vector: " << allLight.size() << endl;

	for (int i = 0; i < allLight.size(); i++)
	{
		for (int j = i + 1; j < allLight.size(); j++)
		{
			RotatedRect templeftLight = allLight[i];
			RotatedRect temprightLight = allLight[j];

			float leftwidth, leftheight, rightwidth, rightheight;
			if (templeftLight.size.width < templeftLight.size.height) //��height��Ϊ����
			{
				leftwidth = templeftLight.size.width;
				leftheight = templeftLight.size.height;
			}
			else
			{
				leftwidth = templeftLight.size.height;
				leftheight = templeftLight.size.width;
			}
			if (temprightLight.size.width < temprightLight.size.height)
			{
				rightwidth = temprightLight.size.width;
				rightheight = temprightLight.size.height;
			}
			else
			{
				rightwidth = temprightLight.size.height;
				rightheight = temprightLight.size.width;
			}

			//Point temppoint(templeftLight.center.x - (templeftLight.size.width / 2 * sin(templeftLight.angle)), templeftLight.center.y + (templeftLight.size.width / 2 * cos(templeftLight.angle)));
			
			//��бʽ���̵�б��
			/*int k;
			if (templeftLight.size.width < templeftLight.size.height)
				k = tan(-templeftLight.angle);
			else
				k = 1 / (-tan(-templeftLight.angle));
			if (!judgePoint(templeftLight.center, temprightLight.center, k))
				continue;*/

			if (leftheight / rightheight < 0.8 || leftheight / rightheight > 1.2) //�߶ȱȹ���
				continue;

			if ((templeftLight.center.y / temprightLight.center.y) < 0.8 ||
				(templeftLight.center.y / temprightLight.center.y) > 1.2) //y�ȹ���
				continue;

			float leftratio = leftheight / leftwidth;
			float rightratio = rightheight / rightwidth;
			if (leftratio / rightratio < 0.2 || leftratio / rightratio > 2) //����ȹ���
				continue;

			float leftarea = leftheight * leftwidth;
			float rightarea = rightheight * rightwidth;
			if (leftarea / rightarea < 0.5 || leftarea / rightarea > 2)  //����ȹ���
				continue;

			leftLight = allLight[i];
			rightLight = allLight[j];

			center = Point((leftLight.center.x + rightLight.center.x) / 2,
				(leftLight.center.y + rightLight.center.y) / 2);
			break;
		}
		break;
	}

	return true;
}

void Armor::drawLight()
{
	//���������
	Point2f leftvertices[4];
	leftLight.points(leftvertices);
	for (int k = 0; k < 4; k++)
		line(image, leftvertices[k], leftvertices[(k + 1) % 4], Scalar(255, 0, 0), 1);

	//�����ҵ���
	Point2f rightvertices[4];
	rightLight.points(rightvertices);
	for (int k = 0; k < 4; k++)
		line(image, rightvertices[k], rightvertices[(k + 1) % 4], Scalar(255, 0, 0), 1);

	//�������ĵ�
	circle(image, center, 8, Scalar(0, 0, 255));
	//����Ԥ���
	circle(image, predictCenter, 8, Scalar(0, 255, 0));
}

void Armor::show()
{
	imshow("image", image);
	waitKey(30);
}

void Armor::setWidth(double p, double a, double f)
{
	pixelWidth = p;
	armorWidth = a;
	focus = f;
}

void Armor::distanceMeasure()
{
	//����
	distance = (armorWidth * focus) / (abs(leftLight.center.x - rightLight.center.x) * pixelWidth);

	//�ı���ʾ
	char disW[50];
	sprintf(disW, "Distance : %.2fcm", distance);
	putText(image, disW, Point(5, 20), FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(0, 255, 0), 2);
}

Point Armor::getCenter() 
{ 
	return center;
}

void Armor::setPredictCenter(Point p) 
{ 
	predictCenter = p; 
}

Kalman::Kalman()
{
	const int stateNum = 4;                                                                             //״ֵ̬4��1����(x,y,��x,��y)  
	const int measureNum = 2;                                                                           //����ֵ2��1����(x,y)    
	KF = KalmanFilter(stateNum, measureNum, 0);
	KF.transitionMatrix = (Mat_<float>(4, 4) << 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1);        //ת�ƾ���A  
	setIdentity(KF.measurementMatrix);                                                                  //��������H  
	setIdentity(KF.processNoiseCov, Scalar::all(10));                                                    //ϵͳ�����������Q  
	setIdentity(KF.measurementNoiseCov, Scalar::all(1));                                            //���������������R  
	setIdentity(KF.errorCovPost, Scalar::all(100000));                                                   //����������Э�������P    
	measurement = Mat::zeros(measureNum, 1, CV_32F);
}

Point Kalman::predict(Point p)
{
	//����������
	measurement.at<float>(0) = (float)p.x;
	measurement.at<float>(1) = (float)p.y;

	//Ԥ��
	Mat prediction = KF.predict();
	Point predictPoint = Point(prediction.at<float>(0), prediction.at<float>(1));

	//����
	KF.correct(measurement);

	return predictPoint;
}