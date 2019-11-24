#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include<opencv/cv.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/core/core.hpp>
//#include<opencv2/contrib/contrib.hpp>
#include<opencv2/videoio/videoio.hpp>

using namespace cv;

#include <iostream>
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();



private slots:
    /* methods*/
    void on_pushButton_open_webcam_clicked();
    //void circleColoredarea(Mat * f1,Mat * f2, Mat * f3,Point * p1,Point * p2);
    void circleColoredarea2pointbased(Mat * f1,Mat * f2, Mat * f3,Point * p1,Point * p2,Point *p3,Point * p4,Scalar s,int thickness);

    void on_pushButton_close_webcam_clicked();

    void update_window();
    static void resizes(VideoCapture * cap,int k,int y);
    void choose_filter(Mat f1, Mat * filtred);




    void on_checkBox_stateChanged();

    void on_checkBox_2_stateChanged();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_action70_triggered();

    void on_action50_triggered();

    void on_action30_triggered();

    void on_action10_triggered();
    void icon( Mat * framewriter,Mat * icon,Point position,Point position2);
    void set3points();
    void get3points(Mat * frame);


    void viderframe(Mat *s ,Scalar color);

    void on_Blue_stateChanged(int arg1);

    void on_Red_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;

    QTimer *timer;
    QPalette pal;
    VideoCapture cap;
    Mat frame;
    Mat framehsv;
    QImage qt_image;
    QImage drawer;
    Point drawsecond,drawfirst,draw_erase1,draw_erase2;
    Mat frame_writer,frameIcon;
    Mat pencil,eraser;
    int seuildrawing;
    int seuilerase;
    int filter=-1;/*aucun filtre initialement*/
    int dimension=3;
    Scalar color1= Scalar(-1,-1,-1);
    Scalar color2=Scalar(-1,-1,-1);
    Scalar color3=Scalar(-1,-1,-1);
    Point color_1;
    Point color_2;
    Point color_3;
    Point B1;
    Point B2;
    Point B3;
    Scalar drawingcolor;
    int seuil=50;
    int h=400,w=500;
    int iframetraite=0;
    int compteur=2;
    int intervalleColor=10;
    Mat frameIntermediere;
};


#endif // MAINWINDOW_H
