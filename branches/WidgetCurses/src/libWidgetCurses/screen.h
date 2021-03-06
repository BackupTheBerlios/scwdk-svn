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
#ifndef WCURSESSCREEN_H
#define WCURSESSCREEN_H

//#include <object.h>
//#include <rect.h>
#include <list>
#include <widget.h>
// #include <thread.h>
// #include <mutex.h>
#include <waitcontrol.h>

namespace wcurses
{

    /**
        @author Serge Lussier,,, <tuxadmin.dev@gmail.com>
    */
    class Screen : public Object
    {
            class  _unode
            {
                public:
                    typedef std::list<_unode> Queu;
                    /*!
                        \fn wcurses::Screen::_unode::_unode()
                     */
                    _unode() : _w ( 0l ), _area ( Rect::nil )
                    {
                        /// @todo implement me
                    }
                    _unode ( Widget* w, const Rect& subR=Rect::nil )
                    {
                        _w = w;
                        _area = subR;
                        if ( ! _area )
                        {
                            _area = w->Geometry();
                            _area -= _area.topleft();
                        }
                    }
                    Rect _area;
                    Widget* _w;
            };
        public:
            Screen();

            Screen ( Object* swParent, unsigned int Flags, const char* NameID );

            ~Screen();
            static Widget* Root(const std::string& _name);
            int RemoveTopLevel ( Widget* w );
            int AddTopLevel ( Widget* w );
    Widget* Toplevel();

    /*!
        \fn wcurses::Screen::Toplevels()
     */
    Widget::list& Toplevels()
    {
        return _toplevels;
    }

        private:
            friend class Application;
            CursesTerm* _term;
            Widget* _root;
            _unode::Queu Queu;
            ///toplevel widget ( Windows ) management and update management
            Widget::list _toplevels;

            mutex* _updateMTX;
            Thread* _runUpdatesLoop;
            WaitControl* _updatesListener;

            int Initialize ( CursesTerm* TermBind ) ;
            bool Update();
            int UpdateWidget ( Widget* w, const Rect& r );
            int _GetToplevelsAbove ( Widget::list& L, Widget* w );
            int _UpdateRoot ( Screen::_unode node );
            bool Loop();
            int _updates_started;

    /*!
        \fn wcurses::Screen::StartUpdatesThread()
     */
    void StartUpdatesThread()
    {
        //_updatesListener->start();
        _updates_started = 1;
        Update();
        //_updatesListener->signal();
    }
    };

}

#endif
