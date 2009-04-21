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
#ifndef WCURSESWINDOW_H
#define WCURSESWINDOW_H

#include <widget.h>

namespace wcurses
{

	/**
		@author Serge Lussier,,, <tuxadmin.dev@gmail.com>
	*/
	class Window : public Widget
	{
		public:
			Window ( Object* Parent, unsigned int Flags, const char* NameID );

			Window ( Widget* Parent );

			~Window();

			bool Active ( int e );
			bool ChangeTheme ( const std::string& strTheme );
			bool Enable ( int e );
			bool Resize ( const Size& sz, bool move_origin );
			bool SetGeometry ( const Rect& r );
			bool Visible ( bool e );
			virtual bool InitView();
			void SetState ( int e, bool y );
			void Show ( int state );
    void Hide();
    String IconText();

protected:
    int SetupLayout();
protected:
    StyleComponents* _frameStyle;
	};

}

#endif
