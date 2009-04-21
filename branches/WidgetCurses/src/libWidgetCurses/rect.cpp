/***************************************************************************
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
 *   59 Temple Place - Suite 330, Boston, MA  -1307, USA.             *
 ***************************************************************************/
#include <Rect>
#include <String>

namespace wcurses
{

    Rect Rect::nil ( 0,0,-1,-1 );
    Rect Rect::_void ( 0,0,-1,-1 );

    Rect::Rect()
    {
        valid();
    }

    Rect::Rect ( int x1, int y1, int w, int h )
    {
        __a ( x1, y1 );
        __b ( x1 + w -1, y1 + h -1 );
        __s ( w,h );
        valid();
    }

    Rect::Rect ( const pxy& a, int w, int h )
    {
        __a ( a.x(), a.y() );
        __b ( __a.x() + w -1, __a.y() + h -1 );
        __s ( w,h );
        valid();
    }

    Rect::Rect ( const pxy& a, const Size& _size )
    {
        Size sz = _size;
        __a ( a.x(), a.y() );
        __s ( sz.Width(), sz.Height() );
        __b ( a.x() + __s.Width() -1, a.y() + __s.Height() -1 );
        valid();
    }

    Rect::~Rect()
    {
    }



    Rect Rect::intersection ( const Rect& r ) const
    {
        // Intersect self with <r>
        Rect R = Rect::nil;
        pxy a,b;
        a ( x() <= r.x() ? r.x() : x(), y() <= r.y() ? r.y() : y() );
        b ( xr() <= r.xr() ? xr() : r.xr(), yr() <= r.yr() ? yr() : r.yr() );

        bool in = contains ( a ) || contains ( b );
        if ( !in ) return R;
        R.assign ( a,b );
        return R;
    }


    Rect Rect::un ( const Rect& r )
    {
        int xx1,xx2,yy1,yy2;
        Rect R;
        xx1 = r.x() < x() ? r.x() : x();
        xx2 = r.xr() > xr() ? r.xr() : xr();
        yy1 = r.y() < y() ? r.y() : y();
        yy2 = r.yr() > yr() ? r.yr() : yr();

        R.assign ( xx1, yy1, xx2, yy2 );
        return R;

    }


    void Rect::resizeto ( const pxy& xy )
    {
        assign ( x(), y(), xy.x(), xy.y() );
    }


    void Rect::resize ( const pxy& dxy )
    {
        assign ( x(), y(), dxy.x(), dxy.y() );
    }


    std::string Rect::tostring() const
    {
        String s;
        s << "rect[" << x() << ", " << y() << ", : " << Width() << '/' << xr() << ", " <<  Height() << '/' << yr() << ']';
        return s.std();
    }


}
