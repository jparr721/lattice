#include <QOpenGLFunctions>
#include <QWindow>

QT_BEGIN_NAMESPACE
class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;
QT_END_NAMESPACE

//! [1]
class GLWindow : public QWindow, protected QOpenGLFunctions {
    Q_OBJECT
  public:
    explicit GLWindow(QWindow* parent = nullptr);
    ~GLWindow();

    virtual void render(QPainter* painter);
    virtual void render();

    virtual void initialize();

    void setAnimating(bool animating);

  public slots:
    void renderLater();
    void renderNow();

  protected:
    bool event(QEvent* event) override;

    void exposeEvent(QExposeEvent* event) override;

  private:
    bool m_animating = false;

    QOpenGLContext* m_context = nullptr;
    QOpenGLPaintDevice* m_device = nullptr;
};
//! [1]
