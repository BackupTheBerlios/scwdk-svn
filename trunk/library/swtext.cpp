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
#include "swtext.h"


//swTAttr swText::_defaultAttributes;

//const char* swText::opref="bF/_nB";
//std::map< char, swText::Operator* > swText::operators;
//std::map< std::string, swText::Operator*> swText::lfoperators;
//bool swText::init_done = false;







#define stripws(e) while( (*(e) == 0x20) && (*(e) !='>')) ++(e)

#define cnum(e) (((e) <='9') && ((e) >='0')) ? (e)-'0' : -1


swText::swText(swObject* parent, const swTAttr defAttr, const std::string idname): swObject(parent, 0,idname.c_str())
{
    swText::_defaultAttributes = defAttr;
    Initialize();
}


swText::swText(): swObject()
{
    Initialize();
}



swText::swText(const std::string& _text)
 :
    swObject(0l,0,"")
{
    Initialize();
}

swText::swText(const String& _text)
:
    swObject()
{
    Initialize();
    //instring << _text;
}

swText::swText(swObject* parent, const std::string& _text)
:
    swObject(parent,0,"")
{
    Initialize();
    //instring << _text;
}

swText::swText(swObject* parent, const String& _text)
:
    swObject(parent,0,"")
{
    Initialize();
    //instring << _text;
}



swText::~swText()
{
}


/*!
    \fn swText::Transform( const std::string& _str )
 */
PStr swText::Transform( const std::string& _str )
{
    if( _str.length() ) instring << _str;
    std::string s;
    _clear=false;
    _pos = 0;
    result = 0l;
    //////// DEBUG TRACE TESTS
    //Debug << " splitting contents:\n" <<
//             " \"" << CHigh << CRed << instring.data() << CReset << "\"";
    ///////////////////////////////////

    // Decomposer le texte en liste de composants <tag>,"texte"
    String::split(components, instring.data(), '<','>', true);
    

    //////// DEBUG TRACE TESTS
    //Dbg << "Number of components:" << CBrown << components.size() << CReset;
    /////////////////////////////
    std::list<std::string>::iterator it;
    _len  = instring.len();
    init_result();
    // Boucle centrale du parser
    for( it = components.begin(); it != components.end(); it++){
        s = *it;
      //  Dbg << "Encoding " << "'" << CCyan << s << CReset << "' @pos=" << _pos;
        (void)EncodeComponent( *it ); // return value is no use here.
    }

    ///////////////////////////////////
    //Dbg << "Clearing unneeded components list items. Not destroying result since it will by the owner";
    if( components.size() ) components.clear();
    //DEND;
    result[_pos] = (TCell)0; //
    return result;
    
}

/*!
    \fn swText::EncodeComponent( const std::string& str )
 */
int swText::EncodeComponent( const std::string& str )
{
    //Debug << str;
    int r =-1;
    std::string::const_iterator e;
    e = str.begin();
    TCell C;
    // Check and execute attr encoding - return immediately if the tag is valid and encoded
    if(  (*e == '<' ) && ( (r = EncodeAttr( str )) >= 0) ) return r;
    //otherwize concat str to the result with cur attr.
    // If clear flag is set  then this was the last => return the to default attributes set on this sequence
    if(_clear) attr = swText::_defaultAttributes;
    C = attr.Data();
    for( ; (e != str.end()) && (_pos < _len); e++,_pos++ ) result[_pos] = C|*e;
   // DEND;
    return 0;
}

/*!
    \fn swText::EncodeAttr( const std::string& str )
        if(  (Op = op_locate( e ) )){
            if(!Op->delegator( e )) return -1;
        } else{
            Dbg << "Failed to get a delegate for '" << *e << '\'';
            DEND;
            while( *e == 0x20 && ( e != str.end() ) && ( *e != ';') ) ++e;
            lfBegin = e;
            while( (e != str.end() ) && ( *e != ';') ) ++e;
            if( e == str.end() ){
                Dbg << " Unexpected EOSstr " ; DEND;
                return -1;
            }
            
            lfEnd = e;
            e = lfBegin;
            if( !TrySwitchToLongFormat(lfBegin, lfEnd) ) return -1; // TrySwitchToLongFormat
            else e = lfEnd;
        }
 
 */
int swText::EncodeAttr( const std::string& str )
{
    swText::Operator* Op = 0l;
    std::string::const_iterator e = str.begin();
    std::string::const_iterator lfBegin, lfEnd;
    if(*e=='<') ++e;
    for( ; e != str.end(); ){
        
        while( *e == 0x20 && ( e != str.end() ) && ( *e != '>') ) ++e;
        if( e == str.end() ){
            Dbg << " Unexpected EOSstr " ; DEND;
            return -1;
        }
        if( *e == '>' ) return 0;
        while( *e == 0x20 && ( e != str.end() ) && ( *e != ';') ) ++e;
        lfBegin = e;
        while( (e != str.end() ) && ( *e != ';') ) ++e;

        if( e == str.end() ){
            Dbg << " Unexpected EOSstr " ; DEND;
            return -1;
        }
        lfEnd = e;
        e = lfBegin;
        if( !TrySwitchToLongFormat(lfBegin, lfEnd) ) return -1; // TrySwitchToLongFormat
        else e = lfEnd;
        ++e;
    }
    // never reached:
    return -100;
}


/*!
    \fn swText::op_bold( std::string::const_iterator& )
 */
bool swText::op_bold( std::string::const_iterator& _e)
{
    //Debug << "Clear: " << _clear << "\n";
    attr.SetAttr( swTAttr::bold, _clear ? false :true );
    //gDebug << (const char*)attr; DEND;
    _clear = false;
    return true;
}


/*!
    \fn swText::op_fg( std::string::const_iterator& )
 */
bool swText::op_fg( std::string::const_iterator& _e)
{
    //Debug;
    swTAttr::Color C(0,0);
    ++_e;
    int v;
    stripws(_e);
    if( (v=cnum( *_e )) < 0 ) return false;
    int bg = attr.BgColor();
    //Dbg << "CUrrent bg=" << bg;DEND;
    C.fg = v;
    //Dbg "Expected fg: " << v;
    C.bg = bg;
    attr << C;
    _clear = false;
    return true;
}


/*!
    \fn swText::op_not( std::string::const_iterator& )
 */
bool swText::op_not( std::string::const_iterator& _e)
{
    _clear = true;
    return true;
}

/*!
    \fn swText::op_newline( std::string::const_iterator& )
 */
 
bool swText::op_newline( std::string::const_iterator& _e)
{
    attr.SetAttr( swTAttr::newline, _clear ? false : true );
    _clear = false;
    return true;
}

/*!
    \fn swText::op_underline( std::string::const_iterator& )
 */
bool swText::op_underline( std::string::const_iterator& _e)
{
    //Debug;
    //Dbg << " _clear set ?" << (char*)(_clear ? " Yes " : " No " );DEND;
     attr.SetAttr( swTAttr::underline, _clear ? false:true );
     _clear = false;
     return true;
}


/*!
    \fn swText::op_bg( std::string::const_iterator& )
 */
bool swText::op_bg( std::string::const_iterator& _e)
{
    swTAttr::Color C(0,0);
    if( _clear) {
        C.fg = attr.FgColor();
        C.bg = _defaultAttributes.BgColor();
        attr << C;
        _clear  = false;
        return true;
    }
    ++_e;
    int v;
    stripws(_e);
   // gDebug << *_e;
    if( (v=cnum( *_e )) < 0 ){
        Dbg << "Illegal value:" << *_e << " <=> " << v ; DEND;
        return false;
    }
    int fg = attr.FgColor();
    C.bg = v;
    C.fg = fg;
    attr << C;
   // Dbg << " Bg set to " << v; DEND;
    _clear = false;
    return true;
}

//// ---- Long format processing

/*!
    \fn swText::op_bold( std::string::const_iterator& )
 */
bool swText::lfop_bold( const std::list<std::string>& L)
{
    //Debug << "Clear: " << _clear << "\n";
    attr.SetAttr( swTAttr::bold, _clear ? false :true );
    //gDebug << (const char*)attr; DEND;
    _clear = false;
    return true;
}


/*!
    \fn swText::op_fg( std::string::const_iterator& )
 */
bool swText::lfop_fg( const std::list<std::string>& L)
{
    //Debug << " construct is: " << L.size() << "\n";
    if( L.size() != 2 ) return false;
    swTAttr::Color C(0,0);
    std::list< std::string >::const_iterator it = L.begin();
    ++it;
    //Dbg << "Color name to get:" << *it << "\n";
    int v = swTAttr::ColorNames[*it];
    int bg = attr.BgColor();
   // Dbg << "Current bg=" << bg;DEND;
    C.fg = v;
   // Dbg "Expected fg: " << v;
    C.bg = bg;
    attr << C;
    _clear = false;
    return true;
}


/*!
    \fn swText::op_not( std::string::const_iterator& )
 */
bool swText::lfop_not( const std::list<std::string>& L)
{
    _clear = true;
    return true;
}

/*!
    \fn swText::op_newline( std::string::const_iterator& )
 */
 
bool swText::lfop_newline( const std::list<std::string>& L )
{
    attr.SetAttr( swTAttr::newline, _clear ? false : true );
    _clear = false;
    return true;
}

/*!
    \fn swText::op_underline( std::string::const_iterator& )
 */
bool swText::lfop_underline( const std::list<std::string>& L)
{
    //Debug;
    //Dbg << " _clear set ?" << (char*)(_clear ? " Yes " : " No " );DEND;
     attr.SetAttr( swTAttr::underline, _clear ? false:true );
     _clear = false;
     return true;
}


/*!
    \fn swText::op_bg( std::string::const_iterator& )
 */
bool swText::lfop_bg( const std::list<std::string>& L)
{
    swTAttr::Color C(0,0);
    if( _clear) {
        C.fg = attr.FgColor();
        C.bg = _defaultAttributes.BgColor();
        attr << C;
        _clear  = false;
        return true;
    }

    //Debug << " construct is: " << L.size() << "\n";
    if( L.size() != 2 ) return false;
    std::list< std::string >::const_iterator it = L.begin();
    ++it;
    //Dbg << "Color name to get:" << *it << "\n";
    int v = swTAttr::ColorNames[*it];
    int fg = attr.FgColor();
    //Dbg << "Current fg=" << fg;DEND;
    C.bg = v;
    //Dbg "Expected bg: " << v;
    C.fg = fg;
    attr << C;
    //Dbg << " Bg set to " << v; DEND;
    _clear = false;
    return true;
}

/*!
    \fn swText::lfop_nul( std::list<std::string>& )
 */
bool swText::lfop_nul( const std::list<std::string>& L )
{
    std::list<std::string >::const_iterator Str;
    //Debug;
    for( Str = L.begin(); Str != L.end(); Str++) std::cerr << *Str <<';';
    //Dbg << " This command is not implemented yet";DEND;
    return true;
    
}

// =--------------------------------------------------------------







/*!
    \fn swText::Initialize()
 */
bool swText::Initialize()
{//bF/_nB";
    gDebug ; DEND;
//     if(swText::init_done){
//         gDebug << "Already init - skipping...\n" ; DEND;
//         return true;
//     }
    //swText::init_done = true;
    //(*this) << new swText::Operator('b', sigc::mem_fun( this, &swText::op_bold))
    //        << new swText::Operator('F', sigc::mem_fun( this, &swText::op_fg  ))
    //        << new swText::Operator('/', sigc::mem_fun( this, &swText::op_not))
    //        << new swText::Operator('_', sigc::mem_fun( this, &swText::op_underline))
    //        << new swText::Operator('n', sigc::mem_fun( this, &swText::op_newline))
    //        << new swText::Operator('B', sigc::mem_fun( this, &swText::op_bg));
    //        // Long format
    AddLfOperator(new swText::Operator("STRONG",    sigc::mem_fun( this, &swText::lfop_bold)));
    AddLfOperator(new swText::Operator("FGCOLOR",   sigc::mem_fun( this, &swText::lfop_fg  )));
    AddLfOperator(new swText::Operator("/",         sigc::mem_fun( this, &swText::lfop_not)));
    AddLfOperator(new swText::Operator("UNDERLINE", sigc::mem_fun( this, &swText::lfop_underline)));
    AddLfOperator(new swText::Operator("BR",        sigc::mem_fun( this, &swText::lfop_newline)));
    AddLfOperator(new swText::Operator("BGCOLOR",   sigc::mem_fun( this, &swText::lfop_bg)));
    AddLfOperator(new swText::Operator("REGION",    sigc::mem_fun( this, &swText::lfop_nul)));
    AddLfOperator(new swText::Operator("CENTER",    sigc::mem_fun( this, &swText::lfop_nul)));
    AddLfOperator(new swText::Operator("RIGHT",     sigc::mem_fun( this, &swText::lfop_nul)));
    AddLfOperator(new swText::Operator("LEFT",      sigc::mem_fun( this, &swText::lfop_nul)));
    AddLfOperator(new swText::Operator("GRID",      sigc::mem_fun( this, &swText::lfop_nul)));
    AddLfOperator(new swText::Operator("FRAME",     sigc::mem_fun( this, &swText::lfop_nul)));
    AddLfOperator(new swText::Operator("FRAMETITLE",sigc::mem_fun( this, &swText::lfop_nul)));
    AddLfOperator(new swText::Operator("CLEAR",     sigc::mem_fun( this, &swText::lfop_nul)));
    
    
    return true;
}

/*!
    \fn swText::op_locate( std::string::const_iterator _e )
 */
swText::Operator* swText::op_locate( std::string::const_iterator _e )
{
    swText::op_iterator it = swText::operators.find(*_e);
    if (it == swText::operators.end() ) return 0l;
    return it->second;
}

/*!
    \fn swText::op_locate( std::string::const_iterator _e )
 */
swText::Operator* swText::lfop_locate( const std::string& _key )
{
    //Debug << "Finding key:" << _key << "...\n";
    swText::lfop_iterator it = swText::lfoperators.find(_key);
    if (it == swText::lfoperators.end() ){
        Dbg << _key << " Not found!@!! return 0l;"; DEND;
        return 0l;
    }
    //DEND;
    return it->second;
}

/*!
    \fn swText::op_calclength()
 */
int swText::op_calclength()
{
        
}



/*!
    \fn swText::init_result()
 */
void swText::init_result()
{
    //Dbg << "Allocating to PStr buffer: " << CHigh << CGreen << _len*( sizeof(TCell) ) << CReset << " bytes" ;
    result = new TCell[_len];
    //swText::_defaultAttributes._d  = swTAttr::normal|swTAttr::ptbl[swcolor::black][swcolor::white]|' ';
    TCell C = swText::_defaultAttributes.Data();
    attr = swText::_defaultAttributes;
    for(int x=0; x< _len; x++) result[x] = C|0;
}



/*!
    \fn swText::ReleaseData()
 */
void swText::ReleaseData()
{
    if( result ){
        //delete [] result;
        result = 0l;
    }
    instring.clear();
}



/*!
    \fn swText::length(PStr p = 0l)
 */
int swText::length(PStr S )
{
    PStr p = S ? S : result;
    if(!p ) return 0;
    int n=0;
    while(*p){
        n++; p++;
    }
    return n;
}


/*!
    \fn swText::TrySwitchToLongFormat( std::string::const_iterator, std::string::const_iterator)
 */
bool swText::TrySwitchToLongFormat( std::string::const_iterator istart, std::string::const_iterator iend)
{

    //Debug ;
    std::string str(istart, iend);
    str = String::Upcase( str );
    std::list<std::string> L;
    String::split(L,str," " ,false,false);
    if(! L.size() ){
        Dbg << "Error splitting attributes comand text";DEND;
        return false;
    }
    //Dbg << "Number of tokens in the attribute command: " << L.size() << "\n";
    str = *(L.begin());
    if(*str.begin() =='/'){
        _clear = true;
        str.erase(0,1);
    }
    Operator* Op = lfop_locate( str );
    if(! Op ){
      //  Dbg << "Attribute command '" << str << "' not found or not implemented yet" ; DEND;
        return false;
    }
    if(! Op->lfdelegator( L ) ){
        //Dbg << "Attribute command '" << str << "' failed - maybe syntax error"; DEND;
        return false;
    }
    return true;
}


