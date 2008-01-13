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
#include "swncurses.h"



// Temporary -- remove after tests
#include "swtattr.h"
// ---------

#include "swmain.h"

swNCurses* swNCurses::Self=0l;









swNCurses::swNCurses(swObject* swParent): swObject(swParent)
{
    Self = this;
}

swNCurses::swNCurses(){
    Self = this;
}

swNCurses::~swNCurses()
{
}


const std::string& swNCurses::NameID()
{
    return swObject::NameID();
}



/*!
    \fn swNCurses::Init()
 */

int swNCurses::Init()
{
    Debug;
    short x,y;
    if(!Self) {
        std::cerr << __PRETTY_FUNCTION__ << " you dumb! You must first create an instance of swNCurses!!!!! Aborting..." << std::endl;
        abort();
    }
/*    D = linuxdl::LinuxDL("/usr/lib/libncurses.so", true);
    if(! D.OpenLib() ) return 128;
    D   << "mvaddch"        >> OutChXY
        << "addch"          >> OutCH
        << "mvaddchstr"     >> OutZPstrXY
        << "mvaddchnstr"    >> OutPstrXY
        << "init_pair"      >> CreateColor
        << "start_color"    >> EnableColors
        << "pair_content"   >> DecodeColor
        << "mvinch"         >> PeekXY
        << "initscr"        >> InitNCurses
        << "getmouse"       >> MouseEvent
        << "mvinchnstr"     >> CopyXYL
        << "cbreak"         >> EnableBreak
        << "mousemask"      >> SetMouseMask
        << "curs_set"       >> CursorON
        << "noecho"         >> NoEcho
        << "keypad"         >> KeyPad
        << "meta"           >> Meta
        << "endwin"         >> End
        << "mvaddstr"       >> OutStrXY
        << "raw"            >> SetToRawMode
        << "getch"          >> WaitEvent
        //<<"color_content"   >> DecodeColor
        << "PAIR_NUMBER"    >> ColorNum
        << "getattrs"       >> getattrs
        << "getcurx"        >> getcurx
        << "getcury"        >> getcury
        << "getbegx"        >> getbegx
        << "getbegy"        >> getbegy
        << "getmaxx"        >> getmaxx
        << "getmaxy"        >> getmaxy
        << "getparx"        >> getparx
        << "getpary"        >> getpary
        << "stdscr"         >> ScrWin;
        //<< "stdscr"         >> stdscr;
        */
        
        
//--------------------------------------------------------------

///@todo Check for NULL PTR!!!!
    initscr();
    //ScrWin = (WINDOW*)D["stdscr"];
    ScrWin = stdscr;
    Dbg << "ncurses::stdscr ( swNCurses::ScrWin ) = " << ScrWin << "max: " << ScrWin->_maxx << ":" << ScrWin->_maxy;
    start_color();
    swTAttr::init();
    raw();
    noecho();
    keypad(ScrWin,true);
    cbreak();
    curs_set(false);
    mousemask( ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, 0 );
    getmaxyx( ScrWin, y, x );
    Dbg << " screen size: " << x << "x" << y;
    _scrSize(x,y); 
    meta(ScrWin, true);
    Dbg<<   " Good to GO!!!" << DEND;
    swMain* Main = swMain::Instance();
    if(!Main){
        Debug << " er... No main app object ??" ;DEND;
        return 128;
    }
    (*Main) += sigc::mem_fun( this, & swNCurses::_CanExit);
    return 0;
}


/*!
    \fn swNCurses::CursesAbort( int )
 */
void swNCurses::CursesAbort( int E )
{

}


/*!
    \fn swNCurses::CursesAbort( const std::string& _erstr )
 */
void swNCurses::CursesAbort( const std::string& _erstr )
{
//     std::cerr << _erstr << "\n" << D.StrDump() << std::endl;
//     D.StrDump();
}


/*!
    \fn swNCurses::Finish()
 */
int swNCurses::Finish()
{
    Debug;
    //End();
    endwin();
    //D.CloseLib();
}
/*!
    \fn swNCurses::LibraryInstance()
    */
// linuxdl::LinuxDL* swNCurses::LibraryInstance()
// {
//     return 0l;//&D;
// }
// 

/*!
    \fn swNCurses::_CanExit( swObject*& _sender )
 */
bool swNCurses::_CanExit( swObject*& _sender )
{
    Debug << " let the app exit";DEND;
    return true;
}


/*!
    \fn swNCurses::WaitEvent()
    \brief Just blocks on ncurses::getch() and check/set for the ESC key for META state
    I did write a better stdin input listener to get immediate real ESC key and real control keys
    but it cannot work ( conflicts with ) in the gpm/ncurses env.
 */
Event* swNCurses::WaitEvent()
{
    int ncurses_event = 0;
    KeyPressEvent* _ke;
    Event* _ev;
    bMeta = false;
    REGET:
    if( (ncurses_event = getch()) != ERR){
        if(ncurses_event == 27){
            bMeta = true;
            goto REGET;
        }
    }
    // If bMeta is set ( meta-key input ) it is surely a keypress input event!!! Thus the goal is to transport the event with the meta flag set.
    return _preProcess( ncurses_event, bMeta );
}





/*!
    \fn swNCurses::_mouseEvent( int nc )
 */
MouseEvent* swNCurses::_mouseEvent( int )
{
    MEVENT mev;
    // Just a test for svn+ssh with KDevelop 3.5.0  Sabayon 3.5 loop1 bet
    if(getmouse( &mev ) == ERR ) return 0l;
    MouseEvent* Mev = new MouseEvent( mev );
    return Mev;
}


/*!
    \fn swNCurses::_inKey(int nc, bool m)
 */
KeyPressEvent* swNCurses::_inKey(int nc, bool m)
{
    KeyPressEvent* key=0l;
    // Preprocess key input:
    // Arrow keys, function keys, ESC, CTRL, etc...
    key = new KeyPressEvent( nc,m);
    if( (nc >= KEY_F(1)) && (nc <= KEY_F(12)) ) {
        key->SetClass( FUNCTIONKEY );
        key->SetEvent( event::KeyFunction );
        return key;
    }
    key->SetClass(CONTROLKEY);
    if( (nc >= KEY_DOWN) && (nc <= KEY_RIGHT)  || ( nc==KEY_HOME) || (nc==KEY_END) ) {
        key->SetEvent( event::DirectionKey );
        return key;
    }
    
    key->SetClass( DATAKEY );
    key->SetEvent( event::KeyPress );
    return key;
}


/*!
    \fn swNCurses::_preProcess( int nce, bool m )
 */
Event* swNCurses::_preProcess( int nce, bool m )
{
    Event* _e = 0l;
    switch( nce ){
    // Pre-process the ncurses event ( input events such as keyin or mouse
        case KEY_MOUSE:
            return _mouseEvent( 0 );
        default:
            return _inKey( nce, m );
    }
    return _e;
}
