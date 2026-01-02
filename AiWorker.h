
#ifndef CHESS_AI_AIWORKER_H
#define CHESS_AI_AIWORKER_H

#include "Headerfile_constant.h"

class AiWorker:public QObject {
    Q_OBJECT
public:
    explicit AiWorker(QObject *parent = nullptr);
    ~AiWorker();
public slots:
    void doSearch(const QVector<int> &boardFlat, int aiPlayer, int last_x, int last_y, int depth);
signals:
    void moveFound(int row,int col,int player);
    void finished();
};


#endif //CHESS_AI_AIWORKER_H