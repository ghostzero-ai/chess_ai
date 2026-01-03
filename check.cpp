#include "board.h"

//本源代码
int check(int (*board)[board_line],int x,int y);//是否有一方胜
bool able_full(int (*board)[board_line],int x,int y);//能否落子
int special_check(int (*board)[board_line],int currentPlayer,int x,int y);//能否活四

int check(int (*board)[board_line],int x,int y) {
    int dx[8]={0,0,1,-1,1,-1,1,-1};
    int dy[8]={1,-1,0,0,1,-1,-1,1};
    int count[4]={1,1,1,1};
    int currentplayer=board[x][y];
    if (currentplayer==0) {
        return 0;
    }
    for(int i=0;i<8;i++) {
        for(int step=1;step<=4;step++) {
            int nx=x+dx[i]*step;
            int ny=y+dy[i]*step;
            if (nx>=board_line||ny>=board_line||nx<0||ny<0) {
                break;
            }
            if (board[nx][ny]==currentplayer) {
                count[i/2]++;
            }else {
                break;
            }
        }
    }

    for(int i=0;i<4;i++) {
        if (count[i]>=5) {
            return currentplayer;
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

int special_check(int (*board)[board_line],int currentPlayer,int x,int y) {
    int dx[8]={0,0,1,-1,1,-1,1,-1};
    int dy[8]={1,-1,0,0,1,-1,-1,1};
    int count[4][3];
    int gap[4];
    memset(count,0,sizeof(count));
    memset(gap,0,sizeof(gap));
    for(int i=0;i<8;i++) {
        for(int step=1;step<=4;step++) {
            int nx = x+dx[i]*step;
            int ny = y+dy[i]*step;
            if (nx>=board_line||ny>=board_line||nx<=-1||ny<=-1) {
                break;
            }
            if (board[nx][ny]==0) {
                gap[i/2]++;
                break;
            }
            if (board[nx][ny]!=0&&(step==1||board[nx-dx[i]][ny-dy[i]]==board[nx][ny])){
                if (board[nx][ny]==1) {
                    count[i/2][1]++;
                }else if (board[nx][ny]==2){
                    count[i/2][2]++;
                }
            }else if (board[nx][ny]!=0) {
                break;
            }
        }
    }
    for (int i=0;i<4;i++) {
        if (count[i][currentPlayer]==3&&gap[i]==2) {
            return currentPlayer;
        }
    }
    return 0;
}