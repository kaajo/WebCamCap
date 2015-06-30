#ifndef VIRTUALROOM_H
#define VIRTUALROOM_H

#include "ivirtualroom.h"

class VirtualRoom : public IVirtualRoom
{
    Q_OBJECT
public:
    explicit VirtualRoom(QObject *parent = 0);

signals:

public slots:

};

#endif // VIRTUALROOM_H
