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
#include "swobject.h"

//swObject::List* swObject::glb_managed = 0l;


swObject::swObject(swObject* swParent):sigc::trackable(),
  m_parent( swParent ),
  m_nameID("swObject"),
  m_flags(0)
{
    m_ChildIT = m_children.end();
    if( swParent ) LinkToParent( swParent );
}


swObject::swObject(swObject* swParent, uint _flags, const char* _nameID):sigc::trackable(),
  m_parent( swParent ),
  m_nameID(_nameID),
  m_flags(_flags)
{
    m_ChildIT = m_children.end();
    if( swParent ) LinkToParent( swParent );
}


swObject::swObject():sigc::trackable(),
  m_parent( 0l ),
  m_nameID("swObject"),
  m_flags(0)
{
    m_ChildIT = m_children.end();
}



swObject::~swObject()
{
    Debug;
    swObject* o;
    if( m_children.empty() ) return;
    for( Iterator it = m_children.begin(); it != m_children.end(); it++){
        o = *it;
        if(o){
            Dbg << "Destroying object:'" << o->NameID() << "'@[" << o << "]";
            delete o;
        }
    }
    Dbg << " clear internal children list container";
    m_children.clear();
    DEND;
}


/*!
    \fn swObject::QMF( swObject::List& L, uint _flags )
    \brief  "Query Match Flags"
    \return Provided list that will contains children that matches the flags
 */
int swObject::QMF( swObject::List& L, uint _flags ) 
{
    List::iterator it;
    for( it = m_children.begin(); it != m_children.end(); it++){
        if( (*it)->m_flags == _flags ) L.push_back( (*it) );
    }
    return L.size();
}


/*!
    \fn swObject::LinkToParent( swObject* _parent )
    \brief Class the _parent's LinkChild(this) method
    \note The call to the swObject::LinkToParent(...) must be done before
          implementing any code in the subclasses
 */
int swObject::LinkToParent( swObject* _parent )
{
    if( !_parent ){
        m_parent = 0;
        return 0;
    }
    m_parent = _parent;
    return _parent->LinkChild( this );
}


/*!
    \fn swObject::LinkChild( swObject* _child )
    \brief add achild object the the children list
 */
int swObject::LinkChild( swObject* _child )
{
    Debug << "linking child object:'" << (_child ? _child->NameID() : "NULL") << " @[" << _child << "]";DEND;
    if(!_child) return m_children.size();
    m_children.push_back( _child );
    return m_children.size();
}
