#include "board.h"
#define MIN -100000
#define MAX 100000
//其他源代码调用
int judge_type(int (*board)[board_line],int x,int y,int (*board_copy)[board_line][3][4],int player);
bool able_full(int (*board)[board_line],int x,int y);//能否落子
int check(int (*board)[board_line]);
int max(int a,int b);
int min(int a,int b);

//本源代码
int evaluate(int (*board)[board_line]);
int Alpha_Beta(int (*board)[board_line],int currentPlayer,int depth,int alpha, int beta);
void ai_move(int (*board)[board_line],int currentPlayer,int x,int y);


int evaluate(int (*board)[board_line]) {
    int evaluation1=0,evaluation2=0;
    int evaluate[10]={100000,1000,500,500,100,100,50,50,30,0};//0五，1双空四，2单空四，3双空三，4单空三，5双空二，6单空二，7双空一，8单空一，9死棋
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

int Alpha_Beta(Node*  node,int alpha, int beta){
    if (node->depth==0||check(node->board)) {
        return evaluate(node->board);
    }
    int max_eval=MIN,min_eval=MAX;
    for(int i=max(node->last_x-3,0);i<min(node->last_x+4,board_line);i++) {
        for(int j=max(node->last_y-3,0);j<min(node->last_y+4,board_line);j++) {
            if (able_full(node->board,i,j)) {

                Node* child=new Node();
                child->extend_parent(node,i,j);
                node->addChild(child);
                int eval=Alpha_Beta(child,alpha,beta);
                if (node->currentPlayer==2) {
                    if (eval>max_eval) {
                        max_eval=eval;
                    }
                    if (eval>alpha) {
                        alpha=eval;
                    }
                    if (alpha>=beta) {
                        return beta;
                    }
                }
                if (node->currentPlayer==1) {
                    if (eval<min_eval) {
                        min_eval=eval;
                    }
                    if (eval<beta) {
                        beta=eval;
                    }
                    if (alpha>=beta) {
                        return alpha;
                    }
                }
            }
        }
    }
    if (node->currentPlayer==2) {
        return max_eval;
    }
    if (node->currentPlayer==1) {
        return min_eval;
    }
}

void ai_move(int (*board)[board_line],int currentPlayer,int x,int y) {
    Node* root=new Node();
    root->depth=4;
    root->copy(board);
    root->currentPlayer=currentPlayer;
    root->last_x=x;
    root->last_y=y;
    int max_score=MIN,max_x=-1,max_y=-1;
    for(int i=max(root->last_x-3,0);i<min(root->last_x+4,board_line);i++) {
        for(int j=max(root->last_y-3,0);j<min(root->last_y+4,board_line);j++) {
            if (able_full(board,i,j)) {
                Node* child=new Node();
                child->extend_parent(root,i,j);
                //root->addChild(child);
                child->score=Alpha_Beta(child,MIN,MAX);
                if (child->score>max_score) {
                    max_score=child->score;
                    max_x=i;
                    max_y=j;
                }
                free(child);
            }
        }
    }
    board[max_x][max_y]=currentPlayer;
}