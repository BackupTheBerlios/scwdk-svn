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
#include "myapplication.h"
#include <painter.h>
#include <label.h>

namespace wcurses
{

    MyApplication::MyApplication()
            : Application()
    {
    }


    MyApplication::~MyApplication()
    {
    }


    int MyApplication::Init()
    {
        return Application::Init();

    }

    int MyApplication::InitializeUI()
    {
        Application::InitializeUI();
        np = new NetSpeedSensorTest(Screen::Root(""));
        np->InitView();
        
        
        return 0;
    }
    /*!
        \fn wcurses::MyApplication::TestPainterDC()
     */
    int MyApplication::TestPainterDC()
    {
        return 0;

    }

}


/*
_mainWindow = new Window ( this, wclass::window, "Main Window Test!!" );
        _mainWindow->SetGeometry ( Rect ( 5,5, 40,20 ) );
        Application::GetScreen ( "" )->AddTopLevel ( _mainWindow );
        Painter* P = _mainWindow->DCPainter();
        _mainWindow->InitView();
        P->DrawFrame ( Style ( colors::cyan, colors::white, Style::bold ), _mainWindow->Interior() );

        //// just test anohther
        Window *_Window = new Window ( this, wclass::window, "Other Window inserted to the top of the toplevel windows" );
        _Window->SetGeometry ( Rect ( 10,10, 40,14 ) );
        P = _Window->DCPainter();
        _Window->InitView();
        Application::GetScreen ( "" )->AddTopLevel ( _Window );
        P->DrawFrame ( Style ( colors::cyan, colors::white, Style::bold ), _Window->Interior() );


        //
        Debug << "test resizecenter layout ";
        LayoutBase* L = _Window->InLayout();
        //Dbg << "InLayout: " << L->Geometry().tostring();

        Label* L1 = new Label ( _mainWindow );
        Label* L2 = new Label ( _Window );
        L1->SetGeometry ( Rect ( 1,1, 20,1 ) );
        L2->SetGeometry ( Rect ( 1,1, 20,1 ) );

        L1->SetText ( "In _mainWindow" );
        L2->SetText ( "In top most Window" );
        // Test force top most _Window to update before Window below.
        L2->Update();
        // Test _mainWindow below _Window to update after the top ( ie under the topmost window)
        L1->Update();
        // Test again top most update before the underneath window ( _mainWindow )
        _Window->Update();
        // finally update underneath Window -- see result:
        _mainWindow->Update();
*/

/*!
    \fn wcurses::MyApplication::StartSensor()
 */
void wcurses::MyApplication::StartSensor()
{
    np->Start();
}
