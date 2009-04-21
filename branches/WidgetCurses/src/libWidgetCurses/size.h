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


#include <xyCoords>


namespace wcurses
{


    /**
    @author Serge Lussier
    */
    class Size
    {

            pxy _s;
            pxy _min;
            pxy _max;

        public:
            Size();
            Size ( const pxy& _size, const pxy& minsize, const pxy& maxsize )
            {
                _s = _size; _min = minsize; _max = maxsize;
            }
            Size ( int w, int h )
            {
                _s ( w,h );
            }

            ~Size();

            void operator() ( int w, int h ) { _s ( w,h ); }
            void operator() ( int w, int h, int mdx, int mdy, int xdx, int xdy )
            {
                _s ( w,h );
                _min( mdx, mdy);
                _max( xdx, xdy );
            }

            int Height() const { return _s.y(); }
            int Width() const { return _s.x(); }

            int MaximumHeight() const { return _max.y(); }
            int MaximumWidth() const { return _max.x(); }

            int MinimumHeight() const { return _min.y(); }
            int MinimumWidth() const{ return _min.x(); }

            void SetMaximumHeight(int dy) { _max.ry()=dy; }
            void SetMaximumWidth(int dx)  { _max.rx()=dx; }
            void SetMinimumHeight(int dy) { _min.ry()=dy; }
            void SetMinimumWidth(int dx) { _min.rx()=dx; }

            void SetMaximum( const pxy& xy )
            {
                _max.rx() = xy.x();
                _max.ry() = xy.y();
            }

            void SetMinimum( const pxy& xy )
            {
                _min.rx() = xy.x();
                _min.ry() = xy.y();
            }



            void SetHeight ( int h )
            {
                _s.ry() = h;
            }

            void SetWidth ( int w )
            {
                _s.rx() = w;
            }
            std::string tostring() const;
            /*template<typename SZ> unsigned long Area(){
                return Width() * sizeof(SZ) * Height();
            }
            */
            unsigned long Area(){
                return _s._x* _s._y;
            }


    };



}
#endif
