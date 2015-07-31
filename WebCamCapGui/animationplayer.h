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

    Animation *actualAnimation() const;
    void setActualAnimation(Animation *actualAnimation);

private slots:
    void play();
    void pause();
    void seek(int position);

private:
    Ui::AnimationPlayer *m_ui = nullptr;

    QTimer *m_animationTimer = nullptr;
    Animation *m_actualAnimation = nullptr;
};

#endif // ANIMATIONPLAYER_H
