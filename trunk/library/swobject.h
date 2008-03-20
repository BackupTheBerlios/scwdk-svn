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
#ifndef SWOBJECT_H
#define SWOBJECT_H

#include "scwdk.h"
#include <list>
//#include <sigc++/sigc++.h>


/*!
    \class swObject
    \brief Mother Of All scwdk classes :-) Common interface base object definition
	\author Serge Lussier  lussier.serge@gmail.com
*/
class swObject : public sigc::trackable
{
public:
    struct interruptible {
        typedef bool result_type;
        template<typename T_iterator>
        result_type operator()(T_iterator first, T_iterator last) const
        {
            for (; first != last; ++first ) if (!*first) return false;
            return true;
        }
    };

    typedef std::list<swObject*> List;
    typedef List::iterator Iterator;
    swObject();
    swObject(swObject* swParent);
    swObject(swObject* swParent, uint _flags, const char* _nameID);

    void SetNameID( const std::string& n ){ m_nameID = n; }
    virtual ~swObject();
    /*!
        \fn swObject::ParentT()
    */
    template< typename T> T* ParentT(){
        return dynamic_cast<T*>(m_parent);
    }
    template< typename T> T* FirstParentAs();

    /*!
        \fn swObject::Parent()
     */
    swObject* Parent() { return m_parent; }
   // static void* operator new

    template<typename T> T* LastChild();
    template<typename T> T* FirstChild();    
    template<typename T> T* NextChild();
    int QMF( swObject::List& L, uint _flags ) ;
    

    /*!
        \fn swObject::Class()
     */
    template<typename T> T* Class( ) { return dynamic_cast<T*>( this ); }

    /*!
        \fn swObject::Class(T*)
     */
    template<typename T> T* Class(T* _o ) { return dynamic_cast<T*>( this ); }

    /*!
        \fn swObject::QMClass( std::list<T*>& L )
     */
    template<typename T> int QMClass( std::list<T*>& L )
    {
        T* pt;
        //swObject* _o = 0l;
        Iterator it;
        for( it = m_children.begin(); it != m_children.end(); it++ ){
            if( (pt = dynamic_cast<T*>( *it )) ) L.push_back( pt );
        }
        return L.size();
    }
    /*!
        \fn swObject::QMParents( std::list<T*>& L )
     */
    template<typename T> int QMParents( std::list<T*>& L )
    {
        T* pt;
        swObject* _o = this;
        while(_o){
            _o = _o->Parent();
            if( (pt = dynamic_cast<T*>( _o )) ) L.push_back( pt );
        }
        return L.size();
    }

    /*!
        \fn swObject::QFlags( uint _flags )
     */
    uint QFlags( uint _flags ) { return m_flags & _flags; }

    /*!
        \fn swObject::SetFlags( uint _flags, bool seton=true )
     */
    virtual uint SetFlags( uint _flags, bool seton=true )
    {
        uint oldf = m_flags;
        if(seton)
            m_flags |= _flags;
        else
            m_flags &= ~_flags;
        flags_( this, m_flags );
        return oldf;
    }

    /*!
        \fn sigc::signal<void, swObject*, uint>::iterator operator += ( sigc::slot<void, swObject*, uint> _delegate )
        \brief
    */
    sigc::signal<void, swObject*, uint>::iterator
    operator += ( sigc::slot<void, swObject*, uint> _delegate ){
        return flags_.connect( _delegate );
    }

    /*!
        \fn swObject::NameID()
     */
    const std::string& NameID() { return m_nameID; }
    virtual int LinkToParent( swObject* _parent );
    virtual int LinkChild( swObject* _child );

    /*!
        \fn swObject::ParentOfClass( T* _type )
     */
    template<typename T> T* ParentOfClass( T* _type )
    {
        if( !( (T*)m_parent )) return (T*)0l;
        return dynamic_cast<T*>(m_parent);
    }
    
protected:
    uint m_flags; // Common flag states
    std::string m_nameID;
    swObject* m_parent;
    swObject::List m_children;
private:
    sigc::signal<void, swObject*, uint> flags_;
    //static swObject::List* glb_managed;
    swObject::Iterator m_ChildIT;
};


    /*!
        \fn swObject::FirstParentAs()
     */
template< typename T> T* swObject::FirstParentAs()
{
    swObject* _o = this;
    T* _parent;
    while( _o ){
        if( ( _parent = _o->ParentT<T>() ) != 0l ) return _parent;
        _o = _o->Parent();
    }
    return (T*)0l;
}

/*!
    \fn swObject::LastChild()
    */
template<typename T> T* swObject::LastChild()
{
    swObject* _o = this;
    T* _child;
    //swObject::Iterator IT = m_ChildIT;
    if( m_children.size() ==0 ) return (T*)0l;
    m_ChildIT = m_children.end();
    --m_ChildIT;
    while( m_ChildIT != m_children.end() ){
        if((_child = dynamic_cast<T*>(*m_ChildIT) )) return _child;
        --m_ChildIT;
    }
    return (T*) 0l;
}

/*!
    \fn swObject::FirstChild()
    */
template<typename T> T* swObject::FirstChild()
{
    T* _child;
    m_ChildIT = m_children.begin();
    while( m_ChildIT != m_children.end() ){
        if((_child = dynamic_cast<T*>(*m_ChildIT) )) return _child;
        ++m_ChildIT;
    }
    return (T*) 0l;
}

/*!
    \fn swObject::NextChild()
    */
template<typename T> T* swObject::NextChild()
{
    T* _child;
    if( m_ChildIT == m_children.end() ) return (T*) 0l;
    ++m_ChildIT;
    while( m_ChildIT != m_children.end() ){
        if((_child = dynamic_cast<T*>(*m_ChildIT) )) return _child;
        ++m_ChildIT;
    }
    return (T*) 0l;
}

// template<typename T > int ClearList( std::list<T*>& L );
#endif
