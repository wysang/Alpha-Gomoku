#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include<QMessageBox>
#include<QMouseEvent>
#include<QString>
#include<QPainter>
#include<stdlib.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    clearChess();
    setMinimumSize(710,700);
    update();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clearChess()
{


     for(int i = 0; i<=15;i++)
     {
         for(int j = 0; j<=15;j++)
         {
             chessBoard[i][j] =2;
         }
     }
     for(int i = 0;i<255;i++)
     {

         chessRem[i][0] = -1;
         chessRem[i][1] = -1;
     }

       isBlack = true;
       chessCount = 0;
       gameover = false;
       aiplay = false;
       bw = 0;
       sbw = 0;
       able_flag = true;
       x_min =0;y_min=0;
       x_max = 15;y_max=15;
       hh = false;
       hc = true;
       isjinshou = false;
 }

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    DrawBackground();//绘制背景
    DrawBoard(p);
    if(chessCount!=0)
    {
        DrawChess(p);
    }

}

void MainWindow::DrawBackground()
{
    QPalette p = this->palette();
    p.setColor(QPalette::Window, QColor(253, 182, 88));
    this->setPalette(p);
}

void MainWindow::DrawBoard(QPainter& p)
{
    //棋盘网格线
    for(int i = 0; i <COLS; i++){

        if(i == 0 || i == 14)
        { // 上下边框画笔设置的粗一些

           p.setPen(QPen(Qt::black, 3, Qt::SolidLine));
        }
        else
        {
            p.setPen(QPen(Qt::black, 1, Qt::SolidLine));
         }
            p.drawLine(MARGIN, MARGIN+i*GRID_SPAN, MARGIN+(COLS-1)*GRID_SPAN, MARGIN+i*GRID_SPAN);
            p.drawLine(MARGIN+i*GRID_SPAN, MARGIN, MARGIN+i*GRID_SPAN, MARGIN+(COLS-1)*GRID_SPAN);
        }
        p.setBrush(Qt::black);
        p.drawRect(40*7+70-4,40*7+70-4,8,8);//画5个黑方块
        p.drawRect(40*3+70-4,40*3+70-4,8,8);
        p.drawRect(40*11+70-4,40*3+70-4,8,8);
        p.drawRect(40*3+70-4,40*11+70-4,8,8);
        p.drawRect(40*11+70-4,40*11+70-4,8,8);
}

void MainWindow::DrawChess(QPainter &p)
{
    //绘制棋子
    int xPos1=0;
    int yPos1=0;
    for(int i = 0; i<ROWS; i++)
    {
        for(int j = 0; j<COLS;j++)
        {
            if(chessBoard[i][j] == 0){
                 xPos1 =i*GRID_SPAN+MARGIN;
                 yPos1 =j*GRID_SPAN+MARGIN;
                p.setBrush(Qt::black);
                p.setPen(QPen(QBrush(Qt::black), 2));
                p.drawEllipse(xPos1-15, yPos1-15, 34, 35);
            }
            else if(chessBoard[i][j] == 1)
            {
                 xPos1 =i*GRID_SPAN+MARGIN;
                 yPos1 =j*GRID_SPAN+MARGIN;
                p.setBrush(Qt::white);
                p.setPen(QPen(QBrush(Qt::white), 2));
                p.drawEllipse(xPos1-15, yPos1-15, 34, 35);
            }
        }

    }
            xPos1 =xIndex*GRID_SPAN+MARGIN;
            yPos1 =yIndex*GRID_SPAN+MARGIN;
            p.setBrush(Qt::NoBrush);
            p.setPen(QPen(QBrush(Qt::red), 2));
            p.drawRect(xPos1-15, yPos1-15, 34, 35);

}

void MainWindow::mousePressEvent(QMouseEvent *ev)
{


    if (ev->button() != Qt::LeftButton) { // 排除鼠标右键点击
        return;
    }
    click(ev->pos());
    qDebug() <<"mouse event";
}

bool MainWindow::isWin(int x, int y)
{
    int dx[8]={0,1,1,1,0,-1,-1,-1};
    int dy[8]={1,1,0,-1,-1,-1,0,1};

    int currentSide=chessBoard[x][y];

    int cnt,r,c,i;
    for(i=0;i<4;i++){
        int dir=i;
        cnt=0;
        r=x;
        c=y;
        while(chessBoard[r][c]==currentSide && r>=0 && r<15 && c>=0 && c<15){

                cnt++;
                r+=dy[dir];
                c+=dx[dir];


        }
        //qDebug() << "---" ;
        //qDebug() << cnt ;
        dir+=4;
        r=x;
        c=y;
        while(chessBoard[r][c]==currentSide && r>=0 && r<15 && c>=0 && c<15){
                cnt++;
                r+=dy[dir];
                c+=dx[dir];
        }
        if(cnt>5){

            return true;
        }


        //qDebug() << "---" ;
    }
    return false;
}
void MainWindow::click(QPoint pt)
{


            //网格坐标位置
            xIndex=(pt.x()-70+40/2)/40;
            yIndex=(pt.y()-70+40/2)/40;
            if(gameover) return;

            //落在棋盘外不能下
            if(xIndex<0||xIndex>ROWS-1||yIndex<0||yIndex>COLS-1) return;
            //如果x，y位置已经有棋子存在，不能下
            if(chessBoard[xIndex][yIndex] !=2 )return;

            //可以下棋
            chessCount++;
            chessRem[chessCount][0] = xIndex;
            chessRem[chessCount][1] = yIndex;            
            jinshou(xIndex,yIndex);
            if(bw==0)
            {
                chessBoard[xIndex][yIndex] = 0;
            }
            else
            {
                chessBoard[xIndex][yIndex] = 1;
            }
            update();
            if(!isjinshou) {
            //update();
            //qDebug() << "hc:" << hc << "  hh: " << hh;
            if( chessCount == 1){
              if(xIndex-1>=0)
              x_min = xIndex-1;
              if(xIndex-1<=15)
              x_max = xIndex+1;
              if(yIndex-1>=0)
              y_min = yIndex-1;
              if(yIndex-1<=15)
              y_max = yIndex+1;
            }
            else
              {resetMaxMin(xIndex,yIndex);}

            if(hc)
            {
                //jinshou(xIndex,yIndex);
                if(isWin(xIndex,yIndex))
                {

                    QString colorName=isBlack?"黑棋":"白棋";
                    QMessageBox message(QMessageBox::NoIcon, " ",colorName+"胜");
                    message.exec();
                    gameover=true;
                    clearChess();
                    update();

                     return;
                 }



                isBlack = !isBlack;
                aiplay = !aiplay;
                bw = 1-bw;

                if(aiplay)
                {
                    if(!computerPlaceChess()) return;

                }

            }
            if(hh)
            {
                if(isWin(xIndex,yIndex))
                {

                    QString colorName=isBlack?"黑棋":"白棋";
                    QMessageBox message(QMessageBox::NoIcon, " ",colorName+"胜");
                    message.exec();
                    gameover=true;
                    clearChess();

                     return;
                 }
                bw = 1-bw;
                isBlack = !isBlack;
            }
            }
            else{
                QMessageBox message(QMessageBox::NoIcon, "禁手 ","白棋胜");
                message.exec();
                gameover=true;
                clearChess();
                update();
                 return;
            }

}

bool MainWindow::computerPlaceChess()
{
    int row,col;
    //AI(col,row);
    //processResponse(row,col);
    //resetMaxMin(xIndex,yIndex);
    putOne(row,col,bw );
    xIndex = row;
    yIndex = col;
    resetMaxMin(xIndex,yIndex);
    qDebug() << "AI :"<< row<<col;
    if(gameover) return true;

    //落在棋盘外不能下
    if(row<0||row>ROWS-1||col<0||col>COLS-1) return true;
    //如果x，y位置已经有棋子存在，不能下
    if(chessBoard[row][col] !=2 )return true;

    //可以下棋
    chessCount++;
    chessRem[chessCount][0] = xIndex;
    chessRem[chessCount][1] = yIndex;
    jinshou(row,col);
    if(!isjinshou)
    {
        if(bw==0)
        {
            chessBoard[row][col] = 0;
        }
        else
        {
            chessBoard[row][col] = 1;
        }

        update();


        if(isWin(row,col))
        {
            QString colorName=isBlack?"黑棋":"白棋";
            QMessageBox message(QMessageBox::NoIcon, " ",colorName+"胜");
            message.exec();
            gameover=true;
            clearChess();
            update();
            return false;
         }

      isBlack = !isBlack;
      aiplay = !aiplay;
      bw = 1-bw;


    }
    else{
        QMessageBox message(QMessageBox::NoIcon, "禁手 ","白棋胜");
        message.exec();
        gameover=true;
        clearChess();
        update();
        return false;
    }


    return true;
}

void MainWindow::on_reset_action_triggered()
{
    qDebug()<<"重新开始";
    clearChess();
    update();
}
void MainWindow::on_goback_action_triggered()
{
     qDebug()<<"悔棋";
     goback();
}

void MainWindow::on_exit_action_triggered()
{
     qDebug()<<"退出";
     this->close();
}
void MainWindow::on_hc_action_triggered()
{
    qDebug()<<"人机博弈";


    QMessageBox message(QMessageBox::Information, "先手选择","您的选择：");
    message.setStandardButtons(QMessageBox::Yes| QMessageBox::No);
    message.setDefaultButton(QMessageBox::Yes);
    message.setButtonText (QMessageBox::Yes,QString("玩家先手"));
    message.setButtonText (QMessageBox::No,QString("机器先手"));
    int ret = message.exec();
    switch (ret) {
    case QMessageBox::Yes:
        qDebug() << "玩家先手";
        clearChess();
        aiplay = false;
        sbw = 0;
        update();
        break;
    case QMessageBox::No:
        qDebug() << "机器先手";
        clearChess();
        aiplay = false;
        xIndex=7;
        yIndex=7;
        chessRem[chessCount][0] = xIndex;
        chessRem[chessCount][1] = yIndex;
        chessCount++;
        chessBoard[xIndex][xIndex] = 0;
        if( chessCount == 1){
          if(xIndex-1>=0)
          x_min = xIndex-1;
          if(xIndex-1<=15)
          x_max = xIndex+1;
          if(yIndex-1>=0)
          y_min = yIndex-1;
          if(yIndex-1<=15)
          y_max = yIndex+1;
        }
        else
          {resetMaxMin(xIndex,yIndex);}

        update();
        isBlack = !isBlack;
        aiplay = false;
        bw = 1-bw;
        sbw = 1;
        break;
    }
    hc = true;
    hh = false;
//    qDebug()<<sbw;
}

void MainWindow::on_rule_action_triggered()
{
    qDebug()<<"规则";
    QString information = "1、无禁手：";
    information.append("\n");
    information.append("   黑白双方依次落子，任一方先在棋盘上形成连续的五个(含五个以上)棋子的一方为胜。");
    information.append("\n");
    information.append("2、有禁手：（走禁手就输，禁手不能落子）");
    information.append("\n");
    information.append("   鉴于无禁手规则黑棋必胜，人们不断采用一些方法限制黑棋先行的优势，以平衡黑白双方的形式。");
    information.append("\n");
    information.append( "   于是针对黑棋的各种禁手逐渐形成。");
    information.append("\n");
    information.append("   禁手主要分为以下几类：");
    information.append("\n");
    information.append("   (1)黑长连禁手：连成六个以上连续相同的棋子。");
    information.append("\n");
    information.append("   (2)黑三三禁手：两个以上的活三。");
    information.append("\n");
    information.append("   (3)黑四四禁手：两个以上的四。");
    information.append("\n");
    information.append("   禁手是针对黑棋而言的，白棋没有任何禁手。");
    QMessageBox message(QMessageBox::Information, "规则 ","游戏规则");
    message.setInformativeText(information);
    message.exec();
}
void MainWindow::on_about_action_triggered()
{
    qDebug()<<"关于";
    QString information;
    information.append("蓝川媚 3014216062");
    information.append("\n");
    information.append("张    悦 3014216055");
    information.append("\n");
    information.append("万雨桑 3014216045");
    information.append("\n");
    information.append("2016.11");
    QMessageBox message(QMessageBox::Information, "关于 ","制作人员：");
    message.setInformativeText(information);
    message.exec();
}
void MainWindow::on_jinshou_action_triggered()
{
    QMessageBox message(QMessageBox::Information, "禁手选择","您的选择：");
    message.setStandardButtons(QMessageBox::Yes| QMessageBox::No);
    message.setDefaultButton(QMessageBox::Yes);
    message.setButtonText (QMessageBox::Yes,QString("有禁手"));
    message.setButtonText (QMessageBox::No,QString("无禁手"));
    int ret = message.exec();
    switch (ret) {
    case QMessageBox::Yes:
        clearChess();
        able_flag = true;
        update();
        break;
    case QMessageBox::No:
        clearChess();
        able_flag = false;
        update();
        break;
    }
}
void MainWindow::on_hh_action_2_triggered(){
    clearChess();
    hh = true;
    hc = false;
    update();
    //qDebug() << "hc:" << hc << "  hh: " << hh;

}
void MainWindow::goback()
{
    //每次人下完，机器立即下，回退两步,还是轮到人下~~isBalck不变
    if(chessCount !=0){
        chessBoard[chessRem[chessCount][0]][chessRem[chessCount][1]] = 2;
        chessCount--;
        chessBoard[chessRem[chessCount][0]][chessRem[chessCount][1]] = 2;
        chessCount--;
        xIndex = chessRem[chessCount-1][0];yIndex = chessRem[chessCount-1][1];
        update();
    }


}


int compar( const void* a, const void* b )
{
    // return *(const int*)b - *(const int*)a; 一开始考虑不周，这一步可能导致数值溢出
    if( ((int*)b)[2] > ((int*)a)[2] ) return +1;
    if( ((int*)b)[2] < ((int*)a)[2] ) return -1;
    return 0;
}


   //----------------------------计算指定方位上的棋型-------------------//
   // x,y 方向线基准一点。
   //ex,ey 指定方向步进向量。
   // k 棋子颜色，0：黑色，1：白色
   // 该方向上的棋子数目 以及 活度

void MainWindow::count(int x, int y, int ex, int ey, int bwf,int& returnx, int& returny) {

    // 该方向没意义,返回0
     if( !makesense(x, y, ex, ey, bwf))//makesense()为判断是否大于5
     {
        returnx = 0;
        returny = 1;
        return;
     }

     // 正方向 以及 反方向棋子个数
   int rt_1 = 1,rt_2 = 1;
   // 总棋子个数
   int rt = 1;
   // 正方向 以及 反方向连子的活度
     int ok_1 = 0,ok_2 =0;
     // 总活度
     int ok = 0;
     // 连子中间有无空格
     bool flag_mid1 =false,flag_mid2 = false;
     // 连子中间空格的位置
     int flag_i1 = 1,flag_i2 = 1;

     if (chessBoard[x][y] != 2) {
        qDebug() << "position x,y must be empty!..";
     }
     int i;
     // 往正方向搜索
     for (i = 1; x + i * ex < 15 && x + i * ex >= 0 && y + i * ey < 15 && y + i * ey >= 0; i++) {
         if (chessBoard[x + i * ex][y + i * ey] == bwf)
             rt_1++;
       // 位置为空,若中空标志为false,则记为中空并继续搜索  否则,break
         else if(chessBoard[x + i * ex][y + i * ey] == 2) {
               if(!flag_mid1) {
                   flag_mid1 = true;
                   flag_i1 = i;
               }
               else
                   break;
           }
         // 位置为对方棋子
         else
           break;
     }
     // 计算正方向活度,,
     // 最后一个位置不超过边界
     if (x + i * ex < 15 && x + i * ex >= 0 && y + i * ey < 15 && y + i * ey >= 0) {
       // 最后一个位置为空位 +1活
       if( chessBoard[x + i * ex][y + i * ey] == 2) {
           ok_1++;
           // 若是在尾部检测到连续的空格而退出搜索,则不算有中空
             if(rt_1 == flag_i1)
               flag_mid1 = false;
             // 若中空的位置在4以下 且 棋子数>=4,则这一边的4非活
             if(flag_mid1 && rt_1 > 3 && flag_i1 < 4) {
               ok_1--;
             }
       }
       // 最后一个位置不是空格,且搜索了2步以上,若前一个是空格,  则不算中空,且为活的边
       else if( chessBoard[x + i * ex][y + i * ey] != bwf && i >= 2)
           if(chessBoard[x + (i-1) * ex][y + (i-1) * ey] == 2) {
               ok_1++;
               flag_mid1 = false;
           }
     }
     // 最后一个位置是边界  搜索了2步以上,且前一个是空格,  则不算中空,且为活的边
     else if(i >= 2 && chessBoard[x + (i-1) * ex][y + (i-1) * ey] == 2) {
       ok_1++;
       flag_mid1 = false;
     }

     // 往反方向搜索
     for (i = 1; x - i * ex >= 0 && x - i * ex < 15 && y - i * ey >= 0 && y - i * ey < 15; i++) {
         if (chessBoard[x - i * ex][y - i * ey] == bwf)
             rt_2++;
         else if(chessBoard[x - i * ex][y - i * ey] == 2) {
               if(!flag_mid2) {
                   flag_mid2 = true;
                   flag_i2 = i;
               }
               else
                   break;
           }
         else
             break;
     }
     // 计算反方向活度
     if (x - i * ex < 15 && x - i * ex >= 0 && y - i * ey < 15 && y - i * ey >= 0) {
       if( chessBoard[x - i * ex][y - i * ey] == 2) {
           ok_2++;
           if(rt_2 == flag_i2)
               flag_mid2 = false;
           if(flag_mid2 && rt_2 > 3 && flag_i2 < 4) {
               ok_2--;
             }
       }
       else if( chessBoard[x - i * ex][y - i * ey] != bwf && i >= 2 )
           if(chessBoard[x - (i-1) * ex][y - (i-1) * ey] == 2) {
               ok_2++;
               flag_mid2 = false;
           }
     }
     else if(i >= 2 && chessBoard[x - (i-1) * ex][y - (i-1) * ey] == 2) {
       ok_2++;
       flag_mid2 = false;
     }

     //------------------分析棋子类型
     // 两边都没中空,直接合成
     if( !flag_mid1 && !flag_mid2 ) {
       rt = rt_1 + rt_2 - 1;
       ok = ok_1 + ok_2;
       returnx = rt;
       returny = ok;
       return;
     }
     // 两边都有中空
     else if( flag_mid1 && flag_mid2 ){
       int temp = flag_i1 + flag_i2 - 1;
       // 判断中间的纯连子数,在5以上,直接返回;  为4,返回活4;
       if(temp >= 5)
       {
           returnx = temp;
           returny = 2;
           return;
       }
       if(temp == 4)
       {
           returnx = temp;
           returny = 2;
           return;
       }
       // 先看有没死4,再看有没活3,剩下只能是死3
       if(rt_1 + flag_i2 - 1 >= 4 || rt_2 + flag_i1 - 1 >= 4)
       {
           returnx = 4;
           returny = 1;
           return;
       }
       if((rt_1+flag_i2-1 == 3 && ok_1 > 0 )|| (rt_2+flag_i1-1 == 3 && ok_2 > 0))
       {
           returnx = 3;
           returny = 2;
           return;
       }
       returnx = 3;
       returny = 1;
       return ;
     }
     // 有一边有中空
     else {
       // 总棋子数少于5,直接合成
       if( rt_1 + rt_2 - 1 < 5 )
       {
           returnx = rt_1 + rt_2 - 1;
           returny = ok_1 + ok_2;
           return ;
       }

       // 多于5,先找成5,再找活4,剩下的只能是死4
       else {
           if(flag_mid1 && rt_2 + flag_i1 - 1 >= 5)
           {
               returnx = rt_2 + flag_i1 - 1;
               returny = ok_2 + 1;
               return ;
           }
           if(flag_mid2 && rt_1 + flag_i2 - 1 >= 5)
           {
               returnx = rt_1 + flag_i2 - 1;
               returnx = ok_1 + 1;
               return ;
           }

           if(flag_mid1 && ((rt_2 + flag_i1 - 1 == 4 && ok_2 == 1) || flag_i1 == 4) )
           {
               returnx=4;
               returny=2;
               return ;
           }
           if(flag_mid2 && ((rt_1 + flag_i2 - 1 == 4 && ok_1 == 1) || flag_i2 == 4) )
           {
               returnx=4;
               returny=2;
               return ;
           }
           returnx = 4;
           returny = 1;
           return;
       }
     }
 }

bool MainWindow::makesense(int x, int y, int ex, int ey, int bwf) {

      int rt = 1;
      for (int i = 1; x + i * ex < 15 && x + i * ex >= 0 && y + i * ey < 15 && y + i * ey >= 0 && rt < 5; i++)
          if (chessBoard[x + i * ex][y + i * ey] != 1 - bwf)
              rt++;
          else
              break;

      for (int i = 1; x - i * ex >= 0 && x - i * ex < 15 && y - i * ey >= 0 && y - i * ey < 15 && rt < 5; i++)
          if (chessBoard[x - i * ex][y - i * ey] != 1 - bwf)
              rt++;
          else
              break;
      return (rt >= 5);
 }


   //------------------------------------ 棋型判别-------------------------------------//
   // x,y 落子位置
   // bwf 棋色  0：黑子，1：白子
   // 对应的棋型： 棋型代码对应如下：
   //             1：成5
   //             2：成活4或者是双死4或者是死4活3
   //             3：成双活3
   //             4：成死3活3
   //             5：成死4
   //             6：单活3
   //             7：成双活2
  //             8：成死3
   //            9：成死2活2
   //            10：成活2
   //             11：成死2
   //             12: 其他
   //             20: 长连禁手
   //             21: 双四禁手
   //            22: 双活三禁手

int  MainWindow::getType(int x, int y, int bwf) {
    int cnt_returnx = -10;
    int cnt_returny = -10;

    if (chessBoard[x][y] != 2)
          return -1;
      int types[4][2];
      for(int i = 0;i<4;i++){
          types[i][0] = -100;
          types[i][i] = -100;
      }
      count(x, y, 0, 1, bwf, cnt_returnx, cnt_returny);// 竖直
      types[0][0] = cnt_returnx; types[0][1] = cnt_returny;
      count(x, y, 1, 0, bwf, cnt_returnx, cnt_returny);   // 横向
      types[1][0] = cnt_returnx; types[1][1] = cnt_returny;
      count(x, y, -1, 1, bwf, cnt_returnx, cnt_returny);	// 斜上
      types[2][0] = cnt_returnx; types[2][1] = cnt_returny;
      count(x, y, 1, 1, bwf, cnt_returnx, cnt_returny);   // 斜下
      types[3][0] = cnt_returnx; types[3][1] = cnt_returny;
      // 各种棋型的方向的数目
      int longfive = 0;
      int five_OR_more = 0;
      int four_died = 0, four_live = 0;
      int three_died = 0, three_live = 0;
      int two_died  = 0, two_live = 0;
      // 各方向上棋型的判别
      for (int k = 0; k < 4; k++) {
        if (types[k][0] > 5) {
            longfive++;              // 长连
            five_OR_more++;
        }
        else if (types[k][0] == 5)
            five_OR_more++;          // 成5
          else if (types[k][0] == 4 && types[k][1] == 2)
            four_live++;             // 活4
          else if (types[k][0] == 4 && types[k][1] != 2)
            four_died++;             // 死4
          else if (types[k][0] == 3 && types[k][1] == 2)
            three_live ++;           // 活3
          else if (types[k][0] == 3 && types[k][1] != 2)
            three_died++;            // 死3
          else if (types[k][0] == 2 && types[k][1] == 2)
            two_live++;              // 活2
          else if (types[k][0] == 2 && types[k][1] != 2)
            two_died++;              // 死2
          else{}

      }
      // 总棋型的判别
      if(bwf == 0 && able_flag) {  		// 黑棋且选择有禁手
        if (longfive != 0)        		// 长连禁手
            return 20;
        if (four_live + four_died >=2)  // 双4禁手
            return 21;
        if (three_live  >=2)        	// 双活三禁手
            return 22;
      }
      if (five_OR_more != 0)
          return 1;   // 成5
      if (four_live != 0 || four_died >= 2 || (four_died != 0 && three_live  != 0))
          return 2;   // 成活4或者是死4活双死4或者是3
      if (three_live  >= 2)
          return 3;   // 成双活3
      if (three_died != 0 && three_live  != 0)
          return 4;   // 成死3活3
      if (four_died != 0)
          return 5;   // 成死4
      if (three_live  != 0)
          return 6;   // 单活3
      if (two_live >= 2)
          return 7;   // 成双活2
      if (three_died != 0)
          return 8;   // 成死3
      if (two_live != 0 && two_died != 0)
          return 9;   // 成死2活2
      if (two_live != 0)
          return 10;  // 成活2
      if (two_died != 0)
          return 11;  // 成死2
      return 12;
  }

   //------------------------------------- 不同棋型对应分数---------------------------------
   // k 棋型代号
   //return 对应分数
int MainWindow::getMark(int k) {
      switch (k) {
      case 1:
          return 100000;
      case 2:
          return 30000;
      case 3:
          return 5000;
      case 4:
          return 1000;
      case 5:
          return 500;
      case 6:
          return 200;
      case 7:
          return 100;
      case 8:
          return 50;
      case 9:
          return 10;
      case 10:
          return 5;
      case 11:
          return 3;
      case 12:
          return 2;
      default:                     //禁手棋型
          return 0;
      }
  }

   //--------------------------对当前棋面进行打分------------------------------------------------------------//

int MainWindow::evaluate() {
    int rt = 0, mt_c = 1, mt_m = 1;
    if(bw == sbw)
        mt_m = 2;
    else
        mt_c = 2;
    int i_min=(x_min==0 ? x_min:x_min-1);
      int j_min=(y_min==0 ? y_min:y_min-1);
      int i_max=(x_max==15 ? x_max:x_max+1);
      int j_max=(y_max==15 ? y_max:y_max+1);
      for (int i = i_min; i < i_max; i++)
          for (int j = j_min; j < j_max; j++)
              if (chessBoard[i][j] == 2) {
                // 电脑棋面分数
                  int type = getType(i, j, 1 - sbw );
                  if(type == 1)      // 棋型1,棋型2以及棋型3,加权.  防止"4个双活3"的局分大于"1个双四"之类的错误出现
                     rt += 30 * mt_c * getMark(type);

                  else if(type == 2)
                    rt += 10 * mt_c * getMark(type);
                  else if(type == 3)
                    rt += 3 * mt_c * getMark(type);
                  else
                    rt += mt_c * getMark(type);
                  // 玩家棋面分数
                  type = getType(i, j, sbw );
                  if(type == 1)
                    rt -= 30 * mt_m * getMark(type);
                  else if(type == 2)
                    rt -= 10 * mt_m * getMark(type);
                  else if(type == 3)
                    rt -= 3 * mt_m * getMark(type);
                  else
                    rt -= mt_m * getMark(type);
              }
      return rt;
  }

  //---------搜索当前搜索状态极大值--------------------------------//
  //alpha 祖先节点得到的当前最小最大值，用于alpha 剪枝
  //beta  祖先节点得到的当前最大最小值，用于beta 剪枝。
  //step  还要搜索的步数
  //return 当前搜索子树极大值
int MainWindow::findMax(int alpha, int beta, int step) {
    int max = alpha;
      if (step == 0) {
          return evaluate();
      }
//-------------------------------------------------------//
      int i_min=(x_min==0 ? x_min:x_min-1);
      int j_min=(y_min==0 ? y_min:y_min-1);
      int i_max=(x_max==15 ? x_max:x_max+1);
      int j_max=(y_max==15 ? y_max:y_max+1);
      int n = 0;
      int type_1,type_2;
      int rt[(i_max-i_min) * (j_max-j_min)][3];
      for(int i = 0;i <(i_max-i_min) * (j_max-j_min);i++)
      {
          rt[i][0]=-1;
          rt[i][1]=-1;
          rt[i][2]=-1;
      }
      for ( int i = i_min; i < i_max; i++)
        for (int j = j_min; j < j_max; j++)
            if (chessBoard[i][j] == 2) {
                  type_1 = getType(i, j, 1 - sbw);
                  type_2 = getType(i, j,sbw);
                  if(able_flag && 1-sbw ==0 && (type_1 == 20 || type_1 == 21 || type_1 == 22)) // 禁手棋位置,不记录
                    continue;
                  rt[n][0] = i;
                  rt[n][1] = j;
                  rt[n][2] = getMark(type_1) + getMark(type_2);
                  n++;
      }
       qsort( rt, sizeof(rt)/sizeof(rt[0]), sizeof(rt[0]), &compar );
       int size = n<7 ? n:7;
//----------------------------------------------------------------------//


      for (int i = 0; i < size; i++) {
          int x = rt[i][0];
           int y = rt[i][1];
        if (getType(x, y, 1 - sbw) == 1)   //电脑可取胜
            return 100 * ( getMark(1) + step*1000 );
          chessBoard[x][y] = 1 - sbw;
          // 预存当前边界值
          int temp1=x_min,temp2=x_max,temp3=y_min,temp4=y_max;
          resetMaxMin(x,y);
          int t = findMin(max, beta, step - 1);
          chessBoard[x][y] = 2;
          // 还原预设边界值
          x_min=temp1;
          x_max=temp2;
          y_min=temp3;
          y_max=temp4;
          if (t > max)
            max = t;
          //beta 剪枝
          if (max >= beta)
              return max;
      }
      return max;
  }


   //-----------------------搜索当前搜索状态极小值---------------------------------//
   //alpha 祖先节点得到的当前最小最大值，用于alpha 剪枝
  //beta  祖先节点得到的当前最大最小值，用于beta 剪枝
  //step  还要搜索的步数
 //return 当前搜索子树极小值。
int MainWindow::findMin(int alpha, int beta, int step) {
    int min = beta;
      if (step == 0) {
          return evaluate();
      }

      //------------------------------------------------------------//
      int i_min=(x_min==0 ? x_min:x_min-1);
      int j_min=(y_min==0 ? y_min:y_min-1);
      int i_max=(x_max==15 ? x_max:x_max+1);
      int j_max=(y_max==15 ? y_max:y_max+1);
      int n1 = 0;
      int type_1,type_2;
      int rt[(i_max-i_min) * (j_max-j_min)][3];
      //qDebug()<< "xy" ;
      for(int i = 0;i <(i_max-i_min) * (j_max-j_min);i++)
      {
          rt[i][0]=-1;
          rt[i][1]=-1;
          rt[i][2]=-1;
      }
      for ( int i = i_min; i < i_max; i++)
        for (int j = j_min; j < j_max; j++)
            if (chessBoard[i][j] == 2) {
                  type_1 = getType(i, j, sbw);
                  type_2 = getType(i, j, 1 - sbw);
                  if(able_flag && sbw==0 && (type_1 == 20 || type_1 == 21 || type_1 == 22)) // 禁手棋位置,不记录
                    continue;
                  rt[n1][0] = i;
                  rt[n1][1] = j;
                  rt[n1][2] = getMark(type_1) + getMark(type_2);
                  n1++;

      }

      qsort( rt, sizeof(rt)/sizeof(rt[0]), sizeof(rt[0]), &compar );
       int size = n1<7 ? n1:7;

      for (int i = 0; i <size; i++) {
          int x = rt[i][0];
          int y = rt[i][1];
          int type1 = getType(x, y, sbw);
          if (type1 == 1)     					  			//玩家成5
           return -100 * ( getMark(1) + step*1000 );
          // 预存当前边界值
          int temp1=x_min,temp2=x_max,temp3=y_min,temp4=y_max;
          chessBoard[x][y] = sbw;
          resetMaxMin(x,y);
          int t = findMax( alpha, min, step - 1 );
          chessBoard[x][y] = 2;
          // 还原预设边界值
          x_min=temp1;
          x_max=temp2;
          y_min=temp3;
          y_max=temp4;
          if (t < min)
            min = t;
          //alpha 剪枝
          if (min <= alpha) {
              return min;
          }
      }
      return min;
  }

void MainWindow::resetMaxMin(int x,int y){
        if(x-1>=0)
        x_min = (x_min<x-1 ? x_min:x-1);
      if(x+1<=15)
        x_max = (x_max>x+1 ? x_max:x+1);
      if(y-1>=0)
        y_min = (y_min<y-1 ? y_min:y-1);
      if(y+1<=15)
        y_max = (y_max>y+1 ? y_max:y+1);

  }

void  MainWindow::putOne(int &x,int &y,int bwf ) {  //bwf 棋色 0:黑色 1：白色
      //int x, y100000000;
      //x = y = -1;
      bwf  = 1- sbw;
      int mx = -100000000;
      // 搜索扩展结点
//-----------------------------------------------------------//
      int i_min=(x_min==0 ? x_min:x_min-1);
      int j_min=(y_min==0 ? y_min:y_min-1);
      int i_max=(x_max==15 ? x_max:x_max+1);
      int j_max=(y_max==15 ? y_max:y_max+1);
      int n = 0;
      int type_1=0,type_2=0;
      int rt[(i_max-i_min) * (j_max-j_min)][3];
      for(int i = 0;i <(i_max-i_min) * (j_max-j_min);i++)
      {
          rt[i][0]=-1;
          rt[i][1]=-1;
          rt[i][2]=-1;
      }
      for ( int i = i_min; i < i_max; i++)
        for (int j = j_min; j < j_max; j++)
            if (chessBoard[i][j] == 2) {
                  type_1 = getType(i, j, bwf);
                  type_2 = getType(i, j, 1 - bwf);
                  if(able_flag && bwf==0 && (type_1 == 20 || type_1 == 21 || type_1 == 22)) // 禁手棋位置,不记录
                    continue;
                  rt[n][0] = i;
                  rt[n][1] = j;
                  rt[n][2] = getMark(type_1) + getMark(type_2);
                  n++;               
      }
      qsort( rt, sizeof(rt)/sizeof(rt[0]),sizeof(rt[0]), &compar );
      int size = n<7 ? n:7;
 //-----------------------------------------------------------//
    for(int t;t<size;t++)
    {
        int i = rt[t][0];
        int j = rt[t][1];
        if (getType(i, j, bwf) == 1) {
            x = i;
            y = j;
            return;
        }

    }
    for(int t;t<size;t++)
    {
        int i = rt[t][0];
        int j = rt[t][1];
        if (getType(i, j,1 - bwf) == 1) {
            x = i;
            y = j;
            return;
        }
    }


      for (int k = 0; k < size; k++) {
          int i = rt[k][0];
          int j = rt[k][1];
          // 有成5,则直接下子,并退出循环..没有,则思考对方情况

          if (getType(i, j, bwf) == 1) {
              x = i;
              y = j;
              break;
          }
          if (getType(i, j,1 - bwf) == 1) {
              x = i;
              y = j;
              break;
          }
          // 预存当前边界值
          int temp1=x_min,temp2=x_max,temp3=y_min,temp4=y_max;
          // 预设己方下棋,并更新边界值
          chessBoard[i][j] = bwf;
          resetMaxMin(i,j);
          // 预测未来
          int t = findMin(-100000000, 100000000, 8);
          // 还原预设下棋位置以及边界值
          chessBoard[i][j] = 2;
          x_min=temp1;
          x_max=temp2;
          y_min=temp3;
          y_max=temp4;
          // 差距小于1000，50%概率随机选取
          if ((t - mx > 1000 || abs(t - mx)<1000)) {
              x = i;
              y = j;
              mx = t;
          }

      }
      return;
  }
void MainWindow::jinshou(int x,int y)
{
    //qDebug() << "bw:" <<bw;
    if( able_flag && bw == 0)
    {//able_flag设置了禁手并且是黑棋在下
     int type = getType(x,y,bw);
     //qDebug() << "type:" <<type;
      //QString str;
     switch(type){
     case 20:
        //str = "黑长连禁手!请选择其它位置下棋!";
        isjinshou = true;
        break;
     case 21:
        //str = "黑四四禁手!请选择其它位置下棋!";
        isjinshou = true;
        break;
     case 22:
        //str = "黑三三禁手!请选择其它位置下棋!";
        isjinshou = true;
        break;
     default :
         isjinshou = false;
         break;
     }
    }

    return;
}
