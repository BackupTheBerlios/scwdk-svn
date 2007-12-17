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
#ifndef CTUISIZE_H
#define CTUISIZE_H


#include <pxy.h>



/**
@author Serge Lussier
*/
class Size{
   
       pxy _s;
       pxy _min;
       pxy _max;
   
   public:
       Size();
       Size( const pxy& _size, const pxy& minsize, const pxy& maxsize ){
           _s = _size; _min = minsize; _max = maxsize;
       }
       Size( int w, int h){
         _s(w,h);
      }
       ;
       ~Size();
   
       void operator()(int w, int h) { _s(w,h); }
   
       int height() const { return _s.y(); }
   
       int maximum_height() const { return _max.y(); }
   
       int maximum_width() const { return _max.x(); }
       int minimum_height() const { return _min.y(); }
   
       int minimum_width() const{ return _min.x(); }
   
       int width() const { return _s.x(); }
   
       void setheight(int h )
       {
          _s.ry() = h;
       }
   
       void setwidth(int w)
       {
           _s.rx() = w;
       }
       std::string tostring() const;
   
   };




#endif
