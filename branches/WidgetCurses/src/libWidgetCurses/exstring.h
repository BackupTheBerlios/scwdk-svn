/***************************************************************************
 *   Copyright (C) 2005 by Serge Lussier,,,   *
 *   tuxin@felix   *
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
#ifndef UXXUXXSTRING_H
#define UXXUXXSTRING_H
#include <iostream>
#include <string>
#include <sstream>
#include <list>
#include <algorithm>
#include <cctype>


/**
	@author Serge Lussier,,, <tuxin@felix>
*/
class String{
public:
//    exstring();

//    ~exstring();
    struct ToLower
    {
        char operator() (char c) const  { return std::tolower(c); }
    };

    struct ToUpper
    {
        char operator() (char c) const  { return std::toupper(c); }
    };
    int __argpos;
    typedef std::list< std::string> list;
    typedef list::iterator iterator;
    typedef std::list<std::string> stdlist;
    typedef std::string::const_iterator std_const_iterator;

    String();
    explicit  String(const std::string& _s){
    _data = _s;
    }
    explicit  String(const char* _s){
        _data = _s;
    }
    int len() const { return _data.length(); }
    int size() const { return _data.size(); }
    const char* operator()() { return _data.c_str(); }
    char operator[](int );
    // public modifiable nstrf nodes list:
    //------------------------------------------
    ~String();
    const char* data() const{
    return _data.c_str();
    }
    const std::string& std() const {  return _data;  }

    template<typename argt> std::string tostring(argt __arg){
        std::ostringstream os;
        os << __arg;
        return os.str();
    }
    // completely disable ostream operator << -- i must do something...
    // behave much like arg() excepted that if there is no matching
    // <%>, then the argument is simply appended to the end ....
    template<typename argt>  String& operator << (argt __arg){
        if(_argf()) _data.insert(__argpos, tostring(__arg));
        else
        _data.append(tostring(__arg));
        return *this;
    }

    String& operator << (const  String& n){
        if(_argf()) _data.insert(__argpos, tostring(n.data()));
        else
        _data.append(tostring(n.data()));
        return *this;
    }
    template<typename argt>  String& binary(argt __arg, bool padd=false, int f=0){
        if(!_argf()) return *this;
        
        int y = sizeof(argt)*8;
        
        std::string r="";
        int x=0;
        argt bit = 0;
        x = 0;
        for(y=y-1; y>=0;y--){
            if( ( bit = __arg & (1<<y)) || padd){
                r +=  bit ? "1" : "0";        
                ++x;
                if(f&&(x>=f)){ x=0; r+= ' '; }
                padd = true;
            }
        }
        
        _data.insert(__argpos, r);
        return *this;
    }
    template<typename argt>  String& arg(argt __arg)
    {
        if(!_argf()) return *this;
        _data.insert(__argpos, tostring(__arg));
        return *this;
    }
    char  operator[](int) const;
    static int  split(std::list<std::string>& l, const std::string& seq, char s1, char s2, bool keeptoken=false);
    static int  split(std::list<std::string>& l, const std::string& seq, const std::string& _1cdelim="", bool keeptoken=false, bool removews=true);
    void clear() {
        if(!_data.empty() ) _data.clear();
    }
    template<typename _argt> String& operator+=(_argt _arg);

    /*!
        \fn String::Lowcase( const std::string& str )
     */
    static std::string Lowcase( const std::string& str )
    {
        std::string res;
        res.resize( str.size() );
        std::transform( str.begin(), str.end(), res.begin(), String::ToLower() );
        return res;
    }

    /*!
        \fn String::Upcase(const std::string& str )
     */
    static std::string Upcase(const std::string& str )
    {
        std::string res;
        res.resize( str.size() );
        std::transform( str.begin(), str.end(), res.begin(), String::ToUpper() );
        return res;
    }

private:
    int findarg();
    bool _argf();
    std::string __argstr;
    std::string _data;

};



#endif
