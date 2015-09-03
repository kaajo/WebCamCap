#include "animationeditor.h"
#include "ui_animationeditor.h"

#include <QFileDialog>

#include "openglscene.h"

AnimationEditor::AnimationEditor(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::AnimationEditor)
{
    m_ui->setupUi(this);

    //setup animation player
    m_player = new AnimationPlayer(this);
    this->addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, m_player, Qt::Orientation::Horizontal);
    connect(m_player, &AnimationPlayer::frameChanged, this, &AnimationEditor::setFrame);

    //actions
    connect(m_ui->actionSave_Animation, &QAction::triggered, this, &AnimationEditor::saveAnimation);

    //tools
    connect(m_ui->idEdit, &QLineEdit::textEdited, this, &AnimationEditor::editMarkerId);
}

AnimationEditor::~AnimationEditor()
{
    delete m_ui;
}
Animation *AnimationEditor::animation() const
{
    return m_animation;
}

void AnimationEditor::setAnimation(Animation *animation)
{
    m_animation = animation;

    if(m_animation->length() > 0)
    {
        OpenGlScene::getInstance()->setFrame(m_animation->frames()[0]);
        m_player->setActualAnimation(m_animation);
    }
}

void AnimationEditor::setFrame(int i)
{
    if(i >= m_animation->frames().size()) return;

    const Frame &frame = m_animation->frames()[i];

    m_currentFrameId = i;

    m_ui->pointList->clear();

    for(const Marker &marker : frame.markers())
    {
        m_ui->pointList->addItem(QString::number(marker.id()));
    }
}

void AnimationEditor::saveAnimation()
{
    QString filename = QFileDialog::getSaveFileName(this,tr("Save Animation"),m_animation->name()+".fbx" , tr(".fbx Files (*.fbx)"));

    if(filename != "")
    {
        m_animation->save(filename);
    }
}

void AnimationEditor::editMarkerId(const QString &newId)
{
    if(! m_ui->pointList->currentItem()) return;

    if(m_ui->checkBox->isChecked())
    {
        for(int i = m_currentFrameId; i < m_animation->frames().size(); ++i)
        {
            Frame &frame = m_animation->frames()[i];
            if(! frame.changeMarkerId(m_ui->pointList->currentItem()->text().toInt(), newId.toInt()))
            {
                break;
            }
        }
    }
    else
    {
        Frame &frame = m_animation->frames()[m_currentFrameId];
        frame.changeMarkerId(m_ui->pointList->currentItem()->text().toInt(), newId.toInt());
    }

    OpenGlScene::getInstance()->setFrame(m_animation->frames()[m_currentFrameId]);
}

