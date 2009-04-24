/***************************************************************************
 *   Copyright (C) 2009 by Serge Lussier,,,   *
 *   tuxadmin.dev@gmail.com   *
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
#include "label.h"
#include <painter.h>

namespace wcurses
{

    Label::Label ( Widget* Parent ) : Widget ( Parent )
    {
        InitView();
    }

    Label::Label ( Widget* Parent, const char* name ) : Widget ( Parent,0, name )
    {
        InitView();
    }

    Label::~Label()
    {
    }

    /*!
        \fn wcurses::Label::Text()
     */
    const std::string Label::Text()
    {
        return _text.std();
    }


    /*!
        \fn wcurses::Label::Justify( int )
     */
    void Label::Justify ( int j )
    {
        /// @todo implement me
        _justify = j;
        _Render();
    }


    /*!
        \fn wcurses::Label::SetText( const std::string& str )
     */
    void Label::SetText ( const std::string& str )
    {
        _text = String ( str );
        _Render();

    }


    /*!
        \fn wcurses::Label::_Render()
     */
    void Label::_Render()
    {
        pxy xy;
        Painter* P = DCPainter();
        P->Clear();

        switch ( _justify )
        {
            case justify::center:
                xy.rx() = ( Width() - _text.len() ) /2;
                xy.ry() = 0;
                break;
            case justify::left:
                xy ( 0,0 );
                break;
            case justify::right:
                xy ( Width() - _text.len(),0 );
                break;
            default:
                xy ( 0,0 );
                break;
        }
        ////// Debug << "Rendering text:" << _text.std();
        P->CPosition ( xy );
        P->WriteStr ( _text.std() );
    }

    /*!
        \fn wcurses::Label::_Scan_()
     */
    bool Label::_Scan_()
    {
        std::string scan = _text.std();
        std::string::size_type n;
        std::string::const_iterator si = scan.begin();
        n= scan.find ( '&', 0 );
        if ( n == std::string::npos ) return false;
        ++n;
        if ( n==std::string::npos ) return false;
        _short = scan[n];
        return true;
    }


}





/*!
    \fn wcurses::Label::InitView()
 */
bool wcurses::Label::InitView()
{
    Debug << "Changing theme for Widget.Label" ; DEND;
    ChangeTheme ( "Widget.Label" );
    return true;
}
