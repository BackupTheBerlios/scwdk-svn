/***************************************************************************
 *   Copyright (C) 2007 by Serge Lussier   *
 *   lussier.serge@gmail.com   *
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
#ifndef SWTATTR_H
#define SWTATTR_H
#include <scwdk.h>

#include "swncurses.h"
#include "exstring.h"
#include <map>
/**
	@author Serge Lussier <lussier.serge@gmail.com>
*/


class swTAttr{
public:
    typedef char* pChar;
    enum A { normal=0, bold=A_BOLD, underline=A_UNDERLINE, reverse=A_REVERSE, blink=A_BLINK, colors=A_COLOR, newline=A_NEWLINE };

    static std::map< std::string, int> ColorNames;
    
    // Bad idea to def a list of swTAttr as instances -- too much overhead copy/constructor/create for each inserted element@!
    // Let be the list to store pointers instead...
    typedef std::list<swTAttr*> list;
    
    struct Color{
        Color( int _bg, int _fg ):
        bg(_bg), fg(_fg){
        }
        int bg,fg;
    };
    swTAttr();
    
    swTAttr( const swTAttr& );
    
    
    swTAttr(int bg, int fg, TCell attr );

    ~swTAttr();

    operator const char* ();
    ///////////////////////
    // Information methods
    /*!
        \fn swTAttr::BgColor()
     */
    int BgColor(){
        swNCurses* n = swNCurses::Instance();
        int P = PAIR_NUMBER(_d);
        short f,b;
        pair_content(P, &f, &b);
        return b;
    }

    /*!
        \fn swTAttr::FgColor()
     */
    int FgColor(){
        swNCurses* n = swNCurses::Instance();
        int P = PAIR_NUMBER(_d);
        short f,b;
        pair_content(P, &f, &b);
        return f;
    }

    /*!
        \fn swTAttr::Attributes()
     */
    TCell Attributes() { return (_d & ~A_COLOR) | A_ATTRIBUTES | A_NEWLINE | A_ALTCHARSET; }

    /*!
        \fn swTAttr::Colors()
     */
    TCell Colors() { return _d & A_COLOR; }
    //////////////////////////////////////////////
    // Modifier methods

    /*!
        \fn swTAttr::SetColors( int, int )
     */
    void SetColors( int bg, int  fg ) { _d = ( _d & ~A_COLOR ) | swTAttr::ptbl[bg][fg]; }
    void AddAttr( TCell a );

    /*!
        \fn swTAttr::SetData( TCell a )
     */
    void SetData( TCell a ) { _d = a; }
    void SetBgColor( int bg );
    void SetFgColor( int fg );
    void SetNewAttr( TCell a );
    void SetAttr( TCell d, bool _set ){
        if (_set) _d |= d;
        else    _d &= ~d;
    }

    ////////////////////////
    // Operators
    swTAttr& operator << ( swTAttr::Color C );
    swTAttr& operator << ( TCell d ) { _d = d; return *this; }
    swTAttr& operator >> ( swTAttr::Color& C ){
        C.fg = FgColor();
        C.bg = BgColor();
        return *this;
    }
    
    /*!
        \fn swTAttr::Data()
     */
    TCell Data() const { return _d; }
    TCell operator()(){ return _d; }

    /*!
        \fn swTAttr::SetReverse( bool t=true )
     */
    void SetReverse( bool t=true ) { SetAttr( swTAttr::reverse, t ); }

    /*!
        \fn swTAttr::SetUnderline( bool t=true )
     */
    void SetUnderline( bool t=true ) { SetAttr( swTAttr::underline, t ); }

    /*!
        \fn swTAttr::SetNewLine( bool t=true )
     */
    void SetNewLine( bool t=true ) { SetAttr( swTAttr::newline, t ); }
    
    /*!
        \fn swTAttr::SetBold( bool t=true )
     */
    void SetBold( bool t=true ) { SetAttr( swTAttr::bold, t ); }
    bool IsSet( TCell );


    /*!
        \fn swTAttr::IsBlink()
     */
    bool IsBlink() { return (_d&swTAttr::blink) !=0; }

    /*!
        \fn swTAttr::IsBold()
     */
    bool IsBold() { return (_d&swTAttr::bold) !=0; }

    /*!
        \fn swTAttr::IsNewLine()
     */
    bool IsNewLine() { return (_d&swTAttr::newline) !=0; }

    /*!
        \fn swTAttr::IsReverse()
     */
    bool IsReverse() { return (_d&swTAttr::reverse) !=0; }

    /*!
        \fn swTAttr::IsUnderline()
     */
    bool IsUnderline() {  return (_d&swTAttr::underline) !=0; }

    /*!
        \fn swTAttr::EncodeColor( int b, int f )
     */
    static inline TCell EncodeColor( int b, int f ){ return swTAttr::ptbl[b][f]; }


    
protected:
    friend class swNCurses;
    friend class swDesktop;
    friend class swText;
    TCell _d;
    public: static TCell ptbl[8][8];    
    protected: static int init();
private:

    /*!
        \fn swTAttr::__merge()
     */
/*    void __merge() {
        _d = (_d & ~A_COLOR) | swTAttr::ptbl[bg][F];
        
    }*/
};

#endif
