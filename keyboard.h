#pragma once

#include <unordered_map>

#include <QKeyEvent>

class Keyboard {
  public:
    // Movement controls
    constexpr static int kUp = Qt::Key_T;
    constexpr static int kDown = Qt::Key_G;
    constexpr static int kForward = Qt::Key_W;
    constexpr static int kBackward = Qt::Key_S;
    constexpr static int kLeft = Qt::Key_A;
    constexpr static int kRight = Qt::Key_D;

    // Simulator controls
    constexpr static int kRestart = Qt::Key_R;
    constexpr static int kPrint = Qt::Key_P;

    Keyboard() = default;
    ~Keyboard() = default;

    int OnKeyPressed(int key);
    int OnKeyReleased(int key);

  private:
    std::unordered_map<int, bool> key_states = {
        {kForward, false}, {kBackward, false}, {kLeft, false},
        {kRight, false},   {kUp, false},       {kDown, false},
        {kRestart, false}, {kPrint, false},
    };

    bool IsValidKey(int key);
};