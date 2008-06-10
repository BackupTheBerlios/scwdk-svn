//
// C++ Interface: nattrstrparser
//
// Description: 
//
//
// Author: Serge Lussier,,, <lussier.serge@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CATTRSTRPARSER_H
#define CATTRSTRPARSER_H

#include <swobject.h>
#include <exstring.h>
#include <swtattr.h>


#include <map>
/**
Simple and limited to ncwk text embedded attributes parser.

	@author Serge Lussier,,, <lussier.serge@gmail.com>
    \note   Fait: <br> <ul>
        <li>Source de la valeur des attributs de d&eacute;part ( par d&eacute;faut ) : SetDefaultAttributes( )</li>
        <li>Initialiser defattr et attr avec les valeurs par d&eacute;faut.</li>
    </ul>
*/
class swText : public swObject
{
public:
    enum StreamCmd { END, ARG }; // Force specific type used for operator <<
    enum TxStyle { plain, text }; 
    typedef sigc::signal< bool, std::string::const_iterator& > Delegator;
    typedef sigc::slot< bool, std::string::const_iterator& > Delegate;

    typedef sigc::signal< bool, const std::list<std::string>&  > LFDelegator;
    typedef sigc::slot< bool, const std::list<std::string>&  > LFDelegate;


    struct Operator {

        swText::Delegator *delegator;
        swText::LFDelegator *lfdelegator;        

        Operator( const char _tk ):tk(_tk){}
        Operator( const char _tk, swText::Delegate delegate ):tk(_tk){
            delegator = new Delegator;
            lfdelegator = 0l;
            delegator->connect( delegate );
        }

        Operator ( const std::string& _key, swText::LFDelegate delegate ): _keyword(_key){
            lfdelegator = new LFDelegator;
            delegator = 0l;
            lfdelegator->connect( delegate );
        }
        Operator& operator += (swText::LFDelegate& delegate){

            lfdelegator = new LFDelegator; 
            lfdelegator->connect( delegate );
            return *this;
        }
        ~Operator(){
            if(lfdelegator) delete lfdelegator;
            if(delegator) delete delegator;
        }

        char tk;
        std::string _keyword;
    };

    
    typedef std::map< char, Operator* >::iterator op_iterator;
    typedef std::map< std::string, Operator*>::iterator lfop_iterator;
    
    swText(swObject* parent, const swTAttr defAttr, const std::string idname);
    swText(const std::string& _text);
    swText(const String& _text);
    swText(swObject* parent, const std::string& _text);
    swText(swObject* parent, const String& _text);
    swText();
    
    virtual ~swText();

    /*!
        \fn swText::SetDefaultAttributes( const swTAttr& _attr )
     */
    swTAttr SetDefaultAttributes( const swTAttr& _attr ) { swText::_defaultAttributes = _attr; }

    /*!
        \fn swText::Data()
     */
    PStr Data() { return result; }
    bool Initialize();
    PStr Transform( const std::string& _str="" );
    PStr Encode( const std::string& _str ){ return Transform( _str ); }
    PStr Encode( const String& _str ){ return Transform( _str.std() ); }
    inline swTAttr CurAttr() { return attr; }
    swText& operator >> (PStr& pstr_ptr) {
        pstr_ptr = result;
        return *this;
    }

    // To make it possible to do: . . . Parser << "he<blah>llo</blah>" >> pResult; -- funny statements!! :-)
    template <typename arg> swText& operator<< ( arg A ){
        //Debug;
        instring << A;
       // Dbg << " apeended str" << instring.std();DEND;
        return *this;
    }
    
    swText& operator << ( swText::StreamCmd E ){
        switch(E){
            case END :
                Transform( );
                break;
        }
        return *this;
    }
    
    swText& operator += (swText::Delegate D){
        delegator.connect( D );
        return *this;
    }
    void init_result();
    void ReleaseData();
    int length(PStr S = 0l);

    /*!
        \fn swText::ClearText()
     */
    void ClearText()
    {
        instring.clear();
    }
private:
    char* optokens;
    std::list<std::string> components;
    String instring;
    swTAttr attr;
    PStr result;
    bool _clear;
    
    std::map< char, Operator* > operators;
    std::map< std::string, Operator*> lfoperators;
    //static const char* opref;
    swText::Delegator delegator;
    int _pos;
    bool op_bold( std::string::const_iterator& );
    bool op_fg( std::string::const_iterator& );
    bool op_not( std::string::const_iterator& );
    bool op_underline( std::string::const_iterator& );
    bool op_newline( std::string::const_iterator& );
    bool op_bg( std::string::const_iterator& );

    bool lfop_bold( const std::list<std::string>&  );
    bool lfop_fg( const std::list<std::string>& L );
    bool lfop_not( const std::list<std::string>& L );
    bool lfop_underline( const std::list<std::string>& L );
    bool lfop_newline( const std::list<std::string>& L );
    bool lfop_bg(  const std::list<std::string>& L );
    bool lfop_nul( const std::list<std::string>& L);
    // Parser << new swText::Operator( 'b', sigc::slot(Parser, &swText::op_bold) );
    swText& operator << ( swText::Operator* op ){
        swText::operators[op->tk] = op;
        return *this;
    }

    int AddLfOperator( swText::Operator* op ){
        swText::lfoperators[op->_keyword] = op;
        return lfoperators.size();
    }
    
    swText::Operator& operator[]( char t ){
        return *(operators[t]);
    }

    swText::Operator& operator[]( const std::string& _key ){
        return *(lfoperators[_key]);
    }
    
    Operator* op_locate( std::string::const_iterator _e );
    Operator* lfop_locate( const std::string& _key );
    int op_calclength();
    bool TrySwitchToLongFormat( std::string::const_iterator, std::string::const_iterator);
    int _len;
    swTAttr _defaultAttributes;
    TxStyle mTxStyle;
protected:
    bool init_done;
    int EncodeAttr( const std::string& str );
    int EncodeComponent( const std::string& str );
};

#endif
