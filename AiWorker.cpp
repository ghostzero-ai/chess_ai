#include "AiWorker.h"
#include "board.h"
extern std::pair<int,int> ai_move(int (*board)[board_line],int currentPlayer,int x,int y,int depth);

AiWorker::AiWorker(QObject *parent):QObject(parent) {}
AiWorker::~AiWorker(){}
void AiWorker::doSearch(const QVector<int> &boardFlat, int aiPlayer, int last_x, int last_y, int depth) {

    int boardCopy[board_line][board_line];
    if (boardFlat.size() != board_line * board_line) {
        emit moveFound(-1, -1, aiPlayer);
        emit finished();
        return;
    }
    for (int r=0;r<board_line;r++){
        for (int c=0;c<board_line;c++) {
            boardCopy[r][c]=boardFlat[r*board_line+c];
        }
    }
    std::pair<int,int> result=ai_move(boardCopy,aiPlayer,last_x,last_y,depth);
    if (result.first>=0&&result.second>=0) {
        emit moveFound(result.first,result.second,aiPlayer);
    }else {
        emit moveFound(-1,-1,aiPlayer);
    }
    emit finished();
}
