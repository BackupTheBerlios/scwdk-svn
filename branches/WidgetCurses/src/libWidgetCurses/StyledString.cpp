//
// C++ Implementation: nattrstrparser
//
// Description:
//
//
// Author: Serge Lussier,,, <lussier.serge@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "StyledString.h"


//Style StyledString::_defaultAttributes;

//const char* StyledString::opref="bF/_nB";
//std::map< char, StyledString::Operator* > StyledString::operators;
//std::map< std::string, StyledString::Operator*> StyledString::lfoperators;
//bool StyledString::init_done = false;







#define stripws(e) while( (*(e) == 0x20) && (*(e) !='>')) ++(e)

#define cnum(e) (((e) <='9') && ((e) >='0')) ? (e)-'0' : -1

namespace wcurses
{
    StyledString::StyledString ( Object* parent, const Style defAttr, const std::string idname ) : Object ( parent,idname.c_str() )
    {
        StyledString::_defaultAttributes = defAttr;
        result = 0l;
    }


    StyledString::StyledString() : Object()
    {
        StyledString::_defaultAttributes = Style(colors::black, colors::white, Style::normal);
        result = 0l;
    }



    StyledString::StyledString ( const std::string& _text )
            :
            Object ( 0l,"" )
    {
        result = 0l;
    }

    StyledString::StyledString ( const String& _text )
            :
            Object()
    {
        result = 0l;
        //instring << _text;
    }

    StyledString::StyledString ( Object* parent, const std::string& _text )
            :
            Object ( parent,"" )
    {

        //instring << _text;
        result = 0l;
    }

    StyledString::StyledString ( Object* parent, const String& _text )
            :
            Object ( parent,"" )
    {

        //instring << _text;
        result=0l;
    }



    StyledString::~StyledString()
    {
        StyledString::Operator* op;
        for ( lfop_iterator I = lfoperators.begin(); I != lfoperators.end(); I++ )
        {
            op = I->second;
            delete op;
        }
        lfoperators.clear();
        if(result) delete [] result;
    }


    /*!
        \fn StyledString::Transform( const std::string& _str )
     */
    PStr StyledString::Transform ( const std::string& _str )
    {
        if ( _str.length() ) instring << _str;
        std::string s;
        _clear=false;
        _pos = 0;
        result = 0l;
        //////// DEBUG TRACE TESTS
        //////// Debug << " splitting contents:\n" <<
//             " \"" << CHigh << CRed << instring.data() << CReset << "\"";
        ///////////////////////////////////

        // Decomposer le texte en liste de composants <tag>,"texte"
        String::split ( components, instring.data(), '<','>', true );


        //////// DEBUG TRACE TESTS
        
        /////////////////////////////
        std::list<std::string>::iterator it;
        _len  = instring.len();
        init_result();
        // Boucle centrale du parser
        for ( it = components.begin(); it != components.end(); it++ )
        {
            s = *it;
            
            ( void ) EncodeComponent ( *it ); // return value is no use here.
        }

        ///////////////////////////////////
        
        if ( components.size() ) components.clear();
        //DEND;
        result[_pos] = ( TCell ) 0; //
        return result;

    }

    /*!
        \fn StyledString::EncodeComponent( const std::string& str )
     */
    int StyledString::EncodeComponent ( const std::string& str )
    {
        //////// Debug << str;
        int r =-1;
        std::string::const_iterator e;
        e = str.begin();
        TCell C;
        // Check and execute attr encoding - return immediately if the tag is valid and encoded
        if ( ( *e == '<' ) && ( ( r = EncodeAttr ( str ) ) >= 0 ) ) return r;
        //otherwize concat str to the result with cur attr.
        // If clear flag is set  then this was the last => return the to default attributes set on this sequence
        if ( _clear ) attr = StyledString::_defaultAttributes;
        C = attr.Data();
        for ( ; ( e != str.end() ) && ( _pos < _len ); e++,_pos++ ) result[_pos] = C|*e;
        // DEND;
        return 0;
    }

    /*!
        \fn StyledString::EncodeAttr( const std::string& str )
            if(  (Op = op_locate( e ) )){
                if(!Op->delegator( e )) return -1;
            } else{
                
                DEND;
                while( *e == 0x20 && ( e != str.end() ) && ( *e != ';') ) ++e;
                lfBegin = e;
                while( (e != str.end() ) && ( *e != ';') ) ++e;
                if( e == str.end() ){
                    
                    return -1;
                }

                lfEnd = e;
                e = lfBegin;
                if( !TrySwitchToLongFormat(lfBegin, lfEnd) ) return -1; // TrySwitchToLongFormat
                else e = lfEnd;
            }

     */
    int StyledString::EncodeAttr ( const std::string& str )
    {
        StyledString::Operator* Op = 0l;
        std::string::const_iterator e = str.begin();
        std::string::const_iterator lfBegin, lfEnd;
        if ( *e=='<' ) ++e;
        for ( ; e != str.end(); )
        {

            while ( *e == 0x20 && ( e != str.end() ) && ( *e != '>' ) ) ++e;
            if ( e == str.end() )
            {
                Dbg << " Unexpected EOSstr " ; DEND;
                return -1;
            }
            if ( *e == '>' ) return 0;
            while ( *e == 0x20 && ( e != str.end() ) && ( *e != ';' ) ) ++e;
            lfBegin = e;
            while ( ( e != str.end() ) && ( *e != ';' ) ) ++e;

            if ( e == str.end() )
            {
                Dbg << " Unexpected EOSstr " ; DEND;
                return -1;
            }
            lfEnd = e;
            e = lfBegin;
            if ( !TrySwitchToLongFormat ( lfBegin, lfEnd ) ) return -1; // TrySwitchToLongFormat
            else e = lfEnd;
            ++e;
        }
        // never reached:
        return -100;
    }


    /*!
        \fn StyledString::op_bold( std::string::const_iterator& )
     */
    bool StyledString::op_bold ( std::string::const_iterator& _e )
    {
        //////// Debug << "Clear: " << _clear << "\n";
        attr.SetAttr ( Style::bold, _clear ? false :true );
        //g////// Debug << (const char*)attr; DEND;
        _clear = false;
        return true;
    }


    /*!
        \fn StyledString::op_fg( std::string::const_iterator& )
     */
    bool StyledString::op_fg ( std::string::const_iterator& _e )
    {
        //////// Debug;
        Style::Color C ( 0,0 );
        ++_e;
        int v;
        stripws ( _e );
        if ( ( v=cnum ( *_e ) ) < 0 ) return false;
        int bg = attr.BgColor();
        
        C.fg = v;
        
        C.bg = bg;
        attr << C;
        _clear = false;
        return true;
    }


    /*!
        \fn StyledString::op_not( std::string::const_iterator& )
     */
    bool StyledString::op_not ( std::string::const_iterator& _e )
    {
        _clear = true;
        return true;
    }

    /*!
        \fn StyledString::op_newline( std::string::const_iterator& )
     */

    bool StyledString::op_newline ( std::string::const_iterator& _e )
    {
        attr.SetAttr ( Style::newline, _clear ? false : true );
        _clear = false;
        return true;
    }

    /*!
        \fn StyledString::op_underline( std::string::const_iterator& )
     */
    bool StyledString::op_underline ( std::string::const_iterator& _e )
    {
        //////// Debug;
        
        attr.SetAttr ( Style::underline, _clear ? false:true );
        _clear = false;
        return true;
    }


    /*!
        \fn StyledString::op_bg( std::string::const_iterator& )
     */
    bool StyledString::op_bg ( std::string::const_iterator& _e )
    {
        Style::Color C ( 0,0 );
        if ( _clear )
        {
            C.fg = attr.FgColor();
            C.bg = _defaultAttributes.BgColor();
            attr << C;
            _clear  = false;
            return true;
        }
        ++_e;
        int v;
        stripws ( _e );
        // g////// Debug << *_e;
        if ( ( v=cnum ( *_e ) ) < 0 )
        {
            Dbg << "Illegal value:" << *_e << " <=> " << v ; DEND;
            return false;
        }
        int fg = attr.FgColor();
        C.bg = v;
        C.fg = fg;
        attr << C;
        
        _clear = false;
        return true;
    }

//// ---- Long format processing

    /*!
        \fn StyledString::op_bold( std::string::const_iterator& )
     */
    bool StyledString::lfop_bold ( const std::list<std::string>& L )
    {
        //////// Debug << "Clear: " << _clear << "\n";
        attr.SetAttr ( Style::bold, _clear ? false :true );
        //g////// Debug << (const char*)attr; DEND;
        _clear = false;
        return true;
    }


    /*!
        \fn StyledString::op_fg( std::string::const_iterator& )
     */
    bool StyledString::lfop_fg ( const std::list<std::string>& L )
    {
        //////// Debug << " construct is: " << L.size() << "\n";
        if ( L.size() != 2 ) return false;
        Style::Color C ( 0,0 );
        std::list< std::string >::const_iterator it = L.begin();
        ++it;
        
        int v = Style::ColorNames[*it];
        int bg = attr.BgColor();
        
        C.fg = v;
        
        C.bg = bg;
        attr << C;
        _clear = false;
        return true;
    }


    /*!
        \fn StyledString::op_not( std::string::const_iterator& )
     */
    bool StyledString::lfop_not ( const std::list<std::string>& L )
    {
        _clear = true;
        return true;
    }

    /*!
        \fn StyledString::op_newline( std::string::const_iterator& )
     */

    bool StyledString::lfop_newline ( const std::list<std::string>& L )
    {
        attr.SetAttr ( Style::newline, _clear ? false : true );
        _clear = false;
        return true;
    }

    /*!
        \fn StyledString::op_underline( std::string::const_iterator& )
     */
    bool StyledString::lfop_underline ( const std::list<std::string>& L )
    {
        //////// Debug;
        
        attr.SetAttr ( Style::underline, _clear ? false:true );
        _clear = false;
        return true;
    }


    /*!
        \fn StyledString::op_bg( std::string::const_iterator& )
     */
    bool StyledString::lfop_bg ( const std::list<std::string>& L )
    {
        Style::Color C ( 0,0 );
        if ( _clear )
        {
            C.fg = attr.FgColor();
            C.bg = _defaultAttributes.BgColor();
            attr << C;
            _clear  = false;
            return true;
        }

        //////// Debug << " construct is: " << L.size() << "\n";
        if ( L.size() != 2 ) return false;
        std::list< std::string >::const_iterator it = L.begin();
        ++it;
        
        int v = Style::ColorNames[*it];
        int fg = attr.FgColor();
        
        C.bg = v;
        
        C.fg = fg;
        attr << C;
        
        _clear = false;
        return true;
    }

    /*!
        \fn StyledString::lfop_nul( std::list<std::string>& )
     */
    bool StyledString::lfop_nul ( const std::list<std::string>& L )
    {
        std::list<std::string >::const_iterator Str;
        //////// Debug;
        for ( Str = L.begin(); Str != L.end(); Str++ ) std::cerr << *Str <<';';
        
        return true;

    }

// =--------------------------------------------------------------







    /*!
        \fn StyledString::Initialize()
     */
    bool StyledString::Initialize()
    {
        AddLfOperator ( new StyledString::Operator ( "STRONG",    sigc::mem_fun ( this, &StyledString::lfop_bold ) ) );
        AddLfOperator ( new StyledString::Operator ( "BOLD",      sigc::mem_fun ( this, &StyledString::lfop_bold ) ) );
        AddLfOperator ( new StyledString::Operator ( "FGCOLOR",   sigc::mem_fun ( this, &StyledString::lfop_fg ) ) );
        AddLfOperator ( new StyledString::Operator ( "/",         sigc::mem_fun ( this, &StyledString::lfop_not ) ) );
        AddLfOperator ( new StyledString::Operator ( "UNDERLINE", sigc::mem_fun ( this, &StyledString::lfop_underline ) ) );
        AddLfOperator ( new StyledString::Operator ( "BR",        sigc::mem_fun ( this, &StyledString::lfop_newline ) ) );
        AddLfOperator ( new StyledString::Operator ( "BREAK",     sigc::mem_fun ( this, &StyledString::lfop_newline ) ) );
        AddLfOperator ( new StyledString::Operator ( "BGCOLOR",   sigc::mem_fun ( this, &StyledString::lfop_bg ) ) );

        // Rectangular/multi-lines result:
        // The following operators will use the PainterDC object for the rectangular result
        // -- not yet implemented - only planned
        AddLfOperator ( new StyledString::Operator ( "REGION",    sigc::mem_fun ( this, &StyledString::lfop_nul ) ) );
        AddLfOperator ( new StyledString::Operator ( "CENTER",    sigc::mem_fun ( this, &StyledString::lfop_nul ) ) );
        AddLfOperator ( new StyledString::Operator ( "RIGHT",     sigc::mem_fun ( this, &StyledString::lfop_nul ) ) );
        AddLfOperator ( new StyledString::Operator ( "LEFT",      sigc::mem_fun ( this, &StyledString::lfop_nul ) ) );
        AddLfOperator ( new StyledString::Operator ( "GRID",      sigc::mem_fun ( this, &StyledString::lfop_nul ) ) );
        AddLfOperator ( new StyledString::Operator ( "FRAME",     sigc::mem_fun ( this, &StyledString::lfop_nul ) ) );
        AddLfOperator ( new StyledString::Operator ( "FRAMETITLE",sigc::mem_fun ( this, &StyledString::lfop_nul ) ) );
        AddLfOperator ( new StyledString::Operator ( "CLEAR",     sigc::mem_fun ( this, &StyledString::lfop_nul ) ) );
        // -- WIDTH | HEIGHT not needed - usage will be '<region {w h};> or '<frame {w h};>' insteaad
        //AddLfOperator ( new StyledString::Operator ( "WIDTH",     sigc::mem_fun ( this, &StyledString::lfop_nul ) ) );
        //AddLfOperator ( new StyledString::Operator ( "HEIGHT",     sigc::mem_fun ( this, &StyledString::lfop_nul ) ) );


        return true;
    }

    /*!
        \fn StyledString::op_locate( std::string::const_iterator _e )
     */
    StyledString::Operator* StyledString::op_locate ( std::string::const_iterator _e )
    {
        StyledString::op_iterator it = StyledString::operators.find ( *_e );
        if ( it == StyledString::operators.end() ) return 0l;
        return it->second;
    }

    /*!
        \fn StyledString::op_locate( std::string::const_iterator _e )
        \todo Change method return type to " const StyledString::Operator"
     */
    StyledString::Operator* StyledString::lfop_locate ( const std::string& _key )
    {
        //////// Debug << "Finding key:" << _key << "...\n";
        StyledString::lfop_iterator it = StyledString::lfoperators.find ( _key );
        if ( it == StyledString::lfoperators.end() )
        {
            Dbg << _key << " Not found! return 0;"; DEND;
            return 0l;
        }
        //DEND;
        return it->second;
        ///@todo return *(it->second);
    }

    /*!
        \fn StyledString::op_calclength()
     */
    int StyledString::op_calclength()
    {

    }



    /*!
        \fn StyledString::init_result()
     */
    void StyledString::init_result()
    {
        
        if(result) delete [] result;
        result = new TCell[_len];
        //StyledString::_defaultAttributes._d  = Style::normal|Style::ptbl[swcolor::black][swcolor::white]|' ';
        TCell C = StyledString::_defaultAttributes.Data();
        attr = StyledString::_defaultAttributes;
        for ( int x=0; x< _len; x++ ) result[x] = C|0;
    }



    /*!
        \fn StyledString::ReleaseData()
     */
    void StyledString::ReleaseData()
    {
        if ( result )
        {
            //delete [] result;
            result = 0l;
        }
        instring.clear();
    }



    /*!
        \fn StyledString::length(PStr p = 0l)
     */
    int StyledString::length ( PStr S )
    {
        PStr p = S ? S : result;
        if ( !p ) return 0;
        int n=0;
        while ( *p )
        {
            n++; p++;
        }
        return n;
    }


    /*!
        \fn StyledString::TrySwitchToLongFormat( std::string::const_iterator, std::string::const_iterator)
     */
    bool StyledString::TrySwitchToLongFormat ( std::string::const_iterator istart, std::string::const_iterator iend )
    {

        //////// Debug ;
        std::string str ( istart, iend );
        str = String::Upcase ( str );
        std::list<std::string> L;
        String::split ( L,str," " ,false,false );
        if ( ! L.size() )
        {
            ////// Debug << "Error splitting attributes command text";DEND;
            return false;
        }
        
        str = * ( L.begin() );
        if ( *str.begin() =='/' )
        {
            _clear = true;
            str.erase ( 0,1 );
        }
        Operator* Op = lfop_locate ( str );
        if ( ! Op )
        {
            ////// Debug << "Attribute command '" << str << "' not found or not implemented yet" ; DEND;
            return false;
        }
        if ( ! ( * ( Op->lfdelegator ) ) ( L ) )
        {
            ////// Debug << "Attribute command '" << str << "' failed - maybe syntax error"; DEND;
            return false;
        }
        return true;
    }


}
