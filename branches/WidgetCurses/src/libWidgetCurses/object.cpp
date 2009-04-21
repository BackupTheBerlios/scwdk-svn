/***************************************************************************
 *   Copyright (C) 2009 by Serge Lussier   *
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
#include "object.h"

namespace wcurses
{
    Object::Object ( Object* swParent ) :sigc::trackable(),
            _parent ( swParent ),
            _nameID ( "Object" ),
            _eventsMask ( 0 )
    {
        _ChildIT = _children.end();
        if ( swParent ) LinkToParent ( swParent );
    }


    Object::Object ( Object* swParent,const char* NameID ) :sigc::trackable(),
            _parent ( swParent ),
            _nameID ( NameID ),
            _eventsMask ( 0 )
    {
        _ChildIT = _children.end();
        if ( swParent ) LinkToParent ( swParent );
    }


    Object::Object() :sigc::trackable(),
            _parent ( 0l ),
            _nameID ( "Object" ),
            _eventsMask ( 0 )
    {
        _ChildIT = _children.end();
    }



    Object::~Object()
    {
        Debug;
        Object* o;
        if ( _children.empty() ) return;
        if ( _children.size() <=0 ) return;
        for ( Iterator it = _children.begin(); it != _children.end(); it++ )
        {
            o = *it;
            if ( o )
            {
                Dbg << "Destroying object:'" << o->NameID() << "'@[" << o << "]";
                delete o;
            }
        }
        Dbg << " clear internal children list container";
        _children.clear();
        DEND;
    }


    /*!
        \fn Object::QueryChildrenEventsMatch ( Object::List& L, event_t e )
        \brief  "Query Match Flags"
        \return Provides a list that will contains sibbling children (first level) that match the given events mask
        \note The method does not query recursively.
     */
    int Object::QueryChildrenEventsMatch ( Object::List& L, event_t e )
    {
        List::iterator it;

        for ( it = _children.begin(); it != _children.end(); it++ )
        {
            if ( ( *it )->QueryEventMask ( e ) ) L.push_back ( ( *it ) );
        }
        return L.size();
    }


    /*!
        \fn Object::LinkToParent( Object* _parent )
        \brief Class the _parent's LinkChild(this) method
        \note The call to the Object::LinkToParent(...) must be done before
              implementing any code in the subclasses
     */
    int Object::LinkToParent ( Object* parent )
    {
        Debug << "Linking self to parent";DEND;
        if ( !parent )
        {
            _parent = 0;
            return 0;
        }
        _parent = parent;
        return _parent->LinkChild ( this );
    }


    /*!
        \fn Object::LinkChild( Object* _child )
        \brief add achild object the the children list
     */
    int Object::LinkChild ( Object* child )
    {
        Debug << "linking child object:'" << ( child ? child->NameID() : "undefined" ) << " @[" << child << "]";DEND;
        if ( !child ) return _children.size();
        _children.push_back ( child );
        return _children.size();
    }
    /*!
        \fn wcurses::Object::DisableEventsSignal()
     */
    void Object::DisableEventsSignal()
    {
        std::list<sigc::connection>::iterator it = _connectedEventsSlots.begin();
        for ( it; it != _connectedEventsSlots.end(); it++ )
        {
            ( *it ).block();
        }
    }


    /*!
        \fn wcurses::Object::EnableEventsSignal()
     */
    void Object::EnableEventsSignal()
    {
        std::list<sigc::connection>::iterator it = _connectedEventsSlots.begin();
        for ( it; it != _connectedEventsSlots.end(); it++ )
        {
            ( *it ).unblock();
        }
    }

    /*!
    \fn wcurses::Object::RespondEvent( Event* E )
    */
    bool Object::RespondEvent ( Event* E )
    {
        // Si on n repond pas a cet event, alors on retourne vrai pour laisser continuer les queries
        if( !QueryEventMask(E->What()) ) return true;
        return false;
    }

}//namespace wcurses end





