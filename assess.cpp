#include "board.h"

void count_gap(int *dx,int *dy,int *count,int *gap,int (*board)[board_line],int x,int y,int (*board_copy)[board_line][3][4],int player);
void add_cover(int *evaluate ,int *ans,int *dx,int *dy,int *count,int *gap,int (*board)[board_line],int x,int y,int (*board_copy)[board_line][3][4],int player);
int judge_type(int (*board)[board_line],int x,int y,int (*board_copy)[board_line][3][4],int player);

//计算直线方向的棋子和空格个数
void count_gap(int *dx,int *dy,int *count,int *gap,int (*board)[board_line],int x,int y,int (*board_copy)[board_line][3][4],int player) {
    for(int i=0;i<8;i++) {
        if (board_copy[x][y][player][i/2]==1) {
            continue;
        }
        for(int step=1;step<=4;step++) {
            if (x+dx[i]*step==board_line||y+dy[i]*step==board_line||x+dx[i]*step==-1||y+dy[i]*step==-1) {
                break;
            }
            if (board[x+dx[i]*step][y+dy[i]*step]==player) {
                count[i/2]++;
            }else {
                if (board[x+dx[i]*step][y+dy[i]*step]==0) {
                    gap[i/2]++;
                }
                break;
            }
        }
    }
}

//计算局面评分，并将计算过的棋子屏蔽，防止重复
void add_cover(int *evaluate ,int *ans,int *dx,int *dy,int *count,int *gap,int (*board)[board_line],int x,int y,int (*board_copy)[board_line][3][4],int player) {
    for(int i=0;i<8;i++) {
        if (!board_copy[x][y][player][i/2]) {
            if (i%2==0&&gap[i/2]) {
                *ans+=evaluate[9-count[i/2]*2+2-gap[i/2]];
            }
            for(int step=1;step<=4;step++) {
                if (board[x+dx[i]*step][y+dy[i]*step]==player) {
                    board_copy[x+dx[i]*step][y+dy[i]*step][player][i/2]=1;
                }else {
                    break;
                }
            }
        }
    }
    board_copy[x][y][player][0]=1;
    board_copy[x][y][player][1]=1;
    board_copy[x][y][player][2]=1;
    board_copy[x][y][player][3]=1;
}

int judge_type(int (*board)[board_line],int x,int y,int (*board_copy)[board_line][3][4],int player) {
    int dx[8]={0,0,1,-1,1,-1,1,-1};
    int dy[8]={1,-1,0,0,1,-1,-1,1};
    int count[4]={1,1,1,1};
    int gap[4]={0,0,0,0};
    int ans=0;
    int evaluate[10]={1000000,100000,10000,5000,1000,500,100,50,10,0};//0五，1双空四，2单空四，3双空三，4单空三，5双空二，6单空二，7双空一，8单空一，9死棋
    //(*board_copy)[board_line][3][4] 三维为黑白，1为黑，2为白；四维为方向，0为x方向，1为y方向，2为xy同方向，3为xy异方向
    count_gap(dx,dy,count,gap,board,x,y,board_copy,player);
    add_cover(evaluate,&ans,dx,dy,count,gap,board,x,y,board_copy,player);
    return ans;
}