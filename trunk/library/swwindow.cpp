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
#include "swwindow.h"

swWindow::swWindow(swObject* swParent): swUiControl(swParent)
{
}


swWindow::swWindow(swObject* swParent, uint _flags, const char* _nameID): swUiControl(swParent, _flags, _nameID)
{
}


swWindow::~swWindow()
{
}


int swWindow::SetGeometry(const Rect& r)
{
    return swUiControl::SetGeometry(r);
}

swUiControl* swWindow::TopLevelParent()
{
    return swUiControl::TopLevelParent();
}

uint swWindow::SetUiFlags(uint f, bool seton)
{
    return swUiControl::SetUiFlags(f, seton);
}

bool swWindow::Activate(bool b)
{
    return swUiControl::Activate(b);
}

swUiControl* swWindow::_mouseSelect(const pxy& xy)
{
    return swUiControl::_mouseSelect(xy);
}

uint swWindow::SetFlags(uint _flags, bool seton)
{
    return swUiControl::SetFlags(_flags, seton);
}

void swWindow::Resize(const Size& newSize)
{
    swUiControl::Resize(newSize);
}

