#include "board.h"
#include <windows.h>

//其他源代码调用
void ai_move(int (*board)[board_line],int currentPlayer,int x,int y);
int check(int (*board)[board_line]);



Board::Board(QWidget *parent ):QWidget(parent) {
    for (int i=0;i<board_line;i++) {
        for (int j=0;j<board_line;j++) {
            board[i][j]=0;
        }
    }
    setFixedSize(gridSize, gridSize);
    setStyleSheet("background-color: #dcb35c;");
};

void Board::paintEvent(QPaintEvent *){

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::black,2));
    for (int row = 0; row < board_line; row++) {
        painter.drawLine(box_initial_left, box_initial_top+row*box_width,box_initial_left+(board_line-1)*box_width,box_initial_top+row*box_width);
        painter.drawLine(box_initial_left+row*box_width,box_initial_top ,box_initial_left+row*box_width,box_initial_top+(board_line-1)*box_width);
    }
    for (int i=0;i<board_line;i++) {
        for (int j=0;j<board_line;j++) {
            if (board[i][j]==1) {
                painter.setBrush(Qt::black);
                painter.drawEllipse(box_initial_left+j*box_width-0.5*circle_wide,box_initial_top+i*box_width-0.5*circle_wide,circle_wide,circle_wide);
            }
            if (board[i][j]==2) {
                painter.setBrush(Qt::white);
                painter.drawEllipse(box_initial_left+j*box_width-0.5*circle_wide,box_initial_top+i*box_width-0.5*circle_wide,circle_wide,circle_wide);
            }
        }
    }
}



void Board::mousePressEvent(QMouseEvent *event)  {
    int x=event->x();
    int y=event->y();
    int row=(y-35)/30;
    int col=(x-35)/30;
    if (row>=0 && row<board_line && col>=0 && col<board_line) {
        /*for (int i=0;i<board_line;i++) {
            qDebug() << board[i][0]<<" "<<board[i][1]<<" "<<board[i][2]<<" "<<board[i][3]<<" "<<board[i][4]<<" "<<board[i][5]<<" "<<board[i][6]<<" "<<board[i][7]<<" "<<board[i][8]<<" "<<board[i][9]<<" "<<board[i][10]<<" "<<board[i][11]<<" "<<board[i][12]<<" "<<board[i][13]<<" "<<board[i][14]<<"\n";
        }*/
        if (board[row][col]==0) {

            board[row][col]=currentPlayer;
            //update();
            repaint();
            if (check(board)==1) {
                qDebug() << "黑胜";
                return ;
            }

            qDebug() << "落子位置: 行" << row << "列" << col
                 << (board[row][col] == 1 ? "(黑棋)" : "(白棋)");
            currentPlayer=(currentPlayer==1)?2:1;
            //Sleep(1000);
            ai_move(board,currentPlayer,row,col);
            repaint();
            if (check(board)==2) {

                qDebug() << "白胜";
                return ;
            }
            //update();
            //qDebug() << "落子位置: 行" << row << "列" << col
            //     << (board[row][col] == 1 ? "(黑棋)" : "(白棋)");
            currentPlayer=(currentPlayer==1)?2:1;
        }
    }
}








