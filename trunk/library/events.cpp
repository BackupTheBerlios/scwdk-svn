/******************************pthread_mutex_unlock(& mtx_evq );*********************************************
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
#include <events.h>


#include <iostream>


using std::cerr;
using std::endl;



namespace event{

   const event_t Timer            = 1;     /// Mouse is in Widget's bounds
   const event_t MouseButtonPress      = 2;     /// mouse button pressed
   const event_t MouseButtonRelease    = 4;     /// mouse button released
   const event_t MouseButtonDblClick   = 8;     /// mouse button double click
   const event_t MouseMove             = 0x10;  /// mouse move
   const event_t KeyPress              = 0x20;  /// key pressed
   const event_t DirectionKey          = 0x21;
   const event_t KeySelect             = 0x22;  /// ENTER keypress == "Select"
   const event_t KeyFunction           = 0x23;  /// Functions key
   const event_t KeyRelease            = 0x40;  /// key released
   const event_t FocusIn               = 0x80;  /// keyboard focus received
   const event_t FocusOut              = 0x100;  /// keyboard focus lost
   const event_t MouseEnter            = 0x200;  /// mouse enters widget
   const event_t MouseLeave            = 0x400;  /// mouse leaves widget
   const event_t Move                  = 0x800;  /// move widget
   const event_t Resize                = 0x1000;  /// resize widget
   const event_t Quit                  = 0x2000;  /// request to quit application
   const event_t DragEnter             = 0x4000;  /// drag moves into widget

   const event_t DragLeave             = 0x10000;  /// drag leaves or is cancelled
   const event_t Drop                  = 0x20000;  /// actual drop
   const event_t DragResponse          = 0x40000;  /// drag accepted/rejected
   const event_t WindowStateChange     = 0x80000;  /// window state has changed
   
   const event_t Termresize            = 0x200000;
   const  event_t TabNextFocus         = 0x400000;
   const  event_t TabPrevFocus          = 0x800000;
   const  event_t Hide                 = 0x1000000;
   const  event_t Show                 = 0x0;
   const  event_t Done                 = 0x2000000;
   const  event_t Save                 = 0x4000000;
   const  event_t Open                 = 0x8000000;
   const  event_t Close                = 0x1000000;
   const  event_t New                  = 0x2000000;
   const  event_t Create               = 0x4000000;
   const  event_t Update               = 0x8000000;
   const  event_t MouseButtonClick     = 0x10000000;
   const  event_t TermResize           = 0x20000000;
   const  event_t ResizeEvent          = 0x40000000;
   const  event_t PostUpdate           = 0x80000000;
   const  event_t Done                 = 0x11111111;
   
   const event_t MaxUser               = 0xFFFFFFF; // last user event id

    const event_t Windows = Move|Resize|MouseButtonPress|MouseButtonRelease|
                            MouseButtonClick|MouseButtonDblClick|WindowStateChange|
                            TabNextFocus|TabPrevFocus|Hide|Show|MouseEnter;
    //const event_t Input;
    //const event_t CanFocus;
}

Event::Event()
{
   _translate();
   /// @note Always Event::_translate() called - for polymorph call, use _translate() after the return from the contructor -- or from within the subclasse's constructor.
}


Event::~Event()
{
}





/*!
    \fn Event::_translate()
 */
void Event::_translate()
{
    /// @todo implement me
}
/*!
    \fn MouseEvent::_translate()
 */
void MouseEvent::_translate()
{

   pos = pxy(_data.x, _data.y);
   identify_event();
}

/*!
    \fn MouseEvent::identify_event()
 */
bool MouseEvent::identify_event()
{
   // reset buttons bit:
   int bs = _data.bstate;
   right = mleft = mmiddle = 0;
   if( ( bs &  BUTTON1_RELEASED ) ||
       ( bs &  BUTTON1_PRESSED  ) ||
       ( bs &  BUTTON1_CLICKED  ) ||
       ( bs &  BUTTON1_DOUBLE_CLICKED) ||
       ( bs &  BUTTON1_TRIPLE_CLICKED)) left = 1;
   else
   if( ( bs &  BUTTON2_RELEASED ) ||
       ( bs &  BUTTON2_PRESSED  ) ||
       ( bs &  BUTTON2_CLICKED  ) ||
       ( bs &  BUTTON2_DOUBLE_CLICKED) ||
       ( bs &  BUTTON2_TRIPLE_CLICKED)) middle = 1;
   else
   if( ( bs &  BUTTON3_RELEASED ) ||
       ( bs &  BUTTON3_PRESSED  ) ||
       ( bs &  BUTTON3_CLICKED  ) ||
       ( bs &  BUTTON3_DOUBLE_CLICKED) ||
       ( bs &  BUTTON3_TRIPLE_CLICKED)) right = 1;

   if( left ) {
      if( _data.bstate & BUTTON1_RELEASED ) SetEvent( event::MouseButtonRelease );
      else
      if( _data.bstate & BUTTON1_PRESSED ) SetEvent( event::MouseButtonPress );
      else
      if( _data.bstate & BUTTON1_CLICKED ) SetEvent( event::MouseButtonClick );
      else
      if( _data.bstate & BUTTON1_DOUBLE_CLICKED ) SetEvent( event::MouseButtonDblClick );
      else
      if( _data.bstate & BUTTON1_TRIPLE_CLICKED ) SetEvent( event::MouseButtonDblClick );
   }
   else
   if( middle ) {
      if( _data.bstate & BUTTON2_RELEASED ) SetEvent( event::MouseButtonRelease );
      else
      if( _data.bstate & BUTTON2_PRESSED ) SetEvent( event::MouseButtonPress );
      else
      if( _data.bstate & BUTTON2_CLICKED ) SetEvent( event::MouseButtonClick );
      else
      if( _data.bstate & BUTTON2_DOUBLE_CLICKED ) SetEvent( event::MouseButtonDblClick );
      else
      if( _data.bstate & BUTTON2_TRIPLE_CLICKED ) SetEvent( event::MouseButtonDblClick );
   }
   else
   if( right ) {
      if( _data.bstate & BUTTON3_RELEASED ) SetEvent( event::MouseButtonRelease );
      else
      if( _data.bstate & BUTTON3_PRESSED ) SetEvent( event::MouseButtonPress );
      else
      if( _data.bstate & BUTTON3_CLICKED ) SetEvent( event::MouseButtonClick );
      else
      if( _data.bstate & BUTTON3_DOUBLE_CLICKED ) SetEvent( event::MouseButtonDblClick );
      else
      if( _data.bstate & BUTTON3_TRIPLE_CLICKED ) SetEvent( event::MouseButtonDblClick );
   }
   else
   if( _oldPos != pos ){
      SetEvent( event::MouseMove );
   }
   return true;
}




/*!
    \fn KeyPressEvent::_translate()
 */
void KeyPressEvent::_translate()
{
    cerr << "==> " << (char*)(bMeta ? "meta-" : "key") << '[' << (char) key << "] (#" << key << ')'  << endl;
}








/*!
    \fn MouseEvent::isLeft()
 */
bool MouseEvent::isLeft()
{
    return left != 0;
}


/*!
    \fn MouseEvent::isMiddle()
 */
bool MouseEvent::isMiddle()
{
    return middle != 0;
}


/*!
    \fn MouseEvent::isRight()
 */
bool MouseEvent::isRight()
{
    return right !=0;
}
