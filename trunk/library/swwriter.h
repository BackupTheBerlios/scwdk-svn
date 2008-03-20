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

#include <swuicontrol.h>
#include <swtattr.h>
#include "exstring.h"

class swUiControl;
/**
The swWriter object is used such as a "Paint device on bitmaps"  in graphics user interface ( X, M$-Windows ,etc..) - but in the simple scwdk ways. UiControls create a swWriter instance, composed of the uicontrol's data parameters through its StartWrite(...)/DoneWrite(...) Mandatory methods - no swWriter object can be created from other classes. I.E.: Only swUiControl ( not even swUiControl subclasses ) can create an instance of swWriter    
 

	@author Serge Lussier,,, <bretzel@pc-desktop>
*/
class swWriter 
{
public:
    enum RTMode { plain=0, text };
    virtual ~swWriter();

    swUiControl* Control(){ return _owner; }
    
    //
    // Cursor control methods
    //
    pxy Position( int x=-1, int y=-1 );
    pxy Position( pxy xy=pxy::invalid );
    pxy operator + ( pxy xy );
    pxy operator ++();
    swWriter& operator << ( swWriter::RTMode mode ){
        _rtmode = mode;
        return *this;
    }

    swWriter& operator << ( const pxy& xy ){
        Position( xy );
        return *this;
    }
    


    //
    // Peek Operations
    //
 /*!
        \fn swWriter::Seek( const pxy& xy=pxy::invalid )
     */
    PStr Seek( const pxy& xy=pxy::invalid ){
        pxy C = xy;
        PStr P;
        //Debug;Dbg <<  "Seeking at " << C.tostring() << "inside "<< _r.tostring();
        if(!C) C = _c;
        if(!C){
            gDebug << " swWriter working pxy" << C.tostring() << " is invalid!";DEND;
            std::cout << __PRETTY_FUNCTION__ << "\n\r \033[0;31mswWriter working pxy" << C.tostring() << " is invalid!\r\033[0m" << std::endl;
            abort();
            return 0l;
        }
        C += _r.topleft();
        if(! _r.contains(C) ){
            gDebug << C.tostring() << "outside " <<  _r.tostring();DEND;
            return 0l;
        }
        P = _ioscr + ( C._y * _owner->Width() + C._x );
        if(!P){
            gDebug << " _ioscr + " << C._y <<"*"<<  _owner->Width() << "+" << C._x << " nil" ;DEND;
        }
        return P;
    }


    /*!
        \fn swWriter::DC()
     */
    PStr DC() { return _ioscr; }
    

    //
    // Text writing operations
    //
    template<typename argt> swWriter& operator << (argt __arg){
        std::ostringstream os;
        os << __arg;
        WriteStr(os.str());
        return *this;
    }
    


    swWriter& operator <<( PStr );
    swWriter& operator << (String);

    int WriteStr( const std::string& _str );
    
    //
    // line drawing ( box, line.. )
    //

    int AscLine( const pxy& xy, int ln, int dir, const swTAttr& A );
    int DrawFrame( const Rect& r, const swTAttr& A);
    void JoinAcs( const pxy& xy, int dir, const swTAttr& A );

    //
    // Blocks IO operations  - in additions to the uiControl interface
    //

    //
    // Text Attributes settings operation
    //
    int Clear();
    int WriteRT( const char* );
    int WritePStr( PStr );

    /*!
        \fn swWriter::Geometry()
     */
    Rect Geometry()    { return _r; }

    /*!
        \fn swWriter::UiControl()
     */
    swUiControl* UiControl() { return _owner; }

    /*!
        \fn swWriter::SetGeometry( const Rect& r )
     */
    void SetGeometry( const Rect& r ) { _r = r; }

    //
    // Scrolling areas operations
    //


    //
    // Static public methods for direct writing into the ncurses screen ... 
    //
    void DrawFrame3D( const Rect& r, const swTAttr& A );
    int Fill( const Rect& r, swTAttr A, chtype C );


private:
    friend class swUiControl;

    swWriter();
    swWriter(swUiControl* swOwner, uint _flags, const char* _nameID);
    swWriter(swUiControl* swOwner, PStr _scriobuf, const Rect& _initialSubRect = Rect::_void);

    swUiControl* _owner;
     
protected:
    /// Current xy position inside _r 
    pxy     _c;
    /// Initial sub rectangle
    Rect    _r;
    /// Current working text attributes values
    swTAttr _a;
    /// current address location into the uiControl screen IO buffer
    PStr    _p;
    PStr   _ioscr;

    RTMode _rtmode;

};

#endif
