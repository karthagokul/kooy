/*****************************************************************************
 *
 *
 * Created: 05-05-2019 2019 by gokul
 *
 * Copyright 2019 gokul. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/


#include "kooy.h"

/**
 * @brief The KooyHub class
 * The hub is the centric component which is responsible to send & recive fragments from the other nodes. The hub processes and propogate the interested topic to the upper layer
 */
class KooyHub
{
  public:
    bool start();
    bool stop();
    bool settopics();
};


class KooyInternal
{
  public:
    bool start();
    bool stop();
    void addListener(const char *aTopic,std::shared_ptr<KooyListener> &aListener);
};

bool KooyInternal::start()
{

}

bool KooyInternal::stop()
{

}

void KooyInternal::addListener(const char *aTopic,std::shared_ptr<KooyListener> &aListener)
{

}


Kooy::Kooy()
  :d(std::make_unique<KooyInternal>())
{

}

bool Kooy::start()
{
  return d->start();
}

bool Kooy::stop()
{
  return d->stop();
}

void Kooy::addListener(const char *aTopic,std::shared_ptr<KooyListener> &aListener)
{
  return d->addListener(aTopic,aListener);
}
