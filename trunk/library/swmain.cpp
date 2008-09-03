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

//EventDelegateGroup swMain::EventsPropagator;


EventDelegate::Delegates swMain::_delegates;

swMain* swMain::_Self=0l;

swText* swMain::g_mTextProcessor = 0l;

swAttrGroup::List swMain::_ColorsList;

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
    //_Self = 0l;
    Debug << " Ending ncurses terminal" ; DEND;
    if(_nc){
        _nc->Finish();
        //delete _nc; //!! doh!!!! here again, DO NOT DELETE HERE, ~swObject will do!!!!!!
    }
    //if(_dsk) delete _dsk; !oops! don't destroy desktop instance yet!!! ~swObject will do though its children iteration
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
event_t swMain::ProcessEvent( Event* _ev )
{
    String msgStr;
    Debug;
    msgStr << "ev:";
    if(!_ev) return 0;
    KeyPressEvent* pKev;
    MouseEvent*    pMev;
    Event*         pEv;
    Debug;
    switch( _ev->Type() ){
        case event::KeyEvent:
            Dbg << " A keyinput event " ;DEND;
            pKev = _ev->toEventType<KeyPressEvent>();
            if(! pKev ) break;
            return _KeyInput( pKev );
        case event::MouseEvent:
            pMev = _ev->toEventType<MouseEvent>();
            if(!pMev) break;
            Dbg << " A mouseinput event ";
            return _MouseEvent( pMev );
        case event::TimerEvent:
            break;
        case event::MessageEvent:
            Dbg << " A Message ( '" << _ev->What() << "') received...";
            if( _ev->What() == event::TermResize ){
                Dbg << " Termresize event received !!!!";
                Dbg << "The new term geometry is: " << Rect(0,0, _nc->Width(), _nc->Height()).tostring();
                _dsk->Resize( Size(_nc->Width(), _nc->Height() )) ;
            }
            DEND;
            return 0;
    }

    return event::Quit;//(event_t)0;
}

/*!
    \fn swMain::DispatchEvents()
 */
event_t swMain::DispatchEvents()
{
    // no need to lock the queu since new events can be generated from within this thread and from other thread and PostEvent does protect the queu
    _evq_x->lock();
        int S = _evq.size();
    _evq_x->unlock();
    if(!S) return 0l;
    Event::iterator i;
    event_t e;
    Debug;
    Event* T;
    if(_evq.empty()) return 0;
    // Iterate all the events in the queu as well as the ones inserted during the loop...
    for(i=_evq.begin(); i!= _evq.end(); i++){
        // Last message may be Event::Quit that was inserted at the end of the queu after all the other messages
        // cancel the loop and process the quit event, if cancelled, inserted events after quit will then be processed otherwize, the app quits!
        e = ProcessEvent( *i );
        Dbg " ProcessEvent returned: " << e;DEND;
        T = *i;
        //_evq.remove( T );
        delete *i;
        if(e == event::Quit ) break;
    }
    _evq.clear();
    return e;
}

/*!
    \fn swMain::Run()
    \brief Setup the program's environment as the main thread, waits for user/system input events, then
    dispatch it until the event::Quit is left.
 */
int swMain::Run()
{
    Event* e;
    event_t ev;
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
        ev = DispatchEvents();
    }while( ev != event::Quit );
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
    //InitEVD();

    swMain::g_mTextProcessor = new swText(this, swTAttr(0,7,A_NORMAL), "main::TextProcessor");
    swMain::g_mTextProcessor->Initialize();

    swMain::_ColorsList["panel"] = new swAttrGroup();
    swMain::_ColorsList["label"] = new swAttrGroup();
    swMain::_ColorsList["inputfield"] = new swAttrGroup();
    swMain::_ColorsList["frame"] = new swAttrGroup();
    swMain::_ColorsList["framecaption"] = new swAttrGroup();
    swMain::_ColorsList["fieldset"] = new swAttrGroup();
    swMain::_ColorsList["scroll"] = new swAttrGroup();
    swMain::_ColorsList[""] = new swAttrGroup();
    
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
event_t swMain::_KeyInput( KeyPressEvent* Kev )
{
    return event::Quit;
}


/*!
    \fn swMain::_MouseEvent( MouseEvent* Mev )
 */
event_t swMain::_MouseEvent( MouseEvent* Mev )
{
	Debug;
 // Get the app-wide system static event delegator
    return event::Quit;
	return 0;
}


/*!
    \fn swMain::_MessageEvent(MessageEvent* msg )
 */
event_t swMain::_MessageEvent(MessageEvent* msg )
{
    /// @todo implement me
    return 0;
}


/*
    \fn swMain::InitEVD()

bool swMain::InitEVD()
{
    EventDelegate& evd = EventsPropagator["keyinput"];
    evd[event::KeyFunction]  += sigc::mem_fun(this, &swMain::_KeyFn);
    /*
    evd[event::KeyPress]     += sigc::mem_fun(this, &swMain::_KeyInput);
    evd[event::DirectionKey] += sigc::mem_fun(this, &swMain::_ArrowKey);

    evd = EventsPropagator["MouseEvent"];
    evd[event::MouseButtonPress] += sigc::mem_fun(this, &swMain::_MouseButtonPress);
    evd[event::MouseButtonClick] += sigc::mem_fun(this, &swMain::_MouseButtonClick);
    evd[event::MouseButtonDblClick] += sigc::mem_fun(this, &swMain::_MouseButtonDblClick);
    evd[event::MouseButtonRelease] += sigc::mem_fun(this, &swMain::_MouseButtonRelease);

}


/*!
    \fn swMain::_KeyFn(Event*)

bool swMain::_KeyFn(Event* ev)
{
    KeyPressEvent* kev;
    Debug ;DEND;

    ///@todo process the functionkey
    return true;
}
*/



