/*****************************************************************************
 *
 *
 * Created: 04-05-2019 2019 by gokul
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


#include <iostream>
#include "kooy.h"
#include <unistd.h>
#include "socket.h"
#include <functional>

using namespace std;
class EventHandler
{
    public:
        void addHandler(std::function<void(int)> callback)
        {
            cout << "Handler added..." << endl;
            // Let's pretend an event just occured
            callback(1);
        }
};

class MyClass
{
    public:
        MyClass();

        // Note: No longer marked `static`, and only takes the actual argument
        void Callback(int x);
    private:
        int private_x;
        EventHandler *handler;
};

MyClass::MyClass()
{
    handler=new EventHandler();
    using namespace std::placeholders; // for `_1`

    private_x = 5;
    handler->addHandler(std::bind(&MyClass::Callback, this, _1));
}

void MyClass::Callback(int x)
{
    // No longer needs an explicit `instance` argument,
    // as `this` is set up properly
    cout << x + private_x << endl;
}


void onnewconnection(const Connection &c)
{
  std::cout<<c.ipAddress<<"oye, new connection";
}

int main()
{
 // Kooy k;
  //k.start();
//  while(1)
 // {
   // std::string t1="reciver-light";
   // k.publish(t1.c_str());
 //   sleep(5);
//  }
 // MyClass m;

  TcpServerSocket serverSocket;
  using namespace std::placeholders; // for `_1`
  serverSocket.setNewConnectionHandler(std::bind(&onnewconnection,_1));
  //class memober var
   //serverSocket.setNewConnectionHandler(std::bind(&onnewconnection,this,_1));
  Connection c;
  c.ipAddress="127.0.0.1";
  c.port=5050;
  if(serverSocket.init(c))
  {
    if(serverSocket.start())
    {
      std::cout<<"Server is Running "<<std::endl;
    }
  }

  sleep(2);

  TcpClientSocket cl;
  cl.connect(c);
 // cl.write("hello");
  cl.disconnect();

  while(1)
  {

  }

  return 0;
}

