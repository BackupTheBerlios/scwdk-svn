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
#include <swthread.h>
#include <iostream>






void *thread_start_entry(void* thread_inst)
{
   swthread* thread_instance = (swthread*) thread_inst;
   thread_instance->run_(); //.emit();
   // -------------------------------
   return 0l;
}


swthread::swthread(): swObject()
{
}

swthread::swthread(swObject* parentobj, const std::string& nameid )
: swObject(parentobj, 0, nameid.c_str())
{

}

swthread::~swthread()
{
   std::cerr << __PRETTY_FUNCTION__ << std::endl << '{' << std::endl << NameID() << std::endl;
   //run_.disconnect_all_slots();
   //end_.disconnect_all_slots();
   //notify_.disconnect_all_slots();
   pthread_cancel(_pthID);
   //std::cerr<< "thread exited ?" << std::endl;
   //std::cerr << std::endl << '}' << std::endl;
}





bool swthread::run()
{
   int rc = pthread_create( &_pthID, 0l, thread_start_entry, (void*)this);
   if(rc) return false;
   return true;
}



bool swthread::exit()
{
   //std::cerr << __PRETTY_FUNCTION__ << std::endl << '{' << std::endl << "." << std::endl;
   // signal the end of the thread just before aborting it!
   if(!end_.empty()) end_();//.emit();
   pthread_cancel(_pthID);
   std::cerr << std::endl << '}' << std::endl;
   return true;
}




