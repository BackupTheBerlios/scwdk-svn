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
#include "swuicontrol.h"
#include "swwriter.h"
#include "swmain.h"

swUiControl::swUiControl(swObject* swParent, uint _flags, const char* _nameID): swObject(swParent, _flags, _nameID)
{
    m_IoScrBuffer = 0l;
    _wr = 0l;
    m_DefTextAttr = swTAttr(4,7, A_BOLD);
    if((! swParent) && (!swParent->Class<swUiControl>()) ) swMain::Instance()->Desktop(0)->AddTopLevel( this );


}


swUiControl::swUiControl(): swObject(0l,0,"unnamed swUiControl")
{
    m_IoScrBuffer = 0l;
    _wr = 0l;
    m_DefTextAttr = swTAttr(4,7, A_BOLD);
    //if((! swParent) || (swParent->Class<swUiControl>())) swMain::Instance()->Desktop(0)->AddTopLevel( this );
    //m_BackBuffer = 0l;
}


swUiControl::swUiControl(swObject* swParent): swObject(swParent, 0,"unnamed swUiControl with a parent")
{
    m_IoScrBuffer = 0l;
    _wr = 0l;
    m_DefTextAttr = swTAttr(4,7, A_BOLD);
    if((! swParent) && (!swParent->Class<swUiControl>())) swMain::Instance()->Desktop(0)->AddTopLevel( this );
    //m_BackBuffer = 0l;
}


swUiControl::~swUiControl()
{
    Debug << " Releasing resources"; DEND;
    if (m_IoScrBuffer) delete [] m_IoScrBuffer;

    if(_wr) delete _wr;

    //DestroyChildren(); // !no let ~swObject do the job
}


uint swUiControl::SetFlags(uint _flags, bool seton)
{
    return swObject::SetFlags(_flags, seton);
}



/*!
    \fn swUiControl::SetGeometry( const Rect& r )
 */
int swUiControl::SetGeometry( const Rect& r )
{
    //swUiControl* _parent = ParentT<swUiControl>();
    swUiControl* _parent = ParentOfClass(this);
    m_geometry = r;
    pxy offset = _parent ? _parent->TopLeft(true)  : pxy(0,0);
    m_topleft = m_geometry.topleft() + offset;


    if(m_IoScrBuffer) delete [] m_IoScrBuffer;
    m_IoScrBuffer = new TCell[ Height() * Width() + Width() ];
    if( _wr ) delete _wr;
    _wr = new swWriter( this, m_IoScrBuffer, r );

    Debug << " Geometry of " << NameID() << " Set to " << m_geometry.tostring() ;DEND;
    return 0;
}


/*!
    \fn swUiControl::StartWrite( Rect _r = Rect::nil )
 */
swWriter* swUiControl::StartWrite(  Rect _r  )
{
    if(!_r ) _r.assign( 0,0, Width(), Height());
    _wr->SetGeometry( _r );
    return _wr;
}


/*!
    \fn swUiControl::EndWrite(  )
 */
int swUiControl::EndWrite(  )
{
    Update ();
    return 0;
}


/*!
    \fn swUiControl::Update( )
    \note This call invoke all children controls to update after me
 */
int swUiControl::Update( )
{
    Debug ;DEND;
    swUiControl* w;
    int I=0;
    swMain::Instance()->Desktop()->RefreshControl( _wr );
    for( Iterator it = m_children.begin(); it != m_children.end(); it++, I++){
        if( (w=(*it)->Class(w)) ){
            Dbg << " updating child:" << w->NameID();
            w->Update();
        }
        else
            Dbg << w->NameID() << " is not of a Control type!";
    }

    return I;
}


/*!
    \fn swUiControl::Clip( Rect &r )
 */
bool swUiControl::Clip( Rect &r )
{
    swUiControl* myparent;
    Rect R(0,0, Width(), Height());
    R += TopLeft(true);
    r &= R;
    // I am toplevel then I am the last to clip the subrect ==> stop clipping:
    if( ( UiFlag( uiflags::toplevel )) || (!Parent() ) ) return false;
    // I am not a toplevel control and I am confined into my parent control ==> passe result rect to my parent
    if( (myparent=FirstParentAs<swUiControl>()) ) myparent->Clip( r );
    return false;
}


/*!
    \fn swUiControl::DestroyChildren()
 */
void swUiControl::DestroyChildren()
{
    swUiControl* pUi = FirstChild<swUiControl>();
    swUiControl* t;
    if(!pUi) return;
    for( ; pUi; pUi = NextChild<swUiControl>() ) delete pUi;
}



/*!
    \fn swUiControl::_mouseSelect( const pxy& xy )
    \brief Test and confirm/infirm if the mouse is targeting inside the geometry's UI control
    \param pxy xy the coordinates in screen absolute of the mouse pointer
    \return swUiControl* self or child target, or NULL if totally outside inner geometry.
    \note No actions taken at this level   - it just returns the UI control instance as the positive response or null if no target .
 */
swUiControl* swUiControl::_mouseSelect( const pxy& xy )
{
    swUiControl* child;
    Rect R = m_geometry;
    R += TopLeft(true);
    if( ! R.contains( xy ) ) return 0l;

    // Still have to check if we are visible "targetable" - that is immuable rule
    if( ! ( Visible() || Enabled() ) ) return 0l;//UiFlag( uiflags::visible ) ) return 0l;

    if(! (child = FirstChild<swUiControl>()) ) return this;
    if( ( child = child->_mouseSelect( xy )) ) return child;
    for(; child; child = NextChild<swUiControl>() ) if( ( child = child->_mouseSelect( xy )) ) return child;
    return this;
}


/*!
    \fn swUiControl::Resize( const Size& newSize )
 */
void swUiControl::Resize( const Size& newSize )
{
    SetGeometry(Rect ( m_geometry.x(), m_geometry.y(), newSize.width(), newSize.height() ));
}


/*!
        \fn swUiControl::Main()
     */
swMain* swUiControl::Main() {
    return swMain::Instance();
}




/*!
    \fn swUiControl::TopLevelParent()
 */
swUiControl* swUiControl::TopLevelParent()
{
    swUiControl* p = this;
    if( UiFlag(uiflags::toplevel) ) return 0l; // null== we are toplevel
    while( (p = p->ParentT<swUiControl>()) ){
        if( p->UiFlag( uiflags::toplevel ) ) return p;
    };
    return 0l;
}


/*!
    \fn swUiControl::SetUiFlags( uint f, bool seton )
 */
uint swUiControl::SetUiFlags( uint f, bool seton )
{
    _uiflags = ( seton ? _uiflags | f : _uiflags & ~f );
    return _uiflags;
}
