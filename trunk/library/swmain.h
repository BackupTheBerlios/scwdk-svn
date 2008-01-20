/***************************************************************************
 *   Copyright (C) 2007 by Serge Lussier,,,   *
 *   bretzel@pc-desktop   *
 *                                                                         *
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
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef SWMAIN_H
#define SWMAIN_H

#include "swobject.h"
#include "swuicontrol.h"
#include "swwriter.h"
#include "swncurses.h"
#include "swdesktop.h"
#include "mutex.h"




/**
The swMain class is the main object of the program ( or application ).
The Main object is served to create and initialize the ncurses screen and
the "main" desktop onto which the toplevel windows / or desktop owned child widgets are taking into life.


	@author Serge Lussier,,, <bretzel@pc-desktop>
*/
class swMain : public swObject
{
    sigc::signal<bool, swObject*&>::accumulated< swObject::interruptible> QueryExit_;
public:

    typedef sigc::slot<bool, swObject*&> QueryExitT;
    
    
    swMain();
    swMain( const std::string& _name, int argc, char** argv );
    virtual ~swMain();
    

    swMain& operator += ( QueryExitT _f ){
        QueryExit_.connect( _f );
        return *this;
    }

    static swMain* Instance() { return swMain::_Self; }

    
    swNCurses* CursesInstance() { return _nc; }
    swDesktop* Desktop( int =0 /* Desktop # - for future use*/) { return _dsk; }
    bool QueryExit(swObject* _sender );
    virtual int RunOptions();
    virtual int Init();
    int Run();
    int PostEvent( Event* _e );
protected:

    typedef std::list< swWriter * > UQ; // Update Queu
    swNCurses* _nc;
    std::list<std::string> _args;
    swDesktop* _dsk;

    static swMain* _Self;
    std::list<Event*> _evq;
    mutex* _evq_x;
protected:
    virtual Event* ProcessEvent( Event* _ev );
    Event* DispatchEvents();
    virtual Event* _KeyInput( KeyPressEvent* Kev );
    virtual Event* _MouseEvent( MouseEvent* Mev );
    virtual Event* _MessageEvent(MessageEvent* msg );
};

#endif
