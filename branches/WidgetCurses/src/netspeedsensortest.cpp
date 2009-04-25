/***************************************************************************
 *   Copyright (C) 2009 by Serge Lussier,,,   *
 *   tuxadmin.dev@gmail.com   *
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
#include "netspeedsensortest.h"
#include <stdio.h>
#include <list>
#include <string>

namespace wcurses
{

    NetSpeedSensorTest::NetSpeedSensorTest ( Widget* Parent ) : Widget ( Parent )
    {
    }


    NetSpeedSensorTest::NetSpeedSensorTest ( Object* Parent, unsigned int Flags, const char* NameID ) : Widget ( Parent, Flags, NameID )
    {
    }


    NetSpeedSensorTest::~NetSpeedSensorTest()
    {
    }


}


/*!
    \fn wcurses::NetSpeedSensorTest::ReadNetProc()
 */
int wcurses::NetSpeedSensorTest::ReadNetProc()
{
    FILE* proc;
    std::list<std::string> L;
    
    proc = fopen ( "/proc/net/dev","r" );
    int ntok,nIfaces=0;
    if ( !proc ) return -1;
    for ( int x = 0; x<2; x++ )
    {
        if ( !fgets ( _line, 256, proc ) )
        {
            fclose ( proc );
            return -1;
        }
    }
    while(fgets(_line, 256,proc)){
        ntok = String::split(L, _line," :");
        if(!ntok){
            fclose(proc);
            return -1;
        }
        
        
    }
    fclose(proc);
    return nIfaces;
    /// @todo implement me
}


/*!
    \fn wcurses::NetSpeedSensorTest::_TimeOut()
 */
bool wcurses::NetSpeedSensorTest::_TimeOut()
{
    /// @todo implement me
}


/*!
    \fn wcurses::NetSpeedSensorTest::InitView()
 */
bool wcurses::NetSpeedSensorTest::InitView()
{
    Widget::InitView();
    // Setup the loop thread 
    _TimerThread = new Thread(this, "Netspeed timer thread");
    (*_TimerThread) += sigc::mem_fun(*this, &NetSpeedSensorTest::Run());
    // ----------------------
    // Setup the UI
    //
    // Get vertical geometry:
    // int nIfaces = QueryIFacesItems();
    // Rect r;
    // int px = Application::GetScreen("")->Width()- 31; // right
    // int py = 2;
    // r.assign(px,py, 30,nIfaces+2); top-right of the screen offset 2,1
    // SetGeometry(r);
    // InitLayout() // initialize layout -- direct vertical layout
    // ----------------------
    _TimerThread->run(); // switch to new thread
    return true;
}
