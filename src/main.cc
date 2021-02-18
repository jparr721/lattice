#include <lattice/main_window.h>

#include <QApplication>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QScreen>
#include <iostream>

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    MainWindow main_window;

    main_window.resize(main_window.sizeHint());
    main_window.showMaximized();

    return app.exec();
}
