//
// Created by ghost on 2025/10/27.
//

#ifndef CHESS_AI_BOARD_H
#define CHESS_AI_BOARD_H

#include "Headerfile_constant.h"

struct Move {
    int row;
    int col;
    int player;
};



class Board:public QWidget {
    Q_OBJECT
public:
    explicit Board(QWidget *parent = nullptr);
    int board[board_line][board_line];
    void startNewGame();
    void undoMove();
    void setPlayerColor(int color);
    void toggleAI(bool enable);
    QVector<int> boardToFlat() const;
public slots:
    void aiPlaceStone(int row, int col, int player);
signals:
    void startSearch(const QVector<int> &boardFlat, int aiPlayer, int last_x, int last_y, int depth);
    void finished();
protected:
    void paintEvent(QPaintEvent *)override;
    void mousePressEvent(QMouseEvent *event) override;
private:
    int currentPlayer;
    bool AI;
    // UI控件
    QPushButton* startButton;
    QPushButton* exitButton;
    QPushButton* undoButton;
    QComboBox* colorComboBox;
    // 游戏状态
    bool gameActive;
    // 悔棋历史记录

    QStack<Move> moveHistory;

    QPoint screenToCell(int sx, int sy);
    void placeStone(int row, int col, int player);




private slots:
    void onStartClicked();
    void onExitClicked();
    void onUndoClicked();
    void onColorChanged(int index);
};




struct Node {
    int board[board_line][board_line]={{0}};
    int currentPlayer;
    int last_x;
    int last_y;
    int score;
    int depth;

    Node *best_children;
    Node *parent;
    std::vector<Node*> children;
    Node ():
    currentPlayer(0),last_x(-1),last_y(-1),score(0),depth(0),best_children()
    {
        best_children=this;
        parent=this;
    }
    void copy_board(int (*board_initial)[board_line]) {
        for(int i=0;i<board_line;i++) {
            for(int j=0;j<board_line;j++) {
                board[i][j]=board_initial[i][j];
            }
        }
    }

    void extend_parent(Node *parent_initial,int x,int y) {
        currentPlayer=(parent_initial->currentPlayer==1)?2:1;
        last_x=x;
        last_y=y;
        depth=parent_initial->depth-1;
        parent=parent_initial;
        for(int i=0;i<board_line;i++) {
            for(int j=0;j<board_line;j++) {
                board[i][j]=parent_initial->board[i][j];
            }
        }
        board[x][y]=parent_initial->currentPlayer;
    }

    void addChild(Node* child) {
        children.push_back(child);
    }





};



#endif //CHESS_AI_BOARD_H