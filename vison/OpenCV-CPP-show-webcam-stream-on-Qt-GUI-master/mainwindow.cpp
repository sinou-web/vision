#include "mainwindow.h"
#include "ui_mainwindow.h"
using namespace std;



Point Barycentre (vector<Point> vec)
{
    if(vec.empty()) return Point(0,0);
    //sort(vec.begin(),vec.end());
    Point barycentre(0,0);

    long size=vec.size();
     for(int i=0;i<size;i++)
     {
        barycentre.x+=vec.at(i).x;
        barycentre.y+=vec.at(i).y;
     }

    barycentre.x/=size;
    barycentre.y/=size;

    return barycentre;

}

void Filtremedian(Mat src,Mat * dst, int voisinage)
{
    //printf("hiiiii median");
    int * voisins= new int[voisinage * voisinage];
    int * voisinsblue= new int[voisinage * voisinage];
    int * voisinsrouge= new int[voisinage * voisinage];

    if(src.rows!=(*dst).rows || src.cols != (*dst).cols) return;

        if( voisinage%2 == 0) return;
        Rect rec_roi = Rect(0,0,voisinage,voisinage);


        for( int x=0;x<src.rows;x++)
        {
            for(int y=0;y<src.cols;y++)
            {
                if(!( x < (voisinage-1)/2 || x>(src.rows-1) -(voisinage-1)/2
                || y < (voisinage-1)/2 || y>(src.cols-1)-(voisinage-1)/2 ))
             {
                    rec_roi.x=  y -(voisinage-1)/2; //switch the x y
                    rec_roi.y= x - (voisinage-1)/2;

                    Mat img_roi =src(rec_roi);

                 /*first color*/
                    for (int i=0;i<voisinage;i++)
                    {
                        for(int j=0;j<voisinage;j++)
                        {
                           voisins[i*voisinage+j]=img_roi.at<Vec3b>(i,j)[0];
                        }
                    }
                    sort(voisins,voisins+(voisinage*voisinage));
                    (*dst).at<Vec3b>(x,y)[0]=(uchar) voisins[(voisinage*voisinage-1)/2];
                    /* second color*/
                    for (int i=0;i<voisinage;i++)
                    {
                        for(int j=0;j<voisinage;j++)
                        {
                           voisinsblue[i*voisinage+j]=img_roi.at<Vec3b>(i,j)[1];
                        }
                    }
                    sort(voisinsblue,voisinsblue+(voisinage*voisinage));
                    (*dst).at<Vec3b>(x,y)[1]=(uchar) voisinsblue[(voisinage*voisinage-1)/2];
                    /*third color*/
                    /* second color*/
                    for (int i=0;i<voisinage;i++)
                    {
                        for(int j=0;j<voisinage;j++)
                        {
                           voisinsrouge[i*voisinage+j]=img_roi.at<Vec3b>(i,j)[2];
                        }
                    }
                    sort(voisinsrouge,voisinsrouge+(voisinage*voisinage));
                    (*dst).at<Vec3b>(x,y)[2]=(uchar) voisinsrouge[(voisinage*voisinage-1)/2];
                }

            }
        }

}
/* Fonction qui sert à calculer le seuil entre les deux points avec les quelles on desire dessiner et retourne le point moyen entre les deux */
Point compareseuil(Point x,Point y, int seuil)
{
    /* distance entre x */
    int distancex=x.x-y.x;
    /*distance entre y*/
    int distancey=x.y-y.y;
    /* valeur absolue de la distance */
    if(distancex<0) distancex=distancex*-1;
     if(distancey<0) distancey=distancey*-1;

    /*si c'est superieur à notre seuil alors on dessine pas on retourne un point (0,0)*/
     if(distancex>seuil || distancey>seuil)
     return Point(0,0);
     /* retourner moyenne des deux valeurs */
    return Point((x.x+y.x)/2,(x.y+y.y)/2);
}
int drawif(Point p1,Point p2)
{
   int difference= p1.x-p2.x;
   int differencey=p1.y-p2.y;

   if(difference <0) difference=difference*-1;
   if(differencey<0) differencey=difference*-1;

  if( difference< 5 && difference< 5) return 0;
          else return 1;


}
/* c++ problem passage par adresse si on desire que l'image se modifie ceci explique la presence de "*" */

void  MainWindow::circleColoredarea2pointbased(Mat * hsv_frame,Mat * frameMEDIAN, Mat * frame_writer, Point * draw1,Point * draw2,Point *draw3 ,Point *draw4,Scalar s,int thickness)
{
    /* vider*/
    std::vector<Point> coloredarea1;
    std::vector<Point> coloredarea2;
    std::vector<Point> coloredarea3;

    Mat black_white_detection;
    /* transformation to hsv HUE*/
        cvtColor((*frameMEDIAN),(*hsv_frame),COLOR_RGB2HSV);
          /* Inrange permet de colorier en blanc les pixels colorées entre l'intervalle données par scalar 1 et scalar 2*/
           /* donc vu qu'on a pas le droit de l'utiliser j'ai utilisé le parcours classic et verification à chaque fois*/
          /* je l'ai gardé pour verification seulement */
         //  inRange((*hsv_frame),Scalar(163,161,116),Scalar(179,228,169),black_white_detection);
          // imshow("red",black_white_detection);
          /*detecter couleurs :simulation de inrange*/
        for(int x=0;x<(*hsv_frame).rows;x++)
          {
              for(int y=0;y<(*hsv_frame).cols;y++)
              {
                  if(this->color1[0] !=-1 )
                  if(((*hsv_frame).at<Vec3b>(x,y)[0]<this->color1[0]+intervalleColor)&&((*hsv_frame).at<Vec3b>(x,y)[0]>=this->color1[0]-intervalleColor)
                          &&
                        (((*hsv_frame).at<Vec3b>(x,y)[1]<this->color1[1]+intervalleColor) &&((*hsv_frame).at<Vec3b>(x,y)[1]>=this->color1[1]-intervalleColor))
                            &&
                          ((*hsv_frame).at<Vec3b>(x,y)[2]<this->color1[2]+intervalleColor) && (*hsv_frame).at<Vec3b>(x,y)[2]>=this->color1[2]-intervalleColor
                          )
                       {
                   /* recuperation des nuages de points colorés color 1*/
                      coloredarea1.push_back(Point(y,x));

                        }

                        if(this->color2[0]!=-1 )
                        if(((*hsv_frame).at<Vec3b>(x,y)[0]<this->color2[0]+intervalleColor)&&((*hsv_frame).at<Vec3b>(x,y)[0]>=this->color2[0]-intervalleColor)
                          &&
                        (((*hsv_frame).at<Vec3b>(x,y)[1]<this->color2[1]+intervalleColor) &&((*hsv_frame).at<Vec3b>(x,y)[1]>=this->color2[1]-intervalleColor))
                            &&
                          ((*hsv_frame).at<Vec3b>(x,y)[2]<this->color2[2]+intervalleColor) && (*hsv_frame).at<Vec3b>(x,y)[2]>=this->color2[2]-intervalleColor
                                )
                        {
                        /* recuperation des nuages de points colorés avec color 2*/
                       coloredarea2.push_back(Point(y,x));

                        }
                           if(this->color3[0] !=-1 )
                        if(((*hsv_frame).at<Vec3b>(x,y)[0]<this->color3[0]+20)&&((*hsv_frame).at<Vec3b>(x,y)[0]>=this->color3[0]-20)
                          &&
                        (((*hsv_frame).at<Vec3b>(x,y)[1]<this->color3[1]+20) &&((*hsv_frame).at<Vec3b>(x,y)[1]>=this->color3[1]-20))
                            &&
                          ((*hsv_frame).at<Vec3b>(x,y)[2]<this->color3[2]+20) && (*hsv_frame).at<Vec3b>(x,y)[2]>=this->color3[2]-20
                                )
                        {
                        /* recuperation des nuages de points colorés avec color 2*/
                       coloredarea3.push_back(Point(y,x));

                        }
              }

          }
            /*center of the first color detected*/
          Point barycenter1=Barycentre(coloredarea1);

          if(barycenter1.x>0) //eviter de cercler le 0,0
         { circle(*frameMEDIAN, barycenter1, 10, Scalar(0,0,255), 7,8, 0);
            //  B1=barycentre1;


          }
          /*center of the second color detected*/
           Point barycenter2=Barycentre(coloredarea2);
          if(barycenter2.x>0)
          {         // B2=barycentre2;

          circle(*frameMEDIAN, barycenter2, 10, Scalar(0,255,0), 7,8, 0);
            }

          /*center of the second color detected*/
           Point barycenter3=Barycentre(coloredarea3);
          if(barycenter3.x>0)
          {
          circle(*frameMEDIAN, barycenter3, 10, Scalar(255,0,0), 7,8, 0);

          }

          if(this->compteur==this->iframetraite)
          { iframetraite=0;
             /* la partie dessin*/
          Point seuil=compareseuil(barycenter1,barycenter2,this->seuil);
          this->seuildrawing=seuil.x;
          /* la fonction comparerseuil return un point (0,0) si une des 2 listes est vide */
          if(seuil.x>0) (*draw1)=seuil;

          if((*draw1).x>0 )
          {
           if( (*draw2).x==0 && (*draw2).y==0) (*draw2)=Point((*draw1).x,(*draw1).y);


           if(drawif((*draw1),(*draw2))==1)
           {
           line(*frame_writer,Point((*frame_writer).rows-(*draw2).x,(*draw2).y),Point((*frame_writer).rows-(*draw1).x,(*draw1).y),s, 8, 8, 0);

           (*draw2)=(*draw1);
            }
           }
         /* partie effacer equivalente à dessin mais juste on dessine avec du blanc avec les 2 points*/
          Point seuil_effacer=compareseuil(barycenter2,barycenter3,this->seuil);
          this->seuilerase=seuil_effacer.x;
          /* la fonction comparerseuil return un point (0,0) si une des 2 listes est vide */
          if(seuil_effacer.x>0) (*draw3)=seuil_effacer;

          if((*draw3).x>0 )
          {
           if( (*draw4).x==0 && (*draw4).y==0) (*draw4)=Point((*draw3).x,(*draw3).y);

           line(*frame_writer,Point((*frame_writer).rows-(*draw4).x,(*draw4).y),Point((*frame_writer).rows-(*draw3).x,(*draw3).y),Scalar(255,255,255), thickness, 8, 0);
            (*draw4)=(*draw3);
          }


}

}
void MainWindow:: icon(Mat * framewriter,Mat * icon ,Point position,Point position2)
{ if(position.x!=0 || position.y!=0  )

       for(int i=position.x;i-position.x<(*icon).rows && i<(*framewriter).rows;i++)
          for(int j=position.y;j-position.y<(*icon).cols && j<(*framewriter).cols;j++)
          {
              if(!((*icon).at<Vec3b>(j-position.y,i-position.x)[2]==255 && (*icon).at<Vec3b>(j-position.y,i-position.x)[1]==255 && (*icon).at<Vec3b>(j-position.y,i-position.x)[0]==255 ))
               (*framewriter).at<Vec3b>(j,i) =(*icon).at<Vec3b>(j-position.y,i-position.x);

          }
}

void MainWindow::viderframe(Mat * frame_writer ,Scalar
                s )
{
    for(int x=0;x<(*frame_writer).rows;x++)
    {
        for(int y=0;y<(*frame_writer).cols;y++)
        {
            /* initialisation de l'image à blanc donc les 3 couleurs à blanc(white honey)*/
            (*frame_writer).at<Vec3b>(x,y)[0]=s[0];
            (*frame_writer).at<Vec3b>(x,y)[1]=s[1];
            (*frame_writer).at<Vec3b>(x,y)[2]=s[2];



        }

    }
}
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    Mat blue =Mat(20,20,CV_8UC3);
    Mat red =Mat(20,20,CV_8UC3);

    Mat open = imread("../open.png",1);
    Mat close=imread("../stop.png",1);



    viderframe(&blue,Scalar(0,0,255));
    viderframe(&red,Scalar(255,0,0));
     cvtColor(blue, blue, CV_BGR2RGB);
     cvtColor(red, red, CV_BGR2RGB);
     cvtColor(open, open, CV_BGR2RGB);
     cvtColor(close, close, CV_BGR2RGB);
       cv::resize(open,open,Size(40,40),0,INTER_CUBIC);
       cv::resize(close,close,Size(40,40),0,INTER_CUBIC);

       pencil=imread("../pencil.png",1);
       eraser=imread("../gomme.png",1);


        cv::resize(pencil,pencil,Size(30,30),0,0,INTER_CUBIC);
        cv::resize(eraser,eraser,Size(30,30),0,0,INTER_CUBIC);
        cvtColor(pencil,pencil,CV_BGR2RGB);
        cvtColor(eraser,eraser,CV_BGR2RGB);

    ui->red->setPixmap(QPixmap::fromImage( QImage((const unsigned char*) (blue.data), blue.cols, blue.rows, QImage::Format_RGB888)));
    ui->red->resize(ui->red->pixmap()->size());


    ui->blue->setPixmap(QPixmap::fromImage( QImage((const unsigned char*) (red.data), red.cols, red.rows, QImage::Format_RGB888)));
    ui->blue->resize(ui->blue->pixmap()->size());


    QPixmap pixmap(QPixmap::fromImage( QImage((const unsigned char*) (open.data),
                                              open.cols, open.rows,3*open.cols, QImage::Format_RGB888)));
    QIcon ButtonIcon(pixmap);
    ui->pushButton_open_webcam->resize(pixmap.rect().size());
    ui->pushButton_open_webcam->setIcon(ButtonIcon);
    ui->pushButton_open_webcam->setIconSize(pixmap.rect().size());
    ui->pushButton_open_webcam->setText("");
    /* button close*/
    QPixmap pixmap2(QPixmap::fromImage( QImage((const unsigned char*) (close.data),
                                              close.cols, close.rows,3*close.cols, QImage::Format_RGB888)));
    QIcon ButtonIcon2(pixmap2);
    ui->pushButton_close_webcam->resize(pixmap2.rect().size());
    ui->pushButton_close_webcam->setIcon(ButtonIcon2);
    ui->pushButton_close_webcam->setIconSize(pixmap2.rect().size());
    ui->pushButton_close_webcam->setText("");





    timer = new QTimer(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::resizes(VideoCapture *cap, int k,int m)
{
   (*cap).set(CV_CAP_PROP_FRAME_WIDTH,k);
   (*cap).set(CV_CAP_PROP_FRAME_HEIGHT,m);
}

void MainWindow::on_pushButton_close_webcam_clicked()
{
    disconnect(timer, SIGNAL(timeout()), this, SLOT(update_window()));
    cap.release();

    Mat image = Mat::zeros(frame.size(),CV_8UC3);

    qt_image = QImage((const unsigned char*) (image.data), image.cols, image.rows, QImage::Format_RGB888);

    ui->label_2->setPixmap(QPixmap::fromImage(qt_image));


    color1=Scalar(-1,-1,-1);
    color2=Scalar(-1,-1,-1);
    color3=Scalar(-1,-1,-1);

    cout << "camera is closed" << endl;

}
void filtreMoyenNVG(Mat  src, Mat * dst,int voisinage)
{

    if(src.rows!=(*dst).rows || src.cols != (*dst).cols) return;

       if( voisinage%2 == 0) return;
       Rect rec_roi = Rect(0,0,voisinage,voisinage);
       int moy =0;

       for( int x=0;x<src.rows;x++)
       {
           for(int y=0;y<src.cols;y++)
           {
               if( x < (voisinage-1)/2 || x>(src.rows-1) -(voisinage-1)/2
               || y < (voisinage-1)/2 || y>(src.cols-1)-(voisinage-1)/2 )
               {
                   (*dst).at<Vec3b>(x,y)=src.at<Vec3b>(x,y);
               }
               else

               {
                   moy=0;
                   rec_roi.x=  y -(voisinage-1)/2;
   ;               rec_roi.y= x - (voisinage-1)/2;
                   Mat img_roi =src(rec_roi);
                   for (int i=0;i<voisinage;i++)
                   {
                       for(int j=0;j<voisinage;j++)
                       {
                           moy+=img_roi.at<Vec3b>(i,j)[0];
                       }
                   }
                   moy/=voisinage*voisinage;
                   (*dst).at<Vec3b>(x,y)[0]=(uchar) moy ;

                   /* second color*/
                   moy=0;

                   for (int i=0;i<voisinage;i++)
                   {
                       for(int j=0;j<voisinage;j++)
                       {
                           moy+=img_roi.at<Vec3b>(i,j)[1];
                       }
                   }
                   moy/=voisinage*voisinage;
                   (*dst).at<Vec3b>(x,y)[1]=(uchar) moy ;
                    /*third color*/

                   moy=0;

                   for (int i=0;i<voisinage;i++)
                   {
                       for(int j=0;j<voisinage;j++)
                       {
                           moy+=img_roi.at<Vec3b>(i,j)[2];
                       }
                   }
                   moy/=voisinage*voisinage;
                   (*dst).at<Vec3b>(x,y)[2]=(uchar) moy ;

               }

           }
       }


}
void MainWindow::choose_filter(Mat frame, Mat *framefiltered)
{

    if(filter==1) Filtremedian(frame,framefiltered,dimension);
    if(filter==0) filtreMoyenNVG(frame,framefiltered,dimension);

}

void MainWindow::set3points(){
    /* init the 2 point to get both colors chosen by the user */
   this->color_1=Point(frame.cols/2+50,frame.rows/2-50);
   this->color_2= Point(frame.cols/2,frame.rows/2-50);
   this->color_3= Point(frame.cols/2-50,frame.rows/2-50);
}

void MainWindow::get3points(Mat * frameMedian)
{
    /* circle  both point where the user can put his two color for detection */
    circle(*frameMedian, color_1, 10, Scalar(0,0,100), 7,8, 0);
    circle(*frameMedian, color_2, 10, Scalar(100,0,0), 7,8, 0);
    circle(*frameMedian, color_3, 10, Scalar(0,0,0), 7,8, 0);

}

void MainWindow::on_checkBox_stateChanged()
{
  if( ui->checkBox->isChecked())
  {
      filter=0;
      ui->checkBox_2->setChecked(false);

  }
  else
      filter=-1;

}
void MainWindow::on_checkBox_2_stateChanged()
{
    if( ui->checkBox_2->isChecked())
    {
        filter=1;
        ui->checkBox->setChecked(false);
    }
    else
        filter=-1;


}

void MainWindow::on_pushButton_clicked()
{
         ui->label_5->setText( QString::number(this->frame.at<Vec3b>(color_1)[0]));
         ui->label_6->setText( QString::number(this->frame.at<Vec3b>(color_1)[1]));
         ui->label_7->setText( QString::number(this->frame.at<Vec3b>(color_1)[2]));
         ui->label->setText( QString::number(this->frame.at<Vec3b>(color_2)[0]));
         ui->label_9->setText( QString::number(this->frame.at<Vec3b>(color_2)[1]));
         ui->label_10->setText( QString::number(this->frame.at<Vec3b>(color_2)[2]));
         /* convertir la frame en hsv pour avoir les valeurs de la couleur en hue saturation and vibration*/

         Mat frame_hsv;

               /* transformation to hsv HUE*/
         cvtColor(frame,frame_hsv,CV_RGB2HSV);



                 /*based on hue detection H S V*/

                 color1=Scalar(frame_hsv.at<Vec3b>(color_1)[0],frame_hsv.at<Vec3b>(color_1)[1],frame_hsv.at<Vec3b>(color_1)[2]);
                 color2=Scalar(frame_hsv.at<Vec3b>(color_2)[0],frame_hsv.at<Vec3b>(color_2)[1],frame_hsv.at<Vec3b>(color_2)[2]);


}

void MainWindow::on_pushButton_2_clicked()
{    Mat frame_hsv;

     /* transformation to hsv HUE*/
     ui->eraseh->setText( QString::number(this->frame.at<Vec3b>(color_3)[0]));
     ui->erases->setText( QString::number(this->frame.at<Vec3b>(color_3)[1]));
     ui->erasev->setText( QString::number(this->frame.at<Vec3b>(color_3)[2]));
      cvtColor(frame,frame_hsv,CV_RGB2HSV);
      color3=Scalar(frame_hsv.at<Vec3b>(color_3)[0],frame_hsv.at<Vec3b>(color_3)[1],frame_hsv.at<Vec3b>(color_3)[2]);
}

void MainWindow::on_action70_triggered()
{
    this->seuil=70;
}

void MainWindow::on_action50_triggered()
{
    this->seuil=50;
}

void MainWindow::on_action30_triggered()
{
    this->seuil=40;
}

void MainWindow::on_action10_triggered()
{
    this->seuil=10;
}


void MainWindow::on_pushButton_open_webcam_clicked()
{   frame_writer= Mat(w,h,CV_8UC3);


     viderframe(&frame_writer,Scalar(255,255,255));
     cap.open(0);

    if(!cap.isOpened())  // Check if we succeeded
    {
        cout << "camera is not open" << endl;
    }
    else
    {
        cout << "camera is open" << endl;

        connect(timer, SIGNAL(timeout()), this, SLOT(update_window()));
        timer->start(90);
    }
}



void MainWindow::update_window()
{
    //printf("filter %d",filter);
    cap >> frame;
    //printf("blue");
    iframetraite++;

        cvtColor(frame, frame, CV_BGR2RGB);



    if(ui->label_2->size().height() >0 ){

        h=ui->label_2->size().height();
    }


    if(ui->label_2->size().width()-w >10 || ui->label_2->size().width()-w <-10){
        w=ui->label_2->size().width();
    }




    cv::resize(frame, frame, Size(w, h), 0, 0, INTER_CUBIC);
    cv::resize(frame_writer, frame_writer, Size(w,h), 0, 0, INTER_CUBIC);

    this->set3points();

    choose_filter(frame,&frame);

    circleColoredarea2pointbased(&framehsv,&frame,&this->frame_writer,&this->drawfirst,&this->drawsecond,&this->draw_erase1,&this->draw_erase2,this->drawingcolor,this->seuil);

    this->get3points(&frame);
    /* le fameux tableau transparent*/
            frameIcon=this->frame_writer.clone();

            if(seuildrawing!=0)
            this->icon((&frameIcon),&pencil,Point(frame.rows-this->drawfirst.x,this->drawfirst.y),Point(frame.rows-this->drawsecond.x,this->drawsecond.y));
           else /* check the eraser*/
            if(seuilerase!=0 )
             this->icon((&frameIcon),&eraser,Point(frame.rows-this->draw_erase1.x,this->draw_erase1.y),Point(frame.rows-this->draw_erase2.x,this->draw_erase2.y));


  /*image drawer: drawing picture setting*/
     drawer = QImage((const unsigned char*) (frameIcon.data), frameIcon.cols, frameIcon.rows, QImage::Format_RGB888);
    ui->drawing->setPixmap(QPixmap::fromImage(drawer));
    ui->drawing->resize(ui->drawing->pixmap()->size());

     Mat hassina=frame.clone();

     for (int i=0; i<frame.rows; i++)
         for ( int j=0;j<frame.cols;j++)
         {
             hassina.at<Vec3b>(i,j)=frame.at<Vec3b>(i,frame.cols-j-1) ;
         }
     frame=hassina;

      qt_image = QImage((const unsigned char*) (frame.data), frame.cols, frame.rows, QImage::Format_RGB888);

      ui->label_2->setPixmap(QPixmap::fromImage(qt_image));

      ui->label_2->resize(ui->label_2->pixmap()->size());}

void MainWindow::on_Blue_stateChanged(int arg1)
{
    if( ui->Blue->isChecked()){
             this->drawingcolor=Scalar(0,0,255);
             ui->Red->setChecked(false);
    }
}

void MainWindow::on_Red_stateChanged(int arg1)
{
    if( ui->Red->isChecked()){
        this->drawingcolor=Scalar(255,0,0);
        ui->Blue->setChecked(false);
    }
}
