#include "board.h"




//其他源代码调用
int add(int count,int gap);





//本源代码
int evaluate_add(Node *node);//用于评估新下子的分数
int score_add(int currentPlayer, int count[8][3], int gap[8]);

int score_add(int currentPlayer, int count[8][3], int gap[8]) {
    int score_add=0;
    for(int i=0;i<8;i+=2) {
        if ((count[i][currentPlayer]||count[i][(currentPlayer==1)?2:1])&&(count[i+1][currentPlayer]||count[i+1][(currentPlayer==1)?2:1])) {
            if (count[i][currentPlayer]&&count[i+1][currentPlayer]) {
                if(count[i][currentPlayer]+count[i+1][currentPlayer]+1>=5) {
                    score_add+=add(5,2);
                }else {
                    score_add+=add(count[i][currentPlayer]+count[i+1][currentPlayer]+1,gap[i]+gap[i+1]);
                    score_add-=add(count[i][currentPlayer],gap[i]);
                    score_add-=add(count[i+1][currentPlayer],gap[i+1]);
                }
            }
            if (count[i][(currentPlayer==1)?2:1]&&count[i+1][(currentPlayer==1)?2:1]) {
                //对方少一个多空，多一个多减一空，我方先加分再扣分
                score_add+=add(count[i][(currentPlayer==1)?2:1],gap[i]+1);
                score_add-=add(count[i][(currentPlayer==1)?2:1],gap[i]);

                score_add+=add(count[i+1][(currentPlayer==1)?2:1],gap[i+1]+1);
                score_add-=add(count[i+1][(currentPlayer==1)?2:1],gap[i+1]);
                }
            if (count[i][currentPlayer]&&count[i+1][(currentPlayer==1)?2:1]) {
                //与我方链接加分，使对方少气加分
                score_add+=add(count[i][currentPlayer]+1,gap[i]);
                score_add-=add(count[i][currentPlayer],gap[i]+1);

                score_add+=add(count[i+1][(currentPlayer==1)?2:1],gap[i+1]+1);
                score_add-=add(count[i+1][(currentPlayer==1)?2:1],gap[i+1]);

            }
            if (count[i][(currentPlayer==1)?2:1]&&count[i+1][currentPlayer]) {
                    //与我方链接加分，使对方少气加分
                score_add+=add(count[i+1][currentPlayer]+1,gap[i]);
                score_add-=add(count[i+1][currentPlayer],gap[i]+1);

                score_add+=add(count[i][(currentPlayer==1)?2:1],gap[i+1]+1);
                score_add-=add(count[i][(currentPlayer==1)?2:1],gap[i+1]);
            }
        }
        if (!(count[i][currentPlayer]||count[i][(currentPlayer==1)?2:1])&&(count[i+1][currentPlayer]||count[i+1][(currentPlayer==1)?2:1])){
            if (count[i+1][currentPlayer]) {
                //与我方链接加分
                score_add+=add(count[i+1][currentPlayer]+1,gap[i]+gap[i+1]);
                score_add-=add(count[i+1][currentPlayer],gap[i]+gap[i+1]);
            }
            if (count[i+1][(currentPlayer==1)?2:1]) {
                //使对方少气加分
                score_add+=add(count[i+1][(currentPlayer==1)?2:1],gap[i]+1);
                score_add-=add(count[i+1][(currentPlayer==1)?2:1],gap[i]);
            }

        }
        if ((count[i][currentPlayer]||count[i][(currentPlayer==1)?2:1])&&!(count[i+1][currentPlayer]||count[i+1][(currentPlayer==1)?2:1])){
            if (count[i][currentPlayer]) {
                //与我方链接加分
                score_add+=add(count[i][currentPlayer]+1,gap[i]+gap[i+1]);
                score_add-=add(count[i][currentPlayer],gap[i]+gap[i+1]);
            }
            if (count[i][(currentPlayer==1)?2:1]) {
                //使对方少气加分
                score_add+=add(count[i][(currentPlayer==1)?2:1],gap[i]+1);
                score_add-=add(count[i][(currentPlayer==1)?2:1],gap[i]);
            }
        }
        if (!(count[i][currentPlayer]||count[i][(currentPlayer==1)?2:1])&&!(count[i+1][currentPlayer]||count[i+1][(currentPlayer==1)?2:1])){
            score_add+=add(1,gap[i]+gap[i+1]);
        }
    }
    return score_add;
}

int evaluate_add(Node *node) {
    int score=0;
    int dx[8]={0,0,1,-1,1,-1,1,-1};
    int dy[8]={1,-1,0,0,1,-1,-1,1};
    int count[8][3];
    int gap[8];
    memset(count,0,sizeof(count));
    memset(gap,0,sizeof(gap));
    for(int i=0;i<8;i++) {

        for(int step=1;step<=4;step++) {
            int nx = node->last_x+dx[i]*step;
            int ny = node->last_y+dy[i]*step;
            if (nx>=board_line||ny>=board_line||nx<=-1||ny<=-1) {
                gap[i]=0;
                break;
            }
            if (node->board[nx][ny]==0) {
                gap[i]=1;
                break;
            }
            if (node->board[nx][ny]!=0&&(step==1||node->board[nx-dx[i]][ny-dy[i]]==node->board[nx][ny])){
                if (node->board[nx][ny]==1) {
                    count[i][1]++;
                }else if (node->board[nx][ny]==2){
                    count[i][2]++;
                }
            }else if (node->board[nx][ny]!=0) {
                gap[i]=0;
                break;
            }
        }
    }
    score=score_add((node->currentPlayer==1)?2:1, count, gap);
    return score;

}
