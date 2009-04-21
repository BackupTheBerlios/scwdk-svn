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

#include <object.h>
#include <exstring.h>
#include <cursesterm.h>
#include <cursesattr.h>


#include <map>
namespace wcurses
{
    /**
    Simple and limited to WidgetCurses text embedded style parser.

        @author Serge Lussier,,, <lussier.serge@gmail.com>
        \note   Fait: <br> <ul>
            <li>Source de la valeur des attributs de d&eacute;part ( par d&eacute;faut ) : SetDefaultAttributes( )</li>
            <li>Initialiser defattr et attr avec les valeurs par d&eacute;faut.</li>
            <li>Transormation effectu&eacute;e sous long format seulement... </li>
            </ul>
                &agrave; faire: <br><ul>
            <li>Transformation sous format court -- en bug -- faut corriger - conflit avec long format</li>
            <li>Formatage du texte &eacute;volu&eacute;</li>
        </ul>
    */
    class StyledString : public Object
    {
        public:
            /*!
                \enum StreamCmd
                \note Insertion de pseudo-commandes de flux:
                <ul>
                    <li>END - Invoque la transformation du texte en stream. ex.:
                    \code
                        StyledStr << "<bgcolor blue fgcolor; white;> teste..." << StyledString::END;
                        //...::END va terminer l'insertion du stream et va invoquer la transformation
                    \endcode
                    
                    </li>
                    <li>CLR - met &agrave;-vide le buffer de transformation ( recommence &agrave; zero la sequence du texte depuis la derni&egrave;re commande END)
                        <ul>
                            <li>Mise &agrave; zero du buffer de texte sans toucher au buffer du stream </li>
                        </ul>
                    \code
                        StyledStr << StyledString::CLR << "<bgcolor blue fgcolor; white;> test..." << StyledString::END;
                    \endcode
                    
                    </li>
                </ul>
            */
            enum StreamCmd { END, ARG, CLR };
            enum TxStyle { plain, text };
            typedef sigc::signal< bool, std::string::const_iterator& > Delegator;
            typedef sigc::slot< bool, std::string::const_iterator& > Delegate;

            typedef sigc::signal< bool, const std::list<std::string>&  > LFDelegator;
            typedef sigc::slot< bool, const std::list<std::string>&  > LFDelegate;


            struct Operator
            {

                StyledString::Delegator *delegator;
                StyledString::LFDelegator *lfdelegator;

                Operator ( const char _tk ) :tk ( _tk ) {}
                Operator ( const char _tk, StyledString::Delegate delegate ) :tk ( _tk )
                {
                    delegator = new Delegator;
                    lfdelegator = 0l;
                    delegator->connect ( delegate );
                }

                Operator ( const std::string& _key, StyledString::LFDelegate delegate ) : _keyword ( _key )
                {
                    lfdelegator = new LFDelegator;
                    delegator = 0l;
                    lfdelegator->connect ( delegate );
                }
                Operator& operator += ( StyledString::LFDelegate& delegate )
                {

                    lfdelegator = new LFDelegator;
                    lfdelegator->connect ( delegate );
                    return *this;
                }
                ~Operator()
                {
                    if ( lfdelegator ) delete lfdelegator;
                    if ( delegator ) delete delegator;
                }

                char tk;
                std::string _keyword;
            };


            typedef std::map< char, Operator* >::iterator op_iterator;
            typedef std::map< std::string, Operator*>::iterator lfop_iterator;

            StyledString ( Object* parent, const Style defAttr, const std::string idname );
            StyledString ( const std::string& _text );
            StyledString ( const String& _text );
            StyledString ( Object* parent, const std::string& _text );
            StyledString ( Object* parent, const String& _text );
            StyledString();

            virtual ~StyledString();

            /*!
                \fn StyledString::SetDefaultAttributes( const Style& _attr )
             */
            Style SetDefaultAttributes ( const Style& _attr ) { StyledString::_defaultAttributes = _attr; }

            /*!
                \fn StyledString::Data()
             */
            PStr Data() { return result; }
            bool Initialize();
            PStr Transform ( const std::string& _str="" );
            PStr Encode ( const std::string& _str ) { return Transform ( _str ); }
            PStr Encode ( const String& _str ) { return Transform ( _str.std() ); }
            inline Style CurAttr() { return attr; }
            StyledString& operator >> ( PStr& pstr_ptr )
            {
                pstr_ptr = result;
                return *this;
            }

            // To make it possible to do: . . . Parser << "he<blah>llo</blah>" >> pResult; -- funny statements!! :-)
            template <typename arg> StyledString& operator<< ( arg A )
            {
                //Debug;
                instring << A;
                // Dbg << " apeended str" << instring.std();DEND;
                return *this;
            }

            StyledString& operator << ( StyledString::StreamCmd E )
            {
                switch ( E )
                {
                    case END :
                        Transform( );
                        break;
                    case CLR:
                        ClearText();
                        break;
                }
                return *this;
            }

            StyledString& operator += ( StyledString::Delegate D )
            {
                delegator.connect ( D );
                return *this;
            }
            void init_result();
            void ReleaseData();
            int length ( PStr S = 0l );

            /*!
                \fn StyledString::ClearText()
             */
            void ClearText()
            {
                instring.clear();
            }

            /*!
                \fn StyledString::Clear()
             */
            void Clear()
            {
                ReleaseData();
            }
        private:
            char* optokens;
            std::list<std::string> components;
            String instring;
            Style attr;
            PStr result;
            bool _clear;

            std::map< char, Operator* > operators;
            std::map< std::string, Operator*> lfoperators;
            //static const char* opref;
            StyledString::Delegator delegator;
            int _pos;
            bool op_bold ( std::string::const_iterator& );
            bool op_fg ( std::string::const_iterator& );
            bool op_not ( std::string::const_iterator& );
            bool op_underline ( std::string::const_iterator& );
            bool op_newline ( std::string::const_iterator& );
            bool op_bg ( std::string::const_iterator& );

            bool lfop_bold ( const std::list<std::string>& );
            bool lfop_fg ( const std::list<std::string>& L );
            bool lfop_not ( const std::list<std::string>& L );
            bool lfop_underline ( const std::list<std::string>& L );
            bool lfop_newline ( const std::list<std::string>& L );
            bool lfop_bg ( const std::list<std::string>& L );
            bool lfop_nul ( const std::list<std::string>& L );
            // Parser << new StyledString::Operator( 'b', sigc::slot(Parser, &StyledString::op_bold) );
            StyledString& operator << ( StyledString::Operator* op )
            {
                StyledString::operators[op->tk] = op;
                return *this;
            }

            int AddLfOperator ( StyledString::Operator* op )
            {
                StyledString::lfoperators[op->_keyword] = op;
                return lfoperators.size();
            }

            StyledString::Operator& operator[] ( char t )
            {
                return * ( operators[t] );
            }

            StyledString::Operator& operator[] ( const std::string& _key )
            {
                return * ( lfoperators[_key] );
            }

            Operator* op_locate ( std::string::const_iterator _e );
            Operator* lfop_locate ( const std::string& _key );
            int op_calclength();
            bool TrySwitchToLongFormat ( std::string::const_iterator, std::string::const_iterator );
            int _len;
            Style _defaultAttributes;
            TxStyle mTxStyle;
        protected:
            bool init_done;
            int EncodeAttr ( const std::string& str );
            int EncodeComponent ( const std::string& str );
    };
}
#endif
