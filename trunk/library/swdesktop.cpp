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
#include "swdesktop.h"
#include "swwriter.h"
#include "swmain.h"

//-------------- TEST -- REMOVE WHEN FINISHED ---------------
#include "swtext.h"
// ----------------------------------------------------------

swDesktop::swDesktop()
{
}


swDesktop::swDesktop(swObject* swParent): swUiControl(swParent)
{
}


swDesktop::swDesktop(swObject* swParent, uint _flags, const char* _nameID): swUiControl(swParent, _flags, _nameID)
{
}


swDesktop::~swDesktop()
{
}


/*!
    \fn swDesktop::Resize( const Size& newSize )
 */
void swDesktop::Resize( const Size& newSize )
{
    swUiControl::Resize( newSize );
    _wr = StartWrite();
    _wr->Clear();
    _wr->Fill( Geometry(), swTAttr(4, 6, 0), ACS_CKBOARD);
    // ------------------------------------------------- TESTS
    swText T;
    T << "<fgcolor red;bgcolor white; /strong;>     Desktop initializecd...<fgcolor black;>Good To GO!!! <fgcolor blue;> Actual size: <fgcolor black;>"
      << Width() << "<fgcolor blue;>:<fgcolor black;>" << Height() << "     ";
    T << swText::END;
    PStr p = T.Data();
    //mvaddchstr( 0, 3, p);// Height()-2, p );
    swWriter* wr = StartWrite();
    (*wr)<< pxy(0,0) << swWriter::text << p;
    
    delete [] p;
    T.ReleaseData();
    // -----------------------------------------------------
    EndWrite();
    RedrawTopLevels();
    
}

/*!
    \fn swDesktop::RefreshControl( swWriter* wr )
 */
int swDesktop::RefreshControl( swWriter* wr )
{
    Debug;
    Dbg << "Control: " << wr->Control()->NameID();
    swUiControl* w;
    Rect r;
    
    if(!wr) return -1;
    w = wr->Control();
    r = wr->Geometry();
    Dbg << " Control's Geometry: " << r.tostring();
    if(!r) return -1;

    r += w->TopLeft(true);
    Dbg << " In Absolute coords: " << r.tostring();
    w->Clip( r );
    Dbg << " Clipped result: " << r.tostring() << " Desktop rect: " << Geometry().tostring();
    
    r &= Geometry();
    Dbg << " visible Region on screen: " << r.tostring();
    if(!r) return -1;
    _unode_priv.push_back( new UNode( w, r ) );
    Dbg << " Update Node inserted [" << w->NameID() << "] into the Desktop's update queu";DEND;
    return _unode_priv.size();
}


/*!
    \fn swDesktop::_drawControl ( UNode* u )
 */
int swDesktop::_drawControl ( UNode* u )
{
    Debug;
    swUiControl* w = u->Control();
    Dbg << " drawing control : "<< w->NameID();
    Rect r = u->Geometry();
    Dbg << " Abs clipped region: " << r.tostring();
    Rect l = r;
    PStr p;
    Dbg << " Abs control position:" << w->TopLeft(true).tostring();
    l -= w->TopLeft(true); // Local area;
    
    Dbg << "Local area:" << l.tostring() << "<-> abs area" << r.tostring();DEND;
    swWriter* wr = w->_wr;
    for( int y = 0; y < l.height(); y ++){
        p = wr->Seek( pxy(l.topleft().x(), l.topleft().y() + y) );
        if( !p ){
            Dbg << "Seek(" << pxy(l.topleft().x(), l.topleft().y() + y).tostring() << ") : invalid: writer geometry:" << wr->Geometry().tostring(); DEND;
            return 128;
        }
        mvaddchnstr( r.topleft().y() + y, r.topleft().x(), p, l.width() );
        ///@todo get pinpoints of the current line.
        //for( int x = 0; x < l.width(); x ++){
            ///@todo check pinpoints then draw visible cells
            
        //}
    }
    // Check z-order dirty controls
    
    //::refresh();
    return 0;
}

/*!
    \fn swDesktop::_do_Updates()
 */
int swDesktop::_do_Updates()
{
    Debug << " Queu size is: " << _unode_priv.size();
    UNode U(0l, Rect::nil);
    if( ! _unode_priv.size() ) return 0;
    for( UNode::iterator it = _unode_priv.begin(); it != _unode_priv.end(); it++){
        Dbg << (*it)->Control()->NameID();
        _drawControl( *it );
        delete (*it); // delete the node.
    }
    _unode_priv.clear();
    Dbg << " Update queu finished -- > Size of toplevel list:" << _toplevels.size();
    //RedrawTopLevels();
    DEND;
    return 0;
}


/*!
    \fn swDesktop::_peerPoint( const pxy& xy )
    \brief check to see if that xy coords is visible and not obstructed by higher z-order toplevels
    \param const pxy& xy - the absolute xy point to check
    \return boolean -  true: xy is not obstrued by higher z-order controls ; false: ... it is
 */
bool swDesktop::_peerPoint( const pxy& xy )
{
    return true;
}


/*!
    \fn swDesktop::_locateControl( swUiControl* ui )
    \brief seeks <b><i>ui</i></b> location into the toplevels controls ( get the top parent and gets the iterator as the position )
    \return the iterator of the top parent'ui into the toplevel list.
 */
swUiControl::iterator swDesktop::_locateControl( swUiControl* ui )
{
    swUiControl* pTlvl=0l;
    swUiControl::iterator iTlvl;
    std::list<swUiControl*> pl;
    QMParents( pl );
    if( pl.size()  ){
        std::list<swUiControl*>::iterator it = pl.end();
        it --;
        pTlvl = *it;
    }
    else pTlvl = ui;

    // locate( get the iterator of )  pTlvl into the toplvels
    for( iTlvl = _toplevels.begin(); iTlvl != _toplevels.end(); iTlvl++){
        if( *iTlvl == pTlvl ) break;
    }
    return iTlvl;
}




/*!
    \fn swDesktop::Init()
 */

int swDesktop::Init()
{
    swMain* Main = swMain::Instance();
    if(!Main){
        Debug << " No main instance ??";DEND;
        return -1;
    }
    // Just a test!!
    (*Main) += sigc::mem_fun(this, &swDesktop::_CanExit);
    // ----------------------------------------------------

    swNCurses* _n = Main->CursesInstance();
    if(! _n ) {
        Debug << " No Curses instance ??";DEND;
        return -1;
    }
    SetGeometry( Rect(0,0, _n->Width(), _n->Height()) );
    _wr->Clear();
    _wr->Fill( Rect(0,0, _n->Width(), _n->Height()), swTAttr(4, 6, 0), ACS_CKBOARD);
    // ------------------------------------------------- TESTS
    swText T;
    T << "<fgcolor red;bgcolor white; /strong;>     Desktop initializecd...<fgcolor black;>Good To GO!!! <fgcolor blue;> Actual size: <fgcolor black;>"
      << Width() << "<fgcolor blue;>:<fgcolor black;>" << Height() << "     ";
    T << swText::END;
    PStr p = T.Data();
    //mvaddchstr( 0, 3, p);// Height()-2, p );
    swWriter* wr = StartWrite();
    (*wr)<< pxy(0,0) << swWriter::text << p;
    delete [] p;
    T.ReleaseData();
    // -----------------------------------------------------
    Update();
    
    return 0;
}



/*!
    \fn swDesktop::AddTopLevel( swUiControl* ctl )
 */
bool swDesktop::AddTopLevel( swUiControl* ctl )
{
    
    Debug;
    swUiControl::list::iterator it = _inToplevel( ctl );
    if( it != _toplevels.end() ) return false;
    _toplevels.push_back( ctl );
    return true;
}


/*!
    \fn swDesktop::_inToplevel( swUiControl* ctl )
 */
swUiControl::iterator swDesktop::_inToplevel( swUiControl* ctl )
{
    swUiControl::iterator it = _toplevels.begin();
    if(_toplevels.empty() ) return _toplevels.end();
    do{
        if( (*it) == ctl ) return it;
        it++;
    }while( it != _toplevels.end() );
    return it;
}


/*!
    \fn swDesktop::RedrawTopLevels()
 */
int swDesktop::RedrawTopLevels()
{
    swUiControl* gui;
    swWriter* wr;
    for( swUiControl::iterator it = _toplevels.begin(); it != _toplevels.end(); it++){
        gui  = *it;
        wr = gui->StartWrite();
        Debug << "Control: " << wr->Control()->NameID();
        if(!wr) continue;
        gui->EndWrite();
    }
    DEND;
    return 0;
}
