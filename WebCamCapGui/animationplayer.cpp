#include "animationplayer.h"
#include "ui_animationplayer.h"

#include "animation.h"

#include <QTimer>

AnimationPlayer::AnimationPlayer(QWidget *parent) :
    QDockWidget(parent),
    m_ui(new Ui::AnimationPlayer)
{
    m_ui->setupUi(this);
}

AnimationPlayer::~AnimationPlayer()
{
    delete m_ui;
}

void AnimationPlayer::play()
{

}

void AnimationPlayer::pause()
{

}

void AnimationPlayer::seek(int position)
{

}

Animation *AnimationPlayer::actualAnimation() const
{
    return m_actualAnimation;
}

void AnimationPlayer::setActualAnimation(Animation *actualAnimation)
{
    m_actualAnimation = actualAnimation;
}

