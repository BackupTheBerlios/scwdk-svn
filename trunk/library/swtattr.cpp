/***************************************************************************
 *   Copyright (C) 2007 by Serge Lussier   *
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
#include "swtattr.h"
#include "exstring.h"
#include "swncurses.h"

TCell swTAttr::ptbl[8][8];



String S;

std::map< std::string, int> swTAttr::ColorNames;

swTAttr::swTAttr()
{
}


swTAttr::swTAttr(const swTAttr& A)
{
    *this  = A;
}


swTAttr::swTAttr(int bg, int fg, TCell attr )
{
    _d = 0;
    SetColors( bg,fg );
    AddAttr(attr);
}


swTAttr::~swTAttr()
{
}


/*!
    \fn swTAttr::AddAttr( TCell a )
 */
void swTAttr::AddAttr( TCell a )
{
    _d|=a;
}


/*!
    \fn swTAttr::SetBgColor( int bg )
 */
void swTAttr::SetBgColor( int bg )
{
    int F = FgColor();
    _d = (_d & ~A_COLOR) | swTAttr::ptbl[bg][F];
}


/*!
    \fn swTAttr::SetFgColor( int fg )
 */
void swTAttr::SetFgColor( int fg )
{
    int B = BgColor();
    _d = (_d & ~A_COLOR) | swTAttr::ptbl[B][fg];
}


/*!
    \fn swTAttr::SetNewAttr( TCell a )
 */
void swTAttr::SetNewAttr( TCell a )
{
    _d &= (~Attributes()) | a;
}


/*!
    \fn swTAttr::operator << ( swTAttr::Color C )
 */
swTAttr& swTAttr::operator << ( swTAttr::Color C )
{
    int B=(C.bg >=0) && (C.bg <= 64) ? C.bg : 1;
    int F=(C.fg >=0) && (C.fg <= 64) ? C.fg : 7;
    _d = (_d & ~A_COLOR) | swTAttr::ptbl[B][F];
}


/*!
    \fn swTAttr::IsSet( TCell )
 */
bool swTAttr::IsSet( TCell d )
{
    return (_d&d) != 0;
}

int swTAttr::init()
{
    gDebug;
    swNCurses* nc = swNCurses::Instance();
        
/*     if(! has_colors() ){
         Dbg << " ncurses::has_colors() failed to be true...";
         DEND;
         return 0;
     }*/
    //start_color(); Already done in swNCurses::Init()
    int BG,FG, N=1;
    for(BG=0; BG < 8; BG++)
        for( FG = 0; FG < 8; FG++)
            init_pair(N++, FG, BG);
    N = 1;
    for(BG=0; BG < 8; BG++)
        for( FG = 0; FG < 8; FG++)
            swTAttr::ptbl[BG][FG] = COLOR_PAIR(N++);
            

    ColorNames["BLACK"] = swcolor::black;
    ColorNames["RED"] = swcolor::red;
    ColorNames["GREEN"] = swcolor::green;
    ColorNames["YELLOW"] = swcolor::yellow;
    ColorNames["BLUE"]=swcolor::blue;
    ColorNames["PURPLE"]=swcolor::purple;
    ColorNames["CYAN"]=swcolor::cyan;
    ColorNames["WHITE"]=swcolor::white;
    
    return 1;
}


swTAttr::operator const char*(){
    S = String();
    S << "swTAttr: ";
    if( _d & swTAttr::underline ) S << "|Underline";
    if( _d & swTAttr::bold      ) S << "|BOLD";
    if( _d & swTAttr::blink   ) S << "|Blink";
    if( _d & swTAttr::reverse)  S << "|Reverse";
    int f = FgColor();
    int b = BgColor();
    S << "|(" << b << ',' << f <<')';
    return S();
    //return str.c_str();
}

