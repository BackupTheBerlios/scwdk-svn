/***************************************************************************
 *   Copyright (C) 2006 by Serge Lussier   *
 *   serge@tuxadmin.net   *
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
 #include "linuxdl.h"
 #include <iostream>
 #include <cstdlib>
 #include <stdio.h>
 #include <exstring.h>

namespace linuxdl{

/*!
    \fn LinuxDL::OpenLib()
 */
bool LinuxDL::OpenLib()
{
    try{
        mDllHandle = dlopen(mLibraryPath.c_str(), RTLD_LAZY);
        if(! mDllHandle ) throw dlerror();
    }
    catch( const std::string& DlError ){
        std::cerr << DlError << std::endl;
        abort();
    }
    return true;

}


int LinuxDL::LoadAllSym()
{
    char sline[128];
    std::string S;
    std::list<std::string> L;
    std::list<std::string>::const_iterator cit;
    String text;
    int N=0,nt;
    FILE* F = popen("readelf -s","r");
    if(!F) return 0;
    do{
        fgets(sline,128,F);
        if(!sline[0]) continue;
        S= sline;
        if(( S.find("FUNC") != std::string::npos) || (S.find("OBJECT") != std::string::npos)) {
            nt = String::split(L, S," ", false,false);
            if(!nt) continue;
            cit = L.begin();
            for(int x = 0; x<8; x++) cit++;
            (*this) << *cit;
            ++N;
            L.clear();
        }
    }while(!feof(F));
    return N;
}


/*!
    \fn linuxdl::LinuxDL::StrDump()
    */
std::string LinuxDL::StrDump()
{
    String S;
    for(dllsymbols_T::iterator it = mSymbolsDict.begin(); it != mSymbolsDict.end(); it++){
        S << it->first << "@" << it->second << "\n ";
    }
    return S.std();
}

void LinuxDL::Abort(){
    abort();
}

}

