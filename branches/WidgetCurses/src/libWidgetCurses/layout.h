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
#ifndef LAYOUT_H
#define LAYOUT_H


#include <Object>
#include "rect.h"
#include "ccutils.h"

//#include "widget.h"

#include <vector>

/*!
    \defgroup LayoutMan  Geometry Layouts Management
    \brief Group of objects -------
    \todo
        <U>Size policies:</U>
        <UL>
            <LI> Resize/SetGeometry -- erase widget's screen_buffer-re-create it to the new size ? </LI>
            <LI> UIControl::Min/Max sizes vs Layout::Min/Max sizes </LI>
        </UL>

*/

namespace wcurses
{
// Need forward control class declaration here because Widget class needs to include this header file
    class Widget;
    /*!
        \class Layout
        \brief Layout item that holds and manage individual uicontrol geometry
            Such mechanism sets child item relative position origin to (0,0) inside the parent's uicontrol object -- <i>not to the layout object</i>.
        \ingroup LayoutMan
        \author Serge Lussier <serge@tuxadmin.net>
    */


////////////////////////////////////////////////////////////////
// This section is for testing and developping  templatized layout classes
// -- Which seems to work !!! :-)

//class LayoutItem;

    /*!
        \class LayoutBase
        \brief Base layout hirarchy
        \author Serge Lussier
        \ingroup UICONTROLS
    */

    class LayoutBase : public Object
    {
        public:
            explicit LayoutBase ( Object* parent ) : Object ( parent ),_uicontrol ( 0 ),_childLayout ( 0 )
            {
                SetNameID ( "LayoutBase" );
            }
            explicit LayoutBase ( LayoutBase* parent ) : Object ( parent ),_uicontrol ( 0 ),_childLayout ( 0 )
            {
                SetNameID ( "LayoutBase" );
                if ( parent ) parent->SetChildLayout ( this );
            }
            void SetChildLayout ( LayoutBase* L ) { _childLayout = L ; }

            /*!
                \fn initialize()
                @return bool true if all initialisation components went true
             */
            virtual bool initialize() {return true;}
            virtual bool SetGeometry ( const Rect& R );
            virtual ~LayoutBase() {}
            virtual Widget* ClientControl() { return _uicontrol;}
            virtual bool SetWidget ( Widget* W );
            /*!
                \fn LayoutBase::Geometry()
             */
            Rect Geometry() { return _geometry; }
        protected:
            Rect _geometry;
            Widget* _uicontrol;
            LayoutBase* _childLayout;

            float _layoutsize;
    };

    /*!
        \class Layout<int>
        \brief Base template layout class - not used - see specialized layout subclasses below
        \author Serge Lussier <serge@tuxadmin.net>
    */
    template <int> class Layout : public LayoutBase
    {
        public:
            Layout ( Object* parent ) : LayoutBase ( parent ) {}
            virtual ~Layout() {};
            virtual bool initialize() { return -1; }

            /*!
                \fn Layout::SetGeometry( const Rect& R )
             */
            bool SetGeometry ( const Rect& R )
            {
                /// @todo implement me
            }
    };

    /*!
        \class Layout<0>
        \brief Directional Layout base implementation class
        \author Serge Lussier <serge@tuxadmin.net>
    */
    template<> class Layout<0> : public LayoutBase
    {
        public:
            Layout ( Object* parent, int yd ) : LayoutBase ( ( Object* ) 0 ), _nitems ( yd )
            {

                SetNameID ( "Layout<0>" );
            }

            virtual ~Layout();
            /*!
                \fn Layout< 0 >::Count()
             */
            int Count() { return _nitems; }
            bool SetWidget ( int position, Widget* W );
            virtual bool initialize() {return true;}
            virtual Widget* ClientControl ( int position );
            LayoutBase* operator[] ( int );
            virtual bool SetGeometry ( const Rect& R );
            virtual int Add ( int );
        protected:
            int _nitems;
            int _widgets;
            std::vector<LayoutBase*> _sublayouts;
    };

    /*!
        \class Layout<directions::horizontal>
        \brief Horizontal Layout implementation class
        \author Serge Lussier <serge@tuxadmin.net>
    */
    template<> class Layout<directions::horizontal> : public Layout<0>
    {
        public:
            Layout ( Object* parent, int nitems ) : Layout<0> ( parent, nitems )
            {
                SetNameID ( "Layout<horizontal>" );
            }
            //virtual ~Layout(){}
            virtual bool SetGeometry ( const Rect& R );
            virtual bool initialize();
            bool SetWidget ( int position, Widget* W );

    };
    /*!
        \class Layout<directions::vertical>
        \brief Vertical Layout implementation class
        \author Serge Lussier <serge@tuxadmin.net>
    */
    template<> class Layout<directions::vertical> : public Layout<0>
    {
        public:
            Layout ( Object* parent, int yd=0 ) : Layout<0> ( parent,yd )
            {
                SetNameID ( "Layout<vertical>" );
            }
            virtual bool SetGeometry ( const Rect& R );

            virtual bool initialize();
            virtual ~Layout() {}
    };

    /*!
        \class Layout<directions::table>
        \brief Grid Layout implementation class
        \author Serge Lussier <serge@tuxadmin.net> */
    template<> class Layout<directions::table> : public Layout<0>
    {
        public:
            Layout ( Object* parent, int cols, int rows ) : Layout<0> ( parent,rows ), _cols ( cols ), _rows ( rows ), _row ( 0 )
            {
                SetNameID ( "Layout<table>" );
            }
            virtual bool SetGeometry ( const Rect& R );
            virtual ~Layout() {}
            virtual bool initialize();
            Layout< directions :: horizontal >* Row ( int row );
            virtual Widget* ClientControl ( int x, int y );
            bool SetWidget ( int, int, Widget* );
        protected:

            int _rows;
            int _cols;
            pxy _xy; /// Current position into the grid.
        private:
            Layout< directions::horizontal >* _row;
            LayoutBase* L; /// Current Layout object into the grid.
    };
/////////////////////////////////////////////////////////////////////


    template <class T> T* ToLayoutType ( LayoutBase* B )
    {
        return dynamic_cast< T* > ( B );
    }

}

#endif
