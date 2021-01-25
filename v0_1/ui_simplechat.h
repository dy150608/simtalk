/********************************************************************************
** Form generated from reading UI file 'simplechat.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIMPLECHAT_H
#define UI_SIMPLECHAT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStackedWidget>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_simplechat
{
public:
    QWidget *centralWidget;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QStackedWidget *stackedWidget_2;
    QWidget *page_3;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QLineEdit *lineEdit;
    QWidget *page_4;
    QPushButton *pushButton_4;
    QLineEdit *lineEdit_2;
    QWidget *page_8;
    QLabel *label_7;
    QLabel *label_8;
    QPushButton *pushButton_9;
    QWidget *page_2;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QPushButton *pushButton_7;
    QStackedWidget *stackedWidget_3;
    QWidget *page_5;
    QListWidget *listWidget;
    QLineEdit *lineEdit_3;
    QLabel *label;
    QPushButton *pushButton_10;
    QPushButton *pushButton_11;
    QPushButton *pushButton_12;
    QWidget *page_6;
    QListWidget *listWidget_2;
    QWidget *page_7;
    QPushButton *pushButton_3;
    QLabel *label_2;
    QLabel *label_6;
    QPushButton *pushButton_8;
    QMenuBar *menuBar;
    QMenu *menu;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *simplechat)
    {
        if (simplechat->objectName().isEmpty())
            simplechat->setObjectName(QString::fromUtf8("simplechat"));
        simplechat->resize(504, 507);
        centralWidget = new QWidget(simplechat);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        stackedWidget = new QStackedWidget(centralWidget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        stackedWidget->setGeometry(QRect(30, 30, 431, 381));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        stackedWidget_2 = new QStackedWidget(page);
        stackedWidget_2->setObjectName(QString::fromUtf8("stackedWidget_2"));
        stackedWidget_2->setGeometry(QRect(60, 50, 301, 271));
        page_3 = new QWidget();
        page_3->setObjectName(QString::fromUtf8("page_3"));
        pushButton = new QPushButton(page_3);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(60, 170, 71, 31));
        pushButton_2 = new QPushButton(page_3);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(170, 170, 71, 31));
        lineEdit = new QLineEdit(page_3);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(100, 60, 113, 27));
        stackedWidget_2->addWidget(page_3);
        page_4 = new QWidget();
        page_4->setObjectName(QString::fromUtf8("page_4"));
        pushButton_4 = new QPushButton(page_4);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(120, 140, 71, 31));
        lineEdit_2 = new QLineEdit(page_4);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(100, 80, 113, 27));
        stackedWidget_2->addWidget(page_4);
        page_8 = new QWidget();
        page_8->setObjectName(QString::fromUtf8("page_8"));
        label_7 = new QLabel(page_8);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(40, 50, 221, 41));
        label_8 = new QLabel(page_8);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(40, 100, 221, 41));
        pushButton_9 = new QPushButton(page_8);
        pushButton_9->setObjectName(QString::fromUtf8("pushButton_9"));
        pushButton_9->setGeometry(QRect(110, 190, 71, 31));
        stackedWidget_2->addWidget(page_8);
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        pushButton_5 = new QPushButton(page_2);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        pushButton_5->setGeometry(QRect(0, 350, 141, 31));
        pushButton_6 = new QPushButton(page_2);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
        pushButton_6->setGeometry(QRect(140, 350, 151, 31));
        pushButton_7 = new QPushButton(page_2);
        pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));
        pushButton_7->setGeometry(QRect(290, 350, 141, 31));
        stackedWidget_3 = new QStackedWidget(page_2);
        stackedWidget_3->setObjectName(QString::fromUtf8("stackedWidget_3"));
        stackedWidget_3->setGeometry(QRect(0, 60, 421, 291));
        page_5 = new QWidget();
        page_5->setObjectName(QString::fromUtf8("page_5"));
        listWidget = new QListWidget(page_5);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(0, 0, 141, 291));
        lineEdit_3 = new QLineEdit(page_5);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(140, 260, 231, 31));
        label = new QLabel(page_5);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(140, 0, 271, 231));
        label->setStyleSheet(QString::fromUtf8("background-color:rgb(233, 255, 252);"));
        pushButton_10 = new QPushButton(page_5);
        pushButton_10->setObjectName(QString::fromUtf8("pushButton_10"));
        pushButton_10->setGeometry(QRect(370, 260, 41, 31));
        pushButton_11 = new QPushButton(page_5);
        pushButton_11->setObjectName(QString::fromUtf8("pushButton_11"));
        pushButton_11->setGeometry(QRect(140, 230, 141, 31));
        pushButton_12 = new QPushButton(page_5);
        pushButton_12->setObjectName(QString::fromUtf8("pushButton_12"));
        pushButton_12->setGeometry(QRect(280, 230, 131, 31));
        stackedWidget_3->addWidget(page_5);
        page_6 = new QWidget();
        page_6->setObjectName(QString::fromUtf8("page_6"));
        listWidget_2 = new QListWidget(page_6);
        listWidget_2->setObjectName(QString::fromUtf8("listWidget_2"));
        listWidget_2->setGeometry(QRect(90, 20, 256, 261));
        stackedWidget_3->addWidget(page_6);
        page_7 = new QWidget();
        page_7->setObjectName(QString::fromUtf8("page_7"));
        pushButton_3 = new QPushButton(page_7);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(120, 190, 181, 27));
        label_2 = new QLabel(page_7);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(120, 90, 181, 101));
        label_2->setStyleSheet(QString::fromUtf8("background-color:rgb(233, 255, 252);"));
        label_2->setMargin(1);
        stackedWidget_3->addWidget(page_7);
        label_6 = new QLabel(page_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(120, 10, 121, 31));
        label_6->setStyleSheet(QString::fromUtf8("background-color:rgb(233, 255, 252);"));
        pushButton_8 = new QPushButton(page_2);
        pushButton_8->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_8->setGeometry(QRect(250, 10, 41, 31));
        pushButton_8->setStyleSheet(QString::fromUtf8(""));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/imgs/res/images/find.PNG"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_8->setIcon(icon);
        stackedWidget->addWidget(page_2);
        simplechat->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(simplechat);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 504, 28));
        menu = new QMenu(menuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        simplechat->setMenuBar(menuBar);
        mainToolBar = new QToolBar(simplechat);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        simplechat->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(simplechat);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        simplechat->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());

        retranslateUi(simplechat);

        stackedWidget->setCurrentIndex(0);
        stackedWidget_2->setCurrentIndex(0);
        stackedWidget_3->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(simplechat);
    } // setupUi

    void retranslateUi(QMainWindow *simplechat)
    {
        simplechat->setWindowTitle(QApplication::translate("simplechat", "simplechat", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("simplechat", "\347\231\273\345\275\225", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("simplechat", "\346\263\250\345\206\214", 0, QApplication::UnicodeUTF8));
        lineEdit->setPlaceholderText(QApplication::translate("simplechat", "\345\270\220\345\217\267\345\220\215", 0, QApplication::UnicodeUTF8));
        pushButton_4->setText(QApplication::translate("simplechat", "\346\263\250\345\206\214", 0, QApplication::UnicodeUTF8));
        lineEdit_2->setPlaceholderText(QApplication::translate("simplechat", "\345\270\220\345\217\267\345\220\215", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("simplechat", "\346\226\260\345\257\206\347\240\201", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("simplechat", "\347\241\256\350\256\244\346\226\260\345\257\206\347\240\201", 0, QApplication::UnicodeUTF8));
        pushButton_9->setText(QApplication::translate("simplechat", "\344\277\256\346\224\271", 0, QApplication::UnicodeUTF8));
        pushButton_5->setText(QApplication::translate("simplechat", "\346\266\210\346\201\257", 0, QApplication::UnicodeUTF8));
        pushButton_6->setText(QApplication::translate("simplechat", "\350\201\224\347\263\273\344\272\272", 0, QApplication::UnicodeUTF8));
        pushButton_7->setText(QApplication::translate("simplechat", "\346\210\221\347\232\204", 0, QApplication::UnicodeUTF8));
        lineEdit_3->setPlaceholderText(QApplication::translate("simplechat", "\350\276\223\345\205\245\345\206\205\345\256\271", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        pushButton_10->setText(QApplication::translate("simplechat", "send", 0, QApplication::UnicodeUTF8));
        pushButton_11->setText(QApplication::translate("simplechat", "-", 0, QApplication::UnicodeUTF8));
        pushButton_12->setText(QApplication::translate("simplechat", "+", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("simplechat", "\351\200\200\345\207\272", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("simplechat", "\346\210\221\347\232\204", 0, QApplication::UnicodeUTF8));
        label_6->setText(QString());
        pushButton_8->setText(QApplication::translate("simplechat", "find", 0, QApplication::UnicodeUTF8));
        menu->setTitle(QApplication::translate("simplechat", "\346\230\223\350\257\264", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class simplechat: public Ui_simplechat {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIMPLECHAT_H
