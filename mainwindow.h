#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtCore>
#include<deque>
using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
    void Mouse_Pressed();
    void showMousePosition(QPoint& pos);
    void fillColor(QPoint& p,QColor &c);
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int mat[3];
    float fmat[3];
    int transformx,transformy;
    float angle;
    int GridSize;
    int radius;
    int a,b;    //major and minor axis of ellipse
    int vertices;
    deque<QPoint> PolygonPoints,Edges;
    bool visited[1000][1000];
    int del;

private slots:
    void on_show_axes_clicked();
    void on_Draw_clicked();
    void on_set_point1_clicked();
    void on_set_point2_clicked();
    void on_grid_clicked();
    void on_gridsizespinBox_valueChanged(int arg1);
    void on_clearGrid_clicked();
    void drawCircle(int xc,int yc, int x1,int y1);
    void draw_m_ellipse(int centerx,int centery,int x1,int y1);
    void draw_p_ellipse(int centerx,int centery,int x1,int y1);    

    //void on_DDAButton_clicked();

    void on_setVertexButton_clicked();
    void on_vertices_valueChanged(int arg1);
    void on_setSeedButton_clicked();
    void on_x_change_valueChanged(int arg1);
    void on_y_change_valueChanged(int arg1);
    void on_rotate_valueChanged(double arg1);



private:
    Ui::MainWindow *ui;
    QPoint p1,p2,p3,p4,seed;
    QImage img;
    void point(int,int,int);
    void delay();
    void dda();
    void bresenham();
    void polar_circle();
    void midpoint_circle();
    void bresenham_circle();
    void polar_ellipse();
    void midpoint_ellipse();
    void drawPolygon();
    void scanLine();
    bool present(int x,int y);
    void boundaryFill(int x,int y);
    void MatrixMultiplication(int a[3][3],int b[3]);
    void MatrixMultiplication(float a[3][3],float b[3]);
    void translation();
    void rotate();
    void scale();
    void reflectx();
    void reflecty();
};

#endif // MAINWINDOW_H
