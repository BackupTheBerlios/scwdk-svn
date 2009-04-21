/***************************************************************************
 *   Copyright (C) 2009 by Serge Lussier,,,   *
 *   tuxadmin.dev@gmail.com   *
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
#ifndef WCURSESCURSESATTR_H
#define WCURSESCURSESATTR_H
#include "ccutils.h"
#include "exstring.h"

// Only for the typedef -- map of Style's are used byt the wcurses::StyleComponent class

#include <map>
#include "cursesterm.h"

namespace wcurses
{

    class CursesAttr{
    };

    /**
        @author Serge Lussier,,, <tuxadmin.dev@gmail.com>
    */
    class Style
    {
        public:
            typedef char* pChar;
            enum A { normal=0, bold=A_BOLD, underline=A_UNDERLINE, reverse=A_REVERSE, blink=A_BLINK, colors=A_COLOR, newline=A_NEWLINE };

            static std::map< std::string, int> ColorNames;

            // Bad idea to def a list of Style as instances -- too much overhead copy/constructor/create for each inserted element@!
            // Let be the list to store pointers instead...
            typedef std::list<Style*> list;

            struct Color
            {
                Color ( int _bg, int _fg ) :
                        bg ( _bg ), fg ( _fg )
                {
                }
                int bg,fg;
            };
            Style();
            Style ( const Style& );


            Style ( int bg, int fg, TCell attr );

            ~Style();

            operator const char* ();
            ///////////////////////
            // Information methods
            /*!
                \fn Style::BgColor()
             */
            int BgColor()
            {

                int P = PAIR_NUMBER ( _d );
                short f,b;
                pair_content ( P, &f, &b );
                return b;
            }

            /*!
                \fn Style::FgColor()
             */
            int FgColor()
            {

                int P = PAIR_NUMBER ( _d );
                short f,b;
                pair_content ( P, &f, &b );
                return f;
            }

            /*!
                \fn Style::Attributes()
             */
            TCell Attributes() { return ( _d & ~A_COLOR ) | A_ATTRIBUTES | A_NEWLINE | A_ALTCHARSET; }

            /*!
                \fn Style::Colors()
             */
            TCell Colors() { return _d & A_COLOR; }
            //////////////////////////////////////////////
            // Modifier methods

            /*!
                \fn Style::SetColors( int, int )
             */
            void SetColors ( int bg, int  fg ) { _d = ( _d & ~A_COLOR ) | Style::ptbl[bg][fg]; }
            void AddAttr ( TCell a );

            /*!
                \fn Style::SetData( TCell a )
             */
            void SetData ( TCell a ) { _d = a; }
            void SetBgColor ( int bg );
            void SetFgColor ( int fg );
            void SetNewAttr ( TCell a );
            void SetAttr ( TCell d, bool _set )
            {
                if ( _set ) _d |= d;
                else    _d &= ~d;
            }

            ////////////////////////
            // Operators
            Style& operator << ( Style::Color C );
            Style& operator << ( TCell d ) { _d = d; return *this; }
            Style& operator >> ( Style::Color& C )
            {
                C.fg = FgColor();
                C.bg = BgColor();
                return *this;
            }

            /*!
                \fn Style::Data()
             */
            TCell Data() const { return _d; }
            TCell operator() () { return _d; }

            /*!
                \fn Style::SetReverse( bool t=true )
             */
            void SetReverse ( bool t=true ) { SetAttr ( Style::reverse, t ); }

            /*!
                \fn Style::SetUnderline( bool t=true )
             */
            void SetUnderline ( bool t=true ) { SetAttr ( Style::underline, t ); }

            /*!
                \fn Style::SetNewLine( bool t=true )
             */
            void SetNewLine ( bool t=true ) { SetAttr ( Style::newline, t ); }

            /*!
                \fn Style::SetBold( bool t=true )
             */
            void SetBold ( bool t=true ) { SetAttr ( Style::bold, t ); }
            bool IsSet ( TCell );


            /*!
                \fn Style::IsBlink()
             */
            bool IsBlink() { return ( _d&Style::blink ) !=0; }

            /*!
                \fn Style::IsBold()
             */
            bool IsBold() { return ( _d&Style::bold ) !=0; }

            /*!
                \fn Style::IsNewLine()
             */
            bool IsNewLine() { return ( _d&Style::newline ) !=0; }

            /*!
                \fn Style::IsReverse()
             */
            bool IsReverse() { return ( _d&Style::reverse ) !=0; }

            /*!
                \fn Style::IsUnderline()
             */
            bool IsUnderline() {  return ( _d&Style::underline ) !=0; }

            /*!
                \fn Style::EncodeColor( int b, int f )
             */
            static inline TCell EncodeColor ( int b, int f ) { return Style::ptbl[b][f]; }



        protected:
            friend class CursesTerm;
            friend class swDesktop;
            friend class swText;
            TCell _d;
        public: static TCell ptbl[8][8];
        public: static Style nul;
        protected: static int init();
        private:

    };
    class StyleComponents {
        public:
            typedef std::map<std::string, StyleComponents*> ComponentsMap;
            StyleComponents( )
            {
            }

            ~StyleComponents(){
                if( _styles.size() ) _styles.clear();
            }
            Style operator[](int n){
                if( _styles.find(n) == _styles.end() ) return Style::nul;
                return _styles[n];
            }

            Style Add( const Style& A ){
                int n = _styles.size() +1;
                _styles[n] = A;
                return  _styles[n];
            }

            StyleComponents& operator << (int K) {
                gDebug << " Key=" << K; DEND;
                _inKey = K;
                return *this;
            }

            StyleComponents& operator << (const Style& A) {
                _styles[_inKey] = A;
                return *this;
            }
            int Size(){ return _styles.size(); }
        private:
            std::map<int, Style> _styles;
            int _inKey;
    };

}

#endif
