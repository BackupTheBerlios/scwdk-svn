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
#include "swlabel.h"

swLabel::swLabel(swObject* swParent): swUiControl(swParent)
{
}


swLabel::swLabel(): swUiControl()
{
}


swLabel::swLabel(swObject* swParent, uint _flags, const char* _nameID): swUiControl(swParent, _flags, _nameID)
{
}


swLabel::~swLabel()
{
}




/*!
    \fn swLabel::operator = ( const std::string& str )
 */
bool swLabel::operator = ( const std::string& str )
{
    bool result=false;
    swWriter* w = StartWrite();
    w->Clear();
    result = _renderText( str );
    EndWrite();
    return result;
}


/*!
    \fn swLabel::operator = ( const String& str )
 */
bool swLabel::operator = ( const String& str )
{
    bool result=false;
    swWriter* w = StartWrite();
    w->Clear();
    result = _renderText( str.std() );
    EndWrite();
    return result;
}


/*!
    \fn swLabel::operator << ( String& str )
 */
swLabel& swLabel::operator << ( String& str )
{
    swWriter* w = StartWrite();
    w->Clear();
    (void)_renderText( str.std() );
    EndWrite();
    return *this;
    
}


/*!
    \fn swLabel::operator << ( std::string& str )
 */
swLabel& swLabel::operator << ( std::string& str )
{
    swWriter* w = StartWrite();
    w->Clear();
    (void)_renderText( str );
    EndWrite();
    return *this;
}


/*!
    \fn swLabel::_renderText( const std::String& str )
 */
bool swLabel::_renderText( const std::String& str )
{
    
}
