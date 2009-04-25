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
#include <unistd.h>

#include <Application>
#include <painter.h>
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





    /*!
        \fn wcurses::NetSpeedSensorTest::ReadNetProc()
     */
    int NetSpeedSensorTest::ReadNetProc()
    {
        FILE* proc;
        std::list<std::string> L;
        Painter* P = DCPainter();
        
        P->Clear();
        int l=0;
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
        }// Two lines skipped

        while ( fgets ( _line, 256,proc ) )
        {
            ntok = String::split ( L, _line," :" );
            if ( !ntok )
            {
                fclose ( proc );
                return -1;
            }
            P->CPosition ( 0, l++ );
            ( void* ) IFaceLine ( L );
        

        }
        P->CPosition(0,l);
        String S;
        S << "cout=" << _IFaces.size();
        P->WriteStr(S.std());
        fclose ( proc );
        return nIfaces;
        /// @todo implement me
    }




    /*!
        \fn wcurses::NetSpeedSensorTest::InitView()
     */
    bool NetSpeedSensorTest::InitView()
    {
        Debug;
        Widget::InitView();
        // Setup the loop thread
        _TimerThread = new Thread ( this, "Netspeed timer thread" );
        _TimerThread->DelegateRun ( sigc::mem_fun ( *this, &NetSpeedSensorTest::Run ) );
        // ----------------------
        //Setup the UI
        //
        // Get vertical geometry: ( Just a hint because new net dev may appears or just be removed from the enum list)
        //int nIfaces = QueryIFacesItems();
        Rect r;
        int px = Screen::Root ( "" )->Width()- 31; // right
        int py = 2;
        r.assign ( px,py, 30,10 /* test -- replace widh: nIfaces+2*/ ); // top-right of the screen offset 2,1
        SetGeometry ( r );
        //InitLayout(); // initialize layout -- direct vertical layout
        // ----------------------
        return true;
    }
    /*!
        \fn wcurses::NetSpeedSensorTest::NetIFaceInfos::Run()
        \brief Boucle centrale de NetSpeed
        \note Cette m&eacute;thode est appell&eacute;e par la m&eacute;tode du thread g&eacute;r&eacute; par NetSpeed
              Elle ne peut &ecirc;tre appel&eacute;e &agrave; partir du thread principale de l'application sinon le programme sera bloqu&eacute;
     */
    bool NetSpeedSensorTest::Run()
    {
        timespec ts;

        Debug;
        do
        {
            ts.tv_sec = 0;
            ts.tv_nsec = 999999999/2;
            int R = nanosleep ( &ts,0l );
//             int R = sleep ( 1 );
            if ( R<0 ) return false;
            Update();
        }
        while ( 1 );
    }



    /*!
        \fn wcurses::NetSpeedSensorTest::Update()
     */
    void NetSpeedSensorTest::Update()
    {
        Debug;
        ReadNetProc();
        Widget::Update();
        DEND;
    }



    /*!
        \fn wcurses::NetSpeedSensorTest::IFaceLine( const std::list<std::string>& L)
     */
    NetSpeedSensorTest::NetIFaceInfos* NetSpeedSensorTest::IFaceLine ( std::list<std::string>& L )
    {
        Debug;
        std::string S;
        int N=0;
        long ldelta;
        float fdelta;
        Painter* P;
        String str;
        P = DCPainter();
        unsigned long long rxtx;
        NetIFaceInfos* iface;
        std::map<std::string, NetIFaceInfos*>::iterator Find;

        for ( std::list<std::string>::iterator i = L.begin(); i != L.end(); i++ )
        {
            S = ( *i );
            //Dbg << N << ":" << S;
            switch ( N )
            {
                case 0:
                    
                    Find = _IFaces.find ( S );
                    if ( Find == _IFaces.end() )
                    {
                        // Create a new dev entry...for now even if no activity...
                        iface = new NetIFaceInfos;
                        _IFaces[S] = iface;
                        iface->strIFace << S;
                        iface->_totalOUT = 0l;
                        iface->_totalIN = 0l;
                        iface->_rx = 0;
                        iface->_tx = 0;
                    }
                    else iface = _IFaces[S];
                    //Dbg << "IFace Name:" << iface->strIFace.std();
                    str << iface->strIFace << ":";
                    break;
                case 1: // Bytes  Received
                    rxtx = atoll ( S.c_str() );
                    Dbg << N << ": Bytes Received,,,:" << rxtx<< ": previous val:" << iface->_totalIN;
                    ldelta = rxtx - iface->_totalIN;
                    iface->_totalIN = rxtx;
                    iface->_rx = ( ldelta*2 ) /1024;
                    //Dbg << "rx=" << iface->_rx;
                    str << " RX:" << iface->_rx;
                    /// @todo Update RX UI
                    break;
                case 9:
                    
                    rxtx = atoll( S.c_str() );
                    ldelta = rxtx - iface->_totalOUT;
                    iface->_totalOUT = rxtx;
                    iface->_tx = ( ldelta*2 ) /1024;
                    //Dbg << "tx=" << iface->_tx;
                    str << " TX:" << iface->_tx;
                    /// @todo Update TX UI
                    break;
                default: break;

            }
            N++;
        }
        Dbg << str.std();
        P->WriteStr ( str.std() );
        str.clear();


        L.clear();
        DEND;
        return iface;
    }

}


/*!
    \fn wcurses::NetSpeedSensorTest::Start()
 */
void wcurses::NetSpeedSensorTest::Start()
{
    Paint();
    Debug << " Starting and switching to new thread[" << _TimerThread->NameID() << "]"; DEND;
    _TimerThread->run(); // switch to new thread

}
