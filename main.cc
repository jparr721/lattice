#include "main_window.h"

#include <iostream>

#include <QApplication>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QScreen>
#include <iostream>

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    MainWindow main_window;
    main_window.resize(main_window.sizeHint());
    std::cout << main_window.width() << std::endl;
    main_window.showMaximized();
    return app.exec();

    // QSurfaceFormat format;
    // format.setSamples(16);

    // Window window;
    // window.setFormat(format);
    // window.resize(1920, 1080);
    // window.show();

    // window.setAnimating(true);

    // return app.exec();
}
