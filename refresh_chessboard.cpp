#include "board.h"
#include <windows.h>
#include "AiWorker.h"

//其他源代码调用
std::pair<int,int> ai_move(int (*board)[board_line],int currentPlayer,int x,int y,int depth);
int check(int (*board)[board_line],int x,int y);


//本源代码



Board::Board(QWidget *parent ):QWidget(parent) {
    //初始化棋盘
    startButton = new QPushButton("开始游戏",this);
    exitButton = new QPushButton("退出游戏", this);
    undoButton = new QPushButton("悔棋", this);
    colorComboBox = new QComboBox(this);
    colorComboBox->addItem("执黑 (先手)");
    colorComboBox->addItem("执白 (后手)");
    setFixedSize(gridSize,gridSize);
    startButton->setGeometry(10,10,90,30);
    exitButton->setGeometry(110, 10, 90, 30);
    undoButton->setGeometry(210, 10, 90, 30);
    colorComboBox->setGeometry(310, 10, 140, 30);
    connect(startButton,&QPushButton::clicked,this,&Board::onStartClicked);
    connect(exitButton,&QPushButton::clicked,this,&Board::onExitClicked);
    connect(undoButton,&QPushButton::clicked,this,&Board::onUndoClicked);
    connect(colorComboBox,QOverload<int>::of(&QComboBox::currentIndexChanged),this,&Board::onColorChanged);


    startNewGame();
};

void Board::startNewGame() {
    for(int i=0;i<board_line;i++) {
        for(int j=0;j<board_line;j++) {
            board[i][j]=0;
        }
    }
    moveHistory.clear();
    gameActive=true;
    AI=true;
    int idx=colorComboBox->currentIndex();
    if (idx==0) {
        currentPlayer=1;
    }else {
        currentPlayer=2;
    }
    if (currentPlayer==2&&AI) {
        int mid=board_line/2;
        board[mid][mid]=1;
        moveHistory.push({mid,mid,1});
    }

    update();
}

void Board::undoMove() {
    if ((!gameActive)||moveHistory.empty()) {
        return;
    }

    if (AI) {
        Move last=moveHistory.top();
        if (last.player==colorComboBox->currentIndex()+1) {
            return ;
        }
        if (!moveHistory.empty()) {
            Move m=moveHistory.top();
            moveHistory.pop();
            board[m.row][m.col]=0;
        }
        if (!moveHistory.empty()) {
            Move m=moveHistory.top();
            moveHistory.pop();
            board[m.row][m.col]=0;
            currentPlayer=m.player;
        }
    }else {
        Move m=moveHistory.top();
        moveHistory.pop();
        board[m.row][m.col]=0;
        currentPlayer=m.player;
    }
    update();
}

void Board::toggleAI(bool enable) {
    AI=enable;
}


void Board::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.fillRect(rect(), QColor(220,179,92));
    painter.setPen(QPen(Qt::black,2));
    for (int row = 0; row < board_line; row++) {
        painter.drawLine(box_initial_left, box_initial_top+row*box_width,box_initial_left+(board_line-1)*box_width,box_initial_top+row*box_width);

    }
    for (int col = 0; col < board_line; col++) {
        painter.drawLine(box_initial_left+col*box_width,box_initial_top ,box_initial_left+col*box_width,box_initial_top+(board_line-1)*box_width);
    }
    for (int i=0;i<board_line;i++) {
        for (int j=0;j<board_line;j++) {
            if (board[i][j]==1) {
                painter.setBrush(Qt::black);
                painter.drawEllipse(box_initial_left+j*box_width-0.5*circle_wide,box_initial_top+i*box_width-0.5*circle_wide,circle_wide,circle_wide);
            }
            if (board[i][j]==2) {
                painter.setBrush(Qt::white);
                painter.drawEllipse(box_initial_left+j*box_width-0.5*circle_wide,box_initial_top+i*box_width-0.5*circle_wide,circle_wide,circle_wide);
            }
        }
    }
}

QPoint Board::screenToCell(int sx,int sy) {
    int col=qRound(double(sx-box_initial_left)/box_width);
    int row=qRound(double(sy-box_initial_top)/box_width);
    return QPoint(row,col);
}

void Board::mousePressEvent(QMouseEvent *event)  {

    if (!gameActive) {
        return;
    }
    if (event->button()!=Qt::LeftButton) {
        return;
    }
    QPoint pos=screenToCell(event->x(),event->y());
    int row=pos.x();
    int col=pos.y();
    if (row<0||row>=board_line||col<0||col>=board_line) {
        return;
    }
    if (board[row][col]!=0) {
        return;
    }
    int player=(colorComboBox->currentIndex()==0)?1:2;
    if (currentPlayer!=player) {
        return;
    }
    placeStone(row,col,player);
    update();
    repaint();

    int win=check(board,row,col);
    if (win!=0) {
        QString msg=(win==1)?"黑棋胜!":"白棋胜!";
        QMessageBox::information(this,"游戏结束",msg);
        gameActive=false;
        return;
    }

    if (AI) {
        
        QVector<int> flat;
        flat.reserve(board_line*board_line);
        for (int i=0;i<board_line;i++) {
            for (int j=0;j<board_line;j++) {
                flat.append(board[i][j]);
            }
        }

        emit startSearch(flat,currentPlayer,row,col,6);
        repaint();
    }
}

void Board::placeStone(int row,int col,int player) {
    if (board[row][col]!=0) {
        return;
    }
    board[row][col]=player;
    moveHistory.push({row,col,player});
    currentPlayer=(currentPlayer==1)?2:1;
}

void Board::onStartClicked() {
    startNewGame();
}

void Board::onExitClicked() {
    QWidget *w=window();
    if (w) {
        w->close();
    }else {
        qApp->quit();
    }
}

void Board::onUndoClicked() {
    undoMove();
}

void Board::onColorChanged(int index) {
    Q_UNUSED(index);
    //startNewGame();
}

QVector<int> Board::boardToFlat() const {
    QVector<int> flat;
    flat.reserve(board_line * board_line);
    for (int i = 0; i < board_line; ++i)
        for (int j = 0; j < board_line; ++j)
            flat.append(board[i][j]);
    return flat;
}

void Board::aiPlaceStone(int row,int col,int player) {
    qDebug() << "aiPlaceStone called in thread:" << QThread::currentThread();
    if (!gameActive) return;
    if (row < 0 || col < 0) {
        qDebug() << "AI 未找到合法落子";
        return;
    }
    if (row >= board_line || col >= board_line) return;
    if (board[row][col] != 0) {
        qDebug() << "目标格已被占用, 跳过";
        return;
    }

    placeStone(row, col, player);
    update();

    int win2 = check(board,row,col);
    if (win2 != 0) {
        QString msg = (win2 == 1) ? "黑棋胜！" : "白棋胜！";
        QMessageBox::information(this, "游戏结束", msg);
        gameActive = false;
        return;
    }
}