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
#ifndef WCURSESOBJECT_H
#define WCURSESOBJECT_H

#include <sigc++/sigc++.h>
#include "ccutils.h"
#include "exstring.h"

#include "events.h"
#include "mutex.h"

namespace wcurses
{

    /**
        @author Serge Lussier <tuxadmin.dev@gmail.com>
    */
    class Object : public sigc::trackable
    {
        public:
            Object();
            Object ( Object* swParent );
            Object ( Object* swParent, const char* NameID );

            /*!
                            \brief
                        */
            struct breakIterator
            {
                typedef bool result_type;
                template<typename T_iterator>
                result_type operator() ( T_iterator first, T_iterator last ) const
                {
                    for ( ; first != last; ++first ) if ( !*first ) return false;
                    return true;
                }
            };

            typedef std::list<Object*> List;
            typedef List::iterator Iterator;
            virtual ~Object();
            /*!
                    \fn Object::ParentT()
                */
            template< typename T> T* ParentT()
            {
                Object* P = _parent;
                return dynamic_cast<T*> ( P );
            }
            template< typename T> T* FirstParentAs();

            /*!
                \fn Object::Parent()
             */
            Object* Parent() { return _parent; }
            // static void* operator new

            template<typename T> T* LastChild();
            template<typename T> T* FirstChild();
            template<typename T> T* NextChild();



            /*!
                \fn Object::Class()
             */
            template<typename T> T* Class( ) { return dynamic_cast<T*> ( this ); }

            /*!
                \fn Object::Class(T*)
             */
            template<typename T> T* Class ( T* _o ) { return dynamic_cast<T*> ( this ); }

            /*!
                \fn Object::QMClass( std::list<T*>& L )
             */
            template<typename T> int QMClass ( std::list<T*>& L )
            {
                T* pt;
                //Object* _o = 0l;
                Iterator it;
                for ( it = _children.begin(); it != _children.end(); it++ )
                {
                    if ( ( pt = dynamic_cast<T*> ( *it ) ) ) L.push_back ( pt );
                }
                return L.size();
            }
            /*!
                \fn Object::QMParents( std::list<T*>& L )
             */
            template<typename T> int QMParents ( std::list<T*>& L )
            {
                T* pt;
                Object* _o = this;
                while ( _o )
                {
                    _o = _o->Parent();
                    if ( ( pt = dynamic_cast<T*> ( _o ) ) ) L.push_back ( pt );
                }
                return L.size();
            }

            /*!
                \fn Object::QueryEventMask( uint _flags )
             */
            bool QueryEventMask ( event_t e ) { return ( _eventsMask & e ) != 0; }
            int QueryChildrenEventsMatch ( Object::List& L, event_t e );
            /*!
                \fn Object::SetEventMask ( event_t e, bool seton=true )
             */
            virtual event_t SetEventMask ( event_t e, bool seton=true )
            {
                event_t oldf = _eventsMask;
                if ( seton )
                    _eventsMask |= _eventsMask;
                else
                    _eventsMask &= ~_eventsMask;
                return oldf;
            }

            /*!
                \fn sigc::signal<void, Object*, uint>::iterator operator += ( sigc::slot<void, Object*, uint> _delegate )
                \brief This operator adds a new slot that an object connects to the events signal
                \note There is no means to track the list of individual connections
            */
            sigc::connection
            operator += ( sigc::slot<void, Object*, event_t> _delegate )
            {
                return Events_.connect ( _delegate );
            }

            /*!
                \fn Object::NameID()
             */
            const std::string& NameID() { return _nameID; }
            void SetNameID ( const std::string& s ) { _nameID = s; }
            virtual int LinkToParent ( Object* _parent );
            virtual int LinkChild ( Object* _child );

            /*!
                \fn Object::ParentOfClass( T* _type )
             */
            template<typename T> T* ParentOfClass ( T* _type )
            {
                if ( ! ( ( T* ) _parent ) ) return ( T* ) 0l;
                return dynamic_cast<T*> ( _parent );
            }
            void DisableEventsSignal();
            void EnableEventsSignal();

            /*!
                \fn wcurses::Object::Unlock()
             */
            void Unlock()
            {
                _glock->unlock();
            }

            /*!
                \fn wcurses::Object::Lock()
             */
            bool Lock()
            {
                if ( _glock->islocked() ) return false;
                _glock->lock();
                return true;
            }

        protected:
            /// events bitmap that the object will react/respond
            event_t _eventsMask;
            std::string _nameID;
            Object* _parent;
            Object::List _children;
            /// Global utility mutex for concurent resources usagewhen needed
            mutex* _glock;
        private:
            sigc::signal<void, Object*, uint> Events_;
            //static Object::List* glb_managed;
            Object::Iterator _ChildIT;
            /** list of connections to the events used to disable and re-enable signal emition according to the
                state of this object. disbaled state will block the signal emition from this object
            */
            std::list<sigc::connection> _connectedEventsSlots;


protected:
    virtual bool RespondEvent( Event* E );
    };





    /*!
        \fn Object::FirstParentAs()
     */
    template< typename T> T* Object::FirstParentAs()
    {
        Object* _o = this;
        T* parent;
        while ( _o )
        {
            if ( ( parent = _o->ParentT<T>() ) != 0l ) return parent;
            _o = _o->Parent();
        }
        return ( T* ) 0l;
    }

    /*!
        \fn Object::LastChild()
        */
    template<typename T> T* Object::LastChild()
    {
        Object* _o = this;
        T* _child;
        //Object::Iterator IT = _ChildIT;
        if ( _children.size() ==0 ) return ( T* ) 0l;
        _ChildIT = _children.end();
        --_ChildIT;
        while ( _ChildIT != _children.end() )
        {
            if ( ( _child = dynamic_cast<T*> ( *_ChildIT ) ) ) return _child;
            --_ChildIT;
        }
        return ( T* ) 0l;
    }

    /*!
        \fn Object::FirstChild()
        */
    template<typename T> T* Object::FirstChild()
    {
        T* _child;
        _ChildIT = _children.begin();
        while ( _ChildIT != _children.end() )
        {
            if ( ( _child = dynamic_cast<T*> ( *_ChildIT ) ) ) return _child;
            ++_ChildIT;
        }
        return ( T* ) 0l;
    }

    /*!
        \fn Object::NextChild()
        */
    template<typename T> T* Object::NextChild()
    {
        T* _child;
        if ( _ChildIT == _children.end() ) return ( T* ) 0l;
        ++_ChildIT;
        while ( _ChildIT != _children.end() )
        {
            if ( ( _child = dynamic_cast<T*> ( *_ChildIT ) ) ) return _child;
            ++_ChildIT;
        }
        return ( T* ) 0l;
    }
}

#endif
