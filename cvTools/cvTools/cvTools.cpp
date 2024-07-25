#include "cvTools.h"
#include<iostream>
#include<opencv2/opencv.hpp>
#include<ctime>

using namespace std;
using namespace cv;

cv::Mat gaussian_low_pass_kernel(cv::Mat scr, float sigma);
cv::Mat gaussian_low_pass_filter(cv::Mat &src, float d0);
cv::Mat gaussian_high_pass_kernel(cv::Mat scr, float sigma);
cv::Mat gaussian_high_pass_filter(cv::Mat &src, float d0);
cv::Mat frequency_filter(cv::Mat &scr, cv::Mat &blur);
cv::Mat image_make_border(cv::Mat &src);
void fftshift(cv::Mat &plane0, cv::Mat &plane1);


// 高斯低通滤波核函数
cv::Mat gaussian_low_pass_kernel(cv::Mat scr, float sigma)
{
	cv::Mat gaussianBlur(scr.size(), CV_32FC1); //，CV_32FC1
	float d0 = sigma;//高斯函数参数，越小，频率高斯滤波器越窄，滤除高频成分越多，图像就越平滑
	for (int i = 0; i < scr.rows; i++) {
		for (int j = 0; j < scr.cols; j++) {
			float d = pow(float(i - scr.rows / 2), 2) + pow(float(j - scr.cols / 2), 2);//分子,计算pow必须为float型
			gaussianBlur.at<float>(i, j) = expf(-d / (2 * d0*d0))/(d0*sqrt(2*M_PI));//expf为以e为底求幂（必须为float型）
		}
	}
	return gaussianBlur;
}
// 高斯低通滤波
cv::Mat gaussian_low_pass_filter(cv::Mat &src, float d0)
{
	cv::Mat padded = image_make_border(src);
	cv::Mat gaussian_kernel = gaussian_low_pass_kernel(padded, d0);
	cv::Mat result = frequency_filter(padded, gaussian_kernel);
	return result;

}
// 高斯高通滤波核函数
cv::Mat gaussian_high_pass_kernel(cv::Mat scr, float sigma)
{
	cv::Mat gaussianBlur(scr.size(), CV_32FC1); //，CV_32FC1
	float d0 = sigma;
	for (int i = 0; i < scr.rows; i++) {
		for (int j = 0; j < scr.cols; j++) {
			float d = pow(float(i - scr.rows / 2), 2) + pow(float(j - scr.cols / 2), 2);//分子,计算pow必须为float型
			gaussianBlur.at<float>(i, j) = 1 - expf(-d / (2 * d0*d0)) / (d0*sqrt(2 * M_PI));
		}
	}
	return gaussianBlur;
}
// 高斯高通滤波
cv::Mat gaussian_high_pass_filter(cv::Mat &src, float d0)
{
	cv::Mat padded = image_make_border(src);
	cv::Mat gaussian_kernel = gaussian_high_pass_kernel(padded, d0);//理想低通滤波器
	cv::Mat result = frequency_filter(padded, gaussian_kernel);
	return result;
	
}
// 频率域滤波
cv::Mat frequency_filter(cv::Mat &scr, cv::Mat &blur)
{
	cv::Mat mask = scr == scr;
	scr.setTo(0.0f, ~mask);

	//创建通道，存储dft后的实部与虚部（CV_32F，必须为单通道数）
	cv::Mat plane[] = { scr.clone(), cv::Mat::zeros(scr.size() , CV_32FC1) };

	cv::Mat complexIm;
	cv::merge(plane, 2, complexIm); // 合并通道 （把两个矩阵合并为一个2通道的Mat类容器）
	cv::dft(complexIm, complexIm); // 进行傅立叶变换，结果保存在自身

	// 分离通道（数组分离）
	cv::split(complexIm, plane);

	// 以下的操作是频域迁移
	fftshift(plane[0], plane[1]);

	// *****************滤波器函数与DFT结果的乘积****************
	cv::Mat blur_r, blur_i, BLUR;
	cv::multiply(plane[0], blur, blur_r);  // 滤波（实部与滤波器模板对应元素相乘）
	cv::multiply(plane[1], blur, blur_i);  // 滤波（虚部与滤波器模板对应元素相乘）
	cv::Mat plane1[] = { blur_r, blur_i };

	// 再次搬移回来进行逆变换
	fftshift(plane1[0], plane1[1]);
	cv::merge(plane1, 2, BLUR); // 实部与虚部合并

	cv::idft(BLUR, BLUR);       // idft结果也为复数
	BLUR = BLUR / BLUR.rows / BLUR.cols;

	cv::split(BLUR, plane);//分离通道，主要获取通道

	return plane[0];
}
// 图像边界处理
cv::Mat image_make_border(cv::Mat &src)
{
	int w = cv::getOptimalDFTSize(src.cols); // 获取DFT变换的最佳宽度
	int h = cv::getOptimalDFTSize(src.rows); // 获取DFT变换的最佳高度

	cv::Mat padded;
	// 常量法扩充图像边界，常量 = 0
	cv::copyMakeBorder(src, padded, 0, h - src.rows, 0, w - src.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));
	padded.convertTo(padded, CV_32FC1);

	return padded;
}
// fft变换后进行频谱搬移
void fftshift(cv::Mat &plane0, cv::Mat &plane1)
{
	// 以下的操作是移动图像  (零频移到中心)
	int cx = plane0.cols / 2;
	int cy = plane0.rows / 2;
	cv::Mat part1_r(plane0, cv::Rect(0, 0, cx, cy));  // 元素坐标表示为(cx, cy)
	cv::Mat part2_r(plane0, cv::Rect(cx, 0, cx, cy));
	cv::Mat part3_r(plane0, cv::Rect(0, cy, cx, cy));
	cv::Mat part4_r(plane0, cv::Rect(cx, cy, cx, cy));

	cv::Mat temp;
	part1_r.copyTo(temp);  //左上与右下交换位置(实部)
	part4_r.copyTo(part1_r);
	temp.copyTo(part4_r);

	part2_r.copyTo(temp);  //右上与左下交换位置(实部)
	part3_r.copyTo(part2_r);
	temp.copyTo(part3_r);

	cv::Mat part1_i(plane1, cv::Rect(0, 0, cx, cy));  //元素坐标(cx,cy)
	cv::Mat part2_i(plane1, cv::Rect(cx, 0, cx, cy));
	cv::Mat part3_i(plane1, cv::Rect(0, cy, cx, cy));
	cv::Mat part4_i(plane1, cv::Rect(cx, cy, cx, cy));

	part1_i.copyTo(temp);  //左上与右下交换位置(虚部)
	part4_i.copyTo(part1_i);
	temp.copyTo(part4_i);

	part2_i.copyTo(temp);  //右上与左下交换位置(虚部)
	part3_i.copyTo(part2_i);
	temp.copyTo(part3_i);
}
cvTools::cvTools(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
	ui.label_ColorCard_1->installEventFilter(this);
	cv::Mat image = cv::imread(".\\image\\R.png");
	if (!image.empty())
	{
		m_src = image.clone();
	}
	
	showimg(image);
	//图像尺寸操作--槽函数
	connect(ui.PB_OPENIMG, &QPushButton::clicked, this, &cvTools::OpenImg);
	connect(ui.PB_RESIZEIMG, &QPushButton::clicked, this, &cvTools::ResizeImg);
	//二值化--槽函数
	connect(ui.S_THRESHOLD, &QSlider::valueChanged, this, &cvTools::ThresholdImg);
	//腐蚀滤波--槽函数
	connect(ui.S_ERODE, &QSlider::valueChanged, this, &cvTools::ErodedImg);
	//膨胀滤波--槽函数
	connect(ui.S_DILATE, &QSlider::valueChanged, this, &cvTools::DilateImg);
	//开操作--槽函数
	connect(ui.S_OPEN, &QSlider::valueChanged, this, &cvTools::OpenExImg);
	//闭操作--槽函数
	connect(ui.S_CLOSE, &QSlider::valueChanged, this, &cvTools::CloseExImg);
	//灰度化--槽函数
	connect(ui.PB_CVTCOLORIMG, &QPushButton::clicked, this, &cvTools::CvtColorImg);
	//直方图--槽函数
	connect(ui.PB_HIST, &QPushButton::clicked, this, &cvTools::HistImg);
	//均值滤波--槽函数
	connect(ui.S_BLUR, &QSlider::valueChanged, this, &cvTools::BlurImg);
	//高斯滤波--槽函数
	connect(ui.S_GAUSSIANBLUR, &QSlider::valueChanged, this, &cvTools::GaussianBlurImg);
	//中值滤波--槽函数
	connect(ui.S_MEDIANBLUR, &QSlider::valueChanged, this, &cvTools::MedianBlurImg);
	//双边滤波--槽函数
	connect(ui.S_bilateralFilter, &QSlider::valueChanged, this, &cvTools::BilateralFilterImg);
	//局部均值滤波--槽函数
	connect(ui.S_BOXFILTER, &QSlider::valueChanged, this, &cvTools::BoxFilterImg);
	//边缘检测--槽函数
	connect(ui.S_CANNYLOW, &QSlider::valueChanged, this, &cvTools::CannyLowImg);
	connect(ui.S_CANNYHIGH, &QSlider::valueChanged, this, &cvTools::CannyHighImg);
	//增强对比度--槽函数
	connect(ui.PB_EnhanceImg, &QPushButton::clicked, this, &cvTools::EnhanceImg);
	//OTSU自动阈值分割--槽函数
	connect(ui.PB_OTSU, &QPushButton::clicked, this, &cvTools::OtsuImg);
	//对比度算法-槽函数
	connect(ui.PB_db1, &QPushButton::clicked, this, &cvTools::DB1);
	connect(ui.PB_db2, &QPushButton::clicked, this, &cvTools::DB2);
	connect(ui.PB_db3, &QPushButton::clicked, this, &cvTools::DB3);
	connect(ui.PB_db4, &QPushButton::clicked, this, &cvTools::DB4);
	//对比度
	connect(ui.S_Contrast, &QSlider::valueChanged, this, &cvTools::Contrast);
	//Gamma
	connect(ui.S_Gamma, &QSlider::valueChanged, this, &cvTools::Gamma);
	//拼图槽函数
	connect(ui.PB_NewMat, &QPushButton::clicked, this, &cvTools::getNewMat);
	//tiff
	connect(ui.PB_ReadTiff, &QPushButton::clicked, this, &cvTools::OpenTiff);
	//BandPass
	connect(ui.PB_BandPass, &QPushButton::clicked, this, &cvTools::BandPass);
	//color
	connect(ui.PB_ChangToColorImg, &QPushButton::clicked, this, &cvTools::ShowColorImg);
	//叠加图片
	connect(ui.PB_Add2Pic, &QPushButton::clicked, this, &cvTools::Add2Pic);


	
	
}

cvTools::~cvTools()
{}
string cvTools::QStr2Str(const QString qStr)
{
	QByteArray data = qStr.toLocal8Bit();
	return string(data);
}

QString cvTools::str2qstr(const string & str)
{
	return QString::fromLocal8Bit(str.data());
}
void cvTools::showimg(cv::Mat mat)
{
	if (mat.channels() == 3)
	{
		QImage img(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
		ui.InputImg->setPixmap(QPixmap::fromImage(img));
	}
	else if (mat.channels() == 1)
	{
		QImage img(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
		ui.InputImg->setPixmap(QPixmap::fromImage(img));
	}
	else
	{
		std::cout << "其他类型的图像" << std::endl;
	}
	
}
void cvTools::showoutimg(cv::Mat mat)
{
	if (mat.channels() == 3)
	{
		QImage img(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
		ui.OutputImg->setPixmap(QPixmap::fromImage(img));
	}
	else if (mat.channels() == 1)
	{
		QImage img(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
		ui.OutputImg->setPixmap(QPixmap::fromImage(img));
	}
	else
	{
		std::cout << "其他类型的图像" << std::endl;
	}
	
	
}
void cvTools::OpenImg()
{
	// 加载OpenCV图像
	 // 弹出文件对话框
	QString imgPath = QFileDialog::getOpenFileName(nullptr, "选择文件", "", "All Files (*);;Text Files (*.png)");

	cv::Mat image = cv::imread(QStr2Str(imgPath));
	if (!image.empty())
	{
		m_src = image.clone();
	}
	showimg(image);
}

bool cvTools::eventFilter(QObject * obj, QEvent * event)
{
	if (obj == ui.label_ColorCard_1 && event->type() == QEvent::Paint)
	{
		///EventFilter中的QPainter设备是需要监视的对象，不能是this父窗口
		//QPainter painter(this);
		QPainter painter(ui.label_ColorCard_1);
		painter.setRenderHint(QPainter::Antialiasing);//反锯齿
		/*int x = ui.label_ColorCard->x();
		int y = ui.label_ColorCard->y();*/
		int x = 0;
		int y = 0;
		int w = ui.label_ColorCard_1->rect().width();
		int h = ui.label_ColorCard_1->rect().height();
		QLinearGradient linearGradient(x, y, x, y + h);//渐变区域
		linearGradient.setColorAt(0, Qt::red);
		//linearGradient.setColorAt(0.33, Qt::yellow);
		//linearGradient.setColorAt(0.66, Qt::green);
		linearGradient.setColorAt(0.5, Qt::white);
		linearGradient.setColorAt(1, Qt::blue);
		painter.setBrush(linearGradient);//设置画刷，则painter.drawRect(rect());绘制出渐变背景
		painter.drawRect(QRect(x, y, w, h));
	}
	return QWidget::eventFilter(obj, event);
}

void readTiff(QString fileName, int & width, int & height, QVector<float>& resultVec)
{
	QByteArray data = fileName.toLocal8Bit();
	string tmpstr = string(data);

	cv::Mat image;     //创建一个空图像image
	image = cv::imread(tmpstr, cv::IMREAD_ANYDEPTH);  //读取文件夹中的图像
	image.convertTo(image, CV_32FC1);
	if (image.empty()) {
		return; // 返回空的QVector
	}
	CV_Assert(image.channels() == 1); // 确保是单通道的Mat

	int rows = image.rows;
	int cols = image.cols;

	width = cols;
	height = rows;
	int Size_byte = width * height;
	resultVec.resize(Size_byte);

	//#pragma omp parallel for num_threads(4)
	for (int i = 0; i < rows; i++) {
		const float* ptr = image.ptr<float>(i);
		for (int j = 0; j < cols; j++) {
			resultVec[i*cols + j] = ptr[j];
		}
	}
}

void cvTools::OpenTiff()
{
	// 加载OpenCV图像
	 // 弹出文件对话框
	/*QString imgPath = QFileDialog::getOpenFileName(nullptr, "选择文件", "", "All Files (*);;Text Files (*.tiff)");

	BandPassimage = cv::imread(QStr2Str(imgPath), cv::IMREAD_UNCHANGED| cv::IMREAD_ANYDEPTH);
	if (!BandPassimage.empty())
	{
		cv::Mat showimage;
		cv::normalize(BandPassimage, showimage, 0, 255, cv::NORM_MINMAX);
		showimage.convertTo(showimage, CV_8U);
		cv::resize(showimage, showimage, cv::Size(ui.InputImg->width(), ui.InputImg->height()));
		showimg(showimage);
	}*/

	QString dirName = QFileDialog::getExistingDirectory(this, ("选择导入照片目录"), "", QFileDialog::Option::ReadOnly);
	if (dirName.isNull())
	{
		return;
	}
	QDir dir(dirName);
	dir.setFilter(QDir::Files);
	dir.setSorting(QDir::Time | QDir::Reversed);
	QFileInfoList list = dir.entryInfoList();

	QVector<QString>allFilepath;
	foreach(QFileInfo file, list)
	{
		QString filePath = file.filePath();
		allFilepath.push_back(filePath);
	}
	std::vector<cv::Mat> imageArray;
	for (int i=0;i< allFilepath.size();i++)
	{
		QByteArray data2 = allFilepath[0].toLocal8Bit();
		string strtiff = string(data2);
		cv::Mat image = cv::imread(strtiff, cv::IMREAD_ANYDEPTH);
		imageArray.push_back(image);
	}
	for (int j=1;j<37;j++)
	{
		
		cv::Mat S = cv::Mat::zeros(imageArray[0].rows, imageArray[0].cols, CV_32FC1);
		for (int i=0;i<100;i++)
		{
			cv::Mat image0 = imageArray[i].clone();
			float tmp = 1.0;
			if (sin(2 * M_PI / 100*i)==0)
			{
				tmp = 0;
			}
			else
			{
				tmp = sin(2 * M_PI / 100*i + M_PI / 18 * j) / sin(2 * M_PI / 100*i);
				
			}
			qDebug() << tmp;
			image0 *= tmp;
			S += image0;
		}
		S /= 100;
		S *= 2;
		float k = -1.56226;
		S /= k;
		QString str = QString::number(j)+ ".tiff";
		QByteArray data = str.toLocal8Bit();
		string strtiff = string(data);
		cv::imwrite(strtiff, S);
		qDebug() << j;
	}
	
}

void cvTools::ResizeImg()
{

	cv::resize(m_src, m_src, cv::Size(ui.InputImg->width(), ui.InputImg->height()));
	showimg(m_src);
}

void cvTools::ThresholdImg(int threshold)
{
	ui.S_THRESHOLD->setRange(0, 255); // 设置滑动条范围为0-255
	ui.LE_THRESHOLD->setText(QString::number(threshold));
	// 进行二值化操作
	cv::Mat binaryImage;
	if (grayImage.empty())
	{
		cv::threshold(m_src, binaryImage, threshold, 255, cv::THRESH_BINARY);
	}
	else
	{
		cv::threshold(grayImage, binaryImage, threshold, 255, cv::THRESH_BINARY);
	}
	showoutimg(binaryImage);

	
}

void cvTools::ErodedImg(int erodesize)
{
	ui.S_ERODE->setRange(1, 50);
	ui.S_ERODE->setSingleStep(2);
	ui.LE_ERODE->setText(QString::number(erodesize));

	// 定义腐蚀操作的结构元素
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(erodesize, erodesize));

	// 进行腐蚀操作
	cv::Mat erodedImage;
	cv::erode(m_src, erodedImage, kernel);
	showoutimg(erodedImage);

	
}

void cvTools::DilateImg(int dilatesize)
{
	ui.S_DILATE->setRange(1, 50);
	ui.S_DILATE->setSingleStep(2);
	ui.LE_DILATE->setText(QString::number(dilatesize));

	// 定义膨胀操作的结构元素
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(dilatesize, dilatesize));

	// 进行膨胀操作
	cv::Mat dilatedImage;
	cv::dilate(m_src, dilatedImage, kernel);
	showoutimg(dilatedImage);

	
}

void cvTools::OpenExImg(int opensize)
{
	ui.S_OPEN->setRange(1, 50);
	ui.S_OPEN->setSingleStep(2);
	ui.LE_OPEN->setText(QString::number(opensize));

	// 定义开操作的结构元素
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(opensize, opensize));

	// 进行开操作
	cv::Mat openedImage;
	cv::morphologyEx(m_src, openedImage, cv::MORPH_OPEN, kernel);
	showoutimg(openedImage);

	
}

void cvTools::CloseExImg(int closesize)
{
	ui.S_CLOSE->setRange(1, 50);
	ui.LE_CLOSE->setText(QString::number(closesize));

	// 定义闭操作的结构元素
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(closesize, closesize));

	// 进行闭操作
	cv::Mat closedImage;
	cv::morphologyEx(m_src, closedImage, cv::MORPH_CLOSE, kernel);
	showoutimg(closedImage);

	
}

void cvTools::CvtColorImg()
{
	// 将图像转换为灰度图像
	
	cv::cvtColor(m_src, grayImage, cv::COLOR_BGR2GRAY);
	showoutimg(grayImage);

	
	
}

void cvTools::HistImg()
{
	if (grayImage.empty())
	{
		cv::Mat image = m_src;
		
		// 分离通道
		std::vector<cv::Mat> bgr_planes;
		cv::split(image, bgr_planes);

		// 设置直方图参数
		int histSize = 256;    // 直方图中的箱数
		float range[] = { 0, 256 };    // 像素值的范围
		const float* histRange = { range };
		bool uniform = true, accumulate = false;

		// 计算每个通道的直方图
		cv::Mat b_hist, g_hist, r_hist;
		cv::calcHist(&bgr_planes[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
		cv::calcHist(&bgr_planes[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
		cv::calcHist(&bgr_planes[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

		// 创建直方图画布
		int histWidth = 512, histHeight = 400;
		cv::Mat histImage(histHeight, histWidth, CV_8UC3, cv::Scalar(0, 0, 0));

		// 归一化直方图
		cv::normalize(b_hist, b_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
		cv::normalize(g_hist, g_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
		cv::normalize(r_hist, r_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());

		// 绘制直方图
		int binWidth = cvRound((double)histWidth / histSize);
		for (int i = 1; i < histSize; i++)
		{
			cv::line(histImage, cv::Point(binWidth * (i - 1), histHeight - cvRound(b_hist.at<float>(i - 1))),
				cv::Point(binWidth * (i), histHeight - cvRound(b_hist.at<float>(i))),
				cv::Scalar(255, 0, 0), 2, 8, 0);
			cv::line(histImage, cv::Point(binWidth * (i - 1), histHeight - cvRound(g_hist.at<float>(i - 1))),
				cv::Point(binWidth * (i), histHeight - cvRound(g_hist.at<float>(i))),
				cv::Scalar(0, 255, 0), 2, 8, 0);
			cv::line(histImage, cv::Point(binWidth * (i - 1), histHeight - cvRound(r_hist.at<float>(i - 1))),
				cv::Point(binWidth * (i), histHeight - cvRound(r_hist.at<float>(i))),
				cv::Scalar(0, 0, 255), 2, 8, 0);
		}

	
		// 显示直方图
		cv::imshow("Histogram", histImage);
	}
	else
	{
		cv::Mat image = grayImage;

		// 计算直方图
		int histSize = 256;
		float range[] = { 0, 256 };
		const float* histRange = { range };
		bool uniform = true, accumulate = false;
		cv::Mat hist;
		cv::calcHist(&image, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);

		// 创建直方图画布
		int histWidth = 512, histHeight = 400;
		cv::Mat histImage(histHeight, histWidth, CV_8UC3, cv::Scalar(0, 0, 0));

		// 归一化直方图
		cv::normalize(hist, hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());

		// 绘制直方图
		int binWidth = cvRound((double)histWidth / histSize);
		for (int i = 1; i < histSize; i++)
		{
			cv::line(histImage, cv::Point(binWidth * (i - 1), histHeight - cvRound(hist.at<float>(i - 1))),
				cv::Point(binWidth * (i), histHeight - cvRound(hist.at<float>(i))),
				cv::Scalar(255, 255, 255), 2, 8, 0);
		}

		//// 添加坐标轴
		//cv::line(histImage, cv::Point(50, histHeight - 50), cv::Point(50, 50), cv::Scalar(255, 255, 255), 2, 8, 0);
		//cv::line(histImage, cv::Point(50, histHeight - 50), cv::Point(histWidth - 50, histHeight - 50), cv::Scalar(255, 255, 255), 2, 8, 0);

		//// 添加坐标轴标签
		//cv::putText(histImage, "0", cv::Point(30, histHeight - 30), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
		//cv::putText(histImage, "255", cv::Point(histWidth - 40, histHeight - 30), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
		//cv::putText(histImage, "Pixel Value", cv::Point(200, histHeight - 10), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
		//cv::putText(histImage, "Number of Pixels", cv::Point(10, 20), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);

		// 显示直方图
		cv::imshow("Histogram", histImage);
	}
	
	
}

void cvTools::BlurImg(int blursize)
{
	ui.S_BLUR->setRange(1, 50);
	ui.S_BLUR->setSingleStep(2);
	ui.LE_BLUR->setText(QString::number(blursize));

	// 创建输出图像
	cv::Mat outputImage;

	// 应用均值滤波
	cv::blur(m_src, outputImage, cv::Size(blursize, blursize));
	showoutimg(outputImage);
}

void cvTools::GaussianBlurImg(int gausssize)
{
	ui.S_GAUSSIANBLUR->setRange(1, 50);
	ui.S_GAUSSIANBLUR->setSingleStep(2);
	ui.LE_GAUSSIANBLUR->setText(QString::number(gausssize));

	// 创建输出图像
	cv::Mat gaussianOutput;
	if (0== gausssize%2)
	{
	}
	else
	{
		// 应用高斯滤波
		cv::GaussianBlur(m_src, gaussianOutput, cv::Size(gausssize, gausssize), 0);
	}
	
	showoutimg(gaussianOutput);
	
}

void cvTools::MedianBlurImg(int medsize)
{
	ui.S_MEDIANBLUR->setRange(1, 50);
	ui.S_MEDIANBLUR->setSingleStep(2);
	ui.LE_MEDIANBLUR->setText(QString::number(medsize));

	// 创建输出图像
	cv::Mat medianOutput;
	if (0 == medsize % 2)
	{
	}
	else
	{
		// 应用中值滤波
		cv::medianBlur(m_src, medianOutput, medsize);
	}

	showoutimg(medianOutput);
}

void cvTools::BilateralFilterImg(int filtersize)
{
	ui.S_bilateralFilter->setRange(1, 50);
	ui.S_bilateralFilter->setSingleStep(2);
	ui.LE_bilateralFilter->setText(QString::number(filtersize));
	// 创建输出图像
	cv::Mat outputImage;

	// 应用双边滤波
	cv::bilateralFilter(m_src, outputImage, filtersize, 10, 10);
	
	showoutimg(outputImage);
}

void cvTools::BoxFilterImg(int boxfiltersize)
{
	ui.S_BOXFILTER->setRange(1, 50);
	ui.S_BOXFILTER->setSingleStep(2);
	ui.LE_BOXFILTER->setText(QString::number(boxfiltersize));
	// 创建输出图像
	cv::Mat meanFilteredOutput;

	// 应用局部均值滤波
	cv::boxFilter(m_src, meanFilteredOutput, -1, cv::Size(boxfiltersize, boxfiltersize));
	showoutimg(meanFilteredOutput);
}

void cvTools::CannyLowImg(int cannysize)
{
	ui.S_CANNYLOW->setRange(0, 255);
	ui.LE_CANNYLOW->setText(QString::number(cannysize));
	int highsize = ui.S_CANNYHIGH->value();
	// 创建输出图像
	cv::Mat cannyOutput;

	// 应用Canny算法进行边缘检测
	cv::Canny(m_src, cannyOutput, cannysize, highsize);
	showoutimg(cannyOutput);
}

void cvTools::CannyHighImg(int cannysize)
{
	ui.S_CANNYHIGH->setRange(0, 255);
	ui.LE_CANNYHIGH->setText(QString::number(cannysize));
	int lowsize = ui.S_CANNYLOW->value();
	// 创建输出图像
	cv::Mat cannyOutput;

	// 应用Canny算法进行边缘检测
	cv::Canny(m_src, cannyOutput, lowsize, cannysize);
	showoutimg(cannyOutput);
}

void cvTools::EnhanceImg()
{
	if (grayImage.empty())
	{
		// 将图像转换为浮点型格式
		cv::Mat inputImage = m_src;
		m_src.convertTo(inputImage, CV_32F);

		// 分离图像的三个通道
		std::vector<cv::Mat> channels;
		cv::split(inputImage, channels);

		// 对每个通道进行对比度增强
		for (int i = 0; i < 3; i++)
		{
			cv::Scalar mean, stddev;
			cv::meanStdDev(channels[i], mean, stddev);
			channels[i] = (channels[i] - mean.val[0]) / stddev.val[0] * 50 + 128;
		}

		// 合并通道
		cv::Mat contrastEnhancedImage;
		cv::merge(channels, contrastEnhancedImage);

		// 将图像转换回8位无符号整型格式
		contrastEnhancedImage.convertTo(contrastEnhancedImage, CV_8U);
		showoutimg(contrastEnhancedImage);
	}
	else
	{
		// 计算图像的均值和标准差
		cv::Scalar mean, stddev;
		cv::meanStdDev(grayImage, mean, stddev);

		// 对图像进行对比度增强
		cv::Mat contrastEnhancedImage = (grayImage - mean.val[0]) / stddev.val[0] * 50 + 128;
		showoutimg(contrastEnhancedImage);
	}
	

	
}

void cvTools::OtsuImg()
{
	cv::cvtColor(m_src, grayImage, cv::COLOR_BGR2GRAY);
	// 应用OTSU算法进行图像二值化
	cv::Mat binaryImage;
	cv::threshold(grayImage,binaryImage, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
	showoutimg(binaryImage);
}

void cvTools::DB1()
{

	cv::Mat onMat = cv::imread("./image/db/onMat.tiff", cv::IMREAD_ANYDEPTH);
	cv::Mat offMat = cv::imread("./image/db/offMat.tiff", cv::IMREAD_ANYDEPTH);

	cv::Mat resultMat;
	resultMat = onMat / offMat;
	cv::imwrite("./image/db/DB1.tiff", resultMat);


}

void cvTools::DB2()
{
	cv::Mat onMat = cv::imread("./image/db/onMat.tiff", cv::IMREAD_ANYDEPTH);
	cv::Mat offMat = cv::imread("./image/db/offMat.tiff", cv::IMREAD_ANYDEPTH);

	cv::Mat resultMat;
	resultMat = offMat / onMat;
	cv::imwrite("./image/db/DB2.tiff", resultMat);
}

void cvTools::DB3()
{
	cv::Mat onMat = cv::imread("./image/db/onMat.tiff", cv::IMREAD_ANYDEPTH);
	cv::Mat offMat = cv::imread("./image/db/offMat.tiff", cv::IMREAD_ANYDEPTH);

	cv::Mat resultMat;
	resultMat = (offMat - onMat)/offMat;
	cv::imwrite("./image/db/DB3.tiff", resultMat);
}

void cvTools::DB4()
{
	cv::Mat onMat = cv::imread("./image/db/onMat.tiff", cv::IMREAD_ANYDEPTH);
	cv::Mat offMat = cv::imread("./image/db/offMat.tiff", cv::IMREAD_ANYDEPTH);

	cv::Mat resultMat;
	resultMat = (offMat - onMat) / (offMat +onMat);
	cv::imwrite("./image/db/DB4.tiff", resultMat);
}

void cvTools::Contrast(int contrast)
{
	ui.S_Contrast->setRange(0, 255);
	ui.S_Contrast->setSingleStep(1);
	double alpha = contrast/100.0; // 对比度调整参数
	cv::Mat m_dst;
	if (grayImage.empty())
	{
		m_src.convertTo(m_dst, -1, alpha, 0);
		
	}
	else
	{
		grayImage.convertTo(m_dst, -1, alpha, 0);
	}
	showoutimg(m_dst);
}

void cvTools::Gamma(int gamma)
{
	ui.S_Gamma->setRange(0, 255);
	ui.S_Gamma->setSingleStep(1);
	double gm = gamma / 100.0; // gamma调整参数
	cv::Mat m_dst;
	cv::Mat lookUpTable(1, 256, CV_8U);
	uchar* p = lookUpTable.ptr();
	for (int i = 0; i < 256; i++) {
		p[i] = cv::saturate_cast<uchar>(pow(i / 255.0, gm) * 255.0);
	}

	if (grayImage.empty())
	{
		cv::LUT(m_src, lookUpTable, m_dst);
	}
	else
	{
		cv::LUT(grayImage, lookUpTable, m_dst);
	}
	showoutimg(m_dst);
}

void cvTools::getNewMat()
{
	int num = ui.LE_Number->text().toInt();
	
	//根据num来讲Mat分割。num越大分割越多
	if (grayImage.empty())
	{//m_src
		
		int h = m_src.rows;
		int w = m_src.cols;
		int rectw = w / sqrt(num);
		int recth = h / sqrt(num);
		cv::Mat NewMat = cv::Mat::ones(m_src.rows, m_src.cols, CV_8UC3);
		for (int i=0;i<sqrt(num);i++)
		{
			for (int j=0;j< sqrt(num);j++)
			{
				int rectx = i * rectw;
				int recty = j * recth;
				//注意越界
				if (rectx+ rectw> w)
				{
					rectx = w - rectw-1;
				}
				if (recty+ recth>h)
				{
					recty = h - recth - 1;
				}
				cv::Rect tmpRect(rectx, recty, rectw, recth);
				cv::Mat a = m_src(tmpRect).clone();
				
				//插入到新图片中
				//我插入图片时，只是讲y，x坐标颠倒了。也可以制定新的规则。
				int xx = (rectx * 2024) % m_src.cols;
				int yy = (recty * 2024) % m_src.rows;
				if (xx + rectw > w)
				{
					xx = w - rectw - 1;
				}
				if (yy + recth > h)
				{
					yy = h - recth - 1;
				}
				cv::Mat roi(NewMat, cv::Rect(xx, yy, rectw, recth)); // 定义ROI
				a.copyTo(roi); // 将a复制到ROI处
			}
			
		}

		showoutimg(NewMat);
	}
	else
	{//grayImage
		int h = grayImage.rows;
		int w = grayImage.cols;
		int rectw = w / sqrt(num);
		int recth = h / sqrt(num);
		cv::Mat NewMat = cv::Mat::ones(grayImage.rows, grayImage.cols, CV_8UC1);
		for (int i = 0; i < sqrt(num); i++)
		{
			for (int j = 0; j < sqrt(num); j++)
			{
				int rectx = i * rectw;
				int recty = j * recth;
				//注意越界
				if (rectx + rectw > w)
				{
					rectx = w - rectw - 1;
				}
				if (recty + recth > h)
				{
					recty = h - recth - 1;
				}
				cv::Rect tmpRect(rectx, recty, rectw, recth);
				cv::Mat a = grayImage(tmpRect).clone();

				//插入到新图片中
				//我插入图片时，只是讲y，x坐标颠倒了。也可以制定新的规则。
				int xx = (rectx * 2024) % grayImage.cols;
				int yy = (recty * 2024) % grayImage.rows;
				if (xx + rectw > w)
				{
					xx = w - rectw - 1;
				}
				if (yy + recth > h)
				{
					yy = h - recth - 1;
				}
				cv::Mat roi(NewMat, cv::Rect(xx, yy, rectw, recth)); // 定义ROI
				a.copyTo(roi); // 将a复制到ROI处
			}

		}

		showoutimg(NewMat);
	}
}

void cvTools::BandPass()
{
	float down = ui.LE_BandPassDown->text().toFloat();
	float up = ui.lineEdit_BandPassUp->text().toFloat();
	float D0 = down;
	float D1 = up;
	//===================================test1============================//
	//Mat lowpass = gaussian_low_pass_filter(BandPassimage, D0);
	//Mat highpass = gaussian_high_pass_filter(BandPassimage, D1);
	//BandPassimageResult = highpass - lowpass;
	//cv::imwrite("lowpass.tiff", lowpass);
	//cv::imwrite("highpass.tiff", highpass);
	//===================================test2============================//
	//// 高斯低通滤波
	//cv::Mat lowpass ;
	//cv::GaussianBlur(BandPassimage, lowpass, cv::Size(5, 5), D0);
	//// 高斯高通滤波
	//cv::Mat highpass;
	//cv::GaussianBlur(BandPassimage, highpass, cv::Size(5, 5), D1);
	//// 将低通滤波结果和高通滤波结果相加
	//BandPassimageResult = highpass - lowpass;
	//cv::imwrite("lowpass.tiff", lowpass);
	//cv::imwrite("highpass.tiff", highpass);
	//===================================test3============================//
	//中值滤波减少椒盐噪声
	//cv::Mat medMat;
	//medianBlur(BandPassimage, medMat, 3); // 参数3表示核的大小
	//双边滤波
	//cv::Mat filtered;
	//cv::bilateralFilter(BandPassimage, filtered, 5, 0.1, 1); // 应用双边滤波，参数分别为滤波器的直径、空间高斯函数标准差和灰度值相似性高斯函数标准差
	//BandPassimageResult = filtered;

	//===============================test5-形态学================//
	cv::Mat largeStructureFiltered;
	cv::Mat smallStructureFiltered;
	// 大结构滤波至50像素
	cv::Mat largeStructureKernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(D1, D1));
	cv::morphologyEx(BandPassimage, largeStructureFiltered, cv::MORPH_CLOSE, largeStructureKernel);
	// 小结构滤波至3像素
	cv::Mat smallStructureKernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(D0, D0));
	cv::morphologyEx(BandPassimage, smallStructureFiltered, cv::MORPH_CLOSE, smallStructureKernel);
	for (int i = 0; i < 9; i++)
	{
		cv::morphologyEx(smallStructureFiltered, smallStructureFiltered, cv::MORPH_CLOSE, smallStructureKernel);
	}
	
	cv::imwrite("largeStructureFiltered.tiff", largeStructureFiltered);
	cv::imwrite("smallStructureFiltered.tiff", smallStructureFiltered);
	BandPassimageResult = smallStructureFiltered -largeStructureFiltered;
	BandPassimageResult = smallStructureFiltered ;


	
	
	

	//==========================结果展示=======================//
	cv::normalize(BandPassimageResult, BandPassimageResult, 0, 1, cv::NORM_MINMAX);
	cv::imwrite("BandPassimageResult.tiff", BandPassimageResult);
	cv::Mat bandpassShow;
	cv::normalize(BandPassimageResult, bandpassShow, 0, 255, cv::NORM_MINMAX);
	bandpassShow.convertTo(bandpassShow, CV_8U);
	// 对图像进行去噪处理
	/*fastNlMeansDenoising(bandpassShow, bandpassShow, 15, 5, 21);*/
	cv::resize(bandpassShow, bandpassShow, cv::Size(ui.InputImg->width(), ui.InputImg->height()));
	showoutimg(bandpassShow);

}

void cvTools::ShowColorImg()
{
	QPixmap pix = grab(QRect(ui.label_ColorCard_1->x(), ui.label_ColorCard_1->y(), ui.label_ColorCard_1->width(), ui.label_ColorCard_1->height()));
	QImage image0 = pix.toImage();
	QVector<double>magValueVec;
	for (int i = 0; i < BandPassimageResult.rows; i++) {
		for (int j = 0; j < BandPassimageResult.cols; j++) {
			magValueVec.push_back(BandPassimageResult.at<float>(i, j));
		}
	}

	QImage colorImg_1 = ChangetoQcolorimg(BandPassimageResult.cols, BandPassimageResult.rows, magValueVec,
		ui.LE_ColorImgDown->text().toDouble(), ui.lineEdit_ColorImgUp->text().toDouble(), image0);

	QImage imgScaled = colorImg_1.scaled(ui.InputImg->width(), ui.InputImg->height(), Qt::IgnoreAspectRatio);
	QPixmap pixmap = QPixmap::fromImage(imgScaled);
	ui.OutputImg->setPixmap(pixmap);

	magValueVec.clear();
	for (int i = 0; i < BandPassimage.rows; i++) {
		for (int j = 0; j < BandPassimage.cols; j++) {
			magValueVec.push_back(BandPassimage.at<float>(i, j));
		}
	}
	colorImg_1 = ChangetoQcolorimg(BandPassimage.cols, BandPassimage.rows, magValueVec,
		ui.LE_ColorImgDown->text().toDouble(), ui.lineEdit_ColorImgUp->text().toDouble(), image0);
	imgScaled = colorImg_1.scaled(ui.InputImg->width(), ui.InputImg->height(), Qt::IgnoreAspectRatio);
	pixmap = QPixmap::fromImage(imgScaled);
	ui.InputImg->setPixmap(pixmap);

}

QImage cvTools::ChangetoQcolorimg(int img_width, int img_height, const QVector<double>& magnetic_strength, double down, double up, QImage & image0)
{
	int w_pic = img_width;
	int h_pic = img_height;
	QVector<QRgb> rgbColourTable;
	QImage img = QImage(w_pic, h_pic, QImage::Format_Indexed8);
	QVector<uchar>grayvalue;



	if (w_pic * h_pic != magnetic_strength.size())
	{
		return img;
	}
	if (down >= up)
	{
		return img;
	}

	for (int i = 0; i < w_pic * h_pic; i++)
	{
		if (magnetic_strength[i] <= down)
		{
			grayvalue.push_back((uchar)0);
		}
		if (magnetic_strength[i] >= up)
		{
			grayvalue.push_back((uchar)255);
		}
		if ((magnetic_strength[i] < up) && (magnetic_strength[i] > down))
		{
			int tmp = (magnetic_strength[i] - down) / (up - down) * 255;
			uchar utmp = (uchar)tmp;
			grayvalue.push_back(utmp);
		}

	}

	for (int row = 0; row < h_pic; ++row) {
		uchar* scanLine = img.scanLine(row); // 获取每一行的地址
		memcpy(scanLine, &grayvalue[row * w_pic], w_pic); // 将数据拷贝到对应地址
	}


	QImage copyImage = img.copy(); // 复制整个图像
	for (int y = 0; y < 256; y++)
	{

		QColor color;
		int index = (int)((float)(y) / (float)(255) * (float)(image0.height() - 2));

		if (index == 0)
		{
			color = image0.pixelColor(QPoint(1, image0.height() - 2));
		}
		else if (index == image0.height() - 2)
		{
			color = image0.pixelColor(QPoint(1, 1));
		}
		else
		{

			color = image0.pixelColor(QPoint(1, image0.height() - 2 - index));
		}

		QRgb rgb = color.rgb();
		rgbColourTable.push_back(rgb);
	}


	copyImage.setColorTable(rgbColourTable);



	return copyImage;
}

void cvTools::Add2Pic()
{
	QString imgPath = QFileDialog::getOpenFileName(nullptr, "选择A", "", "All Files (*);;Text Files (*.tiff)");
	//小尺寸图片
	cv::Mat A = cv::imread(QStr2Str(imgPath), cv::IMREAD_UNCHANGED | cv::IMREAD_ANYDEPTH);
	cv::Mat A_color;
	if (!A.empty())
	{
		cv::normalize(A, A, 0, 1, cv::NORM_MINMAX, CV_32F);
		// 将32F图片A转换为8U图片
		cv::Mat A_8U;
		A.convertTo(A_8U, CV_8U, 255.0);

		// 创建BGRA图片
		cv::Mat BGRA(A.rows, A.cols, CV_8UC4);

		// 映射到伪彩色空间
		cv::applyColorMap(A_8U, BGRA, cv::COLORMAP_JET);

		// 设置透明度为40%
		for (int i = 0; i < BGRA.rows; i++) {
			for (int j = 0; j < BGRA.cols; j++) {
				uchar* pSrc = BGRA.ptr<uchar>(i, j);
				pSrc[3] = 0.4 * 255; // 设置透明度为40%
			}
		}
		A_color = BGRA.clone();
		imshow("A", A_color);
	}
	else
	{
		return;
	}

	imgPath = QFileDialog::getOpenFileName(nullptr, "选择B", "", "All Files (*);;Text Files (*.tiff)");
	//大尺寸图片
	cv::Mat B = cv::imread(QStr2Str(imgPath), cv::IMREAD_UNCHANGED | cv::IMREAD_ANYDEPTH);
	cv::Mat B_color;
	if (!B.empty())
	{
		cv::normalize(B, B, 0, 1, cv::NORM_MINMAX, CV_32F);
		// 将32F图片A转换为8U图片
		cv::Mat B_8U;
		B.convertTo(B_8U, CV_8U, 255.0);

		// 创建BGRA图片
		cv::Mat BGRA(B.rows, B.cols, CV_8UC4);

		// 映射到伪彩色空间
		//cv::applyColorMap(B_8U, BGRA, cv::COLORMAP_JET);

		// 设置透明度为40%
		for (int i = 0; i < BGRA.rows; i++) {
			for (int j = 0; j < BGRA.cols; j++) {
				uchar* pSrc = BGRA.ptr<uchar>(i, j);
				pSrc[0] = B_8U.at<uchar>(i, j);
				pSrc[1] = B_8U.at<uchar>(i, j);
				pSrc[2] = B_8U.at<uchar>(i, j);
				pSrc[3] = 255; // 设置透明度为40%
			}
		}
		B_color = BGRA.clone();
		cv::resize(B_color, B_color, A_color.size());
		imshow("B", B_color);
	}
	else
	{
		return;
	}
	if (A_color.type() != B_color.type()) {
		// 如果数据类型不同，进行类型转换
		B_color.convertTo(B_color, A_color.type());
	}
	if (A_color.size()!= B_color.size())
	{
		cv::resize(B_color, B_color, A_color.size());
	}

	// 将彩色图像A和B_color叠加
	cv::Mat result(A_color.rows, A_color.cols, CV_8UC4);

	for (int i = 0; i < A_color.rows; i++) {
		for (int j = 0; j < A_color.cols; j++) {
			
			uchar* colorA = A_color.ptr<uchar>(i, j);
			uchar* colorB = B_color.ptr<uchar>(i, j);

			// 计算叠加后的颜色值，考虑透明度
			float alphaA = colorA[3] / 255.0;
			float alphaB = colorB[3] / 255.0;

			cv::Vec4b resultColor;
			resultColor[0] = alphaA * colorA[0] + (1 - alphaA) * alphaB * colorB[0];
			resultColor[1] = alphaA * colorA[1] + (1 - alphaA) * alphaB * colorB[1];
			resultColor[2] = alphaA * colorA[2] + (1 - alphaA) * alphaB * colorB[2];
			resultColor[3] = std::max(colorA[3], colorB[3]); // 保留较大的透明度

			result.at<cv::Vec4b>(i, j) = resultColor;
		}
	}

	imshow("C", result);
	//////////////////////////////==========================================/////////////////////////////////////////////
	QString backgroundimagePath = QFileDialog::getOpenFileName(nullptr, "选择背景图片", "", "All Files (*);;Text Files (*.tiff)");
	cv::Mat backMat = cv::imread(QStr2Str(backgroundimagePath), cv::IMREAD_UNCHANGED | cv::IMREAD_ANYDEPTH);
	if (!backMat.empty())
	{
		cv::normalize(backMat, backMat, 0, 1, cv::NORM_MINMAX);
		backMat.convertTo(backMat, CV_8U, 255.0);
		cv::resize(backMat, backMat, cv::Size(206, 306));
		// 转换为RGB格式的cv::Mat
		cv::Mat RGBMat;
		cv::cvtColor(backMat, RGBMat, cv::COLOR_GRAY2RGB);
		// 如果需要透明度，可以使用以下代码
		QImage backimage(RGBMat.data, RGBMat.cols, RGBMat.rows, RGBMat.step, QImage::Format_ARGB32);

		QImage ABC;//伪彩img
		QImage forwardimage = ABC.convertToFormat(QImage::Format_ARGB32);
		for (int y = 0; y < forwardimage.height(); ++y) {
			QRgb *scanLine = reinterpret_cast<QRgb*>(forwardimage.scanLine(y));
			for (int x = 0; x < forwardimage.width(); ++x) {
				QRgb color = scanLine[x];
				int alpha = qAlpha(color) * 0.4; // 40% opacity
				scanLine[x] = qRgba(qRed(color), qGreen(color), qBlue(color), alpha);
			}
		}

		// 确保两张图像的大小相同
		if (backimage.size() != forwardimage.size()) {
			// 可以进行适当的缩放或裁剪操作使它们大小一致
			return;
		}

		// 创建一个新的QImage对象用于存储叠加后的图像
		QImage result(backimage.size(), QImage::Format_ARGB32);
		result.fill(Qt::transparent); // 填充透明背景

		// 将两张图像叠加在一起
		QPainter painter(&result);
		painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
		painter.drawImage(0, 0, backimage);
		painter.drawImage(0, 0, forwardimage);
		painter.end();

	}
	else
	{
		return;
	}

}
