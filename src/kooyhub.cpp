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


#include "kooyhub.h"
#include <future>
#include <unistd.h>

Subscriber::Subscriber(SubscribeListener *aListener, const std::string &aIpAddr,const int &aPort)
  :listener(aListener),ipAddress(aIpAddr),port(aPort)
{

}

void Subscriber::exec()
{
 // std::cout<<__PRETTY_FUNCTION__<<std::endl;
  runner=std::make_shared<std::thread>(&Subscriber::run,this);
}

void Subscriber::run()
{
  //std::cout<<__PRETTY_FUNCTION__<<std::endl;
}

Publisher::Publisher(PublisherListener *aListener, const std::string &aIpAddr,const int &aPort)
  :listener(aListener),ipAddress(aIpAddr),port(aPort),status(0)
{

}
Publisher::~Publisher()
{
  runner.join();
  std::cout<<__PRETTY_FUNCTION__<<std::endl;
}

void Publisher::setTopic(const std::string &aTopic)
{
  topic=aTopic;
}

void Publisher::exec()
{
  std::cout<<__PRETTY_FUNCTION__<<std::endl;
  runner=std::thread(&Publisher::run,this);
}

void Publisher::run()
{
  std::cout<<__PRETTY_FUNCTION__<<std::endl;
  sleep(3);
  if(listener)
  {
    listener->statusChanged(topic,PublishStatus::Success);
  }
  //for cleanup purpose
  status=-1;
}

KooyHub::KooyHub(const std::string &aIpAddress,const int &aPort)
  :ipAddress(aIpAddress),port(aPort)
{

}

void KooyHub::start()
{
  if(subscriber)return;
  std::cout<<__PRETTY_FUNCTION__<<std::endl;
  subscriber=std::make_shared<Subscriber>(this,ipAddress,port);
  subscriber->exec();
}

void KooyHub::stop()
{
  subscriber.reset();
}

void KooyHub::addTopic()
{

}

void KooyHub::subscriptionMonitor()
{

}

void KooyHub::publish(const std::string &aTopic)
{
  std::cout<<__PRETTY_FUNCTION__<<std::endl;

  //Cleanup the previous passive Requests
  KooyHub::publisherPoolCleanup();
  Publisher *p=new Publisher(this,ipAddress,port);
  //This is to avoid cleaing up the same topic request if it comes simultaenously
  std::string newkey=std::to_string(pubPool.size())+aTopic;
  p->setTopic(newkey);
  pubPool.insert({newkey,p});
  p->exec();
}

void KooyHub::statusChanged(const std::string &aTopic,const PublishStatus &aStatus)
{
  std::cout<<__PRETTY_FUNCTION__<<std::endl;
  if(aStatus==PublishStatus::Success)
  {
    std::cout<<aTopic<< ": Publish Successful >"<<std::endl;
  }
  else
  {
    std::cerr<<aTopic<< ": Publish Failed > "<<std::endl;
  }
  std::cout<<"Publish Pool Count : "<<pubPool.size()<<std::endl;
  return;
}

//TODO: We need to use a multimap , because if the key is same ? crash?
void KooyHub::publisherPoolCleanup()
{
  cleanupMutex.lock();
  //std::cout<<__PRETTY_FUNCTION__<<std::endl;
  for( auto it = pubPool.begin(); it != pubPool.end(); )
  {
    Publisher *p=it->second;
    if(p && !p->isActive())
    {
     // std::cout << it->first<<" is passive and to be deleted"<<std::endl;
      delete p;
      it=pubPool.erase(it);
    }
    else
    {
     // std::cout<<it->first<< "Is still active"<<std::endl;
      ++it;
    }

  }
  //std::cout<<"Publish Pool Count : "<<pubPool.size()<<std::endl;
  cleanupMutex.unlock();
}

void KooyHub::statusChanged(const SubscribeStatus &aStatus)
{

}
