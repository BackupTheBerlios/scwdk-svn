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
#ifndef SWNCURSES_H
#define SWNCURSES_H

#include <swobject.h>
#include "size.h"


#include "events.h"
#define __no_export static

//using linuxdl;
/**
This is the ncurses loader class.


	@author Serge Lussier,,, <bretzel@pc-desktop>
*/

class swNCurses : public swObject
{
public:
    swNCurses(swObject* swParent);
    swNCurses();
    virtual ~swNCurses();

    const std::string& NameID();
    int Init();

    /*!
        \fn swNCurses::Height()
     */
    int Height() { return _scrSize.height(); }
    /*!
        \fn swNCurses::Width()
     */
    int Width(){ return _scrSize.width(); }
//     linuxdl::LinuxDL D;
    void CursesAbort( int );
    void CursesAbort( const std::string& _erstr );
    WINDOW* ScrWin;
    
    int Finish();
    static swNCurses* Self;
    static swNCurses* Instance() { return swNCurses::Self; }


    bool _CanExit( swObject*& _sender );
    Event* WaitEvent();
protected:
    Size _scrSize;
    bMeta;

};



#endif
