bool Screen::Update()
    {
        _unode::Queu::iterator qit = Queu.begin();
        Widget* w;
        _unode node;
        Painter* dc;
        PCell io;
        Rect uarea;
        Rect warea;
        Rect obs;
        Widget* _p;
        Widget::Iterator wid;
        //Debug << " Size of queu: " << Queu.size() ;
        if ( !Queu.size() ) return false;
        for ( qit=Queu.begin(); qit != Queu.end(); qit++ )
        {
            node = *qit;
            w = node._w;

            Dbg << w->NameID();
            dc = w->DCPainter();
            //Dbg << "Area to Paint: " << node._area.tostring();
            io = dc->Position ( node._area.topleft() );
            Dbg << "Painter @(" <<io << ")";
            if ( !io ) continue;
            // region du toplevel a updater a l'offset de l'ecran:
            warea = node._area;
            warea += w->TopLeft();
            // garder l'intersection entre la region du update et l'ecran
            warea = Rect ( 0,0, _term->Width(), _term->Height() ) & warea;
            //Dbg "Intersection: " << warea.tostring();
            if ( !warea ) continue;

            // construire la liste des toplevels "au-dessus" du toplevel a rafraichir dans le but de bypasser la region obstruee par un toplevel du dessus...
            Widget::list abovelist;
            int nt = _GetToplevelsAbove ( abovelist, w );
            //Dbg << " Got above widget list: " << nt;
            // ajuster la region a l'offset interne du toplevel
            warea -= w->TopLeft();
            // garder l'intersection la region interne et la region a confinee a l'ecran
            uarea = node._area & warea;
            if ( ! uarea )
            {
                Debug; Debug; Debug; Debug;
                Dbg << " Serious error -- invalid interior -- ABORTING";
                abort();
            }
            // re-ajuster la copie de l'intersection a l'offset de l'ecran
            warea += w->TopLeft();
            // Effectuer le dump de la region a rafraichir:
            // Debug;Debug << "Start refresh at " << warea.topleft().tostring() << " for widget named: " << w->NameID() << " at" << uarea.tostring();
            for ( int y = 0; y < warea.Height(); y++ )
            {
                io = dc->Position ( pxy ( uarea.x(),y+ uarea.y() ) );
                for ( int x = 0; x < warea.Width(); x++ )
                {
                    // Verifier le peerview de la coord de warea + x,y dans la liste des abovelist;
                    // Si le toplevel se trouve etre celui au "top" alors pas de verification...
                    if ( _peer ( abovelist, x+warea.x(),y+warea.y() ) )
                    {
                        io++;
                        continue;
                    }
                    mvaddch ( y+warea.y(), x+warea.x(), *io++ );
                }// for(int x = 0; x < warea.Width(); x++){
            }// for(int y = 0; y < warea.Height(); y++){
        }// for(qit; qit != Queu.end(); qit++){
        ::refresh(); // Try force refresh becuse ncurses::getch() is blocking it;
        Queu.clear();
    }// Screen::Update()