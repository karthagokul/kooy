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


#ifndef KOOYHUB_H
#define KOOYHUB_H

#include <thread>
#include <map>
#include <iostream>
#include <mutex>

class SubscribeListener
{
public:
    enum SubscribeStatus
    {
      Failed,
      Success
    };
     virtual void statusChanged(const SubscribeStatus &aStatus)=0;
};

class Subscriber
{
  public:
    Subscriber(SubscribeListener *aListener, const std::string &aIpAddr,const int &aPort);
    void exec();
  private:
    void run();
  private:
    //The thread which is listening from the socket (UDP for now)
    std::shared_ptr<std::thread> runner;
    std::string ipAddress;
    int port;
    SubscribeListener *listener;
};

/**
 * @brief The PublishStatus enum
 */
enum PublishStatus
{
    Failed,
    Success
};

class PublisherListener
{
  public:
    /**
     * @brief statusChanged
     * @param aTopic
     * @param aStatus
     */
    virtual void statusChanged(const std::string &aTopic,const PublishStatus &aStatus)=0;
};

/**
 * @brief The Publisher class
 */
class Publisher
{
  public:
    /**
     * @brief Publisher
     * @param aListener
     * @param aIpAddr
     * @param aPort
     */
    Publisher(PublisherListener *aListener, const std::string &aIpAddr,const int &aPort);
    ~Publisher();
    void setTopic(const std::string &aTopic);
    void exec();
    bool isActive()
    {
      return 0?true:false;
    }

  private:
    void run();
  private:
    std::thread runner;
    PublisherListener* listener;
    std::string topic;
    std::string ipAddress;
    int port;
    bool status=-1;
};

/**
 * @brief The KooyHub class
 * The hub is the centric component which is responsible to send & recive fragments from the other nodes. The hub processes and propogate the interested topic to the upper layer
 */
class KooyHub:public PublisherListener,public SubscribeListener
{
  public:
    KooyHub(const std::string &aIpAddr,const int &aPort);
    void start();
    void stop();
    void addTopic();
    void publish(const std::string &aTopic);

  private:
    void publisherPoolCleanup();
    void subscriptionMonitor();
    void statusChanged(const SubscribeStatus &aStatus);
    void statusChanged(const std::string &aTopic,const PublishStatus &aStatus);

  private:

    //At a single point of time , there is a chance that muliple request can come so using a thread pool approach
    std::map<std::string,Publisher* > pubPool;
    std::mutex cleanupMutex;
    std::shared_ptr<Subscriber> subscriber;
    std::string ipAddress;
    int port;
};


#endif // KOOYHUB_H
