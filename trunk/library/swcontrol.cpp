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





#include <swcontrol.h>
#include <iostream>





swcontrol::swcontrol():swthread(), mutex(),_cond(false)
{
}
swcontrol::swcontrol(const std::string& nameid ): mutex(),
   swthread(0, nameid){
   _cond=false;
}


int swcontrol::wait()
{
   //lock();
   int CV_ERR =0;
   while( ! _cond ){
      lock();
      // Blocking here ( sleeping ):
      // On bloque ici:
      CV_ERR = pthread_cond_wait( _cv, mutexhandle() );
   }
   //unlock(); // pas besoin de debloquer le mutex...
   if( _state == controlstate::proceed ) proceed_();
   if( _state == controlstate::terminate){
      release();
      return controlstate::terminate;
   }
   //
   _cond = false;
   return state();
}
void swcontrol::signal()
{
   if( _cond ) return;
   _cond = true;
   //lock();
   pthread_cond_signal(_cv );
   //unlock();
}


swcontrol::~swcontrol()
{
   std::cerr << __PRETTY_FUNCTION__ <<  std::endl << '{' << std::endl << "." << std::endl;
   std::cerr << NameID() << std::endl;
   //proceed_();
   std::cerr << "releasing condition variable..." << std::endl;
   release();
   std::cerr << '}' << std::endl;
}



void swcontrol::terminate()
{
   _state = controlstate::terminate;
   if(!_cond ){
      _cond=true;
      swcontrol::signal();
   }

}


bool swcontrol::start()
{
//    std::cerr << __PRETTY_FUNCTION__ << std::endl << '{' << std::endl << "." << std::endl;
//    std::cerr << Name() << std::endl;
   _state = controlstate::proceed;
//    std::cerr << std::endl << '}' << std::endl;
   return run();
}


int swcontrol::init()
{
   mutex::init();
   _cv = new pthread_cond_t;
   pthread_cond_init(_cv, 0l);
   return true;

}


void swcontrol::release()
{
//    std::cerr << __PRETTY_FUNCTION__ << std::endl << '{' << std::endl << "." << std::endl;
//    std::cerr << Name() << std::endl;
   if(_mutex) unlock();
   if(_cv) {
     int ER =  pthread_cond_destroy(_cv);
      if(ER){
         std::cerr << " -- error trying to release condition variable..." << std::endl;
      }
      delete _cv;
      _cv=0l;
   }
   if(_mutex){
      delete _mutex;
      _mutex = 0l;
   }
//    std::cerr << "terminating inner thread instance..." << std::endl;
   swthread::exit();
//    std::cerr << std::endl << '}' << std::endl;
}

bool swcontrol::controller_loop()
{
   do ; while(this->wait() != controlstate::terminate);
   return controlstate::terminate == _state;
}



