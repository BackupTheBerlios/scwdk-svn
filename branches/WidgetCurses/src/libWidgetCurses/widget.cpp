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
#include <Widget>
#include <painter.h>
#include <Application>

namespace wcurses
{

    Widget::Widget ( Object* Parent, unsigned int _wclass, const char* NameID ) : Object ( Parent, NameID ),
            _painter ( 0l ),
            _style ( Style::nul ),
            _StyleComponents ( 0l ),
            _class ( _wclass ),
            _geometry ( Rect ( 0,0,0,0 ) ),
            _layout ( 0l ),
            _buffer ( 0l ),
            _aligment(directions::left)
    {
    }


    Widget::Widget ( Object* Parent ) : Object ( Parent ),
            _painter ( 0l ),
            _style ( Style::nul ),
            _StyleComponents ( 0l ),
            _class ( 0 ),
            _geometry ( Rect ( 0,0,0,0 ) ),
            _layout ( 0l ),
            _buffer ( 0l ),
            _aligment(directions::left)
    {
    }


    Widget::Widget ( Widget* Parent ) : Object ( Parent ),
            _painter ( 0l ),
            _style ( Style::nul ),
            _StyleComponents ( 0l ),
            _class ( 0 ),
            _geometry ( Rect ( 0,0,0,0 ) ),
            _layout ( 0l ),
            _buffer ( 0l ),
            _aligment(directions::left)
    {
    }


    Widget::~Widget()
    {
    }

    /*!
        \fn wcurses::Widget::SetGeometry( const Rect& r )
     */
    bool Widget::SetGeometry ( const Rect& r )
    {
        Widget* _p = FirstParentAs<Widget>();
        _geometry = r;
        //// Debug << " recceived rect: " << r.tostring() << " <-> new geometry: "<<  _geometry.tostring();
        _topLeft = _p ? _p->TopLeft() + r.topleft() : r.topleft();
        return Resize ( _geometry.size() );
    }


    /*!
        \fn wcurses::Widget::Resize( const Size& sz, bool move_origin=false )
     */
    bool Widget::Resize ( const Size& sz, bool move_origin )
    {
        if ( _buffer ) delete [] _buffer;

        _geometry.resize ( pxy ( sz.Width(), sz.Height() ) );
        _Create();
    }

    /*!
        \fn wcurses::Widget::_Create()
     */
    int Widget::_Create()
    {
        _state = states::normal;
        _buffer = new TCell[ _geometry.Width() * _geometry.Height() * sizeof ( TCell ) + _geometry.Width() ];
        _StyleComponents = Application::StyleTheme ( "Widget.Default" );
        StyleComponents& curstyle= *_StyleComponents;
        _painter = new Painter ( this, 0, String ( NameID() + "::Painter" ).data() );
        _painter->SetStyle ( curstyle[_state&_visualStatesBits() ] );
        _painter->Clear();
        return true;
    }

    /*!
        \fn wcurses::Widget::InitView()
     */
    bool Widget::InitView()
    {
        return false;
    }

    /*!
        \fn wcurses::Widget::_updateChild(Widget* cw, const rect& _interior )
        \brief _updateChild va rendre la partie visible du buffer interne du Widget enfant donn&eacute; dans le buffer interne
        de ce Widget. le rendu sera 'clipp&eacute;' aux limite visuelles actuelles
        \note
            Seuls les widgets enfants qui ne sont pas toplevels seront clipp&eacute;s.
     */
    bool Widget::UpdateChild ( Widget* cw, const Rect& _interior )
    {
        // in child rect
        Debug << cw->NameID();
        Rect r = _interior;
        if ( ! r ) r.assign ( 0,0, cw->Geometry().Width(), cw->Geometry().Height() );
        Rect inChild = r;
        // in self rect;
        Rect inSelf  = _geometry;
        inSelf -= _geometry.topleft();
        // Clipped Region at Self
        Rect inSelfClip;

        // Get inSelf clipped child
        r += cw->Geometry().topleft();
        inSelfClip = inSelf & r;
        if ( ! inSelfClip )
        {
            Dbg << "ChildArea outside inrect:" << r.tostring();
            return false; // Child no visible inSelf
        }
        inChild = inSelfClip;
        inChild -= cw->Geometry().topleft();
        Dbg << "Copying " << inChild.tostring() << " into self::" << inSelfClip.tostring(); DEND;
        _painter->CopyWidget ( inSelfClip.topleft(), cw->DCPainter(), inChild );
        Update ( inSelfClip );
        return true;
    }


    /*!
        \fn wcurses::Widget::Update(const Rect& _interior=Rect::nil)
     */
    bool Widget::Update ( const Rect& _interior )
    {
        Debug << "Interior:" << _interior.tostring();
        Rect r = _interior;
        if ( !r ) r.assign ( 0,0, _geometry.Width(), _geometry.Height() );
        Dbg << "interior region to update:" << r.tostring();
        Widget* parentWidget = FirstParentAs<Widget>();
        Dbg << "Has parent Widget? :" << ( parentWidget ? parentWidget->NameID() : "none" );

        if ( ( ! parentWidget ) || WidgetClass ( wclass::toplevel ) )
        {
            Application::Update ( this, r );
            return true;
        }
        return parentWidget->UpdateChild ( this, r );
    }
    /*!
        \fn wcurses::Widget::Show( int state=states::normal )
     */
    void Widget::Show ( int state )
    {
        //SetState(states::disable, false);
        SetState ( states::visible|state, true );

        //_style = ( *_StyleComponents ) [state];
        _painter->SetStyle ( _style );
        ChangeState();
        Update();

    }


    bool Widget::ChangeTheme ( const std::string& strTheme )
    {

        StyleComponents* newStyle = Application::StyleTheme ( strTheme );
        if ( !newStyle ) return false;
        _StyleComponents = newStyle;
        _style = ( *_StyleComponents ) [states::normal];

        return true;
    }






    /*!
        \fn wcurses::Widget::Move( const xyCoords& xy )
     */
    void Widget::Move ( const xyCoords& xy )
    {
        /// @todo implement me
    }


    /*!
        \fn wcurses::Widget::MoveTo(const xyCoords& xy )
     */
    void Widget::MoveTo ( const xyCoords& xy )
    {
        /// @todo implement me
    }


    /*!
        \fn wcurses::Widget::ChangeState()
     */
    void Widget::ChangeState()
    {
        // _style has previously be updated
        _style = ( *_StyleComponents ) [_visualStatesBits() ];
        _painter->SetStyle ( _style );
        _painter->Clear();
        // that's it! let subclasses Widgets do paint themself
    }

    /*!
        \fn wcurses::Widget::SetupLayout()
        \brief Builds the inner layout -- to be implemented in subclasses that use layout management
     */
    int Widget::SetupLayout()
    {
        return 0;
    }
    /*!
        \fn wcurses::Widget::QueryMouseTarget(MouseEvent* M )
        \brief ----------
     */
    Widget* Widget::QueryMouseTarget ( MouseEvent* M )
    {
        Widget* widget = 0l;
        if ( !State ( states::visible ) || State ( states::disabled ) )
        {
            Debug << "Not visible/enabled --  QueryMouseTarget disabled";

        }

        Rect r = Interior();
        r +=  _topLeft;
        if ( ! r.contains ( M->Position() ) ) return 0l;

//         if(! State( states::active ) ) {
//             SetState( states::active );
//             ChangeState();
//         }
        Widget::list L;
        Widget::list::iterator i;
        if ( !QMClass ( L ) ) return this;
        for ( i=L.begin(); i!=L.end(); i++ )
        {
            if ( ( widget = ( *i )->QueryMouseTarget ( M ) ) )
            {
                L.clear();
                return widget;
            }
        }
        return 0l;
    }

    /*!
        \fn wcurses::Widget::Blur()
     */
    void Widget::Blur()
    {
        SetState ( states::normal, true );
        //_style = ( *_StyleComponents ) [_state];
        _painter->SetStyle ( _style );
        Repaint();
    }


    /*!
        \fn wcurses::Widget::Activate()
     */
    void Widget::Activate()
    {
        SetState ( states::active, true );
        //_style = ( *_StyleComponents ) [_state]; // fait dans SetState(..)
        _painter->SetStyle ( _style );
        Widget* W = FirstParentAs<Widget>();
        if ( W ) W->SetActiveChild ( this );
        Repaint();
    }
    /*!
        \fn wcurses::Widget::Repaint()
     */
    void Widget::Repaint()
    {
        _painter->Clear();
        // Iterer les child widgets
        Widget::list L;
        int nw = QMClass ( L );
        if ( !nw ) return;
        for ( Widget::Iterator I = L.begin(); I != L.end(); I++ )
        {
            ( *I )->Repaint();
        }
    }

    /*!
        \fn wcurses::Widget::Paint()
        \brief Initial render painting of the widget. Used to be called from:
        \see wcurses::Widget::InitView()
     */
    void Widget::Paint()
    {
        _painter->SetStyle ( _style );
        Debug << NameID() << ":current style set to Color[" << _style.BgColor() << ", " << _style.FgColor() << "]."; DEND;
        _painter->Clear();

        // Iterer les child widgets
        Widget::list L;
        int nw = QMClass ( L );
        if ( !nw ) return;
        for ( Widget::Iterator I = L.begin(); I != L.end(); I++ )
        {
            ( *I )->Paint();
        }
    }

    /*!
        \fn wcurses::Widget::SetAlignment( int a=directions::left )
     */
    void Widget::SetAlignment ( int a )
    {
        _aligment = a;
    }


    /*!
        \fn wcurses::Widget::Align()
     */
    void Widget::Align()
    {
        int X,Y,W,H;
        if(! _layoutContainer) return ;
        Rect r = _layoutContainer->Geometry();
        if(_aligment&directions::left) X = r.x();
        else
        if(_aligment&directions::right) X = r.Width() - _geometry.Width()-2;
        else
        if(_aligment&directions::center) X = (r.Width() - _geometry.Width())/2;

        if(_aligment& directions::top) Y = r.y();
        else
        if(_aligment& directions::vcenter) Y = (r.Height() - _geometry.Height())/2;
        else
        if(_aligment& directions::bottom)  Y = r.Height()-Height()-2;

        _geometry.assign(X,Y, Width(), Height());
        //SetGeometry(Rect(X,Y, Width(), Height())); // Clears interior and re-create buffer
    }
}
