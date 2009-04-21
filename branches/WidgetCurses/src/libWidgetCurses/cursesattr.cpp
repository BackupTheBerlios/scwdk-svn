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
#include <Style>


namespace wcurses
{
    TCell Style::ptbl[8][8];
    Style Style::nul;

    static String S;
    std::map< std::string, int> Style::ColorNames;

    Style::Style()
    {
    }


    Style::Style ( const Style& A )
    {
        *this  = A;
    }


    Style::Style ( int bg, int fg, TCell attr )
    {
        _d = 0;
        SetColors ( bg,fg );
        AddAttr ( attr );
    }


    Style::~Style()
    {
    }


    /*!
        \fn Style::AddAttr( TCell a )
     */
    void Style::AddAttr ( TCell a )
    {
        _d|=a;
    }


    /*!
        \fn Style::SetBgColor( int bg )
     */
    void Style::SetBgColor ( int bg )
    {
        int F = FgColor();
        _d = ( _d & ~A_COLOR ) | Style::ptbl[bg][F];
    }


    /*!
        \fn Style::SetFgColor( int fg )
     */
    void Style::SetFgColor ( int fg )
    {
        int B = BgColor();
        _d = ( _d & ~A_COLOR ) | Style::ptbl[B][fg];
    }


    /*!
        \fn Style::SetNewAttr( TCell a )
     */
    void Style::SetNewAttr ( TCell a )
    {
        _d &= ( ~Attributes() ) | a;
    }


    /*!
        \fn Style::operator << ( Style::Color C )
     */
    Style& Style::operator << ( Style::Color C )
    {
        int B= ( C.bg >=0 ) && ( C.bg <= 64 ) ? C.bg : 1;
        int F= ( C.fg >=0 ) && ( C.fg <= 64 ) ? C.fg : 7;
        _d = ( _d & ~A_COLOR ) | Style::ptbl[B][F];
    }


    /*!
        \fn Style::IsSet( TCell )
     */
    bool Style::IsSet ( TCell d )
    {
        return ( _d&d ) != 0;
    }

    int Style::init()
    {
        gDebug;
        //Curses* nc = Curses::Instance();

        /*     if(! has_colors() ){
                 Dbg << " has_colors() failed to be true...";
                 DEND;
                 return 0;
             }*/
        //start_color(); Already done in Curses::Init()
        int BG,FG, N=1;
        for ( BG=0; BG < 8; BG++ )
            for ( FG = 0; FG < 8; FG++ )
                init_pair ( N++, FG, BG );
        N = 1;
        for ( BG=0; BG < 8; BG++ )
            for ( FG = 0; FG < 8; FG++ )
                Style::ptbl[BG][FG] = COLOR_PAIR ( N++ );


        ColorNames["BLACK"] = colors::black;
        ColorNames["RED"] = colors::red;
        ColorNames["GREEN"] = colors::green;
        ColorNames["YELLOW"] = colors::yellow;
        ColorNames["BLUE"]=colors::blue;
        ColorNames["PURPLE"]=colors::purple;
        ColorNames["CYAN"]=colors::cyan;
        ColorNames["WHITE"]=colors::white;

        return 1;
    }


    Style::operator const char*()
    {
        S = String();
        S << "Style: ";
        if ( _d & Style::underline ) S << "|Underline";
        if ( _d & Style::bold ) S << "|BOLD";
        if ( _d & Style::blink ) S << "|Blink";
        if ( _d & Style::reverse )  S << "|Reverse";
        int f = FgColor();
        int b = BgColor();
        S << "|(" << b << ',' << f <<')';
        return S();
        //return str.c_str();
    }


}
