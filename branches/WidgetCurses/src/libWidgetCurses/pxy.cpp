/**************************************************************************
 *   Copyright (C) 2005 by Serge Lussier   *
 *   bretzel@tuxweb.homelinux.net   *
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
#include <sstream>
#include <pxy.h>
namespace wcurses
{

    pxy pxy::invalid ( ( bool ) true );


    pxy::pxy() :_x ( -1 ),_y ( -1 ),_bset ( false ),_nil ( true )
    {
    }


    pxy::pxy ( int px, int py ) : _x ( px ), _y ( py ), _bset ( true ), _nil ( false )
    {

    }


    pxy::~pxy()
    {
    }





    std::string pxy::tostring() const
    {
        //std::string S;
        std::ostringstream _if;
        _if << '(' <<_x << ',' << _y << ')';
        return _if.str();
    }





    /*!
        \fn pxy::isNull()
     */
    bool pxy::isNull()
    {
        return !_nil;
    }


    /*!
        \fn pxy::Invalidate()
     */
    void pxy::Invalidate()
    {
        _nil = true;
    }


    /*!
        \fn pxy::Enable()
     */
    void pxy::Enable()
    {
        _nil = false;
    }
}
