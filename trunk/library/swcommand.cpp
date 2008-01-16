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
#include "swcommand.h"

swCommand::swCommand(swObject* swParent): swObject(swParent)
{
}


swCommand::~swCommand()
{
}

/*!
    \fn swCommand::operator ( swCommand::Delegate d )
    \brief This method returns true if the connected slot is the very same instance as the one to test
    this means that a "delegate" slot object must be previously created then passed to the swCommand::operator +=() method,
    and then saved as reference to be used with the operator == ...Otherwize "false" will always be returned ...
    The purpose of this is to be able to control the relationship of the slot ( or delegate ) object  with the command.
    When it is required to disable or to re-enable the delegate into the command, this operator ensure that the delegate
    object is properly dissociated from, or associated to the command.

    So this == operator returns true to tell that the delegate is associated ( connected ) to the command, then false if not.
 */
bool swCommand::operator ==( swCommand::Delegate d )
{
    std::list< swCommand::Delegator::iterator >::iterator I;
    swCommand::Delegator::iterator D;
    if( delegates.empty() ) return false;
    I = delegates.begin();
    do{
        D = *I;
        if ((*D) == d) return true;
    }while( I != delegates.end() );
    return false;
}


