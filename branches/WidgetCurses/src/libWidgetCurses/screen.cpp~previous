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
#include "screen.h"
#include <painter.h>
#include <application.h>
namespace wcurses
{

    Screen::Screen() : Object()
    {
        _updates_started = 0;
    }


    Screen::Screen ( Object* swParent, unsigned int Flags, const char* NameID ) : Object ( swParent, NameID )
    {
        _updates_started = 0;
    }


    Screen::~Screen()
    {
        // _updatesListener->release();
        // delete _updatesListener;
    }





    /*!
        \fn wcurses::Screen::Initialize( CursesTerm* TermBind )
     */
    int Screen::Initialize ( CursesTerm* TermBind )
    {
        Rect r;
        _term = TermBind;
        _root = new Widget ( this, 0, "WidgetCurses::Screen #n" );
        r.assign ( 0,0, _term->Width(), _term->Height() );
        // Debug << "Setting RootWindow geometry:";
        Dbg << r.tostring();DEND;
        _root->SetGeometry ( r );
        _glock = new mutex();
        _glock->init();
        //Dbg << "Verifying Root gemetry: " << _root->Geometry().tostring();
        //_updateMTX = new mutex();
        //_updateMTX->init();
        //_updatesListener = new WaitControl ( "Screen Update Listener" );
        //_updatesListener->init();
        //_updatesListener->delegate ( sigc::mem_fun ( this, &Screen::Update ) );
        //_updatesListener->start(); -- no Delay the loop - Application::Run() will fire it
        Painter *P = _root->DCPainter();
        P->Fill ( Rect ( 1,1, _root->Geometry().Width()-2, _root->Geometry().Height()-2 ), Style ( colors::white, colors::blue, Style::normal ), ACS_CKBOARD );
        _root->Update();
        DEND;
    }
    static bool _peer ( Widget::list& abovelist, int x,int y )
    {
        Widget::list::iterator wid;

        if ( !abovelist.size() ) return false;
        for ( wid = abovelist.begin(); wid != abovelist.end(); wid++ )
            if ( ( *wid )->Geometry().contains ( x,y ) ) return true;
        return false;
    }

    /*!
        \fn wcurses::Screen::Update()
        \brief Lorsque invoqu&eacute;e, cette methode va rafraichir les Widgets de la queu d'updates dans l'ordre d'insertion.
        \note Le code du rafraichissement n'est pas dutout optimis&eacute;. -- c'est une premi&egave;re pour moi et je vais donc optimiser celui-ci
                ult&eacute;rieurement!
                <strong> a faire: </strong>
                <ul>
                    <li> R&eacute;duire grandement les appels de m&eacute;thodes dans les boucles en preparant les donn&eacute;es appropri&eacute;es.
                        \code
                            for ( int y = 0; y < warea.Height(); y++ )
                            {
                                io = dc->Position ( pxy(warea.x(),y+ warea.y()) );
                                for ( int x = 0; x < warea.Width(); x++ )
                                {
                                    // Verifier le peerview de la coord de warea + x,y dans la liste des abovelist;
                                    // Si le toplevel se trouve etre celui au "top" alors pas de verification...
                                    if ( nt )
                                    {
                                        for ( wid = abovelist.begin(); wid != abovelist.end(); wid++ )
                                        {
                                            if ( ( *wid )->Geometry().contains ( x+warea.x(),y+warea.y()  ) )
                                            {
                                                io++;
                                                continue;
                                            }
                                        }
                                    }
                                    mvaddch ( y+warea.y(), x+warea.x(), *io++ );
                                }// for(int x = 0; x < warea.Width(); x++){
                            }// for(int y = 0; y < warea.Height(); y++){

                        \endcode
                    </li>
                    <li> ... to be continued...</li>
                </ul>
     */
    bool Screen::Update()
    {
        _unode::Queu::iterator qit = Queu.begin();
        Widget* w;
        _unode node;
        Painter* dc;
        PCell io;
        Rect uarea;
        Rect warea;
        Rect obs;
        Widget* _p;
        Widget::Iterator wid;
        //// Debug << " Size of queu: " << Queu.size() ;
        if ( !Queu.size() ) return false;
        for ( qit=Queu.begin(); qit != Queu.end(); qit++ )
        {
            node = *qit;
            w = node._w;

            Dbg << w->NameID();
            dc = w->DCPainter();
            //Dbg << "Area to Paint: " << node._area.tostring();
            io = dc->Position ( node._area.topleft() );
            Dbg << "Painter @(" <<io << ")";
            if ( !io ) continue;
            // region du toplevel a updater a l'offset de l'ecran:
            warea = node._area;
            warea += w->TopLeft();
            // garder l'intersection entre la region du update et l'ecran
            warea = Rect ( 0,0, _term->Width(), _term->Height() ) & warea;
            //Dbg "Intersection: " << warea.tostring();
            if ( !warea ) continue;

            // construire la liste des toplevels "au-dessus" du toplevel a rafraichir dans le but de bypasser la region obstruee par un toplevel du dessus...
            Widget::list abovelist;
            int nt = _GetToplevelsAbove ( abovelist, w );
            //Dbg << " Got above widget list: " << nt;
            // ajuster la region a l'offset interne du toplevel
            warea -= w->TopLeft();
            // garder l'intersection la region interne et la region a confinee a l'ecran
            uarea = node._area & warea;
            if ( ! uarea )
            {
                // Debug; // Debug; // Debug; // Debug;
                Dbg << " Serious error -- invalid interior -- ABORTING";
                abort();
            }
            // re-ajuster la copie de l'intersection a l'offset de l'ecran
            warea += w->TopLeft();
            // Effectuer le dump de la region a rafraichir:
            // // Debug;// Debug << "Start refresh at " << warea.topleft().tostring() << " for widget named: " << w->NameID() << " at" << uarea.tostring();
            for ( int y = 0; y < warea.Height(); y++ )
            {
                io = dc->Position ( pxy ( uarea.x(),y+ uarea.y() ) );
                for ( int x = 0; x < warea.Width(); x++ )
                {
                    // Verifier le peerview de la coord de warea + x,y dans la liste des abovelist;
                    // Si le toplevel se trouve etre celui au "top" alors pas de verification...
                    if ( _peer ( abovelist, x+warea.x(),y+warea.y() ) )
                    {
                        io++;
                        continue;
                    }
                    mvaddch ( y+warea.y(), x+warea.x(), *io++ );
                }// for(int x = 0; x < warea.Width(); x++){
            }// for(int y = 0; y < warea.Height(); y++){
        }// for(qit; qit != Queu.end(); qit++){
        ::refresh(); // Try force refresh becuse ncurses::getch() is blocking it;
        Queu.clear();
    }// Screen::Update()


    /*!
            \fn wcurses::Screen::_UpdateRoot(Screen::_unode node)
         */
    int Screen::_UpdateRoot ( Screen::_unode node )
    {

        return 0;
    }

    /*!
        \fn wcurses::Screen::UpdateWidget(Widget* w, const Rect& r )
     */
    int Screen::UpdateWidget ( Widget* w, const Rect& r )
    {
        //// Debug << "Preparing Widget update: " << w->NameID();
        Lock();
        Queu.push_back ( _unode ( w,r ) );
        //Unlock();
        /** \todo switch to the Application events sytem ASAP
            \code
            Application::Instance()->PushEvent(new MessageEvent(event::Update,this) );
            \endcode
        */
        if ( _updates_started ) Update();
        Unlock();
        return Queu.size();
    }



    /*!
        \fn wcurses::Screen::_GetToplevelsAbove ( Widget::list& L, Widget* w )
     */
    int Screen::_GetToplevelsAbove ( Widget::list& L, Widget* w )
    {
        // Debug;
        Widget::list::iterator it=_toplevels.begin();
        Widget*p = 0l;
        // find w;
        if ( w != _root )
        {
            for ( ; it != _toplevels.end(); it++ )
            {
                p = *it;
                if ( p == w ) break;
            }
            ++it;
        }
        for ( it ; it != _toplevels.end(); it++ ) L.push_back ( *it );
        return L.size();
    }


    /*!
    \fn wcurses::Screen::AddTopLevel( Widget* w )
    */
    int Screen::AddTopLevel ( Widget* w )
    {
        _toplevels.push_back ( w );
        return _toplevels.size();
    }

    /*!
        \fn wcurses::Screen::RemoveTopLevel( Widget* w )
    */
    int Screen::RemoveTopLevel ( Widget* w )
    {
        _toplevels.remove ( w );
        return _toplevels.size();
    }
    /*!
        \fn wcurses::Screen::Root(const std::string& _name)
     */
    Widget* Screen::Root ( const std::string& _name )
    {
        Screen* scr = Application::GetScreen ( _name );
        if ( !scr ) return ( Widget* ) 0l;
        return scr->_root;
    }

    /*!
        \fn wcurses::Screen::Toplevel()
     */
    Widget* Screen::Toplevel()
    {
        Widget* W;
        if( _toplevels.empty()) return 0l;
        Widget::list::reverse_iterator i = _toplevels.rbegin();
        //if( i == _toplevels.rend()) return 0l;
        return (*i);
    }

}




