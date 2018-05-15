/********************************************************************************
** Form generated from reading UI file 'fromant_synt.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FROMANT_SYNT_H
#define UI_FROMANT_SYNT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QPushButton *Abutton;
    QPushButton *Ebutton;
    QGroupBox *groupBox;
    QPushButton *genButton;
    QDoubleSpinBox *f_ot;
    QLabel *label_3;
    QLabel *label_4;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *Ibutton;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_2;
    QLabel *label_9;
    QDoubleSpinBox *F1;
    QLabel *label_10;
    QLabel *label_11;
    QDoubleSpinBox *F2;
    QLabel *label_12;
    QLabel *label_13;
    QDoubleSpinBox *F3;
    QLabel *label_14;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout;
    QLabel *label_6;
    QDoubleSpinBox *F;
    QLabel *label_5;
    QLabel *label_7;
    QDoubleSpinBox *BW;
    QLabel *label_8;
    QDoubleSpinBox *Ncascade;
    QPushButton *hbSongButton;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QStringLiteral("Dialog"));
        Dialog->resize(694, 232);
        Abutton = new QPushButton(Dialog);
        Abutton->setObjectName(QStringLiteral("Abutton"));
        Abutton->setGeometry(QRect(20, 150, 75, 23));
        Ebutton = new QPushButton(Dialog);
        Ebutton->setObjectName(QStringLiteral("Ebutton"));
        Ebutton->setGeometry(QRect(200, 150, 75, 23));
        groupBox = new QGroupBox(Dialog);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(20, 20, 281, 121));
        genButton = new QPushButton(groupBox);
        genButton->setObjectName(QStringLiteral("genButton"));
        genButton->setGeometry(QRect(22, 60, 231, 23));
        f_ot = new QDoubleSpinBox(groupBox);
        f_ot->setObjectName(QStringLiteral("f_ot"));
        f_ot->setGeometry(QRect(140, 30, 81, 22));
        f_ot->setMaximum(1e+7);
        f_ot->setValue(80);
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 31, 121, 16));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(230, 33, 46, 13));
        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(10, 90, 91, 23));
        pushButton_2 = new QPushButton(groupBox);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(110, 90, 151, 23));
        Ibutton = new QPushButton(Dialog);
        Ibutton->setObjectName(QStringLiteral("Ibutton"));
        Ibutton->setGeometry(QRect(110, 150, 75, 23));
        groupBox_3 = new QGroupBox(Dialog);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(500, 20, 161, 121));
        gridLayout_2 = new QGridLayout(groupBox_3);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        label_9 = new QLabel(groupBox_3);
        label_9->setObjectName(QStringLiteral("label_9"));

        gridLayout_2->addWidget(label_9, 0, 0, 1, 1);

        F1 = new QDoubleSpinBox(groupBox_3);
        F1->setObjectName(QStringLiteral("F1"));
        F1->setMaximum(1e+7);
        F1->setValue(660);

        gridLayout_2->addWidget(F1, 0, 1, 1, 1);

        label_10 = new QLabel(groupBox_3);
        label_10->setObjectName(QStringLiteral("label_10"));

        gridLayout_2->addWidget(label_10, 0, 2, 1, 1);

        label_11 = new QLabel(groupBox_3);
        label_11->setObjectName(QStringLiteral("label_11"));

        gridLayout_2->addWidget(label_11, 1, 0, 1, 1);

        F2 = new QDoubleSpinBox(groupBox_3);
        F2->setObjectName(QStringLiteral("F2"));
        F2->setMaximum(1e+7);
        F2->setValue(1700);

        gridLayout_2->addWidget(F2, 1, 1, 1, 1);

        label_12 = new QLabel(groupBox_3);
        label_12->setObjectName(QStringLiteral("label_12"));

        gridLayout_2->addWidget(label_12, 1, 2, 1, 1);

        label_13 = new QLabel(groupBox_3);
        label_13->setObjectName(QStringLiteral("label_13"));

        gridLayout_2->addWidget(label_13, 2, 0, 1, 1);

        F3 = new QDoubleSpinBox(groupBox_3);
        F3->setObjectName(QStringLiteral("F3"));
        F3->setMaximum(1e+7);
        F3->setValue(2400);

        gridLayout_2->addWidget(F3, 2, 1, 1, 1);

        label_14 = new QLabel(groupBox_3);
        label_14->setObjectName(QStringLiteral("label_14"));

        gridLayout_2->addWidget(label_14, 2, 2, 1, 1);

        groupBox_2 = new QGroupBox(Dialog);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(310, 20, 181, 121));
        gridLayout = new QGridLayout(groupBox_2);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout->addWidget(label_6, 0, 0, 1, 1);

        F = new QDoubleSpinBox(groupBox_2);
        F->setObjectName(QStringLiteral("F"));
        F->setMaximum(1e+7);
        F->setValue(800);

        gridLayout->addWidget(F, 0, 1, 1, 1);

        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout->addWidget(label_5, 0, 2, 1, 1);

        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout->addWidget(label_7, 1, 0, 1, 1);

        BW = new QDoubleSpinBox(groupBox_2);
        BW->setObjectName(QStringLiteral("BW"));
        BW->setDecimals(10);
        BW->setMaximum(1e+7);
        BW->setSingleStep(1e-6);
        BW->setValue(0.0066);

        gridLayout->addWidget(BW, 1, 1, 1, 1);

        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout->addWidget(label_8, 2, 0, 1, 1);

        Ncascade = new QDoubleSpinBox(groupBox_2);
        Ncascade->setObjectName(QStringLiteral("Ncascade"));
        Ncascade->setMaximum(1e+7);
        Ncascade->setValue(5);

        gridLayout->addWidget(Ncascade, 2, 1, 1, 1);

        hbSongButton = new QPushButton(Dialog);
        hbSongButton->setObjectName(QStringLiteral("hbSongButton"));
        hbSongButton->setGeometry(QRect(20, 180, 161, 32));

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", nullptr));
        Abutton->setText(QApplication::translate("Dialog", "A", nullptr));
        Ebutton->setText(QApplication::translate("Dialog", "\320\255", nullptr));
        groupBox->setTitle(QApplication::translate("Dialog", "Generator", nullptr));
        genButton->setText(QApplication::translate("Dialog", "Generate", nullptr));
        label_3->setText(QApplication::translate("Dialog", "f \320\276\321\201\320\275\320\276\320\262\320\275\320\276\320\263\320\276 \321\202\320\276\320\275\320\260", nullptr));
        label_4->setText(QApplication::translate("Dialog", "\320\223\321\206", nullptr));
        pushButton->setText(QApplication::translate("Dialog", "pulse.wav", nullptr));
        pushButton_2->setText(QApplication::translate("Dialog", "pulse_filtered.wav", nullptr));
        Ibutton->setText(QApplication::translate("Dialog", "\320\230", nullptr));
        groupBox_3->setTitle(QApplication::translate("Dialog", "Formants", nullptr));
        label_9->setText(QApplication::translate("Dialog", "F1", nullptr));
        label_10->setText(QApplication::translate("Dialog", "\320\223\321\206", nullptr));
        label_11->setText(QApplication::translate("Dialog", "F2", nullptr));
        label_12->setText(QApplication::translate("Dialog", "\320\223\321\206", nullptr));
        label_13->setText(QApplication::translate("Dialog", "F3", nullptr));
        label_14->setText(QApplication::translate("Dialog", "\320\223\321\206", nullptr));
        groupBox_2->setTitle(QApplication::translate("Dialog", "Band pass filter", nullptr));
        label_6->setText(QApplication::translate("Dialog", "F", nullptr));
        label_5->setText(QApplication::translate("Dialog", "\320\223\321\206", nullptr));
        label_7->setText(QApplication::translate("Dialog", "BW", nullptr));
        label_8->setText(QApplication::translate("Dialog", "Ncascad", nullptr));
        hbSongButton->setText(QApplication::translate("Dialog", "Happy Birsday Song", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FROMANT_SYNT_H
