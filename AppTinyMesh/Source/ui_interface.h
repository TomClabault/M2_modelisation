/********************************************************************************
** Form generated from reading UI file 'interface.ui'
**
** Created by: Qt User Interface Compiler version 6.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INTERFACE_H
#define UI_INTERFACE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Assets
{
public:
    QAction *actionExit;
    QAction *actionNew_Scene;
    QAction *actionExport_obj;
    QAction *actionExport_ma;
    QAction *actionLoad_ma;
    QAction *actionGL;
    QAction *actionVectorized;
    QAction *actionExport_svg;
    QAction *actionSphere_Tracing_png;
    QWidget *centralwidget;
    QHBoxLayout *hboxLayout;
    QGroupBox *Objects_groupBox;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLineEdit *erosion_count_input;
    QCheckBox *use_ray_marching_checkbox;
    QLabel *erosion_count_label;
    QPushButton *resetcameraButton;
    QPushButton *testSDFButton;
    QPushButton *save_obj_button;
    QPushButton *sphereImplicit;
    QLabel *erosion_sphere_spread_label;
    QLineEdit *erosion_sphere_radius_input;
    QLabel *erosion_sphere_radius_label;
    QLineEdit *erosion_sphere_spread_input;
    QPushButton *boxMesh;
    QPushButton *testBezierButton;
    QPushButton *testRevolutionButton;
    QWidget *widget_GL;
    QGroupBox *Parameters_groupBox;
    QGroupBox *groupBox_4;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QLabel *label_3;
    QLineEdit *lineEdit;
    QGroupBox *groupBox_13;
    QRadioButton *radioShadingButton_1;
    QCheckBox *wireframe;
    QRadioButton *radioShadingButton_2;
    QMenuBar *menubar;
    QMenu *menuFile;

    void setupUi(QMainWindow *Assets)
    {
        if (Assets->objectName().isEmpty())
            Assets->setObjectName("Assets");
        Assets->resize(1200, 1000);
        Assets->setMinimumSize(QSize(420, 300));
        actionExit = new QAction(Assets);
        actionExit->setObjectName("actionExit");
        actionNew_Scene = new QAction(Assets);
        actionNew_Scene->setObjectName("actionNew_Scene");
        actionExport_obj = new QAction(Assets);
        actionExport_obj->setObjectName("actionExport_obj");
        actionExport_ma = new QAction(Assets);
        actionExport_ma->setObjectName("actionExport_ma");
        actionLoad_ma = new QAction(Assets);
        actionLoad_ma->setObjectName("actionLoad_ma");
        actionGL = new QAction(Assets);
        actionGL->setObjectName("actionGL");
        actionVectorized = new QAction(Assets);
        actionVectorized->setObjectName("actionVectorized");
        actionExport_svg = new QAction(Assets);
        actionExport_svg->setObjectName("actionExport_svg");
        actionSphere_Tracing_png = new QAction(Assets);
        actionSphere_Tracing_png->setObjectName("actionSphere_Tracing_png");
        centralwidget = new QWidget(Assets);
        centralwidget->setObjectName("centralwidget");
        hboxLayout = new QHBoxLayout(centralwidget);
        hboxLayout->setSpacing(0);
        hboxLayout->setObjectName("hboxLayout");
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        Objects_groupBox = new QGroupBox(centralwidget);
        Objects_groupBox->setObjectName("Objects_groupBox");
        Objects_groupBox->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Objects_groupBox->sizePolicy().hasHeightForWidth());
        Objects_groupBox->setSizePolicy(sizePolicy);
        Objects_groupBox->setMinimumSize(QSize(350, 250));
        gridLayoutWidget = new QWidget(Objects_groupBox);
        gridLayoutWidget->setObjectName("gridLayoutWidget");
        gridLayoutWidget->setGeometry(QRect(10, 10, 331, 234));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        erosion_count_input = new QLineEdit(gridLayoutWidget);
        erosion_count_input->setObjectName("erosion_count_input");

        gridLayout->addWidget(erosion_count_input, 5, 1, 1, 1);

        use_ray_marching_checkbox = new QCheckBox(gridLayoutWidget);
        use_ray_marching_checkbox->setObjectName("use_ray_marching_checkbox");

        gridLayout->addWidget(use_ray_marching_checkbox, 6, 0, 1, 1);

        erosion_count_label = new QLabel(gridLayoutWidget);
        erosion_count_label->setObjectName("erosion_count_label");

        gridLayout->addWidget(erosion_count_label, 5, 0, 1, 1);

        resetcameraButton = new QPushButton(gridLayoutWidget);
        resetcameraButton->setObjectName("resetcameraButton");
        resetcameraButton->setCheckable(true);
        resetcameraButton->setAutoDefault(false);
        resetcameraButton->setFlat(false);

        gridLayout->addWidget(resetcameraButton, 0, 1, 1, 1);

        testSDFButton = new QPushButton(gridLayoutWidget);
        testSDFButton->setObjectName("testSDFButton");

        gridLayout->addWidget(testSDFButton, 1, 1, 1, 1);

        save_obj_button = new QPushButton(gridLayoutWidget);
        save_obj_button->setObjectName("save_obj_button");

        gridLayout->addWidget(save_obj_button, 7, 0, 1, 1);

        sphereImplicit = new QPushButton(gridLayoutWidget);
        sphereImplicit->setObjectName("sphereImplicit");
        sphereImplicit->setCheckable(false);

        gridLayout->addWidget(sphereImplicit, 1, 0, 1, 1);

        erosion_sphere_spread_label = new QLabel(gridLayoutWidget);
        erosion_sphere_spread_label->setObjectName("erosion_sphere_spread_label");

        gridLayout->addWidget(erosion_sphere_spread_label, 4, 0, 1, 1);

        erosion_sphere_radius_input = new QLineEdit(gridLayoutWidget);
        erosion_sphere_radius_input->setObjectName("erosion_sphere_radius_input");

        gridLayout->addWidget(erosion_sphere_radius_input, 3, 1, 1, 1);

        erosion_sphere_radius_label = new QLabel(gridLayoutWidget);
        erosion_sphere_radius_label->setObjectName("erosion_sphere_radius_label");
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(erosion_sphere_radius_label->sizePolicy().hasHeightForWidth());
        erosion_sphere_radius_label->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(erosion_sphere_radius_label, 3, 0, 1, 1);

        erosion_sphere_spread_input = new QLineEdit(gridLayoutWidget);
        erosion_sphere_spread_input->setObjectName("erosion_sphere_spread_input");

        gridLayout->addWidget(erosion_sphere_spread_input, 4, 1, 1, 1);

        boxMesh = new QPushButton(gridLayoutWidget);
        boxMesh->setObjectName("boxMesh");
        boxMesh->setCheckable(false);

        gridLayout->addWidget(boxMesh, 0, 0, 1, 1);

        testBezierButton = new QPushButton(gridLayoutWidget);
        testBezierButton->setObjectName("testBezierButton");

        gridLayout->addWidget(testBezierButton, 2, 1, 1, 1);

        testRevolutionButton = new QPushButton(gridLayoutWidget);
        testRevolutionButton->setObjectName("testRevolutionButton");

        gridLayout->addWidget(testRevolutionButton, 2, 0, 1, 1);


        hboxLayout->addWidget(Objects_groupBox);

        widget_GL = new QWidget(centralwidget);
        widget_GL->setObjectName("widget_GL");

        hboxLayout->addWidget(widget_GL);

        Parameters_groupBox = new QGroupBox(centralwidget);
        Parameters_groupBox->setObjectName("Parameters_groupBox");
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(Parameters_groupBox->sizePolicy().hasHeightForWidth());
        Parameters_groupBox->setSizePolicy(sizePolicy2);
        Parameters_groupBox->setMinimumSize(QSize(450, 141));
        groupBox_4 = new QGroupBox(Parameters_groupBox);
        groupBox_4->setObjectName("groupBox_4");
        groupBox_4->setGeometry(QRect(10, 20, 171, 121));
        label_2 = new QLabel(groupBox_4);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(10, 20, 61, 21));
        lineEdit_2 = new QLineEdit(groupBox_4);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(70, 50, 81, 20));
        lineEdit_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_3 = new QLabel(groupBox_4);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(10, 50, 61, 21));
        lineEdit = new QLineEdit(groupBox_4);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(70, 20, 81, 20));
        lineEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        groupBox_13 = new QGroupBox(Parameters_groupBox);
        groupBox_13->setObjectName("groupBox_13");
        groupBox_13->setGeometry(QRect(190, 20, 251, 121));
        radioShadingButton_1 = new QRadioButton(groupBox_13);
        radioShadingButton_1->setObjectName("radioShadingButton_1");
        radioShadingButton_1->setGeometry(QRect(20, 20, 61, 20));
        radioShadingButton_1->setChecked(true);
        wireframe = new QCheckBox(groupBox_13);
        wireframe->setObjectName("wireframe");
        wireframe->setGeometry(QRect(90, 20, 81, 17));
        wireframe->setChecked(true);
        radioShadingButton_2 = new QRadioButton(groupBox_13);
        radioShadingButton_2->setObjectName("radioShadingButton_2");
        radioShadingButton_2->setGeometry(QRect(20, 50, 61, 20));
        radioShadingButton_2->setChecked(false);

        hboxLayout->addWidget(Parameters_groupBox);

        Assets->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Assets);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1200, 21));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        Assets->setMenuBar(menubar);

        menubar->addAction(menuFile->menuAction());

        retranslateUi(Assets);

        resetcameraButton->setDefault(false);


        QMetaObject::connectSlotsByName(Assets);
    } // setupUi

    void retranslateUi(QMainWindow *Assets)
    {
        Assets->setWindowTitle(QCoreApplication::translate("Assets", "Implicit Terrain", nullptr));
        actionExit->setText(QCoreApplication::translate("Assets", "Exit", nullptr));
#if QT_CONFIG(shortcut)
        actionExit->setShortcut(QCoreApplication::translate("Assets", "Esc", nullptr));
#endif // QT_CONFIG(shortcut)
        actionNew_Scene->setText(QCoreApplication::translate("Assets", "New Scene", nullptr));
        actionExport_obj->setText(QCoreApplication::translate("Assets", "OBJ File (.obj)", nullptr));
        actionExport_ma->setText(QCoreApplication::translate("Assets", "Maya File (.ma)", nullptr));
        actionLoad_ma->setText(QCoreApplication::translate("Assets", "Load", nullptr));
        actionGL->setText(QCoreApplication::translate("Assets", "GL", nullptr));
        actionVectorized->setText(QCoreApplication::translate("Assets", "Vectorized", nullptr));
        actionExport_svg->setText(QCoreApplication::translate("Assets", "SVG File (.svg)", nullptr));
        actionSphere_Tracing_png->setText(QCoreApplication::translate("Assets", "Sphere Tracing (.png)", nullptr));
        Objects_groupBox->setTitle(QString());
        erosion_count_input->setText(QCoreApplication::translate("Assets", "10", nullptr));
        use_ray_marching_checkbox->setText(QCoreApplication::translate("Assets", "Use Ray Marching", nullptr));
        erosion_count_label->setText(QCoreApplication::translate("Assets", "Erosion count", nullptr));
#if QT_CONFIG(tooltip)
        resetcameraButton->setToolTip(QCoreApplication::translate("Assets", "<html><head/><body><p><span style=\" color:#5500ff;\">Callback #03</span></p><p><span style=\" font-style:italic; color:#aa00ff;\">Create Eurographics 1996 Bird</span></p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        resetcameraButton->setText(QCoreApplication::translate("Assets", "Reset Camera", nullptr));
        testSDFButton->setText(QCoreApplication::translate("Assets", "Test SDF", nullptr));
        save_obj_button->setText(QCoreApplication::translate("Assets", "Save OBJ", nullptr));
#if QT_CONFIG(tooltip)
        sphereImplicit->setToolTip(QCoreApplication::translate("Assets", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#5500ff;\">Callback #01</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-style:italic; color:#aa00ff;\">Create Eurographics 1996 Bear</span></p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        sphereImplicit->setText(QCoreApplication::translate("Assets", "Sphere Implicit", nullptr));
        erosion_sphere_spread_label->setText(QCoreApplication::translate("Assets", "Erosion Sphere Spread (pixels)", nullptr));
        erosion_sphere_radius_input->setText(QCoreApplication::translate("Assets", "0.05", nullptr));
        erosion_sphere_radius_label->setText(QCoreApplication::translate("Assets", "Erosion Sphere Radius", nullptr));
        erosion_sphere_spread_input->setText(QCoreApplication::translate("Assets", "50", nullptr));
#if QT_CONFIG(tooltip)
        boxMesh->setToolTip(QCoreApplication::translate("Assets", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#5500ff;\">Callback #01</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-style:italic; color:#aa00ff;\">Create Eurographics 1996 Bear</span></p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        boxMesh->setText(QCoreApplication::translate("Assets", "Box Mesh", nullptr));
        testBezierButton->setText(QCoreApplication::translate("Assets", "Test Bezier", nullptr));
        testRevolutionButton->setText(QCoreApplication::translate("Assets", "Test Revolution", nullptr));
        Parameters_groupBox->setTitle(QString());
        groupBox_4->setTitle(QCoreApplication::translate("Assets", " Statistics", nullptr));
        label_2->setText(QCoreApplication::translate("Assets", "Vertex", nullptr));
        label_3->setText(QCoreApplication::translate("Assets", "Triangles", nullptr));
        lineEdit->setText(QString());
        groupBox_13->setTitle(QCoreApplication::translate("Assets", "Shading", nullptr));
        radioShadingButton_1->setText(QCoreApplication::translate("Assets", "Normal", nullptr));
        wireframe->setText(QCoreApplication::translate("Assets", "Wireframe", nullptr));
        radioShadingButton_2->setText(QCoreApplication::translate("Assets", "Color", nullptr));
        menuFile->setTitle(QCoreApplication::translate("Assets", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Assets: public Ui_Assets {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INTERFACE_H
