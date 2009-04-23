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
#include "window.h"
#include <Application>
#include "painter.h"
namespace wcurses
{

    Window::Window ( Object* Parent, unsigned int Flags, const char* NameID ) : Widget ( Parent, Flags, NameID )
    {
    }


    Window::Window ( Widget* Parent ) : Widget ( Parent )
    {
    }


    Window::~Window()
    {
    }


    bool Window::Active ( int e )
    {
        return Widget::Active ( e );
    }

    bool Window::ChangeTheme ( const std::string& strTheme )
    {
        return Widget::ChangeTheme ( strTheme );
    }

    bool Window::Enable ( int e )
    {
        return Widget::Enable ( e );
    }

    bool Window::Resize ( const Size& sz, bool move_origin )
    {
        return Widget::Resize ( sz, move_origin );
    }

    bool Window::SetGeometry ( const Rect& r )
    {
        Widget::SetGeometry ( r );
        if ( _layout ) _layout->SetGeometry ( r );

    }

    bool Window::Visible ( bool e )
    {
        return Widget::Visible ( e );
    }

    bool Window::InitView()
    {
        Widget::InitView();
        _frameStyle = Application::StyleTheme("Window.Frame");
        _painter->SetStyle( (*_frameStyle)[states::normal] );
        _painter->DrawFrame((*_frameStyle)[states::normal], Interior() );
        SetupLayout();
        return true;

    }

    void Window::SetState ( int e, bool y )
    {
        Widget::SetState ( e, y );
    }

    void Window::Show ( int state )
    {
        Widget::Show ( state );
        Application::GetScreen("")->AddTopLevel(this);
    }




    /*!
        \fn wcurses::Window::Hide()
     */
    void Window::Hide()
    {
        //SetState(states::visible, false);
        Visible(false);
        Application::GetScreen("")->RemoveTopLevel(this);
    }


    /*!
        \fn wcurses::Window::IconText()
     */
    String Window::IconText()
    {
        /// @todo implement me
    }


    /*!
        \fn wcurses::Window::SetupLayout()
     */
    int Window::SetupLayout()
    {
        Rect r = Interior();
        _layout = new LayoutBase ( this );
        r.resizecenter ( pxy ( -1,-1 ) );
        // Debug << " layout interior rect:" << r.tostring();
        _layout->SetGeometry ( r );
        Dbg << "Layout geometry:" << r.tostring() << " Window: " << Interior().tostring(); /// @todo implement me
        return 0;
    }

}
