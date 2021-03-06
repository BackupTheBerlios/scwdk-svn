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
#ifndef WCURSESLABEL_H
#define WCURSESLABEL_H

#include <widget.h>
#include <exstring.h>

namespace wcurses
{

    /**
        @author Serge Lussier,,, <tuxadmin.dev@gmail.com>
    */
    class Label : public Widget
    {
            String _text;
            int _justify;
            PStr _rtext;
            
        public:
            Label ( Widget* Parent );
            Label ( Widget* Parent, const char* name );

            ~Label();
            const std::string Text();
            void Justify ( int );
            void SetText ( const std::string& str );
            virtual bool InitView();
    int SetText(String str );


        private:
            void _Render();
        protected:
            unsigned char _short;
        protected:
            bool _Scan_();
    };

}

#endif
