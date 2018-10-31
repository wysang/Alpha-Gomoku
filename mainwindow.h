#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    int MARGIN = 70;	//边距
    int GRID_SPAN = 40;//网格间距
    int ROWS = 15;
    int COLS = 15;
    int chessBoard[16][16];//棋盘。2为空，0黑，1白
    int chessRem[225][2];//记录第几次下棋的坐标
    int chessCount = 0;//计算下棋数，检查最后一颗棋子
    int xIndex,yIndex;//网格坐标
    bool isWin(int x,int y);//输赢
    bool isBlack;
    bool gameover = false;
    bool aiplay;//轮到电脑下棋
    bool hc;//人机
    bool hh;//人人
    int sbw = 0;//玩家棋色
    int bw = 0;//当前应下棋色
    int deep = 3;//搜索深度和广度
    int width  = 7;
    int x_min =0,y_min=0;
    int x_max = 15,y_max=15;
    bool able_flag;//有无禁手
    void goback();

    void clearChess();
    void DrawBackground();
    void DrawChess(QPainter& p);
    void DrawBoard(QPainter& p);
    void paintEvent(QPaintEvent *);//绘制棋盘
    void mousePressEvent(QMouseEvent *ev); // 鼠标点击象棋并释放鼠标时候触发
    void click(QPoint pt);
    bool computerPlaceChess();

    void jinshou(int x,int y);
    bool isjinshou = false;

    int nThree;//活三个数
    int flagthree[15][15];//标记活三点
    int flag[15];

    void count(int x, int y,int ex, int ey,int bwf,int& returnx,int& returny);
    bool makesense(int x, int y,int ex, int ey,int bwf);
    int getType(int x, int y,int bwf);
    int getMark(int k);
    int evaluate();
    int findMax(int alpha, int beta, int step);
    int findMin(int alpha, int beta, int step);
    void resetMaxMin(int x, int y);
    void putOne(int &x,int &y,int bwf );

private slots:
    void on_reset_action_triggered();
    void on_goback_action_triggered();
    void on_jinshou_action_triggered();
    void on_exit_action_triggered();
    void on_hc_action_triggered();
    void on_hh_action_2_triggered();
    void on_rule_action_triggered();
    void on_about_action_triggered();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
