/***************************************************************************
 *   Copyright (C) 2005 by Serge Lussier   *
 *   bretzel@tuxweb.homelinux.net   *
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
#include <mutex.h>
#include <iostream>




mutex::mutex():_mutex(0l), _init(false),__locked(false)
{
}


mutex::~mutex()
{
   std::cerr << __PRETTY_FUNCTION__ << std::endl << '{' << std::endl << "." << std::endl;
   if( _mutex ){
      unlock();
      pthread_mutex_destroy(_mutex);
      _mutex = 0l;
   }

   std::cerr << std::endl << '}' << std::endl;
}


int mutex::init()
{
   _mutex= new pthread_mutex_t;
   pthread_mutex_init(_mutex, 0l);
   _init = true;
   __locked = false;
   return true;
}


bool mutex::lock()
{
   if(!_init) return false;
   if(__locked) return false;
   pthread_mutex_lock(_mutex);
   __locked = true;
   return true;
}


void mutex::unlock()
{
   if(!_init) return;
   if(!__locked) return ;
   pthread_mutex_unlock(_mutex);
   __locked = false;
}

