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


// ��˹��ͨ�˲��˺���
cv::Mat gaussian_low_pass_kernel(cv::Mat scr, float sigma)
{
	cv::Mat gaussianBlur(scr.size(), CV_32FC1); //��CV_32FC1
	float d0 = sigma;//��˹����������ԽС��Ƶ�ʸ�˹�˲���Խխ���˳���Ƶ�ɷ�Խ�࣬ͼ���Խƽ��
	for (int i = 0; i < scr.rows; i++) {
		for (int j = 0; j < scr.cols; j++) {
			float d = pow(float(i - scr.rows / 2), 2) + pow(float(j - scr.cols / 2), 2);//����,����pow����Ϊfloat��
			gaussianBlur.at<float>(i, j) = expf(-d / (2 * d0*d0))/(d0*sqrt(2*M_PI));//expfΪ��eΪ�����ݣ�����Ϊfloat�ͣ�
		}
	}
	return gaussianBlur;
}
// ��˹��ͨ�˲�
cv::Mat gaussian_low_pass_filter(cv::Mat &src, float d0)
{
	cv::Mat padded = image_make_border(src);
	cv::Mat gaussian_kernel = gaussian_low_pass_kernel(padded, d0);
	cv::Mat result = frequency_filter(padded, gaussian_kernel);
	return result;

}
// ��˹��ͨ�˲��˺���
cv::Mat gaussian_high_pass_kernel(cv::Mat scr, float sigma)
{
	cv::Mat gaussianBlur(scr.size(), CV_32FC1); //��CV_32FC1
	float d0 = sigma;
	for (int i = 0; i < scr.rows; i++) {
		for (int j = 0; j < scr.cols; j++) {
			float d = pow(float(i - scr.rows / 2), 2) + pow(float(j - scr.cols / 2), 2);//����,����pow����Ϊfloat��
			gaussianBlur.at<float>(i, j) = 1 - expf(-d / (2 * d0*d0)) / (d0*sqrt(2 * M_PI));
		}
	}
	return gaussianBlur;
}
// ��˹��ͨ�˲�
cv::Mat gaussian_high_pass_filter(cv::Mat &src, float d0)
{
	cv::Mat padded = image_make_border(src);
	cv::Mat gaussian_kernel = gaussian_high_pass_kernel(padded, d0);//�����ͨ�˲���
	cv::Mat result = frequency_filter(padded, gaussian_kernel);
	return result;
	
}
// Ƶ�����˲�
cv::Mat frequency_filter(cv::Mat &scr, cv::Mat &blur)
{
	cv::Mat mask = scr == scr;
	scr.setTo(0.0f, ~mask);

	//����ͨ�����洢dft���ʵ�����鲿��CV_32F������Ϊ��ͨ������
	cv::Mat plane[] = { scr.clone(), cv::Mat::zeros(scr.size() , CV_32FC1) };

	cv::Mat complexIm;
	cv::merge(plane, 2, complexIm); // �ϲ�ͨ�� ������������ϲ�Ϊһ��2ͨ����Mat��������
	cv::dft(complexIm, complexIm); // ���и���Ҷ�任���������������

	// ����ͨ����������룩
	cv::split(complexIm, plane);

	// ���µĲ�����Ƶ��Ǩ��
	fftshift(plane[0], plane[1]);

	// *****************�˲���������DFT����ĳ˻�****************
	cv::Mat blur_r, blur_i, BLUR;
	cv::multiply(plane[0], blur, blur_r);  // �˲���ʵ�����˲���ģ���ӦԪ����ˣ�
	cv::multiply(plane[1], blur, blur_i);  // �˲����鲿���˲���ģ���ӦԪ����ˣ�
	cv::Mat plane1[] = { blur_r, blur_i };

	// �ٴΰ��ƻ���������任
	fftshift(plane1[0], plane1[1]);
	cv::merge(plane1, 2, BLUR); // ʵ�����鲿�ϲ�

	cv::idft(BLUR, BLUR);       // idft���ҲΪ����
	BLUR = BLUR / BLUR.rows / BLUR.cols;

	cv::split(BLUR, plane);//����ͨ������Ҫ��ȡͨ��

	return plane[0];
}
// ͼ��߽紦��
cv::Mat image_make_border(cv::Mat &src)
{
	int w = cv::getOptimalDFTSize(src.cols); // ��ȡDFT�任����ѿ��
	int h = cv::getOptimalDFTSize(src.rows); // ��ȡDFT�任����Ѹ߶�

	cv::Mat padded;
	// ����������ͼ��߽磬���� = 0
	cv::copyMakeBorder(src, padded, 0, h - src.rows, 0, w - src.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));
	padded.convertTo(padded, CV_32FC1);

	return padded;
}
// fft�任�����Ƶ�װ���
void fftshift(cv::Mat &plane0, cv::Mat &plane1)
{
	// ���µĲ������ƶ�ͼ��  (��Ƶ�Ƶ�����)
	int cx = plane0.cols / 2;
	int cy = plane0.rows / 2;
	cv::Mat part1_r(plane0, cv::Rect(0, 0, cx, cy));  // Ԫ�������ʾΪ(cx, cy)
	cv::Mat part2_r(plane0, cv::Rect(cx, 0, cx, cy));
	cv::Mat part3_r(plane0, cv::Rect(0, cy, cx, cy));
	cv::Mat part4_r(plane0, cv::Rect(cx, cy, cx, cy));

	cv::Mat temp;
	part1_r.copyTo(temp);  //���������½���λ��(ʵ��)
	part4_r.copyTo(part1_r);
	temp.copyTo(part4_r);

	part2_r.copyTo(temp);  //���������½���λ��(ʵ��)
	part3_r.copyTo(part2_r);
	temp.copyTo(part3_r);

	cv::Mat part1_i(plane1, cv::Rect(0, 0, cx, cy));  //Ԫ������(cx,cy)
	cv::Mat part2_i(plane1, cv::Rect(cx, 0, cx, cy));
	cv::Mat part3_i(plane1, cv::Rect(0, cy, cx, cy));
	cv::Mat part4_i(plane1, cv::Rect(cx, cy, cx, cy));

	part1_i.copyTo(temp);  //���������½���λ��(�鲿)
	part4_i.copyTo(part1_i);
	temp.copyTo(part4_i);

	part2_i.copyTo(temp);  //���������½���λ��(�鲿)
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
	//ͼ��ߴ����--�ۺ���
	connect(ui.PB_OPENIMG, &QPushButton::clicked, this, &cvTools::OpenImg);
	connect(ui.PB_RESIZEIMG, &QPushButton::clicked, this, &cvTools::ResizeImg);
	//��ֵ��--�ۺ���
	connect(ui.S_THRESHOLD, &QSlider::valueChanged, this, &cvTools::ThresholdImg);
	//��ʴ�˲�--�ۺ���
	connect(ui.S_ERODE, &QSlider::valueChanged, this, &cvTools::ErodedImg);
	//�����˲�--�ۺ���
	connect(ui.S_DILATE, &QSlider::valueChanged, this, &cvTools::DilateImg);
	//������--�ۺ���
	connect(ui.S_OPEN, &QSlider::valueChanged, this, &cvTools::OpenExImg);
	//�ղ���--�ۺ���
	connect(ui.S_CLOSE, &QSlider::valueChanged, this, &cvTools::CloseExImg);
	//�ҶȻ�--�ۺ���
	connect(ui.PB_CVTCOLORIMG, &QPushButton::clicked, this, &cvTools::CvtColorImg);
	//ֱ��ͼ--�ۺ���
	connect(ui.PB_HIST, &QPushButton::clicked, this, &cvTools::HistImg);
	//��ֵ�˲�--�ۺ���
	connect(ui.S_BLUR, &QSlider::valueChanged, this, &cvTools::BlurImg);
	//��˹�˲�--�ۺ���
	connect(ui.S_GAUSSIANBLUR, &QSlider::valueChanged, this, &cvTools::GaussianBlurImg);
	//��ֵ�˲�--�ۺ���
	connect(ui.S_MEDIANBLUR, &QSlider::valueChanged, this, &cvTools::MedianBlurImg);
	//˫���˲�--�ۺ���
	connect(ui.S_bilateralFilter, &QSlider::valueChanged, this, &cvTools::BilateralFilterImg);
	//�ֲ���ֵ�˲�--�ۺ���
	connect(ui.S_BOXFILTER, &QSlider::valueChanged, this, &cvTools::BoxFilterImg);
	//��Ե���--�ۺ���
	connect(ui.S_CANNYLOW, &QSlider::valueChanged, this, &cvTools::CannyLowImg);
	connect(ui.S_CANNYHIGH, &QSlider::valueChanged, this, &cvTools::CannyHighImg);
	//��ǿ�Աȶ�--�ۺ���
	connect(ui.PB_EnhanceImg, &QPushButton::clicked, this, &cvTools::EnhanceImg);
	//OTSU�Զ���ֵ�ָ�--�ۺ���
	connect(ui.PB_OTSU, &QPushButton::clicked, this, &cvTools::OtsuImg);
	//�Աȶ��㷨-�ۺ���
	connect(ui.PB_db1, &QPushButton::clicked, this, &cvTools::DB1);
	connect(ui.PB_db2, &QPushButton::clicked, this, &cvTools::DB2);
	connect(ui.PB_db3, &QPushButton::clicked, this, &cvTools::DB3);
	connect(ui.PB_db4, &QPushButton::clicked, this, &cvTools::DB4);
	//�Աȶ�
	connect(ui.S_Contrast, &QSlider::valueChanged, this, &cvTools::Contrast);
	//Gamma
	connect(ui.S_Gamma, &QSlider::valueChanged, this, &cvTools::Gamma);
	//ƴͼ�ۺ���
	connect(ui.PB_NewMat, &QPushButton::clicked, this, &cvTools::getNewMat);
	//tiff
	connect(ui.PB_ReadTiff, &QPushButton::clicked, this, &cvTools::OpenTiff);
	//BandPass
	connect(ui.PB_BandPass, &QPushButton::clicked, this, &cvTools::BandPass);
	//color
	connect(ui.PB_ChangToColorImg, &QPushButton::clicked, this, &cvTools::ShowColorImg);
	//����ͼƬ
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
		std::cout << "�������͵�ͼ��" << std::endl;
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
		std::cout << "�������͵�ͼ��" << std::endl;
	}
	
	
}
void cvTools::OpenImg()
{
	// ����OpenCVͼ��
	 // �����ļ��Ի���
	QString imgPath = QFileDialog::getOpenFileName(nullptr, "ѡ���ļ�", "", "All Files (*);;Text Files (*.png)");

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
		///EventFilter�е�QPainter�豸����Ҫ���ӵĶ��󣬲�����this������
		//QPainter painter(this);
		QPainter painter(ui.label_ColorCard_1);
		painter.setRenderHint(QPainter::Antialiasing);//�����
		/*int x = ui.label_ColorCard->x();
		int y = ui.label_ColorCard->y();*/
		int x = 0;
		int y = 0;
		int w = ui.label_ColorCard_1->rect().width();
		int h = ui.label_ColorCard_1->rect().height();
		QLinearGradient linearGradient(x, y, x, y + h);//��������
		linearGradient.setColorAt(0, Qt::red);
		//linearGradient.setColorAt(0.33, Qt::yellow);
		//linearGradient.setColorAt(0.66, Qt::green);
		linearGradient.setColorAt(0.5, Qt::white);
		linearGradient.setColorAt(1, Qt::blue);
		painter.setBrush(linearGradient);//���û�ˢ����painter.drawRect(rect());���Ƴ����䱳��
		painter.drawRect(QRect(x, y, w, h));
	}
	return QWidget::eventFilter(obj, event);
}

void readTiff(QString fileName, int & width, int & height, QVector<float>& resultVec)
{
	QByteArray data = fileName.toLocal8Bit();
	string tmpstr = string(data);

	cv::Mat image;     //����һ����ͼ��image
	image = cv::imread(tmpstr, cv::IMREAD_ANYDEPTH);  //��ȡ�ļ����е�ͼ��
	image.convertTo(image, CV_32FC1);
	if (image.empty()) {
		return; // ���ؿյ�QVector
	}
	CV_Assert(image.channels() == 1); // ȷ���ǵ�ͨ����Mat

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
	// ����OpenCVͼ��
	 // �����ļ��Ի���
	/*QString imgPath = QFileDialog::getOpenFileName(nullptr, "ѡ���ļ�", "", "All Files (*);;Text Files (*.tiff)");

	BandPassimage = cv::imread(QStr2Str(imgPath), cv::IMREAD_UNCHANGED| cv::IMREAD_ANYDEPTH);
	if (!BandPassimage.empty())
	{
		cv::Mat showimage;
		cv::normalize(BandPassimage, showimage, 0, 255, cv::NORM_MINMAX);
		showimage.convertTo(showimage, CV_8U);
		cv::resize(showimage, showimage, cv::Size(ui.InputImg->width(), ui.InputImg->height()));
		showimg(showimage);
	}*/

	QString dirName = QFileDialog::getExistingDirectory(this, ("ѡ������ƬĿ¼"), "", QFileDialog::Option::ReadOnly);
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
	ui.S_THRESHOLD->setRange(0, 255); // ���û�������ΧΪ0-255
	ui.LE_THRESHOLD->setText(QString::number(threshold));
	// ���ж�ֵ������
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

	// ���帯ʴ�����ĽṹԪ��
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(erodesize, erodesize));

	// ���и�ʴ����
	cv::Mat erodedImage;
	cv::erode(m_src, erodedImage, kernel);
	showoutimg(erodedImage);

	
}

void cvTools::DilateImg(int dilatesize)
{
	ui.S_DILATE->setRange(1, 50);
	ui.S_DILATE->setSingleStep(2);
	ui.LE_DILATE->setText(QString::number(dilatesize));

	// �������Ͳ����ĽṹԪ��
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(dilatesize, dilatesize));

	// �������Ͳ���
	cv::Mat dilatedImage;
	cv::dilate(m_src, dilatedImage, kernel);
	showoutimg(dilatedImage);

	
}

void cvTools::OpenExImg(int opensize)
{
	ui.S_OPEN->setRange(1, 50);
	ui.S_OPEN->setSingleStep(2);
	ui.LE_OPEN->setText(QString::number(opensize));

	// ���忪�����ĽṹԪ��
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(opensize, opensize));

	// ���п�����
	cv::Mat openedImage;
	cv::morphologyEx(m_src, openedImage, cv::MORPH_OPEN, kernel);
	showoutimg(openedImage);

	
}

void cvTools::CloseExImg(int closesize)
{
	ui.S_CLOSE->setRange(1, 50);
	ui.LE_CLOSE->setText(QString::number(closesize));

	// ����ղ����ĽṹԪ��
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(closesize, closesize));

	// ���бղ���
	cv::Mat closedImage;
	cv::morphologyEx(m_src, closedImage, cv::MORPH_CLOSE, kernel);
	showoutimg(closedImage);

	
}

void cvTools::CvtColorImg()
{
	// ��ͼ��ת��Ϊ�Ҷ�ͼ��
	
	cv::cvtColor(m_src, grayImage, cv::COLOR_BGR2GRAY);
	showoutimg(grayImage);

	
	
}

void cvTools::HistImg()
{
	if (grayImage.empty())
	{
		cv::Mat image = m_src;
		
		// ����ͨ��
		std::vector<cv::Mat> bgr_planes;
		cv::split(image, bgr_planes);

		// ����ֱ��ͼ����
		int histSize = 256;    // ֱ��ͼ�е�����
		float range[] = { 0, 256 };    // ����ֵ�ķ�Χ
		const float* histRange = { range };
		bool uniform = true, accumulate = false;

		// ����ÿ��ͨ����ֱ��ͼ
		cv::Mat b_hist, g_hist, r_hist;
		cv::calcHist(&bgr_planes[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
		cv::calcHist(&bgr_planes[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
		cv::calcHist(&bgr_planes[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

		// ����ֱ��ͼ����
		int histWidth = 512, histHeight = 400;
		cv::Mat histImage(histHeight, histWidth, CV_8UC3, cv::Scalar(0, 0, 0));

		// ��һ��ֱ��ͼ
		cv::normalize(b_hist, b_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
		cv::normalize(g_hist, g_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
		cv::normalize(r_hist, r_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());

		// ����ֱ��ͼ
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

	
		// ��ʾֱ��ͼ
		cv::imshow("Histogram", histImage);
	}
	else
	{
		cv::Mat image = grayImage;

		// ����ֱ��ͼ
		int histSize = 256;
		float range[] = { 0, 256 };
		const float* histRange = { range };
		bool uniform = true, accumulate = false;
		cv::Mat hist;
		cv::calcHist(&image, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);

		// ����ֱ��ͼ����
		int histWidth = 512, histHeight = 400;
		cv::Mat histImage(histHeight, histWidth, CV_8UC3, cv::Scalar(0, 0, 0));

		// ��һ��ֱ��ͼ
		cv::normalize(hist, hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());

		// ����ֱ��ͼ
		int binWidth = cvRound((double)histWidth / histSize);
		for (int i = 1; i < histSize; i++)
		{
			cv::line(histImage, cv::Point(binWidth * (i - 1), histHeight - cvRound(hist.at<float>(i - 1))),
				cv::Point(binWidth * (i), histHeight - cvRound(hist.at<float>(i))),
				cv::Scalar(255, 255, 255), 2, 8, 0);
		}

		//// ���������
		//cv::line(histImage, cv::Point(50, histHeight - 50), cv::Point(50, 50), cv::Scalar(255, 255, 255), 2, 8, 0);
		//cv::line(histImage, cv::Point(50, histHeight - 50), cv::Point(histWidth - 50, histHeight - 50), cv::Scalar(255, 255, 255), 2, 8, 0);

		//// ����������ǩ
		//cv::putText(histImage, "0", cv::Point(30, histHeight - 30), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
		//cv::putText(histImage, "255", cv::Point(histWidth - 40, histHeight - 30), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
		//cv::putText(histImage, "Pixel Value", cv::Point(200, histHeight - 10), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
		//cv::putText(histImage, "Number of Pixels", cv::Point(10, 20), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);

		// ��ʾֱ��ͼ
		cv::imshow("Histogram", histImage);
	}
	
	
}

void cvTools::BlurImg(int blursize)
{
	ui.S_BLUR->setRange(1, 50);
	ui.S_BLUR->setSingleStep(2);
	ui.LE_BLUR->setText(QString::number(blursize));

	// �������ͼ��
	cv::Mat outputImage;

	// Ӧ�þ�ֵ�˲�
	cv::blur(m_src, outputImage, cv::Size(blursize, blursize));
	showoutimg(outputImage);
}

void cvTools::GaussianBlurImg(int gausssize)
{
	ui.S_GAUSSIANBLUR->setRange(1, 50);
	ui.S_GAUSSIANBLUR->setSingleStep(2);
	ui.LE_GAUSSIANBLUR->setText(QString::number(gausssize));

	// �������ͼ��
	cv::Mat gaussianOutput;
	if (0== gausssize%2)
	{
	}
	else
	{
		// Ӧ�ø�˹�˲�
		cv::GaussianBlur(m_src, gaussianOutput, cv::Size(gausssize, gausssize), 0);
	}
	
	showoutimg(gaussianOutput);
	
}

void cvTools::MedianBlurImg(int medsize)
{
	ui.S_MEDIANBLUR->setRange(1, 50);
	ui.S_MEDIANBLUR->setSingleStep(2);
	ui.LE_MEDIANBLUR->setText(QString::number(medsize));

	// �������ͼ��
	cv::Mat medianOutput;
	if (0 == medsize % 2)
	{
	}
	else
	{
		// Ӧ����ֵ�˲�
		cv::medianBlur(m_src, medianOutput, medsize);
	}

	showoutimg(medianOutput);
}

void cvTools::BilateralFilterImg(int filtersize)
{
	ui.S_bilateralFilter->setRange(1, 50);
	ui.S_bilateralFilter->setSingleStep(2);
	ui.LE_bilateralFilter->setText(QString::number(filtersize));
	// �������ͼ��
	cv::Mat outputImage;

	// Ӧ��˫���˲�
	cv::bilateralFilter(m_src, outputImage, filtersize, 10, 10);
	
	showoutimg(outputImage);
}

void cvTools::BoxFilterImg(int boxfiltersize)
{
	ui.S_BOXFILTER->setRange(1, 50);
	ui.S_BOXFILTER->setSingleStep(2);
	ui.LE_BOXFILTER->setText(QString::number(boxfiltersize));
	// �������ͼ��
	cv::Mat meanFilteredOutput;

	// Ӧ�þֲ���ֵ�˲�
	cv::boxFilter(m_src, meanFilteredOutput, -1, cv::Size(boxfiltersize, boxfiltersize));
	showoutimg(meanFilteredOutput);
}

void cvTools::CannyLowImg(int cannysize)
{
	ui.S_CANNYLOW->setRange(0, 255);
	ui.LE_CANNYLOW->setText(QString::number(cannysize));
	int highsize = ui.S_CANNYHIGH->value();
	// �������ͼ��
	cv::Mat cannyOutput;

	// Ӧ��Canny�㷨���б�Ե���
	cv::Canny(m_src, cannyOutput, cannysize, highsize);
	showoutimg(cannyOutput);
}

void cvTools::CannyHighImg(int cannysize)
{
	ui.S_CANNYHIGH->setRange(0, 255);
	ui.LE_CANNYHIGH->setText(QString::number(cannysize));
	int lowsize = ui.S_CANNYLOW->value();
	// �������ͼ��
	cv::Mat cannyOutput;

	// Ӧ��Canny�㷨���б�Ե���
	cv::Canny(m_src, cannyOutput, lowsize, cannysize);
	showoutimg(cannyOutput);
}

void cvTools::EnhanceImg()
{
	if (grayImage.empty())
	{
		// ��ͼ��ת��Ϊ�����͸�ʽ
		cv::Mat inputImage = m_src;
		m_src.convertTo(inputImage, CV_32F);

		// ����ͼ�������ͨ��
		std::vector<cv::Mat> channels;
		cv::split(inputImage, channels);

		// ��ÿ��ͨ�����жԱȶ���ǿ
		for (int i = 0; i < 3; i++)
		{
			cv::Scalar mean, stddev;
			cv::meanStdDev(channels[i], mean, stddev);
			channels[i] = (channels[i] - mean.val[0]) / stddev.val[0] * 50 + 128;
		}

		// �ϲ�ͨ��
		cv::Mat contrastEnhancedImage;
		cv::merge(channels, contrastEnhancedImage);

		// ��ͼ��ת����8λ�޷������͸�ʽ
		contrastEnhancedImage.convertTo(contrastEnhancedImage, CV_8U);
		showoutimg(contrastEnhancedImage);
	}
	else
	{
		// ����ͼ��ľ�ֵ�ͱ�׼��
		cv::Scalar mean, stddev;
		cv::meanStdDev(grayImage, mean, stddev);

		// ��ͼ����жԱȶ���ǿ
		cv::Mat contrastEnhancedImage = (grayImage - mean.val[0]) / stddev.val[0] * 50 + 128;
		showoutimg(contrastEnhancedImage);
	}
	

	
}

void cvTools::OtsuImg()
{
	cv::cvtColor(m_src, grayImage, cv::COLOR_BGR2GRAY);
	// Ӧ��OTSU�㷨����ͼ���ֵ��
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
	double alpha = contrast/100.0; // �Աȶȵ�������
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
	double gm = gamma / 100.0; // gamma��������
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
	
	//����num����Mat�ָnumԽ��ָ�Խ��
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
				//ע��Խ��
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
				
				//���뵽��ͼƬ��
				//�Ҳ���ͼƬʱ��ֻ�ǽ�y��x����ߵ��ˡ�Ҳ�����ƶ��µĹ���
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
				cv::Mat roi(NewMat, cv::Rect(xx, yy, rectw, recth)); // ����ROI
				a.copyTo(roi); // ��a���Ƶ�ROI��
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
				//ע��Խ��
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

				//���뵽��ͼƬ��
				//�Ҳ���ͼƬʱ��ֻ�ǽ�y��x����ߵ��ˡ�Ҳ�����ƶ��µĹ���
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
				cv::Mat roi(NewMat, cv::Rect(xx, yy, rectw, recth)); // ����ROI
				a.copyTo(roi); // ��a���Ƶ�ROI��
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
	//// ��˹��ͨ�˲�
	//cv::Mat lowpass ;
	//cv::GaussianBlur(BandPassimage, lowpass, cv::Size(5, 5), D0);
	//// ��˹��ͨ�˲�
	//cv::Mat highpass;
	//cv::GaussianBlur(BandPassimage, highpass, cv::Size(5, 5), D1);
	//// ����ͨ�˲�����͸�ͨ�˲�������
	//BandPassimageResult = highpass - lowpass;
	//cv::imwrite("lowpass.tiff", lowpass);
	//cv::imwrite("highpass.tiff", highpass);
	//===================================test3============================//
	//��ֵ�˲����ٽ�������
	//cv::Mat medMat;
	//medianBlur(BandPassimage, medMat, 3); // ����3��ʾ�˵Ĵ�С
	//˫���˲�
	//cv::Mat filtered;
	//cv::bilateralFilter(BandPassimage, filtered, 5, 0.1, 1); // Ӧ��˫���˲��������ֱ�Ϊ�˲�����ֱ�����ռ��˹������׼��ͻҶ�ֵ�����Ը�˹������׼��
	//BandPassimageResult = filtered;

	//===============================test5-��̬ѧ================//
	cv::Mat largeStructureFiltered;
	cv::Mat smallStructureFiltered;
	// ��ṹ�˲���50����
	cv::Mat largeStructureKernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(D1, D1));
	cv::morphologyEx(BandPassimage, largeStructureFiltered, cv::MORPH_CLOSE, largeStructureKernel);
	// С�ṹ�˲���3����
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


	
	
	

	//==========================���չʾ=======================//
	cv::normalize(BandPassimageResult, BandPassimageResult, 0, 1, cv::NORM_MINMAX);
	cv::imwrite("BandPassimageResult.tiff", BandPassimageResult);
	cv::Mat bandpassShow;
	cv::normalize(BandPassimageResult, bandpassShow, 0, 255, cv::NORM_MINMAX);
	bandpassShow.convertTo(bandpassShow, CV_8U);
	// ��ͼ�����ȥ�봦��
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
		uchar* scanLine = img.scanLine(row); // ��ȡÿһ�еĵ�ַ
		memcpy(scanLine, &grayvalue[row * w_pic], w_pic); // �����ݿ�������Ӧ��ַ
	}


	QImage copyImage = img.copy(); // ��������ͼ��
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
	QString imgPath = QFileDialog::getOpenFileName(nullptr, "ѡ��A", "", "All Files (*);;Text Files (*.tiff)");
	//С�ߴ�ͼƬ
	cv::Mat A = cv::imread(QStr2Str(imgPath), cv::IMREAD_UNCHANGED | cv::IMREAD_ANYDEPTH);
	cv::Mat A_color;
	if (!A.empty())
	{
		cv::normalize(A, A, 0, 1, cv::NORM_MINMAX, CV_32F);
		// ��32FͼƬAת��Ϊ8UͼƬ
		cv::Mat A_8U;
		A.convertTo(A_8U, CV_8U, 255.0);

		// ����BGRAͼƬ
		cv::Mat BGRA(A.rows, A.cols, CV_8UC4);

		// ӳ�䵽α��ɫ�ռ�
		cv::applyColorMap(A_8U, BGRA, cv::COLORMAP_JET);

		// ����͸����Ϊ40%
		for (int i = 0; i < BGRA.rows; i++) {
			for (int j = 0; j < BGRA.cols; j++) {
				uchar* pSrc = BGRA.ptr<uchar>(i, j);
				pSrc[3] = 0.4 * 255; // ����͸����Ϊ40%
			}
		}
		A_color = BGRA.clone();
		imshow("A", A_color);
	}
	else
	{
		return;
	}

	imgPath = QFileDialog::getOpenFileName(nullptr, "ѡ��B", "", "All Files (*);;Text Files (*.tiff)");
	//��ߴ�ͼƬ
	cv::Mat B = cv::imread(QStr2Str(imgPath), cv::IMREAD_UNCHANGED | cv::IMREAD_ANYDEPTH);
	cv::Mat B_color;
	if (!B.empty())
	{
		cv::normalize(B, B, 0, 1, cv::NORM_MINMAX, CV_32F);
		// ��32FͼƬAת��Ϊ8UͼƬ
		cv::Mat B_8U;
		B.convertTo(B_8U, CV_8U, 255.0);

		// ����BGRAͼƬ
		cv::Mat BGRA(B.rows, B.cols, CV_8UC4);

		// ӳ�䵽α��ɫ�ռ�
		//cv::applyColorMap(B_8U, BGRA, cv::COLORMAP_JET);

		// ����͸����Ϊ40%
		for (int i = 0; i < BGRA.rows; i++) {
			for (int j = 0; j < BGRA.cols; j++) {
				uchar* pSrc = BGRA.ptr<uchar>(i, j);
				pSrc[0] = B_8U.at<uchar>(i, j);
				pSrc[1] = B_8U.at<uchar>(i, j);
				pSrc[2] = B_8U.at<uchar>(i, j);
				pSrc[3] = 255; // ����͸����Ϊ40%
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
		// ����������Ͳ�ͬ����������ת��
		B_color.convertTo(B_color, A_color.type());
	}
	if (A_color.size()!= B_color.size())
	{
		cv::resize(B_color, B_color, A_color.size());
	}

	// ����ɫͼ��A��B_color����
	cv::Mat result(A_color.rows, A_color.cols, CV_8UC4);

	for (int i = 0; i < A_color.rows; i++) {
		for (int j = 0; j < A_color.cols; j++) {
			
			uchar* colorA = A_color.ptr<uchar>(i, j);
			uchar* colorB = B_color.ptr<uchar>(i, j);

			// ������Ӻ����ɫֵ������͸����
			float alphaA = colorA[3] / 255.0;
			float alphaB = colorB[3] / 255.0;

			cv::Vec4b resultColor;
			resultColor[0] = alphaA * colorA[0] + (1 - alphaA) * alphaB * colorB[0];
			resultColor[1] = alphaA * colorA[1] + (1 - alphaA) * alphaB * colorB[1];
			resultColor[2] = alphaA * colorA[2] + (1 - alphaA) * alphaB * colorB[2];
			resultColor[3] = std::max(colorA[3], colorB[3]); // �����ϴ��͸����

			result.at<cv::Vec4b>(i, j) = resultColor;
		}
	}

	imshow("C", result);
	//////////////////////////////==========================================/////////////////////////////////////////////
	QString backgroundimagePath = QFileDialog::getOpenFileName(nullptr, "ѡ�񱳾�ͼƬ", "", "All Files (*);;Text Files (*.tiff)");
	cv::Mat backMat = cv::imread(QStr2Str(backgroundimagePath), cv::IMREAD_UNCHANGED | cv::IMREAD_ANYDEPTH);
	if (!backMat.empty())
	{
		cv::normalize(backMat, backMat, 0, 1, cv::NORM_MINMAX);
		backMat.convertTo(backMat, CV_8U, 255.0);
		cv::resize(backMat, backMat, cv::Size(206, 306));
		// ת��ΪRGB��ʽ��cv::Mat
		cv::Mat RGBMat;
		cv::cvtColor(backMat, RGBMat, cv::COLOR_GRAY2RGB);
		// �����Ҫ͸���ȣ�����ʹ�����´���
		QImage backimage(RGBMat.data, RGBMat.cols, RGBMat.rows, RGBMat.step, QImage::Format_ARGB32);

		QImage ABC;//α��img
		QImage forwardimage = ABC.convertToFormat(QImage::Format_ARGB32);
		for (int y = 0; y < forwardimage.height(); ++y) {
			QRgb *scanLine = reinterpret_cast<QRgb*>(forwardimage.scanLine(y));
			for (int x = 0; x < forwardimage.width(); ++x) {
				QRgb color = scanLine[x];
				int alpha = qAlpha(color) * 0.4; // 40% opacity
				scanLine[x] = qRgba(qRed(color), qGreen(color), qBlue(color), alpha);
			}
		}

		// ȷ������ͼ��Ĵ�С��ͬ
		if (backimage.size() != forwardimage.size()) {
			// ���Խ����ʵ������Ż�ü�����ʹ���Ǵ�Сһ��
			return;
		}

		// ����һ���µ�QImage�������ڴ洢���Ӻ��ͼ��
		QImage result(backimage.size(), QImage::Format_ARGB32);
		result.fill(Qt::transparent); // ���͸������

		// ������ͼ�������һ��
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
