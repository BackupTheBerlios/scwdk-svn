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
#ifndef LINUXDL_H
#define LINUXDL_H
#include <string>
#include <map>
#include <dlfcn.h>
#include <iostream>
#include <list>
#include <cstdlib>

#include <sigc++/sigc++.h>

namespace linuxdl{


/*!
    \class LinuxDL
    \author Serge Lussier <serge@tuxadmin.net>
    \brief The LinuxDL is the proxy interface for opening,loading and resolve a dynamic library and its needed symbols through the libdl API.
    <UL>
        <li> Opens the shared library file, given by its attribute member named <b><i>mLibraryPath</i></b>.</li>
        <li> Loads, resolves needed symbols individually or through the insertion operator <b><<</b>
        <ul>
            <li> Single symbol: \code DllInstance << "function"; \endcode</li>
            <li> Several symbols at once: \code DllInstance << "functionName1" << "functionName2" << "public_var"; \endcode<li>
            <li> All exported symboles: \code DllInstance.LoadAllSym(); \endcode </li>
        </ul>
    </ul>
    The LinuxDL object will abort if a given symbol name isn't found .
    
    Example for resolving and calling a function:
    \code
    // ...Instanciate LinuxDL and open /usr/lib/libncurses.so:
    linuxdl::dlf3<int,int,int, const char*>::T OutCharAt;
    linuxdl::LinuxDL dll("/usr/lib/libncurses.so");
    dll.OpenLib();
    dll << "mvwaddch" >> OutCharAt;
    
    if( OutCharAt) OutCharAt( py,px,ch);
    ...
    \endcode

    Regarding functions symbols and their signature:<br>
    The LinuxDL class has one template return type of a function_ptr with unspecified arguments list: <b><i>typedef return_type (*T)(...);</i></b> Then objects of type <i><b>T</b></i> receives the address of the resolved symbol.
    When calling <b><i>T</i></b>, it is the responsability of the caller to KNOW the signature of that function, thus providing the proper number and type of those arguments.<br>
    <h3>Overhead </h3>
    It is <b><u><i>recommended</i></u></b> to define the function_ptrs table in advance and to load the symbols at once into it.
    ~LinuxDL(): Automaticaly close the library and kleanup internal private data.
    \note Is is <i>important</i> to know that this class handles <i>only</i> external C dynamic libraries.<br>
           External classes ( such as "plugins" ) management is in the way....
    \author Serge Lussier <serge@tuxadmin.net>
*/

// template<typename t> t _nul(){
//     return (t)0;        
// };
// 
// template<typename ret, typename _1 > ret _nul( _1 ){
//     return (ret)0;
// };
// 
// template<typename ret, typename  _1, typename _2 > ret _nul( _1, _2) {
//     return (ret)0;
// };
// 
// template<typename ret, typename _1, typename _2, typename _3 > ret _nul( _1, _2, _3 ) {
//     return (ret)0;
// };
// 
// 
// template<typename ret, typename _1, typename _2, typename _3, typename _4  > ret _nul( _1, _2, _3, _4 ) {
//     return  (ret)0;
// };
// 
// template<typename ret, typename _1, typename _2, typename _3, typename _4, typename _5  > ret _nul( _1, _2, _3, _4, _5 ){
//     return  (ret)0;
// };
// 
// template<typename ret, typename _1, typename _2, typename _3, typename _4, typename _5, typename _6  > ret _nul( _1, _2, _3, _4, _5, _6 ){
//     return (ret)0;
// };
// 

/*!
    \class template<typename t> class dlf
    \brief   This one is a dangerous highly unstable naquadah bomb
    \note This one is a dangerous highly unstable naquadah bomb
*/
template<typename t> class dlf{
    public:
        typedef t (*T)(...);
};
// ------------------------------------

template<typename t> class dlf0{
public:
    typedef t (*T)();
    typedef sigc::slot<T> nul;
        
};

template<typename ret, typename _1 > class dlf1{
public:
    typedef ret (*T)( _1 );
    
};

template<typename ret, typename  _1, typename _2 > class dlf2{
public:
    typedef ret (*T)( _1, _2 );
};

template<typename ret, typename _1, typename _2, typename _3 > class dlf3{
    public:
        typedef ret (*T)( _1,_2, _3 );
};


template<typename ret, typename _1, typename _2, typename _3, typename _4  > class dlf4{
    public:
        typedef ret (*T)( _1,_2,_3, _4 );
};

template<typename ret, typename _1, typename _2, typename _3, typename _4, typename _5  > class dlf5{
    public:
        typedef ret (*T)( _1,_2,_3,_4,_5 );
};

template<typename ret, typename _1, typename _2, typename _3, typename _4, typename _5, typename _6  > class dlf6{
    public:
        typedef ret (*T)( _1,_2,_3,_4,_5, _6 );
};




class LinuxDL {
public:
    /// Internal symobols dictionary that maps the symbol name with a pointer to its instance
    typedef std::map< const std::string, void* > dllsymbols_T;
    
    /*!
        \fn LinuxDL()
        \brief The default constructor
    */
    LinuxDL(){mDllHandle = 0;b_AbortOnErr = true; }
    /*!
        \fn LinuxDL( const std::string& dllName, bool _abrt=true )
        \brief Initialize with the supplied lib path and a boolean flag
        \param dllName the shared library filename
        \param _abrt  Tell LinuxDL to abort or not the program if a symbol isn't found into the shared lib.
    */
    LinuxDL( const std::string& dllName, bool _abrt=true ):
        mLibraryPath(dllName){
        mDllHandle = 0l;
        b_AbortOnErr = _abrt;
    }
    ~LinuxDL() {
        if( !mSymbolsDict.empty() ) mSymbolsDict.clear();
        if( mDllHandle ) dlclose( mDllHandle );
    }
    /*!
        \fn Sym( const std::string& symName )
        \brief Find and return the instance of the given symbol.
        \param symName The symbol name ( function or variable )
        \return (void*) ptr to the instance of the symbole.
    */
    void* Sym( const std::string& symName ){
        dllsymbols_T::iterator it = mSymbolsDict.find( symName );
        if ( it == mSymbolsDict.end() ) return 0;
        _lastQPtr = it->second;
        return  it->second; // mSymbolsDict[symName];

    }

    /*!
        \fn operator[]( const std::string& symName )
        \brief Convenient operator to provide simple syntax such as Dll["symbol"];
        \param symName The symbol name ( function or variable )
        \return (void*) ptr to the instance of the symbole.
    */
    void* operator[]( const std::string& symName ){ return Sym( symName ); }
    
    /*!
        \fn operator << (const std::string& symName )
        \brief Insertion operator that resolves the given symbol.
        \param symName  The symbol name
        \return self
    */
    LinuxDL& operator << (const std::string& symName ){
        void* F;
        try{
            std::string S;
            F = dlsym(mDllHandle, symName.c_str());
            if(!F){
                S = dlerror();
                throw S;
            
            }
        }
        catch( const std::string& S ){
            std::cerr << S << std::endl;
            abort();
        }
        mSymbolsDict[symName]= F;
        _lastQPtr = F;
        std::cerr << "Symbol '" << symName << "' loaded @" << mSymbolsDict[symName] << std::endl;
        return *this;
    }

    /*!
        \fn operator >> ( T& fptr )
        \brief convenient operator to get syntax such as
        \code
            DL << "symname" >> func_ptr_type_instance;
        \endcode
        :-)!
    */
    template<typename T> LinuxDL& operator >> ( T& fptr){
        fptr = (T)_lastQPtr;
        return *this;
    }

    /*!
        \fn operator >> ( std::string& _str )
        \brief funny weird convenient operator to get syntax such as
        \code
            DL >> symname >> func_ptr_type_instance;
        \endcode
        :-)!
    */
    
    LinuxDL& operator >> (  std::string& _str ){
        if ( Sym( _str ) ) _lastQPtr = Sym(_str);
        else abort();
        return *this;
    }
    bool OpenLib();
/*!
    \fn LinuxDL::LoadAllSym()
    \brief Tries to load all the exported lib symbols through a pipe to the bin-util "readelf -s" in the good old "C" style...
    \note <h1>Attention: Experimental</h1>
 */
    int LoadAllSym();

    /*!
        \fn CloseLib()
        \brief  Close the library...
     */
    void CloseLib(){
        if(mDllHandle) dlclose( mDllHandle );
        mDllHandle = 0l;
    }

    std::string StrDump();

private:
    dllsymbols_T mSymbolsDict;
    void* mDllHandle;
    void* _lastQPtr;
    void Abort();
    std::string mLibraryPath;
    bool b_AbortOnErr;
};

}  /// @todo implement me
#endif
