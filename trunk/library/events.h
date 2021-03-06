/***************************************************************************
 *   Copyright (C) 2004 by Serge Lussier                                   *
 *   serge.lussier@videotron.ca                                            *
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
#ifndef NCDKNEVENT_H
#define NCDKNEVENT_H

#include "scwdk.h"
//#include "swobject.h"

//#include <ncurses.h>

#include "rect.h"
#include <map>


#include "exstring.h"


typedef unsigned long event_t;


#define CONTROLKEY  1
#define FUNCTIONKEY 2
#define DATAKEY     3


namespace event{

   extern const event_t Timer;
   extern const  event_t MouseButtonPress;           // mouse button pressed
   extern const  event_t MouseButtonRelease;         // mouse button released
   extern const  event_t MouseButtonDblClick;        // mouse button double click
   extern const  event_t MouseMove;               // mouse move
   extern const  event_t KeyPress;                // key pressed
   extern const  event_t DirectionKey;
   extern const  event_t KeySelect;
   extern const  event_t KeyRelease;              // key released
   extern const  event_t KeyFunction;
   extern const  event_t FocusIn;                 // keyboard focus received
   extern const  event_t FocusOut;                // keyboard focus lost
   extern const  event_t MouseEnter;              // mouse enters widget
   extern const  event_t MouseLeave;              // mouse leaves widget
   extern const  event_t Move;                    // move widget
   extern const  event_t Resize;                  // resize widget
   extern const  event_t Quit;                    // request to quit application
   extern const  event_t DragEnter;               // drag moves into widget
   extern const  event_t DragMove;                // drag moves in widget
   extern const  event_t DragLeave;               // drag leaves or is cancelled
   extern const  event_t Drop;                    // actual drop
   extern const  event_t DragResponse;            // drag accepted/rejected
   extern const  event_t WindowStateChange;       // window state has changed
   extern const  event_t User;                   // first user event id
   extern const  event_t TermResize;
   extern const  event_t TabNextFocus;
   extern const  event_t TabPrevFocus;
   ////  Below events may be only responded through delegates ...exept more mouse and keyin
   extern const  event_t Hide;
   extern const  event_t Show;
   extern const  event_t Save;
   extern const  event_t Open;
   extern const  event_t Close;
   extern const  event_t New;
   extern const  event_t Create;
   extern const  event_t Update;
   extern const  event_t TopLevelUpdate;
   extern const  event_t TermResize;
   extern const  event_t ResizeEvent;
   extern const  event_t PostUpdate;
   extern const  event_t MouseButtonClick;

   extern const  event_t MaxUser;                // last user event id

    extern const event_t Windows;
    extern const event_t Input;
    extern const event_t CanFocus;
    extern const event_t Done;
   std::string toString( event_t );
   enum Type{
      TimerEvent = 1,
      KeyEvent,
      MouseEvent,
      MessageEvent
   };


} // namespace event



/**
Event class and its subclasses are those which hold an event managed by the Application events queu.

@author Serge Lussier
*/

//using std::list;

class Event{
public:

   typedef std::list<Event*> List;
   typedef List::iterator iterator;
   typedef List::const_iterator const_iterator;
   
   Event();
   Event(event::Type t, event_t v) : __type(t), __e(v){  }
   Event(int _ncurses_event) : __nce(_ncurses_event){  }
   virtual ~Event();

    /*!
        \fn Event::toEventType()
        \brief Converts this event instance to an Event subclass object if applicable.
        \return non-null if dynamic_cast is successfull
     */
    template<typename _T> _T*  toEventType() { return  dynamic_cast<_T*>(this); }
    
    /*!
       \fn Event::Event()
       \brief returns the event value
     */
     int operator ()() { return __e; }


    /*!
       \fn Event::NcursesEvent()
       \brief returns the raw ncurses event value
     */
     int NcursesEvent() { return __nce; }

    /*!
        \fn Event::SetEvent( event_t e )
        \brief sets the event value
     */
    void SetEvent( event_t e ) { __e = e; }

    /*!
        \fn Event::SetEvType( event::Type t )
        \brief sets the event type class of the event
     */
    void SetEvType( event::Type t ) { __type = t;  }

    /*!
        \fn Event::Type( )
        \brief returns the - event type
     */
    event::Type Type() { return __type; }

    /*!
        \fn Event::What()
     */
    event_t What() { return __e; }

    /*!
        \fn Event::ToString()
     */
    String ToString()
    {
       String S;
       S << "No string for event " << What();
       return S;
    }

private:
    event_t __e;           ///< The event value
    event::Type __type;    ///< event type
    int __nce;             ///< Raw ncurses event value
    virtual void _translate();
};

class KeyPressEvent : public Event{
public:
    typedef sigc::signal< bool, KeyPressEvent*> KeyInputDelegate;
   KeyPressEvent(): Event(){}

   KeyPressEvent(int kv, bool meta):Event(event::KeyEvent, event::KeyPress){
      key = kv;
      bMeta = meta;
      _translate();
   }
    /*!
    \fn KeyPressEvent::KeyValue()
        \brief returns the key value
    */
    int KeyValue(){ return key; }
   /*!
       \fn KeyPressEvent::isMeta()
       \brief returns true if this key input is a meta key ( i.e.: esc-k seq; alt-key etc...
    */
   bool isMeta()    { return bMeta;  }

    /*!
        \fn KeyPressEvent::SetClass( int K )
     */
    void SetClass( int K )
    {
        Class = K;
    }

    /*!
        \fn KeyPressEvent::KeyClass()
     */
    int KeyClass()
    {
        return Class;
    }

private:
    friend class swMain;
   virtual void _translate();
   int key;
   bool bMeta;
   int Class;
   static KeyInputDelegate delegate;

    // Make the operator private and give the access only to a swMain instance
    void operator += ( sigc::slot< bool, KeyPressEvent*> slot ){
        KeyPressEvent::delegate.connect( slot );
    }
   
};




class MouseEvent : public Event{
public:
    typedef sigc::signal< bool, MouseEvent*> MouseEventDelegate;
   MouseEvent() : Event(event::MouseEvent, 0){}
   MouseEvent(MEVENT MEV):
      Event(event::MouseEvent, 0 ),
      _data(MEV){
      _translate();
   }

    /*!
        \fn MouseEvent::Y()
     */
    int Y()
    {
        return pos.y();
    }

    /*!
        \fn MouseEvent::X()
     */
    int X()
    {
        return pos.x();
    }
    pxy& Position() { return pos; }
    pxy& Local() { return localPos; }
    

   int left :1;
   int middle: 1;
   int right: 1;

    bool isLeft();
    bool isMiddle();
    bool isRight();   
    virtual String ToString();
private:
    friend class swMain;;
    MEVENT _data;
    long mleft, mright, mmiddle;
    pxy pos;
    pxy localPos;
    pxy _oldPos;
    virtual void _translate();
    bool identify_event();
    static MouseEventDelegate delegate;
    void operator += ( sigc::slot< bool, MouseEvent* > slot ){
        MouseEvent::delegate.connect( slot );
    }

};

class MessageEvent : public Event{
public:
    typedef sigc::signal< bool, MessageEvent*> MessageDelegate;
   MessageEvent(): Event() {}
   MessageEvent(event_t ev) : Event(event::MessageEvent, ev){ _sender = 0l;}
   MessageEvent(event_t ev, void* sender) : Event(event::MessageEvent, ev){ _sender = sender;}
   void* Sender() { return _sender; }
private:
    friend class swMain;
    void* _sender;
    static MessageDelegate delegate;
    void operator += ( sigc::slot< bool, MessageEvent* > slot ){
        MessageEvent::delegate.connect( slot );
    }
};



        
//class NMouse




template<typename _DT, typename T> class MsgEvent : public Event{
   public:
   MsgEvent(): Event() {}
   MsgEvent(event_t ev) : Event(event::MessageEvent, ev){ _sender = 0l;}
   MsgEvent(event_t ev, T* sender) : Event(event::MessageEvent, ev){ _sender = sender;}

   MsgEvent( event_t ev, T* sender, _DT data) : Event(event::MessageEvent, ev){
      _sender = sender;
      data_priv = data;

   }
   T*  Sender() { return _sender; }
   _DT Data() { return data_priv; }
private:
    T* _sender;
    _DT data_priv;
};


class TimerEvent : public Event{
public:

};





#endif
