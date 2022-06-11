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
#### 成员为装甲板的各项参数
vector容器内装有所有可能的灯条\
左右灯条\
装甲板中心点\
预测点

#### 函数包括
图像的二值化\
寻找灯条\
灯条匹配\
画出最小外接矩形\
测距

## armor.cpp
`BinaryImage()`的参数列表可以选择灯条颜色，使用通道分离后二值化\
`findLight()`用面积和面积比初步过滤轮廓，push_pack进allLight\
`fixLight()`首先对allLight的x坐标从小到大排序，然后遍历allLight，用各种比值匹配左右灯条，如长宽比、高度比等\
`distanceMeasure()`测距部分采用相似三角形的原理，已知相机焦距和被测物体宽度可以求出距离\
卡尔曼滤波也在此实现

## main.cpp
依次使用相应函数，可对视频或图片进行处理
```cpp
VideoCapture capture()   //读取视频
Kalman KF                //初始化卡尔曼滤波器
setWidth()               //设置像素宽度、相机焦距和被测物体宽度
BinaryImage()            //二值化
findLight()              //寻找灯条
fixLight()               //匹配灯条
distanceMeasure()        //测距
setPredictCenter()       //预测
drawLight()              //拟合灯条和装甲板中心点
show()                   //显示画面
```
