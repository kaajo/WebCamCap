#ifndef ANIMATIONPLAYER_H
#define ANIMATIONPLAYER_H

#include <QDockWidget>

class QTimer;
class Animation;

namespace Ui {
class AnimationPlayer;
}

class AnimationPlayer : public QDockWidget
{
    Q_OBJECT

public:
    explicit AnimationPlayer(QWidget *parent = 0);
    ~AnimationPlayer();

    void setActualAnimation(Animation *actualAnimation);

signals:
    void frameChanged(int i);

private slots:
    void play();
    void nextFrame();
    void pause();
    void next();
    void previous();
    void seek(int position);

private:
    Ui::AnimationPlayer *m_ui = nullptr;

    bool m_playing = false;

    Animation *m_animation = nullptr;
};

#endif // ANIMATIONPLAYER_H
