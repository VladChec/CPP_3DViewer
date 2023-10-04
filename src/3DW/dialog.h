#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <iostream>
#include <QMainWindow>
#define GL_SILENCE_DEPRECATION
#include <QDebug>
#include <QFileDialog>
#include <QtOpenGL>
#include <QtWidgets/QWidget>
#include <QtOpenGLWidgets>
#include <QColorDialog>

#include <QLabel>
#include <QPixmap>
#include <QScreen>
#include <vector>
#include <cstdint>
#include <QTimer>
#include <QSettings>
#include <QTextEdit>
#include <math.h>
#include "../QtGifImage-master/src/gifimage/qgifimage.h"
#include <QPainter>
#include <QImage>
#include "../Controller/Controller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();
    QVector<QImage> mas_image;
protected:
    s21::Model model_;
    s21::ModelController modelController;
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private slots:
        void on_button_openFile_clicked();
        void wheelEvent(QWheelEvent *event);
       void on_spinBox_rotateX_textChanged(const QString &arg1);
       void on_dial_rotateX_sliderMoved(int position);
        void on_spinBox_rotateY_textChanged(const QString &arg1);
       void on_dial_rotateY_sliderMoved(int position);
       void on_spinBox_rotateZ_textChanged(const QString &arg1);
       void on_dial_rotateZ_sliderMoved(int position);
       void on_spinBox_translateX_textChanged(const QString &arg1);
       void on_slider_translateX_sliderMoved(int position);
       void on_spinBox_translateY_textChanged(const QString &arg1);
       void on_slider_translateY_sliderMoved(int position);
        void on_spinBox_translateZ_textChanged(const QString &arg1);
        void on_slider_translateZ_sliderMoved(int position);
        void on_spinBox_scale_textChanged(const QString &arg1);
        void on_pushButton_vertColor_clicked();
        void on_pushButton_linesColor_clicked();
        void on_pushButton_backColor_clicked();
        void on_doubleSpinBox_thick_valueChanged(double arg1);
        void on_spinBox_pointsSize_valueChanged(int arg1);
        void on_comboBox_disMetod_currentIndexChanged(int index);
        void on_checkBox_dashed_stateChanged(int arg1);
        void on_checkBox_solid_stateChanged(int arg1);
        void on_pushButton_screenShot_clicked();
        void on_radioButton_central_clicked(bool checked);
        void on_radioButton_parallel_clicked(bool checked);
        void on_pushButton_gif_clicked();
        void save_gif();
        void create_screen();
        void on_pushButton_screenShot_2_clicked();
        void on_pushButton_screenShot_3_clicked();


private:

    Ui::Dialog *ui;
//     QOpenGLWidget *glWidget;

     int xRot;
     int yRot;
     int zRot;
    int dx;
    int dy;
    int dz;
     QTimer *timer;
     QTimer *timer_for_gif;
     QThread *somethread;
     double time = 0;
     int xyz = 1;
     int flag_record = 0;

};
#endif // DIALOG_H
