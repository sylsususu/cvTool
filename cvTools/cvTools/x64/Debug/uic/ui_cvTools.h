/********************************************************************************
** Form generated from reading UI file 'cvTools.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CVTOOLS_H
#define UI_CVTOOLS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_cvToolsClass
{
public:
    QLabel *InputImg;
    QLabel *OutputImg;
    QGroupBox *groupBox1;
    QPushButton *PB_OPENIMG;
    QPushButton *PB_RESIZEIMG;
    QPushButton *PB_CVTCOLORIMG;
    QPushButton *PB_HIST;
    QPushButton *PB_EnhanceImg;
    QPushButton *PB_OTSU;
    QPushButton *PB_ReadTiff;
    QGroupBox *groupBox2;
    QSlider *S_THRESHOLD;
    QLineEdit *LE_THRESHOLD;
    QGroupBox *groupBox3;
    QSlider *S_ERODE;
    QLineEdit *LE_ERODE;
    QGroupBox *groupBox4;
    QSlider *S_DILATE;
    QLineEdit *LE_DILATE;
    QGroupBox *groupBox4_2;
    QSlider *S_OPEN;
    QLineEdit *LE_OPEN;
    QGroupBox *groupBox4_3;
    QSlider *S_CLOSE;
    QLineEdit *LE_CLOSE;
    QGroupBox *groupBox2_2;
    QSlider *S_BLUR;
    QLineEdit *LE_BLUR;
    QGroupBox *groupBox2_3;
    QSlider *S_GAUSSIANBLUR;
    QLineEdit *LE_GAUSSIANBLUR;
    QGroupBox *groupBox2_4;
    QSlider *S_MEDIANBLUR;
    QLineEdit *LE_MEDIANBLUR;
    QGroupBox *groupBox2_5;
    QSlider *S_bilateralFilter;
    QLineEdit *LE_bilateralFilter;
    QGroupBox *groupBox2_6;
    QSlider *S_BOXFILTER;
    QLineEdit *LE_BOXFILTER;
    QGroupBox *groupBox;
    QSlider *S_CANNYLOW;
    QLabel *label;
    QSlider *S_CANNYHIGH;
    QLabel *label_2;
    QLineEdit *LE_CANNYLOW;
    QLineEdit *LE_CANNYHIGH;
    QLabel *label_3;
    QGroupBox *groupBox_2;
    QPushButton *PB_db1;
    QPushButton *PB_db2;
    QPushButton *PB_db3;
    QPushButton *PB_db4;
    QGroupBox *groupBox_3;
    QSlider *S_Contrast;
    QGroupBox *groupBox_4;
    QSlider *S_Gamma;
    QGroupBox *groupBox_5;
    QPushButton *PB_NewMat;
    QLineEdit *LE_Number;

    void setupUi(QWidget *cvToolsClass)
    {
        if (cvToolsClass->objectName().isEmpty())
            cvToolsClass->setObjectName(QStringLiteral("cvToolsClass"));
        cvToolsClass->resize(1030, 969);
        InputImg = new QLabel(cvToolsClass);
        InputImg->setObjectName(QStringLiteral("InputImg"));
        InputImg->setGeometry(QRect(10, 10, 500, 500));
        InputImg->setStyleSheet(QStringLiteral("image:url(:/cvTools/image/R.png);"));
        OutputImg = new QLabel(cvToolsClass);
        OutputImg->setObjectName(QStringLiteral("OutputImg"));
        OutputImg->setGeometry(QRect(520, 10, 500, 500));
        OutputImg->setStyleSheet(QStringLiteral(""));
        groupBox1 = new QGroupBox(cvToolsClass);
        groupBox1->setObjectName(QStringLiteral("groupBox1"));
        groupBox1->setGeometry(QRect(10, 520, 100, 441));
        PB_OPENIMG = new QPushButton(groupBox1);
        PB_OPENIMG->setObjectName(QStringLiteral("PB_OPENIMG"));
        PB_OPENIMG->setGeometry(QRect(10, 20, 75, 23));
        PB_RESIZEIMG = new QPushButton(groupBox1);
        PB_RESIZEIMG->setObjectName(QStringLiteral("PB_RESIZEIMG"));
        PB_RESIZEIMG->setGeometry(QRect(10, 50, 75, 23));
        PB_CVTCOLORIMG = new QPushButton(groupBox1);
        PB_CVTCOLORIMG->setObjectName(QStringLiteral("PB_CVTCOLORIMG"));
        PB_CVTCOLORIMG->setGeometry(QRect(10, 80, 75, 23));
        PB_HIST = new QPushButton(groupBox1);
        PB_HIST->setObjectName(QStringLiteral("PB_HIST"));
        PB_HIST->setGeometry(QRect(10, 110, 75, 23));
        PB_EnhanceImg = new QPushButton(groupBox1);
        PB_EnhanceImg->setObjectName(QStringLiteral("PB_EnhanceImg"));
        PB_EnhanceImg->setGeometry(QRect(10, 140, 75, 23));
        PB_OTSU = new QPushButton(groupBox1);
        PB_OTSU->setObjectName(QStringLiteral("PB_OTSU"));
        PB_OTSU->setGeometry(QRect(10, 170, 75, 23));
        PB_ReadTiff = new QPushButton(groupBox1);
        PB_ReadTiff->setObjectName(QStringLiteral("PB_ReadTiff"));
        PB_ReadTiff->setGeometry(QRect(10, 200, 75, 23));
        groupBox2 = new QGroupBox(cvToolsClass);
        groupBox2->setObjectName(QStringLiteral("groupBox2"));
        groupBox2->setGeometry(QRect(120, 520, 100, 80));
        S_THRESHOLD = new QSlider(groupBox2);
        S_THRESHOLD->setObjectName(QStringLiteral("S_THRESHOLD"));
        S_THRESHOLD->setGeometry(QRect(10, 20, 81, 22));
        S_THRESHOLD->setOrientation(Qt::Horizontal);
        LE_THRESHOLD = new QLineEdit(groupBox2);
        LE_THRESHOLD->setObjectName(QStringLiteral("LE_THRESHOLD"));
        LE_THRESHOLD->setEnabled(false);
        LE_THRESHOLD->setGeometry(QRect(10, 50, 81, 20));
        groupBox3 = new QGroupBox(cvToolsClass);
        groupBox3->setObjectName(QStringLiteral("groupBox3"));
        groupBox3->setGeometry(QRect(120, 610, 100, 80));
        S_ERODE = new QSlider(groupBox3);
        S_ERODE->setObjectName(QStringLiteral("S_ERODE"));
        S_ERODE->setGeometry(QRect(10, 20, 81, 22));
        S_ERODE->setOrientation(Qt::Horizontal);
        LE_ERODE = new QLineEdit(groupBox3);
        LE_ERODE->setObjectName(QStringLiteral("LE_ERODE"));
        LE_ERODE->setEnabled(false);
        LE_ERODE->setGeometry(QRect(10, 50, 81, 20));
        groupBox4 = new QGroupBox(cvToolsClass);
        groupBox4->setObjectName(QStringLiteral("groupBox4"));
        groupBox4->setGeometry(QRect(120, 700, 100, 80));
        S_DILATE = new QSlider(groupBox4);
        S_DILATE->setObjectName(QStringLiteral("S_DILATE"));
        S_DILATE->setGeometry(QRect(10, 20, 81, 22));
        S_DILATE->setOrientation(Qt::Horizontal);
        LE_DILATE = new QLineEdit(groupBox4);
        LE_DILATE->setObjectName(QStringLiteral("LE_DILATE"));
        LE_DILATE->setEnabled(false);
        LE_DILATE->setGeometry(QRect(10, 50, 81, 20));
        groupBox4_2 = new QGroupBox(cvToolsClass);
        groupBox4_2->setObjectName(QStringLiteral("groupBox4_2"));
        groupBox4_2->setGeometry(QRect(120, 790, 100, 80));
        S_OPEN = new QSlider(groupBox4_2);
        S_OPEN->setObjectName(QStringLiteral("S_OPEN"));
        S_OPEN->setGeometry(QRect(10, 20, 81, 22));
        S_OPEN->setOrientation(Qt::Horizontal);
        LE_OPEN = new QLineEdit(groupBox4_2);
        LE_OPEN->setObjectName(QStringLiteral("LE_OPEN"));
        LE_OPEN->setEnabled(false);
        LE_OPEN->setGeometry(QRect(10, 50, 81, 20));
        groupBox4_3 = new QGroupBox(cvToolsClass);
        groupBox4_3->setObjectName(QStringLiteral("groupBox4_3"));
        groupBox4_3->setGeometry(QRect(120, 880, 100, 80));
        S_CLOSE = new QSlider(groupBox4_3);
        S_CLOSE->setObjectName(QStringLiteral("S_CLOSE"));
        S_CLOSE->setGeometry(QRect(10, 20, 81, 22));
        S_CLOSE->setOrientation(Qt::Horizontal);
        LE_CLOSE = new QLineEdit(groupBox4_3);
        LE_CLOSE->setObjectName(QStringLiteral("LE_CLOSE"));
        LE_CLOSE->setEnabled(false);
        LE_CLOSE->setGeometry(QRect(10, 50, 81, 20));
        groupBox2_2 = new QGroupBox(cvToolsClass);
        groupBox2_2->setObjectName(QStringLiteral("groupBox2_2"));
        groupBox2_2->setGeometry(QRect(230, 520, 100, 80));
        S_BLUR = new QSlider(groupBox2_2);
        S_BLUR->setObjectName(QStringLiteral("S_BLUR"));
        S_BLUR->setGeometry(QRect(10, 20, 81, 22));
        S_BLUR->setOrientation(Qt::Horizontal);
        LE_BLUR = new QLineEdit(groupBox2_2);
        LE_BLUR->setObjectName(QStringLiteral("LE_BLUR"));
        LE_BLUR->setEnabled(false);
        LE_BLUR->setGeometry(QRect(10, 50, 81, 20));
        groupBox2_3 = new QGroupBox(cvToolsClass);
        groupBox2_3->setObjectName(QStringLiteral("groupBox2_3"));
        groupBox2_3->setGeometry(QRect(230, 610, 100, 80));
        S_GAUSSIANBLUR = new QSlider(groupBox2_3);
        S_GAUSSIANBLUR->setObjectName(QStringLiteral("S_GAUSSIANBLUR"));
        S_GAUSSIANBLUR->setGeometry(QRect(10, 20, 81, 22));
        S_GAUSSIANBLUR->setOrientation(Qt::Horizontal);
        LE_GAUSSIANBLUR = new QLineEdit(groupBox2_3);
        LE_GAUSSIANBLUR->setObjectName(QStringLiteral("LE_GAUSSIANBLUR"));
        LE_GAUSSIANBLUR->setEnabled(false);
        LE_GAUSSIANBLUR->setGeometry(QRect(10, 50, 81, 20));
        groupBox2_4 = new QGroupBox(cvToolsClass);
        groupBox2_4->setObjectName(QStringLiteral("groupBox2_4"));
        groupBox2_4->setGeometry(QRect(230, 700, 100, 80));
        S_MEDIANBLUR = new QSlider(groupBox2_4);
        S_MEDIANBLUR->setObjectName(QStringLiteral("S_MEDIANBLUR"));
        S_MEDIANBLUR->setGeometry(QRect(10, 20, 81, 22));
        S_MEDIANBLUR->setOrientation(Qt::Horizontal);
        LE_MEDIANBLUR = new QLineEdit(groupBox2_4);
        LE_MEDIANBLUR->setObjectName(QStringLiteral("LE_MEDIANBLUR"));
        LE_MEDIANBLUR->setEnabled(false);
        LE_MEDIANBLUR->setGeometry(QRect(10, 50, 81, 20));
        groupBox2_5 = new QGroupBox(cvToolsClass);
        groupBox2_5->setObjectName(QStringLiteral("groupBox2_5"));
        groupBox2_5->setGeometry(QRect(230, 790, 100, 80));
        S_bilateralFilter = new QSlider(groupBox2_5);
        S_bilateralFilter->setObjectName(QStringLiteral("S_bilateralFilter"));
        S_bilateralFilter->setGeometry(QRect(10, 20, 81, 22));
        S_bilateralFilter->setOrientation(Qt::Horizontal);
        LE_bilateralFilter = new QLineEdit(groupBox2_5);
        LE_bilateralFilter->setObjectName(QStringLiteral("LE_bilateralFilter"));
        LE_bilateralFilter->setEnabled(false);
        LE_bilateralFilter->setGeometry(QRect(10, 50, 81, 20));
        groupBox2_6 = new QGroupBox(cvToolsClass);
        groupBox2_6->setObjectName(QStringLiteral("groupBox2_6"));
        groupBox2_6->setGeometry(QRect(230, 880, 100, 80));
        S_BOXFILTER = new QSlider(groupBox2_6);
        S_BOXFILTER->setObjectName(QStringLiteral("S_BOXFILTER"));
        S_BOXFILTER->setGeometry(QRect(10, 20, 81, 22));
        S_BOXFILTER->setOrientation(Qt::Horizontal);
        LE_BOXFILTER = new QLineEdit(groupBox2_6);
        LE_BOXFILTER->setObjectName(QStringLiteral("LE_BOXFILTER"));
        LE_BOXFILTER->setEnabled(false);
        LE_BOXFILTER->setGeometry(QRect(10, 50, 81, 20));
        groupBox = new QGroupBox(cvToolsClass);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(340, 520, 120, 121));
        S_CANNYLOW = new QSlider(groupBox);
        S_CANNYLOW->setObjectName(QStringLiteral("S_CANNYLOW"));
        S_CANNYLOW->setGeometry(QRect(30, 20, 81, 22));
        S_CANNYLOW->setOrientation(Qt::Horizontal);
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(1, 20, 21, 16));
        S_CANNYHIGH = new QSlider(groupBox);
        S_CANNYHIGH->setObjectName(QStringLiteral("S_CANNYHIGH"));
        S_CANNYHIGH->setGeometry(QRect(30, 50, 81, 22));
        S_CANNYHIGH->setOrientation(Qt::Horizontal);
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(1, 50, 31, 16));
        LE_CANNYLOW = new QLineEdit(groupBox);
        LE_CANNYLOW->setObjectName(QStringLiteral("LE_CANNYLOW"));
        LE_CANNYLOW->setEnabled(false);
        LE_CANNYLOW->setGeometry(QRect(10, 90, 41, 20));
        LE_CANNYHIGH = new QLineEdit(groupBox);
        LE_CANNYHIGH->setObjectName(QStringLiteral("LE_CANNYHIGH"));
        LE_CANNYHIGH->setEnabled(false);
        LE_CANNYHIGH->setGeometry(QRect(70, 90, 41, 20));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(50, 93, 21, 16));
        groupBox_2 = new QGroupBox(cvToolsClass);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(340, 650, 120, 151));
        PB_db1 = new QPushButton(groupBox_2);
        PB_db1->setObjectName(QStringLiteral("PB_db1"));
        PB_db1->setGeometry(QRect(10, 20, 75, 23));
        PB_db2 = new QPushButton(groupBox_2);
        PB_db2->setObjectName(QStringLiteral("PB_db2"));
        PB_db2->setGeometry(QRect(10, 50, 75, 23));
        PB_db3 = new QPushButton(groupBox_2);
        PB_db3->setObjectName(QStringLiteral("PB_db3"));
        PB_db3->setGeometry(QRect(10, 80, 75, 23));
        PB_db4 = new QPushButton(groupBox_2);
        PB_db4->setObjectName(QStringLiteral("PB_db4"));
        PB_db4->setGeometry(QRect(10, 110, 75, 23));
        groupBox_3 = new QGroupBox(cvToolsClass);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(340, 810, 121, 71));
        S_Contrast = new QSlider(groupBox_3);
        S_Contrast->setObjectName(QStringLiteral("S_Contrast"));
        S_Contrast->setGeometry(QRect(10, 30, 101, 22));
        S_Contrast->setOrientation(Qt::Horizontal);
        groupBox_4 = new QGroupBox(cvToolsClass);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(340, 880, 120, 80));
        S_Gamma = new QSlider(groupBox_4);
        S_Gamma->setObjectName(QStringLiteral("S_Gamma"));
        S_Gamma->setGeometry(QRect(10, 40, 101, 22));
        S_Gamma->setOrientation(Qt::Horizontal);
        groupBox_5 = new QGroupBox(cvToolsClass);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        groupBox_5->setGeometry(QRect(470, 520, 111, 121));
        PB_NewMat = new QPushButton(groupBox_5);
        PB_NewMat->setObjectName(QStringLiteral("PB_NewMat"));
        PB_NewMat->setGeometry(QRect(10, 60, 75, 23));
        LE_Number = new QLineEdit(groupBox_5);
        LE_Number->setObjectName(QStringLiteral("LE_Number"));
        LE_Number->setGeometry(QRect(20, 20, 61, 20));

        retranslateUi(cvToolsClass);

        QMetaObject::connectSlotsByName(cvToolsClass);
    } // setupUi

    void retranslateUi(QWidget *cvToolsClass)
    {
        cvToolsClass->setWindowTitle(QApplication::translate("cvToolsClass", "cvTools", Q_NULLPTR));
        InputImg->setText(QString());
        OutputImg->setText(QString());
        groupBox1->setTitle(QApplication::translate("cvToolsClass", "\345\233\276\347\211\207\346\223\215\344\275\234", Q_NULLPTR));
        PB_OPENIMG->setText(QApplication::translate("cvToolsClass", "\346\211\223\345\274\200\345\233\276\347\211\207", Q_NULLPTR));
        PB_RESIZEIMG->setText(QApplication::translate("cvToolsClass", "\347\274\251\346\224\276\345\233\276\347\211\207", Q_NULLPTR));
        PB_CVTCOLORIMG->setText(QApplication::translate("cvToolsClass", "\347\201\260\345\272\246\345\214\226", Q_NULLPTR));
        PB_HIST->setText(QApplication::translate("cvToolsClass", "\347\233\264\346\226\271\345\233\276", Q_NULLPTR));
        PB_EnhanceImg->setText(QApplication::translate("cvToolsClass", "\345\242\236\345\274\272\345\257\271\346\257\224\345\272\246", Q_NULLPTR));
        PB_OTSU->setText(QApplication::translate("cvToolsClass", "OTSU\347\256\227\346\263\225", Q_NULLPTR));
        PB_ReadTiff->setText(QApplication::translate("cvToolsClass", "\350\257\273\345\217\226tiff", Q_NULLPTR));
        groupBox2->setTitle(QApplication::translate("cvToolsClass", "\344\272\214\345\200\274\345\214\226", Q_NULLPTR));
        groupBox3->setTitle(QApplication::translate("cvToolsClass", "\350\205\220\350\232\200\346\273\244\346\263\242", Q_NULLPTR));
        groupBox4->setTitle(QApplication::translate("cvToolsClass", "\350\206\250\350\203\200\346\273\244\346\263\242", Q_NULLPTR));
        groupBox4_2->setTitle(QApplication::translate("cvToolsClass", "\345\274\200\346\223\215\344\275\234", Q_NULLPTR));
        groupBox4_3->setTitle(QApplication::translate("cvToolsClass", "\351\227\255\346\223\215\344\275\234", Q_NULLPTR));
        groupBox2_2->setTitle(QApplication::translate("cvToolsClass", "\345\235\207\345\200\274\346\273\244\346\263\242", Q_NULLPTR));
        groupBox2_3->setTitle(QApplication::translate("cvToolsClass", "\351\253\230\346\226\257\346\273\244\346\263\242", Q_NULLPTR));
        groupBox2_4->setTitle(QApplication::translate("cvToolsClass", "\344\270\255\345\200\274\346\273\244\346\263\242", Q_NULLPTR));
        groupBox2_5->setTitle(QApplication::translate("cvToolsClass", "\345\217\214\350\276\271\346\273\244\346\263\242", Q_NULLPTR));
        groupBox2_6->setTitle(QApplication::translate("cvToolsClass", "\345\261\200\351\203\250\345\235\207\345\200\274\346\273\244\346\263\242", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("cvToolsClass", "\350\276\271\347\274\230\346\243\200\346\265\213", Q_NULLPTR));
        label->setText(QApplication::translate("cvToolsClass", "LOW", Q_NULLPTR));
        label_2->setText(QApplication::translate("cvToolsClass", "HIGH", Q_NULLPTR));
        label_3->setText(QApplication::translate("cvToolsClass", "----", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("cvToolsClass", "\345\257\271\346\257\224\345\272\246\347\256\227\346\263\225", Q_NULLPTR));
        PB_db1->setText(QApplication::translate("cvToolsClass", "on/off", Q_NULLPTR));
        PB_db2->setText(QApplication::translate("cvToolsClass", "off/on", Q_NULLPTR));
        PB_db3->setText(QApplication::translate("cvToolsClass", "off-on/off", Q_NULLPTR));
        PB_db4->setText(QApplication::translate("cvToolsClass", "on/off-on", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("cvToolsClass", "\345\257\271\346\257\224\345\272\246", Q_NULLPTR));
        groupBox_4->setTitle(QApplication::translate("cvToolsClass", "Gamma", Q_NULLPTR));
        groupBox_5->setTitle(QApplication::translate("cvToolsClass", "\346\213\274\345\233\276", Q_NULLPTR));
        PB_NewMat->setText(QApplication::translate("cvToolsClass", "\345\274\200\345\247\213\346\213\274\345\233\276", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class cvToolsClass: public Ui_cvToolsClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CVTOOLS_H
