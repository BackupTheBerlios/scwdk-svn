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
#include "swmain.h"


swMain* swMain::_Self=0l;

swMain::swMain()
 : swObject()
{
    _Self = this;
}


swMain::swMain( const std::string& _name, int argc, char** argv ): swObject( 0l, 0, _name.c_str() )
{
    _Self = this;
    for( int i=0; i<argc; i++ ) _args.push_back( argv[i] );
    
}


swMain::~swMain()
{
    _Self = 0l;
    if(_nc){
        _nc->Finish();
        delete _nc;
    }
    if(_dsk) delete _dsk;

}




/*!
    \fn swMain::QueryExit(swObject* _sender )
    \brief Calls all registered object to be notified of the exit request.
    \param swObject* _sender : The object that requests the exit - ( modifiable, receives the address of the object that cancels the request )
    \return bool true: go ahead and exit; false: exit request cancelled by one of the connected delegates
    
 */
bool swMain::QueryExit(swObject* _sender )
{
    Debug;
    bool F = QueryExit_( _sender );
    if(!F){
        Dbg << " Exit cancelled by object named: " << (_sender ? _sender->NameID() : "unknown(nul)");
        DEND;
        return F;
    }
    return true;
}



/*!
    \fn swMain::Run()
    \brief Setup the program's environment as the main thread, waits for user/system input events, then
    dispatch it until the event::Quit is the only remaining event to proceed.
 */
int swMain::Run()
{
    int r;
    Debug << "Initializing...";
    if( (r = Init()) ) return r;
    // nothing else to do for now  - just test queryexit
    do{
        r = _nc->WaitEvent();
        

/*!
    \fn ncdk::NApplication::translate_ncurses_event ( int )
    \brief Identify the event code from the ncurses::getch() and then translate the event to the ncdk events engine.

void NApplication::translate_ncurses_event ( int __nce )
{
   if( __nce == ERR ) return ;
   // is mouse event ?
   switch( __nce ){
      case KEY_RESIZE:
         PostEvent( new NMessageEvent(event::TermResize) );
         break;
      case KEY_MOUSE:
         cerr << _HERE_ << "Mouse event " << endl;
         translate_mouse_ev ( __nce );
      break;
      case KEY_MESSAGE:
         translate_message_ev( __nce );
      break;
      default: // Assume keyboard input key event
         //cerr << _HERE_ << "key even- value=" << __nce << "[" << (char)__nce << ']' <<   endl;
         translate_keyinput_ev ( __nce );
      break;
   }
}
*/
    Desktop( 0 )->_do_Updates();
    (void)QueryExit( this );
    Dbg << "returned from QueryExit_"; DEND;
    //_nc->WaitEvent();
    getch();
    return 0;
}


/*!
    \fn swMain::Init()
 */
int swMain::Init()
{
    Debug;
    int r =0;
    _nc = new swNCurses(this);
    if( ( r = _nc->Init() ) ) {
        Dbg << " swNcurses::Init() returned fail ...";
        return r;
    }
    _dsk = new swDesktop( this, 0, "swMain::Desktop = default");
    if( ( r = _dsk->Init() ) ){
        Dbg << " Desktop failed to init ???";DEND;
        return r;
    }
    if( ( r = RunOptions() ) ) return r;
    return 0;
}


/*!
    \fn swMain::RunOptions()
 */
int swMain::RunOptions()
{
    Debug;
    for( std::list< std::string >::iterator it = _args.begin(); it != _args.end(); it++ )
        Dbg << *it;
    DEND;
    return 0; // test
}
