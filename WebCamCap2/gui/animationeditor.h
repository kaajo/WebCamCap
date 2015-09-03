#ifndef ANIMATIONEDITOR_H
#define ANIMATIONEDITOR_H

#include <QMainWindow>

#include "animationplayer.h"
#include "animation.h"

namespace Ui {
class AnimationEditor;
}

class AnimationEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit AnimationEditor(QWidget *parent = 0);
    ~AnimationEditor();

    Animation *animation() const;
    void setAnimation(Animation *animation);

public slots:
    void setFrame(int i);

    void saveAnimation();
    void editMarkerId(const QString &newId);

private:
    Ui::AnimationEditor *m_ui;

    AnimationPlayer *m_player;

    int m_currentFrameId = -1;
    Animation *m_animation = nullptr;
};

#endif // ANIMATIONEDITOR_H
