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


#ifndef KOOY_H
#define KOOY_H

#include <memory>
class KooyInternal;

/**
 * @brief The KooyFragment struct
 */
struct KooyFragment
{
    char *topic;
    void *data;
};

/**
 * @brief The KooyListener class
 */
class KooyListener
{
  public:
    /**
     * @brief onFragment
     * @param aFragment
     */
    virtual void onFragment(std::shared_ptr<KooyFragment> &aFragment)=0;
};

/**
 * @brief The Kooy class
 */
class Kooy
{
  public:
    Kooy();
    /**
     * @brief start
     * @return
     */
    bool start();
    /**
     * @brief stop
     * @return
     */
    bool stop();
    /**
     * @brief addListener
     * @param aTopic
     * @param aListener
     */
    void addListener(const char *aTopic,std::shared_ptr<KooyListener> &aListener);
  private:
    std::shared_ptr<KooyInternal> d;

};

#endif // KOOY_H
