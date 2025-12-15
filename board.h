//
// Created by ghost on 2025/10/27.
//

#ifndef CHESS_AI_BOARD_H
#define CHESS_AI_BOARD_H

#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QPushButton>
#define gridSize 500
#define board_line 15
#define box_width 30
#define box_initial_left 50
#define box_initial_top 50
#define circle_wide 20

class Board:public QWidget {
    Q_OBJECT
public:
    explicit Board(QWidget *parent = nullptr);
    int board[board_line][board_line]={{0}};
protected:
    void paintEvent(QPaintEvent *)override;
    void mousePressEvent(QMouseEvent *event) override;
private:
    int currentPlayer=1;
};


struct Node {
    int board[board_line][board_line];
    int currentPlayer;
    int last_x;
    int last_y;
    int score;
    int depth;

    Node *best_children;
    Node *parent;
    std::vector<Node*> children;
    Node ():
    currentPlayer(0),last_x(-1),last_y(-1),score(0),depth(0),board{},best_children()
    {
        best_children=this;
    }
    void copy(int (*board_initial)[board_line]) {
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