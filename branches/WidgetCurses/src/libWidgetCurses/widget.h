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
#ifndef WCURSESWIDGET_H
#define WCURSESWIDGET_H

#include <object.h>
#include <list>
#include <cursesattr.h>
#include <layout.h>


namespace wcurses
{
    class Painter;
    /**
    Base class of managed UI components.

        @author Serge Lussier,,, <tuxadmin.dev@gmail.com>
    */
    class Widget : public Object
    {
        public:
            typedef std::list<Widget*> list;
            typedef std::list<Widget*>::iterator  Iterator;
            typedef std::list<Widget*>::const_iterator  ConstIterator;

            Widget ( Object* Parent, unsigned int Flags, const char* NameID );

            Widget ( Object* Parent );

            Widget ( Widget* Parent );
            inline int Width() { return _geometry.Width(); }
            inline int Height() { return _geometry.Height(); }

            virtual ~Widget();
            virtual bool SetGeometry ( const Rect& r );
            Rect Geometry() { return _geometry; }
            virtual bool Resize ( const Size& sz, bool move_origin=false );
            PCell DC() { return _buffer; }
            Style CurrentStyle() { return _style; }
            virtual int State ( int f )
            {
                return ( _state & f ) != 0;
            }
            pxy TopLeft() { return _topLeft; }
            Painter* DCPainter() { return _painter ; }
            virtual bool InitView();
            virtual bool Update ( const Rect& _interior=Rect::nil );

            /*!
                \fn wcurses::Widget::Active(bool e =-1)
             */
            virtual bool Active ( int e = -1 )
            {
                if ( !e ) SetState ( states::active, false );
                if ( e>0 ) SetState ( states::active, true );
                return ( _state & states::active ) !=0;
            }

            /*!
                \fn wcurses::Widget::Enable( bool e =-1 )
             */
            virtual bool Enable ( int e =-1 )
            {
                if ( !e ) SetState ( states::disabled, true );
                if ( e>0 ) SetState ( states::disabled, false );
                return ( _state & states::disabled ) !=0;
            }

            /*!
                \fn wcurses::Widget::Visible( bool e=-1)
             */
            virtual bool Visible ( bool e=-1 )
            {
                if ( !e ) SetState ( states::visible, false );
                if ( e>0 ) SetState ( states::visible, true );
                return ( _state & states::visible ) !=0;
            }

            /*!
                \fn wcurses::Widget::Setstate( const int e , bool y=true)
             */
            virtual void SetState ( const int e , bool y=true )
            {
                int E,S;
                S = _state;
                if ( y ){
                    // retenir le bit des etat visuels
                    E = e & _visualStatesBits();
                    if(E){
                        // eteindre les autres bits visuel ( ils sont exculsif )
                        S = _state & (~_visualStatesBits());
                        _state = E|S; // assembler le nouveau bit visuel avec les autres bits d'etat
                        // changer ou re-affecter le style selon le bit visuel actuel
                        _style = (*_StyleComponents)[_visualStatesBits()];
                    }else _state |= e; // bit visuel absent de la requete -- alors just ajouter | merger le bit d'etat
                }
                else _state &= ~e;

            }
            virtual void Show ( int state=states::normal );
            virtual bool ChangeTheme ( const std::string& strTheme ) ;

            /*!
                \fn wcurses::Widget::Interior()
             */
            Rect Interior()
            {
                Rect r = _geometry;
                r -= r.topleft();
                return r;
            }

            /*!
                \fn wcurses::Widget::Position const xyCoords& xy )
             */
            void Position ( const xyCoords& xy )
            {
                _geometry.moveto ( xy );
            }
            LayoutBase* InLayout() { return _layout; }
            void Move ( const xyCoords& xy );
            void MoveTo ( const xyCoords& xy );

            /*!
                \fn wcurses::Widget::WidgetClass( int c )
             */
            int WidgetClass ( int c )
            {
                return ( _class & c ) !=0;
            }
            virtual void ChangeState();

            /*!
                \fn wcurses::Widget::ParentLayout()
             */
            LayoutBase* ParentLayout()
            {
                Widget *p = FirstParentAs<Widget>();
                if ( p ) return p->InLayout();
                return 0l;
            }
            virtual void Blur();
            virtual void Activate();
            virtual void Repaint();

            /*!
                \fn wcurses::Widget::SetActiveChild( Widget * W)
                \brief Set the child widget as the active widget used to set the chain between parent<->links
             */
            bool SetActiveChild ( Widget * W )
            {
                _activeChild = W;
                if(Visible()) return false;
                if(!State(states::visible)) Activate();

                return true;
            }
            virtual void Paint();

        protected:
            /// Screen IO engine
            Painter* _painter;
            /// Root Layout of the widget to manage children into the specialized sublayouts
            LayoutBase*   _layout;
            /// ParentLayout
            LayoutBase*   _parentLayout;
            virtual bool UpdateChild ( Widget* cw, const Rect& _interior=Rect::nil );
            virtual int SetupLayout();
            virtual Widget* QueryMouseTarget ( MouseEvent* M );
            Widget* _activeChild;
        private:
            friend class Application;
            /// Widget UI class @see uiclass namespace
            int _class;
            /// current Widget's UI state { normal|active|visible|move|minimized }
            int _state;
            /// Local geometry of the Widget from it's parent origin
            Rect     _geometry;
            /// Absolute origin
            pxy      _topLeft;
            /// Current style of the Widget
            Style    _style;
            /// Current style components ( current theme )
            StyleComponents*   _StyleComponents;
            /// Private internal screen bitmap buffer
            PCell    _buffer;
            /// list of dirty regions to be updated -- not used yet -- might not at all after all...
            std::list<Rect*> _dirtyRegions;
            virtual int _Create();

    /*!
        \fn wcurses::Widget::_visualStatesBits()
     */
    int _visualStatesBits()
    {
        return _state &  states::visualex;
    }
    };

}

#endif
