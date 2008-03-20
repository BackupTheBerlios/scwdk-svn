/***************************************************************************
 *   Copyright (C) 2007 by Serge Lussier,,,   *
 *   lussier.serge@gmail.com   *
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
#ifndef __scwdk__
#define __scwdk__



//////// Generaly used in all sources
#include <iostream>
#include <string>
//////// ---------------------------



#define gDebug std::cerr << "\ndbg trace: in function: \n" << __PRETTY_FUNCTION__ << "\n(not swObject subclass):->"
#define Debug std::cerr << "\ndbg trace: in function: \n" << __PRETTY_FUNCTION__ << "\n   NameID:[" << NameID() << "] :->"
#define NDebug std::cerr << "dbg trace: in function: \n" << __PRETTY_FUNCTION__ << "\n (not swObject subclass):->"
#define DEND std::cerr << "\ndbg trace end" <<  std::endl
#define Dbg std::cerr << "\n  :->"





#ifndef SWOBJECT_H
    //#include <swobject.h>
#endif


#ifndef _SIGC_TRACKABLE_HPP_
    #include <sigc++/sigc++.h>
#endif

#ifndef NCURSES_INC
#define NCURSES_INC
    #include <ncurses.h>
    typedef chtype TCell;
    typedef TCell* PCell;
    typedef PCell PStr;  // Just an alias to declare array of TCell.
                         // Juste un alias pour declarer un tableau de TCell.

    /// adds a flag to the ncurses bitmap components that defines the NEWLINE flag
    #define A_NEWLINE NCURSES_BITS(1UL,23)
#endif

// Defines standard ANSI commands for colors - used with the Debug macros

#define CHigh "\033[1m"
#define CReset "\033[0m"
#define CUnderline "\033[5m"

#define CRed "\033[31m"
#define CGreen "\033[32m"
#define CBrown "\033[33m"
#define CBlue "\033[34m"
#define CPurple "\033[35m"
#define CCyan "\033[36m"
#define CWhite "\033[37m"


namespace state{
    const int disable = 0;
    const int normal  = 1;
    const int active  = 2;
    const int move    = 4;
}


namespace swcolor{

    const int black = 0;
    const int red   = 1;
    const int green = 2;
    const int brown  = 3;
    const int blue = 4;
    const int purple = 5;
    const int cyan = 6;
    const int white = 7;
    const int yellow  = 3;
    // More to come
}

namespace direction{
    const int    up=1;
    const int    down=2;
    const int    left=4;
    const int    right=8;
    const int    horizontal=0x10;
    const int    vertical= 0x20;
    const int    table = horizontal|vertical;
    const int    top = 0x40;
    const int    bottom = 0x80;
    const int    topleft = top|left;
    const int    topright = top|right;
    const int    bottomleft = bottom|left;
    const int    bottomright = bottom|right;
    const int    all=0xFF;
}

namespace justify{
    const int up=1;
    const int down=2;
    const int left=4;
    const int right=8;
    const int center=0x10;
    const int middle=0x20;
}


namespace uiflags{
    const int normal  = 1;
    const int visible   = 2;
    const int max      = 4;
    const int min       = 8;
    const int wrap    = 0x10;
    const int con      = 0x20;
    const int autoscroll  = 0x40;
    const int toplevel    = 0x80;
}





#ifndef uint
typedef unsigned int uint;
#endif

#ifndef ulong
typedef unsigned long ulong;
#endif

#ifndef uchr
typedef unsigned char uchr;
#endif

#ifndef MSGDEFS_H
#include "msgdefs.h"
#endif

#endif
