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
#ifndef WCURSESAPPLICATION_H
#define WCURSESAPPLICATION_H

#include <object.h>
#include <cursesterm.h>
#include <StyledString.h>
#include <cursesattr.h>
#include <widget.h>
#include <screen.h>

namespace wcurses
{

    /**
        @author Serge Lussier,,, <tuxadmin.dev@gmail.com>
    */
    class Application : public Object
    {
        public:
            Application();

            virtual ~Application();
            virtual int Init();
            virtual bool Quit();
            virtual int InitializeUI();
            virtual int Run();
            static StyledString& TextProcessor() { return Application::_textProc; }
            static StyleComponents* StyleTheme ( const std::string& StyleName );
            static StyleComponents* AddTheme ( const std::string& stylename );
            static int Update ( Widget* w, const Rect& _interior );
            static Screen* GetScreen ( const std::string& _name );
            static Application* Instance() { return Application::_self; }
            int PushEvent ( Event* E );
        private:
            static std::map<std::string, Screen*> _screens;
            static Screen* _curscreen;

            static CursesTerm* _terminal;
            static StyledString _textProc;
            static StyleComponents::ComponentsMap StyleThemes;
            Event::List _eventsQueu;
            static int InitializeStyles();
            void ProcessEvent ( Event* E );
            int _doEvents();
            bool ProcessKeyEvent ( KeyPressEvent* K );
            Widget* _activeWidget;
            bool ProcessMessageEvent ( MessageEvent* G );
            bool ProcessMouseEvent ( MouseEvent* M );
            virtual int MouseButtonClick ( MouseEvent* M );
            virtual int MouseButtonRelease ( MouseEvent* M );
            virtual int MouseButtonPress ( MouseEvent* M );
            static Application* _self;
        protected:

            sigc::signal<bool, MessageEvent*> messageListeners_;
            sigc::signal<bool, KeyPressEvent*> keyPressCaptured_;
            sigc::signal<bool, MouseEvent*> mouseCaptured_;
            Widget* _curTarget;
        protected:
            virtual Widget* QueryTarget ( MouseEvent* M );
            int MouseButtonDblClick ( MouseEvent* M );
    void _switchActiveTarget();
    };

}

#endif


