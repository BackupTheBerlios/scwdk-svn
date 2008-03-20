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
#ifndef SWWINDOW_H
#define SWWINDOW_H

#include "swuicontrol.h"

/**
Just a subclass of the ui control base - Manages the toplevel related behaviours automatically

	@author Serge Lussier,,, <bretzel@pc-desktop>
*/
class swWindow : public swUiControl
{
public:
    swWindow(swObject* swParent);
    
    swWindow(swObject* swParent, uint _flags, const char* _nameID);

    ~swWindow();

    int SetGeometry(const Rect& r);
    swUiControl* TopLevelParent();
    uint SetUiFlags(uint f, bool seton);
    virtual bool Activate(bool b);
    virtual uint SetFlags(uint _flags, bool seton);
    virtual void Resize(const Size& newSize);

protected:
    virtual swUiControl* _mouseSelect(const pxy& xy);

};

#endif
