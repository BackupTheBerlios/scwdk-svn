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
#include <exstring.h>
#include <vector>




String::String()
{
}


String::~String()
{
    if( ! _data.empty()) _data.clear();
}

char  String::operator[](int I)
{
      if(I >= _data.length()) return 0;
      return _data[I];
}

/**
    \fn String::split(std::list<std::string>& l, const std::string& seq, char s1, char s2, bool keeptok )
    \brief Splits a string into tokens separeted by supplied delimiters.
    \author Serge Lussier ( lussier.serge@gmail.com )
*/
int  String::split(std::list<std::string>& l, const std::string& seq, char s1, char s2, bool keeptok)
{

   typedef std::string::size_type ST;
   int KT = keeptok ? 0: 1;
   std::vector<int> tok_s1;
   std::vector<int> tok_s2;
   if(l.size()) l.clear();
   ST pos=0, start=0, LEN=seq.size();
   while( pos < LEN ){
      if(seq[pos] == s1){
         start = pos;
         if(s2){
            while( (pos <LEN) && (seq[pos] != s2)) ++pos;
            if(pos <LEN){
               tok_s2.push_back(pos);
               tok_s1.push_back(start);
               start = pos+1;
            }
         }
         else tok_s1.push_back(start);
      }
      ++pos;
   }
   if(s2){
      if( ( tok_s1.size() != tok_s2.size() ) || (tok_s1.size() == 0) ){
         //screwed: return the original string
         l.push_back(seq);
         return 1;
      }
      if(tok_s1.size()){
         if(tok_s1[0]) l.push_back(seq.substr(0, tok_s1[0] - KT) );
         for(pos = 0; pos < tok_s1.size(); pos++){
            if(pos>0){
               int c = tok_s1[pos] - tok_s2[pos-1];
               if(c > 1) l.push_back(seq.substr( tok_s2[pos-1]+1, c-1));
            }
            l.push_back(seq.substr( tok_s1[pos], tok_s2[pos]-tok_s1[pos]+1 ));
         }
      }
      if( tok_s2.back() < (LEN-1)) l.push_back(seq.substr(tok_s2.back()+1, (LEN)-(tok_s2.back()+1)));
   }
   return l.size();
}
/**
    \fn String::split(std::list<std::string>& l, const std::string& seq, const std::string& _1cdelim, bool keeptoken=false, bool _removews=true )
    \brief Splits a string into tokens separeted by supplied delimiters.
    \author Serge Lussier ( lussier.serge@gmail.com )
*/
int  String::split(std::list<std::string>& L, const std::string& seq, const std::string& _1cdelim, bool keeptoken, bool _removews )
{
    typedef std::string::size_type ST;
    std::string delims = _1cdelim;
    std::string STR;
    if(delims.empty()) delims = "\n\r";
    if(_removews) delims += " ";

    ST pos=0, LEN = seq.size();
    while(pos < LEN ){
        STR=""; // Init/clear the STR token buffer
        // remove any delimiters including optional (white)spaces
        while( (delims.find(seq[pos]) != std::string::npos) && (pos < LEN) ) ++pos;
        // leave if @eos
        if(pos==LEN) return L.size();
        // Save token data
        while( (delims.find(seq[pos]) == std::string::npos) && (pos < LEN) ) STR += seq[pos++];
        // put valid STR buffer into the supplied list
        //std::cout << "[" << STR << "]";
        if( ! STR.empty() ) L.push_back(STR);
    }
    return L.size();
}


bool  String::_argf()
{
   bool done=false;
   if(!__argstr.empty()) __argstr = "";
   char s[32];
   int b,e;
   char c;
   __argpos = _data.find('%');
   if(__argpos == std::string::npos) return false;
   b = __argpos;
               // construire une boucle meme si pour le moment je code avec un seul char
   while(1){ // loop even if I right now only parse a one-char code.
      ++b;
      c = _data[b];
      if(isdigit(c)){
         __argstr += c;
         break;
      }
      switch(c){
      // Theese case's switch are only to validate that this is really an argument.
         case 'x':// Hex integer format in lowercase.
         case 'X':// Hex integer format in upper case
         case 'b'://Binary format integer ( no padding with leading 0's
         case 'B'://Binary format integer ( with leading zero's )
         case 'o':// Octal integer format
         case 'O'://
         case 'd':// normal signed integer decimal format
         case 'u':// normal unsigned integer decimal format
         case 's':
            __argstr += c;
            break;
         default:
            __argstr= "";
            break;
      } // switch trap: no more code needed for now...
      break;
   }
   
   if(__argstr.empty()) return false;
   _data.erase(__argpos, __argstr.length()+1);
   return true;
}

template<typename _argT>  String&  String::operator+=(_argT _arg)
{
      std::string str = _arg.tostring();
      _data.append( str );
      return *this;
}




