#include "board.h"
#include "AiWorker.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Board boardWidget;
    boardWidget.show();

    // 创建并启动 AI 线程/worker
    QThread *thread = new QThread;
    AiWorker *worker = new AiWorker();
    worker->moveToThread(thread);

    //QObject::connect(thread, &QThread::finished, worker, &QObject::deleteLater);
    //QObject::connect(worker, &AiWorker::finished, thread, &QThread::quit);



    // 将 worker 的结果信号连接回 Board 的槽（主线程执行）
    QObject::connect(worker, &AiWorker::moveFound, &boardWidget, &Board::aiPlaceStone);

    // 将 Board 的 startSearch 信号连接到 worker 的 doSearch 槽（跨线程调用）
    QObject::connect(&boardWidget, &Board::startSearch, worker, &AiWorker::doSearch,Qt::QueuedConnection);
    thread->start();

    int ret = app.exec();

    // 退出时清理线程
    thread->quit();
    thread->wait();
    delete thread;
    return ret;
}