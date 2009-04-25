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
#include "painter.h"
#include <StyledString.h>
#include <application.h>

namespace wcurses
{

    Painter::Painter ( Widget* swParent, uint,  const char* ) :
            _owner ( swParent )
    {
        if ( ! _owner ) return;
        _ioscr  = _owner->DC();
        _r.assign ( 0,0, _owner->Width(), _owner->Height() );
        _a = _owner->CurrentStyle();
    }


    Painter::Painter ( Widget* swOwner, PStr _scriobuf, const Rect& _initialSubRect ) :
            _owner ( swOwner ), _ioscr ( _scriobuf ), _r ( _initialSubRect )
    {
        _a = _owner->CurrentStyle();
    }

    Painter::Painter()
    {
    }


    Painter::~Painter()
    {
    }


//
// Cursor control methods
//

    /*!
        \fn Painter::Position( int x=-1, int y=-1 )
     */
    pxy Painter::CPosition ( int x, int y )
    {
        pxy C = _c;
        if ( _r.contains ( x+_r.x(), y+_r.y() ) )
        {
            _c ( x,y );
            return C;
        }
        
        return pxy::invalid;
    }


    /*!
        \fn Painter::Position( pxy xy=pxy::nul )
     */
    pxy Painter::CPosition ( pxy xy )
    {
        return CPosition ( xy.x(), xy.y() );
    }


    /*!
        \fn Painter::operator + ( pxy xy )
     */
    pxy Painter::operator + ( pxy xy )
    {
        pxy C = _c;
        _c += xy;
        if ( !_r.contains ( _c + _r.topleft() ) )
        {
            _c = C;
            return pxy::invalid;
        }
        return C;
    }


    /*!
        \fn Painter::operator ++()
     */
    pxy Painter::operator ++()
    {
        pxy C = _c;
        
        _c += pxy ( 1,0 );
        
        if ( !_r.contains ( _c +_r.topleft() ) )
        {
            
            if ( _owner->WidgetClass ( wclass::wrap ) )
                if ( ! CPosition ( 0, _c._y+1 ) ) return C;
            _c = C;
            return pxy::invalid;
        }
        return C;
    }

    //
    // Text Attributes settings operation
    //


    /*!
        \fn Painter::Clear()
     */
    int Painter::Clear()
    {
        PStr p;
        for ( int y=0; y<_r.Height(); y ++ )
        {
            if ( ! ( p = Seek ( pxy ( 0, y ) ) ) ) return -1;
            for ( int x = 0; x < _r.Width(); x ++ ) *p++ = _a.Data() | 0x20;
        }
        return 0;
    }

    /*!
        \fn Painter::Fill( const Rect& r, Style A )
     */
    int Painter::Fill ( const Rect& r, Style A, chtype C )
    {
        PStr p;
        for ( int y=0; y<_r.Height(); y ++ )
        {
            if ( ! ( p = Seek ( pxy ( 0, y ) ) ) ) return -1;
            for ( int x = 0; x < _r.Width(); x ++ ) *p++ = A.Data() | C;
        }
        return 0;
    }

    /*!
        \fn Painter::operator <<( PStr )
     */
    Painter& Painter::operator << ( PStr _str )
    {
        int r = WritePStr ( _str );
        return *this;
    }


    /*!
        \fn Painter::operator << (String)
     */
    Painter& Painter::operator << ( String _str )
    {
        WriteStr ( _str.std() );
        return *this;
    }


    /*!
        \fn Painter::WriteStr( const std::string& _str )
     */
    int Painter::WriteStr ( const std::string& _str )
    {
        
        PStr p, s;
        int r=0;
        if ( _rtmode == text )
        {
            
            return WriteRT ( _str.c_str() );
        }
        p = new TCell[  _str.length() +1 ];
        s = p;
        std::string::const_iterator i = _str.begin();
        for ( ; i != _str.end(); i++ ) *s++ = _a.Data() | *i;
        *s = 0;
        r = WritePStr ( p );
        delete [] p;
        return r;
    }


    /*!
        \fn Painter::WriteRT( const char* )
     */
    int Painter::WriteRT ( const char* _str )
    {
        
        PStr pStr;
        StyledString& T = Application::TextProcessor();
        T.SetDefaultAttributes ( _owner->CurrentStyle() );
        
        T << _str << StyledString::END;
        _a = T.CurAttr();
        pStr = T.Data();
        
        if ( !pStr )
        {
            
            return -1;
        }

        
        int r = WritePStr ( pStr );
        
        delete [] pStr;
        T.Clear();
        return r;
    }


    /*!
        \fn Painter::WritePStr( PStr )
     */
    int Painter::WritePStr ( PStr pStr )
    {
        
        pxy xy = _c;
        PStr p = pStr, _pos = Seek ( _c );
        if ( ! ( p||_pos ) ){
            
            return 0;
        }
        do
        {
            if ( ! ( _pos = Seek ( _c ) ) )
            {
                std::cerr << "\033[0;31mIllegal Seek @" << _c.tostring() << " Aborting\r" <<std::endl;
                abort();
                return -1;
            }
            *_pos = *p++;
            if ( ! ++ ( *this ) ) return 0;
        }
        while ( *p );
        return p-pStr;
    }


    /*!
        \fn Painter::AscLine( const pxy& xy, int ln, int dir, const Style& A )
     */
    int Painter::AscLine ( const pxy& xy, int ln, int dir, const Style& A )
    {
        PStr p = Seek ( xy );
        if ( ! _r.contains ( xy ) ) return -1;
        if ( ! p )  return -1;
        int x,y, n=0;
        x = xy.x(); y = xy.y();
        switch ( dir )
        {
            case directions::left:
                do
                {
                    *p = ACS_HLINE|A.Data();
                    ++n;
                    p = Seek ( pxy ( xy.x()-n, xy.y() ) );
                    if ( !p ) return -1;
                }
                while ( n < ln );
                break;
            case directions::right:
                do
                {
                    *p = ACS_HLINE|A.Data();
                    ++n;
                    p = Seek ( pxy ( xy.x() +n, xy.y() ) );
                    if ( !p ) return -1;
                }
                while ( n < ln );
                break;
            case directions::down:
                do
                {
                    *p = ACS_VLINE|A.Data();
                    ++n;
                    p = Seek ( pxy ( xy.x(), xy._y+n ) );
                    if ( !p ) return -1;
                }
                while ( n < ln );
                break;
            case directions::up:
                do
                {
                    *p = ACS_VLINE|A.Data();
                    ++n;
                    p = Seek ( pxy ( xy.x(), xy._y-n ) );
                    if ( !p ) return -1;
                }
                while ( n < ln );
                break;
        }
        return 0;
    }


    /*!
        \fn Painter::DrawFrame(  const Style& A, const Rect& r=Rect::nil )
        \note < r > must not be equals to Rect::nil
     */
    int Painter::DrawFrame (  const Style& A,const Rect& R )
    {
        Rect r;
        r=R;
        if(!R){
            r = _owner->Geometry();
            r -= _owner->Geometry().topleft();
        }
        AscLine ( pxy ( r.x() +1,r.y() ), r.Width()-2, directions::right, A );
        AscLine ( pxy ( r.x() +1,r.Height()-1 ), r.Width()-2, directions::right, A );
        AscLine ( pxy ( r.x(),r.Height()-2 ), r.Height()-2, directions::up, A );
        AscLine ( pxy ( r.Width()-1,r.Height()-2 ), r.Height()-2, directions::up, A );

        JoinAcs ( r.topleft(), directions::topleft, A );
        
        JoinAcs ( r.bottomright(), directions::bottomright, A );
        JoinAcs ( r.bottomleft(), directions::bottomleft, A );
        JoinAcs ( r.topright(), directions::topright, A );

        /// @todo implement me
        return 0;
    }


    /*!
        \fn Painter::JoinAcs( const pxy& xy, int dir, const Style& A )
     */
    void Painter::JoinAcs ( const pxy& xy, int dir, const Style& A )
    {
        PStr pos = Seek ( xy );
        if ( ! pos ) return ;
        switch ( dir )
        {
            case directions::topleft:
                *pos = ACS_ULCORNER|A.Data();
                break;
            case directions::topright:
                *pos = ACS_URCORNER|A.Data();
                break;
            case directions::bottomleft:
                *pos = ACS_LLCORNER|A.Data();
                break;
            case directions::bottomright:
                *pos = ACS_LRCORNER|A.Data();
                break;
            case directions::down: // TEE point down
                *pos = ACS_TTEE|A.Data();
                break;
            case directions::left:
                *pos = ACS_RTEE|A.Data();
                break;
            case directions::right:
                *pos = ACS_LTEE|A.Data();
                break;
            case directions::up:
                *pos = ACS_BTEE|A.Data();
                break;
        }
    }


    /*!
        \fn Painter::DrawFrame3D( const Rect& r, const Style& A )
     */
    void Painter::DrawFrame3D ( const Rect& r, const Style& A )
    {
        Style _A = A;
        _A.SetAttr ( A_BOLD, true );
        AscLine ( pxy ( r.x() +1,r.y() ), r.Width()-2, directions::right, _A );
        _A.SetAttr ( A_BOLD, false );
        AscLine ( pxy ( r.x() +1,r.Height()-1 ), r.Width()-2, directions::right, _A );
        _A.SetAttr ( A_BOLD, true );
        AscLine ( pxy ( r.x(),r.Height()-2 ), r.Height()-2, directions::up, _A );
        _A.SetAttr ( A_BOLD, false );
        AscLine ( pxy ( r.Width()-1,r.Height()-2 ), r.Height()-2, directions::up, _A );

        _A.SetAttr ( A_BOLD, true );
        JoinAcs ( r.topleft(), directions::topleft, _A );
        _A.SetAttr ( A_BOLD, false );
        JoinAcs ( r.bottomright(), directions::bottomright, _A );
        _A.SetAttr ( A_BOLD, false );
        JoinAcs ( r.bottomleft(), directions::bottomleft, _A );
        _A.SetAttr ( A_BOLD, false );
        JoinAcs ( r.topright(), directions::topright, _A );/// @todo implement me
    }

    /*!
        \fn wcurses::Painter::CopyWidget( const pxy& xyOffset, Painter* ChildPainter, const Rect& childRect)
     */
    bool Painter::CopyWidget ( const pxy& xyOffset, Painter* ChildPainter, const Rect& childRect )
    {

        PCell OwnerIO = Position ( xyOffset );
        PCell ChildIO = ChildPainter->Position ( childRect.topleft() );
        
        for ( int y = 0; y < childRect.Height(); y ++ )
        {
            
            OwnerIO = Position ( pxy(xyOffset.x(), xyOffset.y() +y) );
            ChildIO = ChildPainter->Position ( pxy ( childRect.topleft().x(), childRect.topleft().y() +y ) );
            for ( int x = 0; x < childRect.Width(); x++ ){
                
                *OwnerIO++ = *ChildIO++;
            }

        }
        return true;
    }


    /*!
        \fn wcurses::PainterDC::Render(const xyCoords& xy)
     */
    template <typename ioDC> void PainterDC<ioDC>::Render(const xyCoords& xy)
    {

    }
    template<typename ioT> int Painter::RenderDC(PainterDC<ioT>* _dc, const xyCoords& xy){
        return 0;
    }

}







/*!
    \fn wcurses::Painter::Copy(const Rect& _region=Rect::nil)
 */
wcurses::PainterDC<TCell>* wcurses::Painter::Copy(const Rect& _region)
{
    Rect R=_region;
    if(!R)  R = _r;
    PainterDC<TCell>* pDC = new PainterDC<TCell>(0l, Size(R.Width(), R.Height()));
    PCell io_src, io_dest;

    for( int y = 0; y<R.Height(); y++){
        io_src = Position( xyCoords(R.x(), R.y()+y) );
        io_dest=  pDC->Position( xyCoords(0,R.y()+y) );
        for(int x = 0; x <R.Width(); x++) *io_dest++ = *io_src++;
    }
    return pDC;
    
}
