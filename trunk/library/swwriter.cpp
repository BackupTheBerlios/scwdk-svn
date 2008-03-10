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
#include "swwriter.h"
#include "swtext.h"
swWriter::swWriter(swUiControl* swParent, uint,  const char*): swObject(),
_owner( swParent )
{
    if(! _owner ) return;
    _ioscr  = _owner->DC();
    _r.assign( 0,0, _owner->Width(), _owner->Height() );
    _a = _owner->DefAttr();
}


swWriter::swWriter(swUiControl* swOwner, PStr _scriobuf, const Rect& _initialSubRect ): swObject(),
_owner(swOwner), _ioscr(_scriobuf ), _r( _initialSubRect )
{
    _a = _owner->DefAttr();
}

swWriter::swWriter(): swObject()
{
}


swWriter::~swWriter()
{
}


//
// Cursor control methods
//

/*!
    \fn swWriter::Position( int x=-1, int y=-1 )
 */
pxy swWriter::Position( int x, int y )
{
    pxy C = _c;
    if( _r.contains( x+_r.x(), y+_r.y() ) ){
        _c(x,y);
        return C;
    }
    Debug <<  x << ":" << y << " out of region!!" ; DEND;
    return pxy::invalid;
}


/*!
    \fn swWriter::Position( pxy xy=pxy::nul )
 */
pxy swWriter::Position( pxy xy )
{
    return Position( xy.x(), xy.y() );
}


/*!
    \fn swWriter::operator + ( pxy xy )
 */
pxy swWriter::operator + ( pxy xy )
{
    pxy C = _c;
    _c += xy;
    if( !_r.contains( _c + _r.topleft() ) ){
        _c = C;
        return pxy::invalid;
    }
    return C;
}


/*!
    \fn swWriter::operator ++()
 */
pxy swWriter::operator ++()
{
    pxy C = _c;
    //Debug << _c.tostring();DEND;
    _c += pxy(1,0);
    //Dbg << "After += pxy(1,0):" << _c.tostring();
    if( !_r.contains( _c +_r.topleft() ) ){
        Debug << _c.tostring() << " outside boundaries";DEND;
        if( _owner->UiFlag( uiflags::wrap ) )
            if(! Position( 0, _c._y+1) ) return C;
            _c = C;
            return pxy::invalid;
    }
    return C;
}

    //
    // Text Attributes settings operation
    //


/*!
    \fn swWriter::Clear()
 */
int swWriter::Clear()
{
    PStr p;
    for(int y=0; y<_r.height(); y ++ ){
        if(! (p = Seek( pxy(0, y )) ) ) return -1;
        for(int x = 0; x < _r.width(); x ++) *p++ = _a.Data() | 0x20;
    }
    return 0;
}

/*!
    \fn swWriter::Fill( const Rect& r, swTAttr A )
 */
int swWriter::Fill( const Rect& r, swTAttr A, chtype C )
{
    PStr p;
    for(int y=0; y<_r.height(); y ++ ){
        if(! (p = Seek( pxy(0, y )) ) ) return -1;
        for(int x = 0; x < _r.width(); x ++) *p++ = A.Data() | C;
    }
    return 0;
}

/*!
    \fn swWriter::operator <<( PStr )
 */
swWriter& swWriter::operator <<( PStr _str )
{
    int r = WritePStr( _str );
    return *this;
}


/*!
    \fn swWriter::operator << (String)
 */
swWriter& swWriter::operator << (String _str)
{
    WriteStr( _str.std() );
    return *this;
}


/*!
    \fn swWriter::WriteStr( const std::string& _str )
 */
int swWriter::WriteStr( const std::string& _str )
{
    PStr p, s;
    int r=0;
    if( _rtmode == text ) return WriteRT( _str.c_str() );
    p = new TCell[  _str.length() +1 ];
    s = p;
    std::string::const_iterator i = _str.begin();
    for(; i != _str.end(); i++) *s++ = _a.Data() | *i;
    *s = 0;
    r = WritePStr( p );
    delete [] p;
    return r;
}


/*!
    \fn swWriter::WriteRT( const char* )
 */
int swWriter::WriteRT( const char* _str)
{
    PStr pStr;
    swText T(this, _owner->DefAttr(), " ");
    //Debug << " ( " << _str << ") " << _c.tostring(); DEND;
    T << _str << swText::END;
    _a = T.CurAttr();
    if( (pStr = T.Data() ) == 0l ){
        Debug << " swText::Transform failed!!!" ; DEND;
        return -1;
    }
    
    //Debug << "pStr=" <<  pStr  ;DEND;
    int r = WritePStr( pStr );
    delete [] pStr;
    return r;
}


/*!
    \fn swWriter::WritePStr( PStr )
 */
int swWriter::WritePStr( PStr pStr)
{
    pxy xy = _c;
    PStr p = pStr, _pos = Seek( _c );
    if( ! (p||_pos) )return 0;
    do{
        if(!(_pos = Seek( _c  ))){
            std::cerr << "\033[0;31mIllegal Seek @" << _c.tostring() << " Aborting\r" <<std::endl;
            abort();
            return -1;
        }
        *_pos = *p++;
        if(! ++(*this)  ) return 0;
    }while(*p);
    return p-pStr;
}


/*!
    \fn swWriter::AscLine( const pxy& xy, int ln, int dir, const swTAttr& A )
 */
int swWriter::AscLine( const pxy& xy, int ln, int dir, const swTAttr& A )
{
    PStr p = Seek( xy );
    if( ! _r.contains(xy) ) return -1;
    if(! p )  return -1;
    int x,y, n=0;
    x = xy.x(); y = xy.y();
    switch (dir){
        case direction::left:
            do{
                *p = ACS_HLINE|A.Data();
                ++n;
                p = Seek(pxy(xy.x()-n, xy.y()));
                if(!p) return -1;
            }while( n < ln );
            break;
        case direction::right:
            do{
                *p = ACS_HLINE|A.Data();
                ++n;
                p = Seek(pxy(xy.x()+n, xy.y()));
                if(!p) return -1;
            }while(n < ln );
            break;
        case direction::down:
            do{
                *p = ACS_VLINE|A.Data();
                ++n;
                p = Seek(pxy(xy.x(), xy._y+n));
                if(!p) return -1;
            }while( n < ln );
            break;
        case direction::up:
            do{
                *p = ACS_VLINE|A.Data();
                ++n;
                p = Seek(pxy(xy.x(), xy._y-n ));
                if(!p) return -1;
            }while( n < ln );
            break;    
    }
    return 0;
}


/*!
    \fn swWriter::DrawFrame( const Rect& r, const swTAttr& A)
    \note < r > must not be equals to Rect::nil 
 */
int swWriter::DrawFrame( const Rect& r, const swTAttr& A)
{
    AscLine( pxy(r.x()+1,r.y()), r.width()-2, direction::right, A );
    AscLine( pxy(r.x()+1,r.height()-1), r.width()-2, direction::right, A );
    AscLine( pxy(r.x(),r.height()-2), r.height()-2, direction::up, A );
    AscLine( pxy(r.width()-1,r.height()-2), r.height()-2, direction::up, A );

    JoinAcs( r.topleft(), direction::topleft, A );
    JoinAcs( r.bottomright(), direction::bottomright, A );
    JoinAcs( r.bottomleft(), direction::bottomleft, A );
    JoinAcs( r.topright(), direction::topright, A );
    
    /// @todo implement me
    return 0;
}


/*!
    \fn swWriter::JoinAcs( const pxy& xy, int dir, const swTAttr& A )
 */
void swWriter::JoinAcs( const pxy& xy, int dir, const swTAttr& A )
{
    PStr pos = Seek( xy );
    if(! pos ) return ;
    switch(dir){
        case direction::topleft:
            *pos = ACS_ULCORNER|A.Data();
            break;
        case direction::topright:
            *pos = ACS_URCORNER|A.Data();
            break;
        case direction::bottomleft:
            *pos = ACS_LLCORNER|A.Data();
            break;
        case direction::bottomright:
            *pos = ACS_LRCORNER|A.Data();
            break;
        case direction::down: // TEE point down
            *pos = ACS_TTEE|A.Data();
            break;
        case direction::left:
            *pos = ACS_RTEE|A.Data();
            break;
        case direction::right:
            *pos = ACS_LTEE|A.Data();
            break;
        case direction::up:
            *pos = ACS_BTEE|A.Data();
            break;
    }
}


/*!
    \fn swWriter::DrawFrame3D( const Rect& r, const swTAttr& A )
 */
void swWriter::DrawFrame3D( const Rect& r, const swTAttr& A )
{
    swTAttr _A = A;
    _A.SetAttr( A_BOLD, true );
    AscLine( pxy(r.x()+1,r.y()), r.width()-2, direction::right, _A );
    _A.SetAttr( A_BOLD, false );
    AscLine( pxy(r.x()+1,r.height()-1), r.width()-2, direction::right, _A );
    _A.SetAttr( A_BOLD, true );
    AscLine( pxy(r.x(),r.height()-2), r.height()-2, direction::up, _A );
    _A.SetAttr( A_BOLD, false );
    AscLine( pxy(r.width()-1,r.height()-2), r.height()-2, direction::up, _A );

    _A.SetAttr( A_BOLD, true );
    JoinAcs( r.topleft(), direction::topleft, A );
    _A.SetAttr( A_BOLD, false );
    JoinAcs( r.bottomright(), direction::bottomright, A );
    _A.SetAttr( A_BOLD, true );
    JoinAcs( r.bottomleft(), direction::bottomleft, A );
    _A.SetAttr( A_BOLD, false );
    JoinAcs( r.topright(), direction::topright, A );/// @todo implement me
}


