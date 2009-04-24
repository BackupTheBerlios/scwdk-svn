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
#ifndef SWWRITER_H
#define SWWRITER_H


#include <cursesattr.h>
#include "exstring.h"
#include <widget.h>

#include <rect.h>
#include <list>

namespace wcurses
{

    template <typename ioDC>
    class PainterDC : public Object
    {
        public:
            typedef std::list<PainterDC<ioDC>* > List;
            //typedef std::list<PainterDC<>* >::iterator Iterator;

            PainterDC<ioDC>() : Object ( 0l ) {_sz = Size ( 0,0 ); _data= 0l; }
            PainterDC<ioDC> ( Object* parent, const Size& s ) : Object ( parent ), _sz ( s ) {_data = 0l;}
            ~PainterDC() { if ( _data ) delete [] _data; }
            bool Create ( const Size& S = Size ( 0,0 ) )
            {
                if ( S.Width() )
                {
                    _sz = S;
                    _data = new ioDC[_sz.Area() + _sz.Width() * sizeof ( ioDC ) ];
                }
                return true;
            }
            void Render(const xyCoords& xy);

        private:
            Size    _sz;
            ioDC*   _data;

    };
    /**
    The Painter object is used such as a "Paint device on bitmaps"  in graphics user interface ( X, M$-Windows ,etc..) - but in the simple scwdk ways. UiControls create a Painter instance, composed of the uicontrol's data parameters through its StartWrite(...)/DoneWrite(...) Mandatory methods - no Painter object can be created from other classes. I.E.: Only Widget ( not even Widget subclasses ) can create an instance of Painter


        @author Serge Lussier,,, <bretzel@pc-desktop>
    */
    class Painter
    {
        public:
            enum RTMode { plain=0, text };
            Painter();
            Painter ( Widget* swOwner, uint _flags, const char* _nameID );
            Painter ( Widget* swOwner, PStr _scriobuf, const Rect& _initialSubRect = Rect::_void );
            virtual ~Painter();

            //Widget* Control() { return _owner; }

            //
            // Cursor control methods
            //
            pxy CPosition ( int x=-1, int y=-1 );
            pxy CPosition ( pxy xy=pxy::invalid );
            pxy operator + ( pxy xy );
            pxy operator ++();
            Painter& operator << ( Painter::RTMode mode )
            {
                _rtmode = mode;
                return *this;
            }

            Painter& operator << ( const pxy& xy )
            {
                CPosition ( xy );
                return *this;
            }



            //
            // Peek Operations
            //
            /*!
                   \fn Painter::Position( const pxy& xy=pxy::invalid )
                */
            PStr Position ( const pxy& xy=pxy::invalid )
            {
                pxy C = xy;
                PStr P;
                
                if ( !C ) C = _c;
                if ( !C )
                {
                    
                    std::cout << __PRETTY_FUNCTION__ << "\n\r \033[0;31mPainter working pxy" << C.tostring() << " is invalid!\r\033[0m" << std::endl;
                    abort();
                    return 0l;
                }
                C += _r.topleft();
                if ( ! _r.contains ( C ) )
                {
                    
                    Dbg << "ABORTING!";DEND;
                    abort();
                    return 0l;
                }
                P = _ioscr + ( C._y * _owner->Width() + C._x );
                if ( !P )
                {
                    
                    abort();
                }
                return P;
            }

            /*!
                   \fn Painter::Seek( const pxy& xy=pxy::invalid )
                   \brief exactly the same as @see Position - actually calls the Position method.
            */
            PStr Seek ( const pxy& xy=pxy::invalid ) { return Position ( xy ); }
            /*!
                \fn Painter::DC()
             */
            PStr DC() { return _ioscr; }


            //
            // Text writing operations
            //
            template<typename argt> Painter& operator << ( argt __arg )
            {
                std::ostringstream os;
                os << __arg;
                WriteStr ( os.str() );
                return *this;
            }



            Painter& operator << ( PStr );
            Painter& operator << ( String );

            int WriteStr ( const std::string& _str );

            //
            // line drawing ( box, line.. )
            //

            int AscLine ( const pxy& xy, int ln, int dir, const Style& A );
            int DrawFrame (  const Style& A,const Rect& r=Rect::nil);
            void JoinAcs ( const pxy& xy, int dir, const Style& A );

            //
            // Blocks IO operations  - in additions to the uiControl interface
            //

            //
            // Text Attributes settings operation
            //
            int Clear();
            int WriteRT ( const char* );
            //int WriteRT( const String& S ){ return WriteRT(S.std().c_str());}
            int WritePStr ( PStr );

            /*!
                \fn Painter::Geometry()
             */
            Rect Geometry()    { return _r; }

            /*!
                \fn Painter::UiControl()
             */
            Widget* UiControl() { return _owner; }

            /*!
                \fn Painter::SetGeometry( const Rect& r )
             */
            void SetGeometry ( const Rect& r ) { _r = r; }

            //
            // Scrolling areas operations
            //


            //
            // Static public methods for direct writing into the ncurses screen ...
            //
            void DrawFrame3D ( const Rect& r, const Style& A );
            int Fill ( const Rect& r, Style A, chtype C );
            bool CopyWidget ( const pxy& xyOffset, Painter* ChildPainter, const Rect& childRect );

            /*!
                \fn wcurses::Painter::SetStyle( const Style& A )
             */
            void SetStyle ( const Style& A )
            {
                _a = A;
            }

            template<typename ioT> int RenderDC(PainterDC<ioT>* _dc, const xyCoords& xy);
        private:
            friend class Widget;


            Widget* _owner;

        protected:
            /// Current xy position inside _r
            pxy     _c;
            /// Initial sub rectangle
            Rect    _r;
            /// Current working text attributes values
            Style _a;
            /// current address location into the uiControl screen IO buffer
            PStr    _p;
            PStr   _ioscr;

            RTMode _rtmode;

    };
}

#endif
