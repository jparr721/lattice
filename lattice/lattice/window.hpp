//
//  window.hpp
//  lattice
//
//  Window is a class for building and managing the openGL context
//  as well as the parameters needed.
//  Created by Jarred Parr on 2/1/21.
//

#ifndef window_hpp
#define window_hpp

#include <QtWidgets/qwidget.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qslider.h>

class GLWidget;

class Window : public QWidget {
    Q_OBJECT

  public:
    Window();
    ~Window() = default;

  public slots:
    void DrawGL();

  protected:
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

  private:
    GLWidget* gl_widget;

    void CreateSlider(QLabel* label, QSlider* slider);
};

#endif /* window_hpp */
