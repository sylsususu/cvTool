#include "cvTools.h"

cvTools::cvTools(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
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

		//// ����������
		//cv::line(histImage, cv::Point(50, histHeight - 50), cv::Point(50, 50), cv::Scalar(255, 255, 255), 2, 8, 0);
		//cv::line(histImage, cv::Point(50, histHeight - 50), cv::Point(histWidth - 50, histHeight - 50), cv::Scalar(255, 255, 255), 2, 8, 0);

		//// �����������ǩ
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
	cv::bilateralFilter(m_src, outputImage, filtersize, 80, 80);
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