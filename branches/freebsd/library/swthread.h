/***************************************************************************
 *   Copyright (C) 2005 by Serge Lussier                                   *
 *   serge@tuxadmin.net                                                    *
 *  ---------------------------------------------------------------------  *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 ***************************************************************************/
#ifndef PTHREAD_CTHREAD_H
#define PTHREAD_CTHREAD_H
#include <swobject.h>
#include <mutex.h>

class swthread : public swObject {
public:
    swthread();
        // Signal-slots handles management:
    //typedef boost::signals::connection hookhandle_t;

        //-------------------------------------------------------
        // Notifier handles and slots management:
        // Handles to notifier are the client functions slots connected to the thread object to receive
        // notifications when the thread object emits notifications at the condition set in the exec_hook
    typedef sigc::signal<bool, swthread*> TNotifyDelegator;
    typedef sigc::slot<bool, swthread*>  TNotifyDelegate;
    // -------------------------------------------------------
    // hooks to the thread exec management
    // There are 2 hooks signals : the RUN and END signals that calls the proper hook handles
    typedef sigc::signal<bool> THookDelegator;
    typedef sigc::slot<bool> THookDelegate;
    //--------------------------------------------------------
    typedef sigc::signal<void> TTerminateDelegator;
    typedef sigc::slot<void> TTerminateDelegate;

    swthread(swObject* parentobj, const std::string& nameid);
    virtual ~swthread();

    TNotifyDelegator::iterator
        operator +=( TNotifyDelegate _slot ) { return notify_.connect( _slot ); }
    THookDelegator::iterator
        operator +=( THookDelegate  _hook ) { return run_.connect( _hook); }
    TTerminateDelegator::iterator
        operator +=( TTerminateDelegate _hook) { return end_.connect( _hook ); }

    bool run();
    bool notify(){
        return notify_(this);//.emit(this);
    }
    bool exit();
    inline bool terminate(){ exit(); }

private:
friend void *thread_start_entry(void*);
    TNotifyDelegator notify_;
    THookDelegator run_;
    TTerminateDelegator end_;
    pthread_t _pthID;

};




#endif
