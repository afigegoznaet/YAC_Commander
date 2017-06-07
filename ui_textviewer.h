/********************************************************************************
** Form generated from reading UI file 'textviewer.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEXTVIEWER_H
#define UI_TEXTVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_TextViewer
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;

    void setupUi(QDialog *TextViewer)
    {
        if (TextViewer->objectName().isEmpty())
            TextViewer->setObjectName(QStringLiteral("TextViewer"));
        TextViewer->resize(793, 549);
        verticalLayout = new QVBoxLayout(TextViewer);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tabWidget = new QTabWidget(TextViewer);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));

        verticalLayout->addWidget(tabWidget);


        retranslateUi(TextViewer);

        tabWidget->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(TextViewer);
    } // setupUi

    void retranslateUi(QDialog *TextViewer)
    {
        TextViewer->setWindowTitle(QApplication::translate("TextViewer", "Dialog", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class TextViewer: public Ui_TextViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEXTVIEWER_H
