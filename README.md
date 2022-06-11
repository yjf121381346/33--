# 33-参赛了就队 视觉程序说明
---
## 编译使用
使用Visual Studio 2017 版本 15.9\
_MSC_VER = 1916\
OpenCV版本4.0.0

## armor.h
### Kalman类
定义了一个卡尔曼滤波器
`KalmanFilter KF`

以及测量矩阵`Mat measurement`

包含一个预测函数`predict()`，返回预测点
### Armor类
成员为装甲板的各项参数：\
左右灯条\
装甲板中心点\
预测点

函数包括：\
图像的二值化\
寻找灯条\
灯条匹配\
画出最小外接矩形\
测距

## armor.cpp
`BinaryImage()`的参数列表可以选择灯条颜色
通道分离后二值化\
`findLight()`用面积和面积比初步过滤轮廓，push_pack进allLight\
`fixLight()`首先对allLight的x坐标从小到大排序，然后遍历用各种比值匹配左右灯条\
`distanceMeasure()`测距部分采用相似三角形，已知相机焦距和被测物体宽度可以求出距离\
卡尔曼滤波也在此实现

## main.cpp
依次使用相应函数，可对视频或图片进行处理
```cpp
VideoCapture capture()
Kalman KF
setWidth()
BinaryImage()
findLight()
fixLight()
distanceMeasure()
setPredictCenter()
drawLight()
show()
```
