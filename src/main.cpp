#include <QApplication>
#include <QGLWidget>
#include <QDebug>
#include <QMouseEvent>
//#include <QWheelEvent>
#include <QTimer>
#include <cmath>
#include <GL/glu.h>
#include <iostream>
#include <chrono>
#include <Eigen/Dense>

using Eigen::Vector3d;
using Eigen::Matrix4d;

#define PI 3.14159265

class GLWidget : public QGLWidget{
    void initializeGL(){

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        last_update_ = std::chrono::steady_clock::now();
        update_timer_ = new QTimer(this);
        connect(update_timer_, SIGNAL(timeout()), this, SLOT(update()));
        update_timer_->start(0.017);
        is_dragging_ = false;
        last_dragging_x_ = 0.0;
        last_dragging_y_ = 0.0;
        trans_ << 1.0, 0.0, 0.0, 0.0,
                 0.0, 1.0, 0.0, 0.0,
                 0.0, 0.0, 1.0, 0.0,
                 0.0, 0.0, 0.0, 1.0;
    }

/// @note camera decides renderer size
    void resizeGL(int width, int height){
        if (height==0) height=1;
        glViewport(0,0,width,height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
     }

//void updateAngles(){
//    std::chrono::steady_clock::time_point current_time;
//    current_time = std::chrono::steady_clock::now();
//    double ellapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(current_time - last_update_).count();
//    //angle_z_ += 0.00005 * ellapsed_time;
//    //angle_y_ += 0.00005 * ellapsed_time;
//    last_update_ = current_time;
//}


    void mouseMoveEvent(QMouseEvent *event){

        if (event->buttons() & Qt::LeftButton) {
           if(is_dragging_) {
               trans_tmp_ << 1.0, 0.0, 0.0, (event->x() - last_dragging_x_) * 0.005,
                             0.0, 1.0, 0.0, -(event->y() - last_dragging_y_) * 0.005,
                             0.0, 0.0, 1.0, 0.0,
                             0.0, 0.0, 0.0, 1.0;
               trans_ = trans_tmp_ * trans_;

           }
           else{
                is_dragging_ = true;

           }
        }
        else if (event->buttons() & Qt::RightButton) {
           if(is_dragging_) {
               double tmp_rot_y_ = (event->x() - last_dragging_x_) * 0.003;
               double tmp_rot_x_ = (event->y() - last_dragging_y_) * 0.003;
               trans_tmp_ << cos(tmp_rot_y_),  0.0, sin(tmp_rot_y_), 0.0,
                             0.0            ,  1.0,             0.0, 0.0,
                             -sin(tmp_rot_y_), 0.0, cos(tmp_rot_y_), 0.0,
                             0.0             , 0.0,             0.0, 1.0;
               trans_ = trans_tmp_ * trans_;
               trans_tmp_ << 1.0,             0.0,              0.0, 0.0,
                             0.0, cos(tmp_rot_x_), -sin(tmp_rot_x_), 0.0,
                             0.0, sin(tmp_rot_x_),  cos(tmp_rot_x_), 0.0,
                             0.0,             0.0,              0.0, 1.0;
               trans_ = trans_tmp_ * trans_;
           }
           else{
                is_dragging_ = true;

           }

        }
        last_dragging_x_ = event->x();
        last_dragging_y_ = event->y();

    }

    void mouseReleaseEvent(QMouseEvent* event){
            is_dragging_ = false;
    }

    void wheelEvent(QWheelEvent *event){
        double wheel_position = event->delta();

        if(std::abs(wheel_position) > 700){return;}
        trans_tmp_ << 1.0, 0.0, 0.0, 0.0,
                     0.0, 1.0, 0.0, 0.0,
                     0.0, 0.0, 1.0, wheel_position*0.0008,
                     0.0, 0.0, 0.0, 1.0;
        trans_ = trans_tmp_ * trans_;
    }
    void paintGrid(){
            glLineWidth(5.0);
            glBegin(GL_LINES);
            glColor3ub(220,220,220);
        for(int i=-100; i<100; i=i+5){
            glVertex3d(i, 100.0, 0.0);
            glVertex3d(i, -100.0, 0.0);

            glVertex3d(100.0, i, 0.0);
            glVertex3d(-100.0, i, 0.0);
        }
            glEnd();

    }

    Vector3d MToEulerAngles(const Matrix4d& m){

        double sy = sqrt(m(0, 0) * m(0, 0) +  m(1,0) * m(1,0));

        bool singular = sy < 1e-6; // If

        double x, y, z;
        if (!singular)
        {
            x = atan2(m(2,1) , m(2,2));
            y = atan2(-m(2,0), sy);
            z = atan2(m(1,0), m(0,0));
        }
        else
        {
            x = atan2(-m(1,2), m(1,1));
            y = atan2(-m(2,0), sy);
            z = 0;
        }
        return Vector3d(x, y, z);
    }

    void paintGL(){
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity( );

        gluLookAt(0, 0, 10,0,0,0,0,1,0);

        glTranslated(trans_(0,3), trans_(1,3), trans_(2,3));
        Vector3d xyz = MToEulerAngles(trans_);
        glRotated(xyz(2)* 180 / PI,0,0,1);
        glRotated(xyz(1)* 180 / PI,0,1,0);
        glRotated(xyz(0)* 180 / PI,1,0,0);

        paintGrid();

        glBegin(GL_QUADS);

        glColor3ub(255,0,0); // Red

        glVertex3d(1,1,1);

        glVertex3d(1,1,-1);
        glVertex3d(-1,1,-1);
        glVertex3d(-1,1,1);

        glColor3ub(0,255,0); // Green
        glVertex3d(1,-1,1);
        glVertex3d(1,-1,-1);
        glVertex3d(1,1,-1);
        glVertex3d(1,1,1);

        glColor3ub(0,0,255); // Blue
        glVertex3d(-1,-1,1);
        glVertex3d(-1,-1,-1);
        glVertex3d(1,-1,-1);
        glVertex3d(1,-1,1);

        glColor3ub(255,255,0); // Yellow
        glVertex3d(-1,1,1);
        glVertex3d(-1,1,-1);
        glVertex3d(-1,-1,-1);
        glVertex3d(-1,-1,1);

        glColor3ub(0,255,255); // Cyan
        glVertex3d(1,1,-1);
        glVertex3d(1,-1,-1);
        glVertex3d(-1,-1,-1);
        glVertex3d(-1,1,-1);

        glColor3ub(255,0,255); // Magenta
        glVertex3d(1,-1,1);
        glVertex3d(1,1,1);
        glVertex3d(-1,1,1);
        glVertex3d(-1,-1,1);

        glEnd();

        glFlush();
    }
    private:
        std::chrono::steady_clock::time_point last_update_;
        QTimer* update_timer_;
        bool is_dragging_;
        double last_dragging_x_;
        double last_dragging_y_;
        Matrix4d trans_tmp_;
        Matrix4d trans_;
};

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    GLWidget widget;
    widget.resize(800,600);
    widget.show();
    return app.exec();
}

