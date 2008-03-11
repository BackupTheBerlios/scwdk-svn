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
#ifndef SWDESKTOP_H
#define SWDESKTOP_H

#include <swuicontrol.h>

class swWriter;

struct  UNode{
    typedef std::list< UNode* > list;
    typedef list::iterator iterator;
    swUiControl* _w;
    Rect _r;
    UNode( swUiControl *w, const Rect& r) : _w(w), _r(r){}

    swUiControl* Control() { return _w; }
    Rect Geometry() { return _r; }
};




/*!
    \class swDesktop
    \brief The desktop object 
	\author Serge Lussier,,, <bretzel@pc-desktop>
*/
class swDesktop : public swUiControl
{
public:
    
    swDesktop();
    
    swDesktop(swObject* swParent);
    
    swDesktop(swObject* swParent, uint _flags, const char* _nameID);
    
    virtual ~swDesktop();

    swUiControl::list& Toplevels() { return _toplevels; }
    
    bool _peerPoint( const pxy& xy );
    virtual int Init();
    int _drawControl ( UNode* u );
    virtual void Resize( const Size& newSize );
    bool AddTopLevel( swUiControl* ctl );
    swUiControl::iterator _inToplevel( swUiControl* ctl );
private:
    swUiControl::list _toplevels;
    UNode::list _unode_priv;

protected:
    friend class swUiControl;
    int RefreshControl( swWriter* wr );
    swUiControl::iterator _locateControl( swUiControl* ui );
private:
    int _do_Updates();
    bool _CanExit( swObject*& _Sender ){
        _Sender = this;
        return false;
    }
    friend class swMain;

    swUiControl::iterator _plsIT; ///< PeerpointLocationStartITerator
};

#endif
