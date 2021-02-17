#pragma once

#include <unordered_map>

#include <QKeyEvent>

class Keyboard {
  public:
    constexpr static int kUp = Qt::Key_T;
    constexpr static int kDown = Qt::Key_G;
    constexpr static int kForward = Qt::Key_W;
    constexpr static int kBackward = Qt::Key_S;
    constexpr static int kLeft = Qt::Key_A;
    constexpr static int kRight = Qt::Key_D;

    Keyboard() = default;
    ~Keyboard() = default;

    int OnKeyPressed(int key);
    int OnKeyReleased(int key);

  private:
    std::unordered_map<int, bool> key_states = {
        {kForward, false},
        {kBackward, false},
        {kLeft, false},
        {kRight, false},
        {kUp, false},
        {kDown, false},
    };

    bool IsValidKey(int key);
};