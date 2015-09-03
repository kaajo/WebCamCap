#include "animationplayer.h"
#include "ui_animationplayer.h"

#include "animation.h"
#include "openglscene.h"

#include <QTimer>

AnimationPlayer::AnimationPlayer(QWidget *parent) :
    QDockWidget(parent),
    m_ui(new Ui::AnimationPlayer)
{
    m_ui->setupUi(this);

    connect(m_ui->animationSlider, &QSlider::valueChanged, this, &AnimationPlayer::seek);
    connect(m_ui->playButton, &QPushButton::pressed, this, &AnimationPlayer::play);
    connect(m_ui->pauseButton, &QPushButton::pressed, this, &AnimationPlayer::pause);
    connect(m_ui->nextButton, &QPushButton::pressed, this, &AnimationPlayer::next);
    connect(m_ui->prevButton, &QPushButton::pressed, this, &AnimationPlayer::previous);
}

AnimationPlayer::~AnimationPlayer()
{
    delete m_ui;
}

void AnimationPlayer::play()
{
    if(m_playing) return;

    m_playing = true;
    nextFrame();
}

void AnimationPlayer::nextFrame()
{
    int thisFrame = m_ui->animationSlider->value()+1;

    m_ui->animationSlider->setValue(thisFrame);

    if(m_ui->animationSlider->value() == m_ui->animationSlider->maximum())
    {
        pause();
        return;
    }

    if(m_playing)
    {
        QTimer::singleShot(m_animation->frames()[thisFrame+1].elapsedTime(), this, &AnimationPlayer::nextFrame);
    }
}

void AnimationPlayer::pause()
{
    m_playing = false;
}

void AnimationPlayer::next()
{
    m_ui->animationSlider->setValue(m_ui->animationSlider->value()+1);
}

void AnimationPlayer::previous()
{
    m_ui->animationSlider->setValue(m_ui->animationSlider->value()-1);
}

void AnimationPlayer::seek(int position)
{
    OpenGlScene::getInstance()->setFrame(m_animation->frames()[position]);

    emit frameChanged(position);
}

void AnimationPlayer::setActualAnimation(Animation *actualAnimation)
{
    m_animation = actualAnimation;
    m_ui->animationSlider->setMaximum(actualAnimation->frames().size()-1);
}

