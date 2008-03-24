/***************************************************************************
 *   Copyright (C) 2007 by Serge Lussier,,,   *
 *   bretzel@pc-desktop   *
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


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <cstdlib>
#include "swncurses.h"
#include "swtext.h"
#include "swmain.h"

#include "swwindow.h"



class Main : public swMain{
public:
    Main( int argc, char** argv );
    virtual int Init();
    virtual ~Main(){}

private:
    swWindow* tc;

};


Main::Main( int argc, char** argv ) : swMain( "Test de scwdk-0.1a", argc,argv){

}

/*!
    \fn int Main::Init()
    \brief example of virtual Init method.
    \image html myMain-shot1.jpg
*/
int Main::Init(){

    int r = swMain::Init();
    if(r) return r;
    swNCurses* nc = CursesInstance();
    if(!nc) return 128; // not supposed to be;
    swText T(0l, swTAttr(4,3, 0),"swText long format tests");


    tc = new swWindow(this, 0, "Test of swWindow");
    tc->SetGeometry( Rect(1,8, 100,8 ) );
    tc->InitView();
    swWriter* wr = tc->StartWrite( );
    //wr->Clear();
    swTAttr A = wr->Control()->DefAttr();

    (*wr) << pxy( 1,1 ) << "swWriter::plain: "<< swWriter::plain <<  "<fgcolor red; bgcolor white; /strong; underline;>   uicontrol!!!   </underline;>";
    (*wr) << pxy( 1,2 ) << "swWriter::text : "  <<swWriter::text  << "<fgcolor red; bgcolor white; /strong; underline;>   uicontrol!!!   </underline;>";
    (*wr) << " UiControl's def text attributes: " << (const char*)A;
    //wr->DrawFrame( wr->Geometry(), tc->DefAttr() );
    // test:
    //wr->JoinAcs( pxy (wr->Geometry().width()/2, 0), direction::down, tc->DefAttr() );
    //wr->JoinAcs( pxy (wr->Geometry().width()/2, tc->Height()-1), direction::up, tc->DefAttr() );
    //wr->JoinAcs( pxy (wr->Geometry().x(), tc->Height()-3) , direction::right, tc->DefAttr() );
    //wr->JoinAcs( pxy (wr->Geometry().width()-1, tc->Height()-3) , direction::left, tc->DefAttr() );
    //wr->AscLine( pxy(1,tc->Height()-3), tc->Width()-2, direction::right, tc->DefAttr() );

    (*wr) << pxy( 1, tc->Height()-2) << swWriter::text << "<fgcolor cyan;strong;underline;>texte dans une zone encadree.";
    //tc->EndWrite();


    swUiControl* ct = new swUiControl(tc, 0, "child of tc");
    ct->SetGeometry(Rect( 5,3 , 40,3 ) );
    wr = ct->StartWrite();
    wr->Clear();
    (*wr) << pxy(1,1) << swWriter::text << "<bgcolor black;fgcolor yellow; strong;>[ child control embbeded in parent   ]";
    wr->DrawFrame( wr->Geometry(), swTAttr( 2,swcolor::cyan, A_BOLD) );
    //ct->EndWrite();
    tc->EndWrite();
    return 0;
}
using namespace std;

int main(int argc, char *argv[])
{
    Main* myMain = new Main(  argc, argv );
    myMain->Run();
    delete myMain;
    gDebug << " finished exiting with 0";DEND;
    return EXIT_SUCCESS;
}
//     T << "<fgcolor black; bgcolor cyan;> This is scwdk"
//       << "<fgcolor red;> ( Simple Console Widget DevKit )"
//         <<"<fgcolor black;> Version <fgcolor yellow;>0.1<fgcolor black;>a<fgcolor yellow; strong;> WIP "
//         << swText::END;
//
//     PStr pstr = T.Data();
//     Debug << " longueur de pstr: " << T.length( pstr ); DEND;
//     mvaddchstr( 1,1, pstr);
//     delete [] pstr;
//     T.ReleaseData();
//
//     T << "< bgcolor blue; fgcolor white; strong; > Allo le monde!"
//         "<underline; fgcolor yellow;> ///todo </underline; fgcolor white;>\""
//         "< cr; br; nl;>\"  <fgcolor cyan;>This is called Text processing </StRoNg;> low cyan on blue...?"
//         "<region 2 2 60 20; GRID 10 5; FRAME; FRAMETITLE;> Plug proper delegates to swText to enable commands </FRAMETITLE;>" << swText::END;
//     pstr = T.Data();
//     mvaddchstr( 5,0, pstr );
//     T.ReleaseData();
//     delete [] pstr;


