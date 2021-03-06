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
#ifndef CTUIRECT_H
#define CTUIRECT_H


#include <size.h>


/**
Class that manages and maintains rectangular geometry value.

@author Serge Lussier
*/
class Rect{
public:
   typedef std::list<Rect> list;
   typedef std::list<Rect>::iterator iterator;
   typedef std::list<Rect>::const_iterator const_iterator;

   //typedef std::stack<Rect> stack;
   typedef std::vector<Rect> array;
   typedef std::vector<Rect>::iterator array_iterator;
   typedef std::vector<Rect>::const_iterator const_array_iterator;

   Rect();
   Rect(int x1, int y1, int w, int h);
   Rect( const pxy& a, int w, int h);
   Rect( const pxy& a, const Size& _size);
   ~Rect();

   bool valid(){
      return (height() >= 1) && (width() >= 1);
   }
   bool operator !()const { return (__s.height() < 1) || (__s.width() < 1); }
   bool operator ==(const Rect& r) const{
      return __a == r.topleft() && __b==r.bottomright();
   }
   Rect operator&(const Rect& r)const  { return intersection(r); }
   Rect& operator&=(const Rect& r)  { *this = intersection(r); return *this;}
   Rect operator |(const Rect& r) { return un(r); }
   Rect operator +=(const Rect& r) { move(r.topleft()); return *this;}
   Rect operator -=(const Rect& r) { move( -r.topleft().x(), -r.topleft().y()); return *this; }
   Rect operator -=(const pxy& xy) { move( -xy.x(), -xy.y()); return *this;  }
   Rect operator +=(const pxy& xy) { move(xy); return *this;}

   const pxy& bottomright() const { return __b; }

   const pxy& topleft() const { return __a; }
   bool contains(int px, int py) const { return contains( pxy(px,py) ); }
   bool contains(  const pxy& p) const{
      return ( ( p.x() >= __a.x() ) && ( p.x() <= __b.x() ) && ( p.y() >= __a.y() ) && ( p.y() <= __b.y() ) );
   }
   Rect intersection( const Rect& ) const;
   bool axin( const Rect& ax ){
      return false;
   }


   int yr() const { return __b.y(); }

   int xr() const { return __b.x(); }

   int x() const { return __a.x(); }

   int y() const { return __a.y(); }

   void assign( int a, int b, int w, int h )
   {
      __a.rx() = a; __a.ry() = b;
      __b.rx() = __a.x() + w -1;
      __b.ry() = __a.y() + h -1;
      __s( w,h );
      valid();
   }

   void assign( const pxy& a, const pxy& b)
   {
       __a = a; __b = b;
       __s( __b.x() - __a.x() + 1, __b.y() - __a.y() +1 );
       valid();
   }
   Rect un( const Rect& r);


   void resizeto( const pxy& xy );
   void resize( const pxy& dxy);

   void moveto( const pxy& xy )
   {
      __a.rx() = xy.x();
      __a.ry() = xy.y();
      __b.rx() = __a.x() + __s.width() -1;
      __b.ry() = __a.y() + __s.width() -1;
      valid();
   }

   void move( const pxy& dxy )
   {
      __a.rx() += dxy.x();
      __a.ry() += dxy.y();
      __b.rx() += dxy.x();
      __b.ry() += dxy.y();
      // No validation check needed here...
   }

   void move( int dx, int dy ) { move( pxy( dx, dy ) ); }

   void moveto( int a, int b ) { moveto( pxy( a,b ) ); }

   int height() const  { return __s.height(); }

   const Size& size() const { return __s; }

   int width() const { return __s.width(); }
   std::string tostring() const;


    /*!
        \fn stdui::Rect::resizecenter( const pxy& dxy )
     */
    void resizecenter( const pxy& dxy ){
        resize( dxy );
        move( -1*dxy.x(), -1*dxy.y() );
    }


    /*!
        \fn stdui::Rect::bottomleft()
     */
    pxy bottomleft() const
    {
        return pxy(x(), yr());
    }

    /*!
        \fn stdui::Rect::topright()
     */
    pxy topright() const
    {
        return pxy( xr(), y() );
    }

   static Rect nil;
   static Rect _void;
private:
   pxy __a;
   pxy __b;
   Size __s;
   bool _nullb;
   friend class NWidget;
};








#endif

