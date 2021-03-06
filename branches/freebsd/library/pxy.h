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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef CTUIPXY_H
#define CTUIPXY_H

#include <vector>
#include <list>





/**
@author Serge Lussier
*/



 class pxy{
 public:
    typedef std::list<pxy> list;
    typedef std::list<pxy>::iterator iterator;
    typedef std::list<pxy>::const_iterator const_iterator;

    //typedef std::stack<pxy> stack;
    typedef std::vector<pxy> array;
    typedef std::vector<pxy>::iterator array_iterator;
    typedef std::vector<pxy>::const_iterator const_array_iterator;

     pxy();

     pxy(int px, int py);

     pxy(bool){ _nil = true; }

     ~pxy();

     int& ry() { return _y; }

     int x() const { return _x; }

     int y() const { return _y; }

     int& rx() { return _x; }
    
     void operator-=(const pxy& xy) { _x -= xy.x(); _y -= xy.y(); }

     void operator+=(const pxy& xy) { _x += xy.x(); _y += xy.y(); }
     pxy operator+(const pxy& p)const  { return pxy( p.x() + _x, p.y() + _y ); }

     pxy operator-(const pxy& p)const  { return pxy( _x - p.x(), _y - p.y()); }

     void operator()(int nx, int ny) { _x = nx; _y = ny; _bset = true; _nil = false; }

     void move(int dx, int dy) { _x += dx; _y += dy; }
     std::string tostring() const;

     bool operator == (const pxy& xy ) const { return _x == xy._x && _y == xy._y; }

     void operator=(const pxy& xy) { _x = xy.x(); _y = xy.y(); _bset = xy._bset; _nil = !xy._bset; }

     bool operator !=(const pxy& xy) { return xy.x() != _x || xy.y() != _y; }

     bool operator <(const pxy& xy) const { return (_x<xy._x) || (_y < xy._y ); }
     
    bool isNull();
    void Invalidate();
    void Enable();

    /*!
        \fn pxy::operator !()
     */
    bool operator !()
    {
        return _nil;
    }
    static pxy invalid;
 //private: -- Make data public for faster access to avoid function calls overhead!
     int _bset;
     int _x;
     int _y;
protected:
    bool _nil;
 };


#endif
