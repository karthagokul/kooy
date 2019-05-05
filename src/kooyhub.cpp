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

//Heartbeat
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <cstring>

#define HEARTBEAT_PORT 5555
#define HEARTBEAT_SERVER "239.255.255.250"

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
  //temporary
  std::cout<<__PRETTY_FUNCTION__<<std::endl;
  unsigned  int sock, length, fromlen, n;
  struct sockaddr_in server;
  struct sockaddr_in from;
  char buf[1024];
  sock=socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0)return ;
  length = sizeof(server);
  bzero(&server,length);
  server.sin_family=AF_INET;
  server.sin_addr.s_addr=INADDR_ANY;
  server.sin_port=htons(HEARTBEAT_PORT);
  u_int yes = 1;
  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*) &yes, sizeof(yes)) < 0)
    return ;
  struct ip_mreq mreq;
  mreq.imr_multiaddr.s_addr = inet_addr(HEARTBEAT_SERVER);
  mreq.imr_interface.s_addr = htonl(INADDR_ANY);
  if (
      setsockopt(
        sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*) &mreq, sizeof(mreq)
        ) < 0
      ){
    perror("setsockopt");
    return ;
  }
  if (bind(sock,(struct sockaddr *)&server,length)<0)
    return ;
  fromlen = sizeof(struct sockaddr_in);
  while (1) {
    n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
    if (n < 0) return ;
    printf("Recived %s\n",buf);
    bzero(&buf,0);
  }
  //temporary end
}

Publisher::Publisher(PublisherListener *aListener, const std::string &aIpAddr,const int &aPort)
  :listener(aListener),ipAddress(aIpAddr),port(aPort),status(0)
{

}
Publisher::~Publisher()
{
  runner.join();
  //std::cout<<__PRETTY_FUNCTION__<<std::endl;
}

void Publisher::setTopic(const std::string &aTopic)
{
  //std::cout<<"Setting "<<aTopic<<std::endl;
  topic=aTopic;
}

void Publisher::exec()
{
  //std::cout<<__PRETTY_FUNCTION__<<std::endl;
  runner=std::thread(&Publisher::run,this);
}

void Publisher::run()
{
  //std::cout<<__PRETTY_FUNCTION__<<std::endl;
  //Temporary
  int sock;
  size_t length, n;
  struct sockaddr_in server, from; // IP Addressing(ip, port, type) Stuff
  struct hostent *hp; // DNS stuff
  char buffer[256];
  sock= socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0) return ;
  u_int yes = 1;
  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*) &yes, sizeof(yes)) < 0)
    return ;
  server.sin_family = AF_INET;
  hp = gethostbyname(HEARTBEAT_SERVER);
  if (hp==0) return ;
  bcopy((char *)hp->h_addr, (char *)&server.sin_addr, hp->h_length);
  server.sin_port = htons(HEARTBEAT_PORT);
  length=sizeof(struct sockaddr_in);
  bzero(buffer,256);
  strcpy(buffer,topic.c_str());
  n=sendto(sock,buffer,strlen(buffer),0, (struct sockaddr*)&server,length);
  if (n < 0) return ;
  //temporary end
  //
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
  //std::cout<<__PRETTY_FUNCTION__<<std::endl;

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
  //  std::cout<<__PRETTY_FUNCTION__<<std::endl;
  if(aStatus==PublishStatus::Success)
  {
    std::cout<<aTopic<< ": Publish Successful >"<<std::endl;
  }
  else
  {
    std::cerr<<aTopic<< ": Publish Failed > "<<std::endl;
  }
  //std::cout<<"Publish Pool Count : "<<pubPool.size()<<std::endl;
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
