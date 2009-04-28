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
#ifndef WCURSESNETSPEEDSENSORTEST_H
#define WCURSESNETSPEEDSENSORTEST_H

#include <Widget>
#include <map>
#include <Thread>
#include <Label>

namespace wcurses
{

    /**
        @author Serge Lussier,,, <tuxadmin.dev@gmail.com>
    */
    class NetSpeedSensorTest : public Widget
    {
        public:

            struct NetIFaceInfos
            {
                String strIFace;
                unsigned int _rx;// updated in each iteration
                unsigned long long _totalIN; // Persistant Accumulated value -- manual reset
                unsigned long long _totalOUT;// Persistant Accumulated value ..;
                unsigned int _tx;// updated in each iteration
                Label* L;
                NetIFaceInfos()
                {

                }
                int InitView() {return 0;}
                void Update()
                {
                    String S = String ( "<fgcolor blue>%1:<fgcolor red;>%2" )
                               .arg ( strIFace.data() ).arg ( _rx );

                    L->SetText ( S );
                    L->Update();
                }
            }; // End Class


            NetSpeedSensorTest ( Widget* Parent );

            NetSpeedSensorTest ( Object* Parent, unsigned int Flags, const char* NameID );

            ~NetSpeedSensorTest();
            bool InitView();
            bool Run();
            virtual int SetupLayout();
    void Start();
        private:
            Thread* _TimerThread;
            int _msDelay;
            char _line[256];
            std::map<std::string, NetIFaceInfos*> _IFaces;

            int ReadNetProc();
            void Update();
            NetIFaceInfos* IFaceLine (  std::list<std::string>& L );
    };

}

#endif
