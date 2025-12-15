#include "board.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QWidget window;
    window.setWindowTitle("五子棋");
    window.resize(gridSize+20, gridSize+20);
    Board board(&window);
    board.setGeometry(10,10,gridSize,gridSize);
    window.show();
    return app.exec();
}