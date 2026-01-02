#include "board.h"

int add(int count,int gap);
void count_gap(int *dx,int *dy,int *count,int *gap,int (*board)[board_line],int x,int y,int (*board_copy)[board_line][3][4],int player);
void add_cover(int *ans,int *dx,int *dy,int *count,int *gap,int (*board)[board_line],int x,int y,int (*board_copy)[board_line][3][4],int player);
int judge_type(int (*board)[board_line],int x,int y,int (*board_copy)[board_line][3][4],int player);
int evaluate(int (*board)[board_line]);


int add(int count,int gap) {
    //int evaluate[10]=EVALUATE;
    if (count<=0) {
        return 0;
    }
    if (count>=5) {
        return EVALUATE[0];
    }
    if (gap==0) {
        return EVALUATE[9];
    }
    if (gap==1) {
        if (count==1) {
            return EVALUATE[8];
        }
        if (count==2) {
            return EVALUATE[6];
        }
        if (count==3) {
            return EVALUATE[4];
        }
        if (count==4) {
            return EVALUATE[2];
        }
    }
    if (gap==2) {
        if (count==1) {
            return EVALUATE[7];
        }
        if (count==2) {
            return EVALUATE[5];
        }
        if (count==3) {
            return EVALUATE[3];
        }
        if (count==4) {
            return EVALUATE[1];
        }
    }
}

//计算直线方向的棋子和空格个数
void count_gap(int *dx,int *dy,int *count,int *gap,int (*board)[board_line],int x,int y,int (*board_copy)[board_line][3][4],int player) {
    for(int i=0;i<8;i++) {
        if (board_copy[x][y][player][i/2]==1) {
            continue;
        }
        for(int step=1;step<=4;step++) {
            int nx = x+dx[i]*step;
            int ny = y+dy[i]*step;
            if (nx>=board_line||ny>=board_line||nx<=-1||ny<=-1) {
                break;
            }
            if (board[nx][ny]==player) {
                count[i/2]++;
            }else {
                if (board[nx][ny]==0) {
                    gap[i/2]++;
                }
                break;
            }
        }
    }
}

//计算局面评分，并将计算过的棋子屏蔽，防止重复
void add_cover(int *ans,int *dx,int *dy,int *count,int *gap,int (*board)[board_line],int x,int y,int (*board_copy)[board_line][3][4],int player) {
    for(int i=0;i<8;i++) {
        if (!board_copy[x][y][player][i/2]) {
            if (i%2==0&&gap[i/2]) {
                *ans+=add(count[i/2],gap[i/2]);
            }
            for(int step=1;step<=4;step++) {
                int nx = x+dx[i]*step;
                int ny = y+dy[i]*step;
                if (nx>=board_line||ny>=board_line||nx<=-1||ny<=-1) {
                    break;
                }
                if (board[nx][ny]==player) {
                    board_copy[nx][ny][player][i/2]=1;
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
    //int evaluate[10]=EVALUATE;//0五，1双空四，2单空四，3双空三，4单空三，5双空二，6单空二，7双空一，8单空一，9死棋
    //(*board_copy)[board_line][3][4] 三维为黑白，1为黑，2为白；四维为方向，0为x方向，1为y方向，2为xy同方向，3为xy异方向
    count_gap(dx,dy,count,gap,board,x,y,board_copy,player);
    add_cover(&ans,dx,dy,count,gap,board,x,y,board_copy,player);
    return ans;
}

int evaluate(int (*board)[board_line]) {
    int evaluation1=0,evaluation2=0;
    //int evaluate[10]=EVALUATE;
    int board_copy[board_line][board_line][3][4];//三维为黑白，1为黑，2为白；四维为方向，0为x方向，1为y方向，2为xy同方向，3为xy异方向
    memset(board_copy,0,sizeof(board_copy));
    for(int i=0;i<board_line;i++) {
        for(int j=0;j<board_line;j++) {
            if (board[i][j]==1) {
                evaluation1+=judge_type(board,i,j,board_copy,1);
            }
            if (board[i][j]==2) {
                evaluation2+=judge_type(board,i,j,board_copy,2);
            }
        }
    }
    return evaluation2-evaluation1;
}

