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
#ifndef SWUICONTROL_H
#define SWUICONTROL_H

#include "swobject.h"
#include "rect.h"
#include "layout.h"
#include "swtattr.h"

// Doxygen and graphviz api documentation format


/*!
    \defgroup UICONTROLS User Interface Controls
    \brief This module holds the descriptions of the User Interface classes.
*/

class swWriter;

/*!
    \class swUiControl
    \brief This is the base class that has the common user interface methods and attributes
    \ingroup UICONTROLS
	\author Serge Lussier <lussier.serge@gmail.com>
*/
class swUiControl : public swObject
{
public:
    typedef std::list< swUiControl* > list;
    typedef swUiControl::list::iterator iterator;
    
    swUiControl(swObject* swParent, uint _flags, const char* _nameID);
    
    swUiControl();
    
    swUiControl(swObject* swParent);

    virtual ~swUiControl();

    virtual uint SetFlags(uint _flags, bool seton=true);
    int SetGeometry( const Rect& r );

    /*!
        \fn swUiControl::Height()
     */
    int Height() { return m_geometry.height(); }

    /*!
        \fn swUiControl::Width()
     */
    int Width() { return m_geometry.width(); }
    
    swWriter* StartWrite( Rect _r = Rect::_void );
    int EndWrite( );

    /*!
        \fn swUiControl::TopLeft( bool abs=false )
     */
    pxy TopLeft( bool getabs=false ) {
        if( getabs ) return m_topleft;
        return m_geometry.topleft();
    }

    /*!
        \fn swUiControl::DefAttr()
     */
    swTAttr DefAttr() { return m_DefTextAttr; }

    /*!
        \fn swUiControl::UiFlag( int )
     */
    bool UiFlag( int _v ) { return (_uiflags & _v ) != 0;  }

    /*!
        \fn swUiControl::DC()
     */
    PStr DC() {  return m_IoScrBuffer; }
    int Update( );
    bool Clip( Rect &r );

    /*!
        \fn swUiControl::Geometry()
     */
    Rect Geometry() { return Rect(0,0,Width(), Height() ); }
    void DestroyChildren();

    /*!
        \fn swUiControl::Activate( bool b=true )
        \brief Default method to call to set the control to be in Focuessed mode ( input events routed to this control )
        \param bool b = true by default - false to set to unfocuss mode.
        \return bool - not used yet - to be determined.
        \author Serge Lussier (bretzel)
     */
    virtual bool Activate( bool b=true ){
        return false;
    }

    /*!
        \fn swUiControl::Enabled( int e = -1 )
     */
    virtual bool Enabled( int e = -1 ) {
        if( e > 0 ) SetFlags( state::disable, false );
        else
            SetFlags( state::disable, true );
        return m_flags & (!state::disable);
    }

    /*!
        \fn swUiControl::Visible( int v =-1 )
     */
    virtual bool Visible( int v = -1 ) {
        if( v < 0 ) return _uiflags & ( uiflags::visible );
        if( v ) _uiflags |= uiflags::visible;
        else _uiflags &= ~uiflags::visible;
        return 0;
    }
    virtual void Resize( const Size& newSize );


    
protected:
    /// Relative position (from parent UI) and geometry of this UI control
    Rect m_geometry;
    /// This is the absolute topleft position of this UI control
    pxy m_topleft;
    LayoutBase* m_layout;
    swTAttr m_DefTextAttr;
    int _uiflags;
private:
    /// Own screen surface data buffer - which IO operations are done using the swWriter interface
    PStr m_IoScrBuffer;
    swWriter* _wr;

    friend class swDesktop;
protected:
    virtual swUiControl* _mouseSelect( const pxy& xy );
};

#endif
