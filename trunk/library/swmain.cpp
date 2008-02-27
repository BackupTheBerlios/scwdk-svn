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
    \fn swMain::ProcessEvent( Event* _ev )
    \brief This is the central event action that is dispatched to the target(s)
    through one of the transport methods
 */
Event* swMain::ProcessEvent( Event* _ev )
{
    String msgStr;
    Debug;
    msgStr << "ev:";
    // for now just quit!

    MouseEvent* Mev;
    KeyPressEvent* Kev;
    switch ( _ev->Type() ){
        case event::MouseEvent:
            Mev = _ev->toEventType<MouseEvent>();
            if(!Mev) return 0l;
            msgStr << Mev->ToString();
            _ev = _MouseEvent( Mev );
            break;
        default:
            break;
    }
    _ev->SetEvent( event::Quit );
    Dbg << msgStr( );DEND;
    return _ev;
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
    for(i=_evq.begin(); i!= _evq.end(); i++){
        // Last message may be Event::Quit that was inserted at the end of the queu after all the other messages
        _e = *i;
        _e = ProcessEvent( *i );
        if( _e->What() == event::Quit ) break; // cancel the loop and process the quit event, if cancelled, inserted events after quit will then be processed otherwize, the app quits!
    }
    return _e;
}

/*!
    \fn swMain::Run()
    \brief Setup the program's environment as the main thread, waits for user/system input events, then
    dispatch it until the event::Quit is left.
 */
int swMain::Run()
{
    Event* e;
    int r;
    Debug << "Initializing...";
    if( (r = Init()) ) return r;

    do{
        _dsk->_do_Updates();
        e = _nc->WaitEvent();
        ///@todo Process the event
            //if( (e = _preProcessEvent( e )) ){
            PostEvent( e );
        //}
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

    // Create the events queu mutex
    _evq_x = new mutex;
    _evq_x->init();

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




/*!
    \fn swMain::_KeyInput( KeyPressEvent* Kev )
 */
Event* swMain::_KeyInput( KeyPressEvent* Kev )
{
    /// @todo implement me
}


/*!
    \fn swMain::_MouseEvent( MouseEvent* Mev )
 */
Event* swMain::_MouseEvent( MouseEvent* Mev )
{
	Debug;
 // Get the app-wide system static event delegator
    EventsDelegator::MouseEvent( Mev );
       
	switch( Mev->What() ){
		case event::MouseButtonPress:
		case event::MouseButtonClick:	
			//_selectFocus( Mev ); // Same or other UI controle selected 
	}
    switch ( Mev->What() ){
        case event::MouseButtonClick:
            // Just do the UI 's command - if any
        default:
            // no- no default events;
    }
	return Mev;    
}


/*!
    \fn swMain::_MessageEvent(MessageEvent* msg )
 */
Event* swMain::_MessageEvent(MessageEvent* msg )
{
    /// @todo implement me
}
