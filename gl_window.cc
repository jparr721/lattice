#include "gl_window.h"

#include <QOpenGLContext>
#include <QOpenGLPaintDevice>
#include <QPainter>

//! [1]
GLWindow::GLWindow(QWindow* parent) : QWindow(parent) {
    setSurfaceType(QWindow::OpenGLSurface);
}
//! [1]

GLWindow::~GLWindow() { delete m_device; }
//! [2]
void GLWindow::render(QPainter* painter) { Q_UNUSED(painter); }

void GLWindow::initialize() {}

void GLWindow::render() {
    if (!m_device)
        m_device = new QOpenGLPaintDevice;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    m_device->setSize(size() * devicePixelRatio());
    m_device->setDevicePixelRatio(devicePixelRatio());

    QPainter painter(m_device);
    render(&painter);
}
//! [2]

//! [3]
void GLWindow::renderLater() { requestUpdate(); }

bool GLWindow::event(QEvent* event) {
    switch (event->type()) {
    case QEvent::UpdateRequest:
        renderNow();
        return true;
    default:
        return QWindow::event(event);
    }
}

void GLWindow::exposeEvent(QExposeEvent* event) {
    Q_UNUSED(event);

    if (isExposed())
        renderNow();
}
//! [3]

//! [4]
void GLWindow::renderNow() {
    if (!isExposed())
        return;

    bool needsInitialize = false;

    if (!m_context) {
        m_context = new QOpenGLContext(this);
        m_context->setFormat(requestedFormat());
        m_context->create();

        needsInitialize = true;
    }

    m_context->makeCurrent(this);

    if (needsInitialize) {
        initializeOpenGLFunctions();
        initialize();
    }

    render();

    m_context->swapBuffers(this);

    if (m_animating)
        renderLater();
}
//! [4]

//! [5]
void GLWindow::setAnimating(bool animating) {
    m_animating = animating;

    if (animating)
        renderLater();
}
//! [5]
