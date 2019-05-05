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
    void addListener(const char *aTopic,std::shared_ptr<KooyListener> &aListener);

  private:
    std::shared_ptr<KooyHub> hub;
};

bool KooyInternal::start()
{
  std::cout<<__PRETTY_FUNCTION__<<std::endl;
  hub=std::make_shared<KooyHub>(SERVER,PORT);
  hub->start();
  //testing
  sleep(1);
  std::string t1="bedroom-light";
  hub->publish(t1);
 // std::string t2="kitchen-light";
  hub->publish(t1);
}

bool KooyInternal::stop()
{

}

void KooyInternal::addListener(const char *aTopic,std::shared_ptr<KooyListener> &aListener)
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

void Kooy::addListener(const char *aTopic,std::shared_ptr<KooyListener> &aListener)
{
  return d->addListener(aTopic,aListener);
}
