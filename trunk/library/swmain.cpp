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
    _evq_x = 0l;
}


swMain::swMain( const std::string& _name, int argc, char** argv ): swObject( 0l, 0, _name.c_str() )
{
    _Self = this;
    for( int i=0; i<argc; i++ ) _args.push_back( argv[i] );
    _evq_x = 0l;
}


swMain::~swMain()
{
    _Self = 0l;
    if(_nc){
        _nc->Finish();
        delete _nc;
    }
    if(_dsk) delete _dsk;
    delete _evq_x;
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
    \fn swMain::PostEvent( Event* _e )
    
 */
int swMain::PostEvent( Event* _e )
{
    /// @todo Implement threading protection ( mutex )
    _evq_x->lock();
    _evq.push_back( _e );
    _evq_x->unlock();
    return _evq.size();
    
}


/*!
    \fn swMain::_prepare_mouse_ev( int nc )
 */
Event* swMain::_prepare_mouse_ev( int nc )
{
    MEVENT mev;
    if(getmouse( &mev ) == ERR ) return 0l;
    MouseEvent* Mev = new MouseEvent( mev );
    return mev;
}

/*!
    \fn swMain::_prepare_keyinput_ev( Event* _ev )
 */
Event* swMain::_prepare_keyinput_ev( Event* _ev )
{
    event_t ev = 0;
    NMessageEvent* Mes;
    KeyPressEvent* Kev;
    if( e == KEY_RESIZE){
        PostEvent( (Mes = new MessageEvent(event::TermResize) ) );
        return Mes;
    }
    PostEvent( ( Kev = new NKeyPressEvent( e, bMeta ) ) );
    bMeta = false;    /// @todo implement me
    return  Kev;
}

/*!
    \fn swMain::_prepare_message_ev(int nc )
 */
Event* swMain::_prepare_message_ev(int nc )
{
    /// @todo implement me
}

/*!
    \fn swMain::_preProcessEvent( Event* _ev )
 */
Event* swMain::_preProcessEvent( Event* _ev )
{
    int nc = _ev->NcursesEvent();
    MessageEvent* me;
    Event* E;
    if( _ev == ERR ) return ;
    // is mouse event ?
    switch( nc ){
        case KEY_RESIZE:
            me = new MessageEvent(event::TermResize);
            E = me;
            break;
        case KEY_MOUSE:
            E = _prepare_mouse_ev ( nc );
        break;
//        case KEY_MESSAGE:
//            E = _prepare_message_ev( nc );
//        break;
        default: // Assume keyboard input key event
            //cerr << _HERE_ << "key even- value=" << __nce << "[" << (char)__nce << ']' <<   endl;
            E = _prepare_keyinput_ev (  );
        break;
    }
    // delete original Event instance.
    delete _ev;
    // Return resulting final Event may be 0L (if event is invalid or not to be propagated)
    return E;
}

/*!
    \fn swMain::DispatchEvents()
 */
Event* swMain::DispatchEvents()
{
    // no need to lock the queu since new events can be generated from within this thread and from other thread and PostEvent does protect the queu
    _evq_x->lock();
        int S = _evq.size();
    _evq_x->unlock();
    if(!S) return 0l;
    Event::iterator i;
    Event* _e;
    // Iterate all the events in the queu as well as the ones inserted during the loop...
    for(; i!= _evq.end(); i++){
        // Last message may be Event::Quit that was inserted at the end of the queu after all the other messages
        _e = ProcessEvent( *i );
        if( _e->What() == Event::Quit ) break; // cancel the loop and process the quit event, if cancelled, inserted events after quit will then be processed otherwize, the app quits!
    }
    return _e;
}

/*!
    \fn swMain::Run()
    \brief Setup the program's environment as the main thread, waits for user/system input events, then
    dispatch it until the event::Quit is the only remaining event to proceed.
 */
int swMain::Run()
{
    Event* e;
    Debug << "Initializing...";
    if( (r = Init()) ) return r;

    do{
        _dsk->Refresh();
        e = _nc->WaitEvent();
        ///@todo Process the event
        if( (e = _preProcessEvent( e )) ){
            PostEvent( e );
        }
        e = DispatchEvents();
    }while( e->What() != event::Quit );
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


