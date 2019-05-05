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
#include "kooyhub.h"

#include <unistd.h>

#define PORT 5555
#define SERVER "127.0.0.1"

class KooyInternal
{
  public:
    bool start();
    bool stop();
    void addListener(const char *aTopic);
    void publish(const char *topic);

  private:
    std::shared_ptr<KooyHub> hub;
};

bool KooyInternal::start()
{
  // std::cout<<__PRETTY_FUNCTION__<<std::endl;
  hub=std::make_shared<KooyHub>(SERVER,PORT);
  hub->start();
}

void KooyInternal::publish(const char *topic)
{
  hub->publish(topic);
}

bool KooyInternal::stop()
{

}

void KooyInternal::addListener(const char *aTopic)
{

}


Kooy::Kooy()
  :d(std::make_shared<KooyInternal>())
{

}

bool Kooy::start()
{
  std::cout<<__PRETTY_FUNCTION__<<std::endl;
  return d->start();
}

bool Kooy::stop()
{
  return d->stop();
}

void Kooy::publish(const char *topic)
{
  return d->publish(topic);
}

void Kooy::addListener(const char *aTopic)
{
  return d->addListener(aTopic);
}
