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
#ifndef SWLABEL_H
#define SWLABEL_H

#include <swuicontrol.h>
#include <swtext.h>

/**
	@author Serge Lussier,,, <bretzel@pc-desktop>
*/
class swLabel : public swUiControl
{
public:
    swLabel(swObject* swParent);
    
    swLabel();
    
    swLabel(swObject* swParent, uint _flags, const char* _nameID);

    virtual ~swLabel();
    bool operator = ( const std::string& );
    bool operator = ( const String& str );

    swLabel& operator << ( swText::TxStyle st ){
        mTxStyle = st;
        return *this;
    }
    swLabel& operator << ( String& str );
    swLabel& operator << ( std::string& str );
    
    
private:
    swText::TxStyle mTxStyle;
protected:
    bool _renderText( const std::String& str );
};

#endif
