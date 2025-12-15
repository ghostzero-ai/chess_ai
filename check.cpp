#include "board.h"

//本源代码
int check(int (*board)[board_line]);//是否有一方胜
bool able_full(int (*board)[board_line],int x,int y);//能否落子


int check(int (*board)[board_line]) {
    int dx[8]={1,0,-1,0,1,-1,1,-1};
    int dy[8]={0,1,0,-1,1,-1,-1,1};
    for(int i=0;i<board_line;i++) {
        for(int j=0;j<board_line;j++) {
            for(int k=0;k<8;k++) {
                if (!board[i][j])continue;
                if (i+4>=board_line&&(k==0||k==4||k==6))continue;
                if (j+4>=board_line&&(k==1||k==4||k==7))continue;
                if (i-4<0&&(k==2||k==5||k==7))continue;
                if (j-4<0&&(k==3||k==5||k==6))continue;
                if (board[i][j]==board[i+dx[k]*1][j+dy[k]*1]&&board[i][j]==board[i+dx[k]*2][j+dy[k]*2]&&board[i][j]==board[i+dx[k]*3][j+dy[k]*3]&&board[i][j]==board[i+dx[k]*4][j+dy[k]*4]) {
                    return board[i][j];
                }
            }
        }
    }
    return 0;
}//是否有一方胜


bool able_full(int (*board)[board_line],int x,int y) {
    if ((x>=0&&x<board_line)&&(y>=0&&y<board_line)&&(!board[x][y])) {
        return true;
    }
    return false;
}//能否落子

