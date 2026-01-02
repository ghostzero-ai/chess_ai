
#include "AiWorker.h"
#include "board.h"

#define MIN -1000000000
#define MAX 1000000000

//其他源代码调用
int judge_type(int (*board)[board_line],int x,int y,int (*board_copy)[board_line][3][4],int player);
bool able_full(int (*board)[board_line],int x,int y);//能否落子
int check(int (*board)[board_line],int x,int y);
int evaluate(int (*board)[board_line]);
int evaluate_add(Node *node);
int special_check(int (*board)[board_line],int currentPlayer,int x,int y);

//本源代码

int Alpha_Beta(Node*  node,int alpha, int beta);
std::pair<int,int> ai_move(int (*board)[board_line],int currentPlayer,int x,int y,int depth);
std::pair<int,int> special_move(int (*board)[board_line],int currentPlayer);



int Alpha_Beta(Node*  node,int alpha, int beta){
    if (node->depth==0||check(node->board,node->last_x,node->last_y)) {
        return evaluate(node->board);

        //return node->score;
    }


    int max_eval=MIN,min_eval=MAX;
    std::vector<std::pair<std::pair<int, int>, std::pair<int, double>> > children;//分别为x坐标，y坐标，分数，中心系数


    /*if (node->parent!=nullptr) {
        for (int i=std::max(node->parent->last_x-2,0);i<std::min(node->parent->last_x+3,board_line);i++) {
            for (int j=std::max(node->parent->last_y-2,0);j<std::min(node->parent->last_y+3,board_line);j++) {
                if (able_full(node->board,i,j)) {
                    Node* child=new Node();
                    child->extend_parent(node,i,j);
                    double distance=sqrt(pow(node->parent->last_x-i,2)+pow(node->parent->last_y-j,2));
                    double center_number=1+(5/(1+distance))*0.1;
                    children.push_back({{i, j},{evaluate_add(child),center_number}});
                    delete child;

                }
            }
        }
    }
    for (int i=std::max(node->last_x-5,0);i<std::min(node->last_x+6,board_line);i++) {
        for (int j=std::max(node->last_y-5,0);j<std::min(node->last_y+6,board_line);j++) {
            if (able_full(node->board,i,j)) {
                Node* child=new Node();
                child->extend_parent(node,i,j);
                double distance=sqrt(pow(node->last_x-i,2)+pow(node->last_y-j,2));
                double center_number=1+(5/(1+distance))*0.1;
                children.push_back({{i, j},{evaluate_add(child),center_number}});
                delete child;

            }
        }
    }*/

    int board_check[board_line][board_line];
    memset(board_check,0,sizeof(board_check));
    for (int i=0;i<board_line;i++) {
        for (int j=0;j<board_line;j++) {
            board_check[i][j]=node->board[i][j];
            if (!board_check[i][j]) {
                board_check[i][j]=3;
                int dx[8]={0,0,1,-1,1,-1,1,-1};
                int dy[8]={1,-1,0,0,1,-1,-1,1};
                for(int k=0;k<8;k++) {
                    for(int step=1;step<=3;step++) {
                        int nx=i+dx[k]*step;
                        int ny=j+dy[k]*step;

                        if (nx>=board_line||ny>=board_line||nx<0||ny<0) {
                            break;
                        }
                        if (node->board[nx][ny]!=0) {
                            board_check[i][j]=0;
                            break;
                        }
                    }
                }
            }
        }
    }

    for (int i=0;i<board_line;i++) {
        for (int j=0;j<board_line;j++) {
            if (able_full(node->board,i,j)&&(!board_check[i][j])) {
                Node* child=new Node();
                child->extend_parent(node,i,j);
                double distance=sqrt(pow(node->last_x-i,2)+pow(node->last_y-j,2));
                //double center_number=1+(5/(1+distance))*0.1;
                double center_number=1;
                children.push_back({{i, j},{evaluate_add(child),center_number}});
                delete child;
            }
        }
    }

    int K=std::min(children.size(),static_cast<size_t>(to_next_screen));
    if (K==0) {
        for (int i=0;i<board_line;i++) {
            for (int j=0;j<board_line;j++) {
                if (able_full(node->board,i,j)) {
                    Node* child=new Node();
                    child->extend_parent(node,i,j);
                    double distance=sqrt(pow(node->last_x-i,2)+pow(node->last_y-j,2));
                    double center_number=1+(5/(1+distance))*0.1;
                    children.push_back({{i, j},{evaluate_add(child),center_number}});
                    delete child;
                }
            }
        }
        K=std::min(children.size(),static_cast<size_t>(to_next_screen));
        if (K==0) {
            return evaluate(node->board);
        }
    }
    std::partial_sort(children.begin(), children.begin()+K,children.end(),[](const auto& a, const auto& b){ return a.second.first*a.second.second>b.second.first*b.second.second;});

    for (int i=0;i<K;i++) {
        Node* child=new Node();
        child->extend_parent(node,children[i].first.first,children[i].first.second);
        child->score=children[i].second.first;
        node->children.push_back(child);
    }

    children.clear();
    children.shrink_to_fit();
    //释放vector内存

    for (int i=0;i<K;i++) {
        int eval=Alpha_Beta(node->children[i],alpha,beta);


        if (node->currentPlayer==2) {
            if (eval>max_eval) {
                max_eval=eval;
            }
            if (eval>alpha) {
                alpha=eval;
            }
            if (alpha>=beta) {
                for (auto child : node->children) {delete child;}
                node->children.clear();
                return max_eval;
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
                for (auto child : node->children) {delete child;}
                node->children.clear();
                return min_eval;
            }
        }
    }

    if (node->currentPlayer==2) {
        for (auto child : node->children) {delete child;}
        node->children.clear();
        return max_eval;
    }
    if (node->currentPlayer==1) {
        for (auto child : node->children) {delete child;}
        node->children.clear();
        return min_eval;
    }
}

std::pair<int,int> ai_move(int (*board)[board_line],int currentPlayer,int x,int y,int depth) {
    qDebug()<<"*********************\n";
    Node* root=new Node();
    root->depth=depth;
    root->copy_board(board);
    root->currentPlayer=currentPlayer;
    root->last_x=x;
    root->last_y=y;

    root->score=evaluate(root->board);

    std::pair<int,int> special_m=special_move(board,currentPlayer);
    if (special_m.first!=-1&&special_m.second!=-1) {
        return special_m;
    }

    int board_check[board_line][board_line];
    memset(board_check,0,sizeof(board_check));
    for (int i=0;i<board_line;i++) {
        for (int j=0;j<board_line;j++) {
            board_check[i][j]=board[i][j];
            if (!board_check[i][j]) {
                board_check[i][j]=3;
                int dx[8]={0,0,1,-1,1,-1,1,-1};
                int dy[8]={1,-1,0,0,1,-1,-1,1};
                for(int k=0;k<8;k++) {
                    for(int step=1;step<=3;step++) {
                        int nx=i+dx[k]*step;
                        int ny=j+dy[k]*step;

                        if (nx>=board_line||ny>=board_line||nx<0||ny<0) {
                            break;
                        }
                        if (board[nx][ny]!=0) {
                            board_check[i][j]=0;
                            break;
                        }
                    }
                }
            }
        }
    }



    int max_score=MIN,max_x=-1,max_y=-1;
    for(int i=0;i<board_line;i++) {
        for(int j=0;j<board_line;j++) {
            if (able_full(board,i,j)&&(!board_check[i][j])) {
                qDebug()<<i<<" "<<j<<"\n";
                Node* child=new Node();
                child->extend_parent(root,i,j);
                evaluate_add(child);
                int eval=Alpha_Beta(child,MIN,MAX);
                if (eval>max_score) {
                    max_score=eval;
                    max_x=i;
                    max_y=j;
                }
                delete child;

            }
        }
    }
    delete root;
    qDebug()<<max_x<<" "<<max_y<<"\n";
    return std::pair<int,int>(max_x,max_y);

}

std::pair<int,int> special_move(int (*board)[board_line],int currentPlayer) {
    for(int i=0;i<board_line;i++) {
        for(int j=0;j<board_line;j++) {
            if (able_full(board,i,j)) {
                board[i][j]=currentPlayer;
                if (check(board,i,j)) {
                    board[i][j]=0;
                    return std::pair<int,int>(i,j);
                }
                board[i][j]=0;
            }
        }
    }

    for(int i=0;i<board_line;i++) {
        for(int j=0;j<board_line;j++) {
            if (able_full(board,i,j)) {
                board[i][j]=(currentPlayer==1)?2:1;
                if (check(board,i,j)) {
                    board[i][j]=0;
                    return std::pair<int,int>(i,j);
                }
                board[i][j]=0;
            }
        }
    }

    for(int i=0;i<board_line;i++) {
        for(int j=0;j<board_line;j++) {
            if (able_full(board,i,j)) {
                board[i][j]=currentPlayer;
                if (special_check(board,currentPlayer,i,j)) {
                    board[i][j]=0;
                    return std::pair<int,int>(i,j);
                }
                board[i][j]=0;
            }
        }
    }
    return std::pair<int,int>(-1,-1);
}