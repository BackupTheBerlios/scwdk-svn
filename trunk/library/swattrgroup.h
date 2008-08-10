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
#ifndef SWATTRGROUP_H
#define SWATTRGROUP_H

#include "scwdk.h"

#include <string>
#include <map>

/**
	@author Serge Lussier,,, <bretzel@pc-desktop>
*/

struct swAttrGroup{
    typedef std::map<const string, swAttrGroup*> List;
    swAttrGroup();

    ~swAttrGroup();
    // Totaly accessible and modifiable node.
    swTAttr& operator [](int st ){
        switch(st){
            case state::disable:
                return _disabled;
            case state::active:
                return _active;
            case state::normal:
                return _normal;
            default:
                return normal;
        }
        return _normal;
    }
    swTAttr _normal;
    swTAttr _active;
    swTAttr _disabled;
};

#endif
