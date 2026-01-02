
#ifndef CHESS_AI_HEADERFILE_CONSTANT_H
#define CHESS_AI_HEADERFILE_CONSTANT_H

#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QPushButton>
#include <QComboBox>
#include <QStack>
#include <QMessageBox>
#include <QTimer>
#include <vector>
#include <utility>
#include <algorithm>
#include <QThread>
#include <cmath>
#include <QVector>

#define gridSize 500
#define board_line 15
#define box_width 30
#define box_initial_left 50
#define box_initial_top 50
#define circle_wide 20

#define to_next_screen 16

//#define EVALUATE {1000000,1000000,80000,50000,1000,800,100,5,5,0}
constexpr int EVALUATE[] = {1000000,1000000,80000,50000,1000,800,100,5,5,0};//0五，1双空四，2单空四，3双空三，4单空三，5双空二，6单空二，7双空一，8单空一，9死棋










#endif //CHESS_AI_HEADERFILE_CONSTANT_H