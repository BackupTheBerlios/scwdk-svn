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
#include <Layouts>
#include <Widget>




////////////////////////////////////////////////////////////////
// This section is for testing and developping  templatized layout classes


namespace wcurses
{
    /*!
        \fn Layout< 0 >::SetWidget(int position, swObject* W )
     */
    bool Layout< 0 >::SetWidget ( int position, Widget* W )
    {
        if ( position >= _sublayouts.size() ) return false;
        LayoutBase* L = _sublayouts[position];
        if ( !L ) return false;
        L->SetWidget ( W );
        return true;
    }



    bool Layout< directions :: table >::initialize()
    {
        Layout<directions::horizontal>* H;
        if ( !Layout<0>::initialize() ) return false;
        // _sublayouts initiated.
        // Create The grid:
        for ( int i=0; i< _rows; i++ )
        {
            H = new Layout<directions::horizontal> ( this, _cols );
            H->initialize();
            _sublayouts.push_back ( H );
        }
        return true;
    }

    /*!
        \fn Layout< directions :: horizontal >::initialize()
     */
    bool Layout< directions :: horizontal >::initialize()
    {
        Layout<0>::initialize();
        LayoutBase* L ;

        for ( int h = 0; h <= _nitems; h++ )
        {
            L = new LayoutBase ( this );
            _sublayouts.push_back ( L );
            L->initialize();
        }
        return true;
    }


    /*!
        \fn Layout< directions :: vertical >::initialize()
     */
    bool Layout< directions :: vertical >::initialize()
    {
        Layout<0>::initialize();
        LayoutBase* L ;
        for ( int h = 0; h <= Count(); h++ )
        {
            L = new LayoutBase ( this );
            _sublayouts.push_back ( L );
            L->initialize();
        }
        return true;
    }





    /*!
        \fn Layout< directions :: table >::Row( int row )
     */
    /**
     *
     * @param row
     * @return
     */
    Layout< directions :: horizontal >* Layout< directions :: table >::Row ( int row )
    {
        ////// Debug << " Request for Row#" << row ;
        if ( row >= Count() )
        {
            Dbg << " Row " << row << " is out of bounds! - returning (Layout< directions :: horizontal >*)0l(NULL)!"; DEND;
            return 0l;
        }
        L = _sublayouts[row];
        if ( !L ) return ( Layout< directions :: horizontal >* ) 0l;
        _row = L->Class< Layout< directions::horizontal> >();
        Dbg << "Row instance=(Layout<" << directions::horizontal << ">*) address: " << _row; DEND;
        return _row;
    }



    /*!
        \fn LayoutBase::SetGeometry( const Rect& R )
     */
    bool LayoutBase::SetGeometry ( const Rect& R )
    {
        LayoutBase* cl; // Child Layout if any
        _geometry = R;
        if ( _uicontrol ) _uicontrol->SetGeometry ( R );
        else
        {
            if ( ! ( cl = _childLayout ) ) return true;
            cl->SetGeometry ( R );
        }
        return true;
    }


    /*!
        \fn Layout< 0 >::SetGeometry( const Rect& R )
        \brief virtual method transport to the child classes - this one does nothing.
     */
    bool Layout< 0 >::SetGeometry ( const Rect& R )
    {
        return false;
    }


    /*!
        \fn Layout< directions :: horizontal >::SetGeometry( const Rect& R )
     */
    bool Layout< directions :: horizontal >::SetGeometry ( const Rect& R )
    {
        _geometry = R;
        int dx,i;
        Rect r[_nitems];// unless using _sublayuts.push_back...
        int N = _nitems;
        ////// Debug << "testing vector::size with push_back: then that  size is: " << _sublayouts.size(); DEND;
        int lw = R.Width() / N;

        LayoutBase* L;
        for ( i = 0; i < _nitems; i++ )
        {
            r[i].assign ( R.x() + ( lw*i ), R.y(), lw, R.Height() );

        }/// @todo implement me
        i = _nitems-1;
        if ( r[i].xr() < R.xr() )
        {
            dx = R.xr()-r[i].xr();

            r[i].assign ( R.x() + ( lw*i ), R.y(), lw+dx, R.Height() );
        }
        for ( i = 0; i < N; i++ )
        {
            L = _sublayouts[i];
            L->SetGeometry ( r[i] );
        }

    }


    /*!
        \fn Layout< directions :: vertical >::SetGeometry( const Rect& R )
     */
    bool Layout< directions :: vertical >::SetGeometry ( const Rect& R )
    {
        _geometry = R;
        int N = _sublayouts.size();
        int lh = R.Height() / N;
        LayoutBase* L;
        for ( int i = 0; i < N; i++ )
        {
            L = _sublayouts[i];
            L->SetGeometry ( Rect ( R.x() , R.y() + ( lh*i ), R.Width(), lh-1 ) );
        }

    }


    /*!
        \fn Layout< directions :: table >::SetGeometry( const Rect& R )
     */
    bool Layout< directions :: table >::SetGeometry ( const Rect& R )
    {
        int N = _rows;
        int h = R.Height() / N;
        for ( int y = 0; y < N; y++ )
        {
            _row = Row ( y );
            if ( !_row ) return false;
            _row->SetGeometry ( Rect ( R.x(), R.y() + ( h*y ), R.Width(), h ) );
        }
    }




    /*!
        \fn Layout< 0 >::ClientControl( int position )
     */
    Widget* Layout< 0 >::ClientControl ( int position )
    {
        if ( position >= _nitems ) return 0l;
        return _sublayouts[position]->ClientControl();
    }



    Widget* Layout<directions::table>::ClientControl ( int x, int y )
    {
        Layout<directions::horizontal>* r = Row ( y ); if ( !r ) return 0l;
        LayoutBase* B = ( *r ) [x]; if ( !B ) return 0l;
        return B->ClientControl();
    }


    Layout<0>::~Layout()
    {
        ////// Debug << "Cleaup time!";
        for ( int i =0 ; i < _nitems ; i++ )
        {
            LayoutBase* L = _sublayouts[i];
            Dbg << " destroying layout #" << i ;
            if ( L ) delete L;
        }
//    _sublayouts.clear();
        Dbg << " layouts array (std::vector) cleared."; DEND;
    }


    /*!
        \fn Layout< directions :: horizontal >::SetWidget( int position,Widget* W )
     */
    bool Layout< directions :: horizontal >::SetWidget ( int position, Widget* W )
    {
        if ( position >= Count() ) return false;
        LayoutBase* L = _sublayouts[position];
        if ( !L ) return false;
        L->SetWidget ( W );
        return true;
    }


    /*!
        \fn Layout< 0 >::operator[](int)
     */
    LayoutBase* Layout< 0 >::operator[] ( int X )
    {
        if ( X >= Count() ) return 0l;
        return _sublayouts[X]; // May be NULL if not yet set or removed;
    }


    /*!
        \fn Layout< directions :: table >::SetWidget( int, int, Widget* )
     */
    bool Layout< directions :: table >::SetWidget ( int col , int row , Widget* W )
    {
        Layout<directions::horizontal>* H = Row ( row );
        if ( !H ) return false;
        LayoutBase* B = ( *H ) [col];
        if ( !B ) return false;
        B->SetWidget ( W );
        return true;
    }

    int Layout< 0 >::Add ( int x )
    {
        LayoutBase* L;
        if ( !x ) return _sublayouts.size();
        do
        {
            L =  new LayoutBase ( this );
            _sublayouts.push_back ( L );
            L->initialize();
            --x;
        }
        while ( x );
        _nitems = _sublayouts.size();
        return _nitems;
    }
    bool LayoutBase::SetWidget ( Widget* W )
    {
        _uicontrol  = W;
        W->SetLayoutContainer ( this );
        return true;
    }
}
