/*****************************************************************************
 *
 *
 * Created: 06-05-2019 2019 by gokul
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


#ifndef SOCKET_H
#define SOCKET_H

#include <thread>
#include <iostream>
#include <functional>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>

#define LOG(msg) std::cout << __PRETTY_FUNCTION__<< " " << msg << std::endl ;
#define ERROR(msg)std::cerr <<"[ERROR ] " <<__FILE__<<" Line: "<<__LINE__ << " "<< msg << std::endl ;

/**
 * @brief The Connection struct
 */
struct Connection
{
    std::string ipAddress;
    uint port;
};

class Socket
{
  protected:
    Connection hostInfo;
    enum Type
    {
      Tcp,
      Udp
    };

    Socket();
    bool init(const Type &aTye);
    bool bind();
    virtual bool setHost(const Connection &aHost);

  public:
    bool is_valid() const { return sock_fd != -1; }
    void setReadHandler(std::function<void(const std::string &adata)> callback);
    virtual bool write(const std::string &aData);
    virtual void close();
    virtual ~Socket();

  protected:
    int sock_fd;
    sockaddr_in m_addr;
    std::function<void(const std::string &adata)> readCallBack;
};


class TcpServerSocket:public Socket
{
  private:
    std::function<void(const Connection &)> newConnectionCallBack;
    std::thread runner;
  public:
    TcpServerSocket();
    ~TcpServerSocket();
    void setNewConnectionHandler(std::function<void(const Connection &)> callback);
    virtual bool init(const Connection &aHost);
    virtual bool start();
    virtual bool stop()
    {
      //TODO
    }

  protected:
    int read_from_client (int filedes);
    void exec();

};

class TcpClientSocket:public Socket
{
  public:
    TcpClientSocket();
    virtual bool connect(const Connection &host);
    virtual bool disconnect();
    ~TcpClientSocket();
};

class UdpSender:public Socket
{
  public:
};

class UdpClient:public Socket
{
  public:
};

#endif // SOCKET_H
