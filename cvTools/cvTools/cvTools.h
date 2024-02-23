#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>
#include "ui_cvTools.h"
#include <QLabel>
#include <QImage>
#include <QPixmap>
#include <QPushButton>
#include <QApplication>
#include <QFileDialog>
#include <string>
#include <QSlider>
#include <QVBoxLayout>
#include <opencv2/opencv.hpp>
#include <omp.h>
#define M_PI 3.1415926
using namespace std;
using namespace cv;
class cvTools : public QWidget
{
    Q_OBJECT

public:
    cvTools(QWidget *parent = nullptr);
    ~cvTools();
	string QStr2Str(const QString qStr);											//QString转string，防止中文乱码
	QString str2qstr(const string& str);											//string转QString
	void showimg(cv::Mat mat);
	void showoutimg(cv::Mat mat);
private:
    Ui::cvToolsClass ui;
	cv::Mat m_src;
	cv::Mat grayImage;
	cv::Mat BandPassimage;
public slots:
	void OpenImg();
	void OpenTiff();
	void ResizeImg();
	void ThresholdImg(int threshold);
	void ErodedImg(int erodesize);
	void DilateImg(int dilatesize);
	void OpenExImg(int opensize);
	void CloseExImg(int closesize);
	void CvtColorImg();
	void HistImg();
	void BlurImg(int blursize);
	void GaussianBlurImg(int gausssize);
	void MedianBlurImg(int medsize);
	void BilateralFilterImg(int filtersize);
	void BoxFilterImg(int boxfiltersize);
	void CannyLowImg(int cannysize);
	void CannyHighImg(int cannysize);
	void EnhanceImg();
	void OtsuImg();
	void DB1();
	void DB2();
	void DB3();
	void DB4();
	void Contrast(int contrast);
	void Gamma(int gamma);
	void getNewMat();
	void BandPass();
};
