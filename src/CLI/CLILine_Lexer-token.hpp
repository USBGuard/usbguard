/* -*- C++ -*-   vim: set syntax=cpp: 
 * (C) 2004-2009 Frank-Rene Schaefer
 * ABSOLUTELY NO WARRANTY
 */
#ifndef __QUEX_INCLUDE_GUARD__TOKEN__GENERATED____CLILINE____TOKEN
#define __QUEX_INCLUDE_GUARD__TOKEN__GENERATED____CLILINE____TOKEN

#include <quex/code_base/definitions>
#include <quex/code_base/asserts>
#include <quex/code_base/compatibility/stdint.h>
#include <quex/code_base/MemoryManager>

/* LexemeNull object may be used for 'take_text'. */
QUEX_NAMESPACE_LEXEME_NULL_OPEN
extern QUEX_TYPE_CHARACTER   QUEX_LEXEME_NULL_IN_ITS_NAMESPACE;
QUEX_NAMESPACE_LEXEME_NULL_CLOSE



#   line 7 "quex/code_base/token/CppDefault.qx"

#       include <iostream>
#       include <string>

#include <quex/code_base/converter_helper/identity>
   

#   line 29 "CLILine_Lexer-token.hpp"

 
namespace CLILine {


class Token;

/* NOTE: The following function might have to be defined by the user if one
 *       does not rely on quex's function generation.                        */
extern const char*  QUEX_NAME_TOKEN(map_id_to_name)(const QUEX_TYPE_TOKEN_ID);

inline void    QUEX_NAME_TOKEN(construct)(Token* __this);
inline void    QUEX_NAME_TOKEN(destruct)(Token* __this);
inline void    QUEX_NAME_TOKEN(copy)(Token* me, const Token* That);
inline bool    QUEX_NAME_TOKEN(take_text)(Token*            __this, 
                                          QUEX_TYPE_ANALYZER*         analyzer, 
                                          const QUEX_TYPE_CHARACTER*  Begin, 
                                          const QUEX_TYPE_CHARACTER*  End);

#ifdef QUEX_OPTION_TOKEN_REPETITION_SUPPORT
inline size_t  QUEX_NAME_TOKEN(repetition_n_get)(Token* __this);
inline void    QUEX_NAME_TOKEN(repetition_n_set)(Token* __this, size_t N);
#endif 

class Token {
public:
    Token();
    Token(const Token& That);
    /* operator=(..): USE WITH CAUTION--POSSIBLE MAJOR PERFORMANCE DECREASE! 
     *                BETTER USE __copy(That)                                */
    Token& operator=(const Token& That) 
    { QUEX_NAME_TOKEN(copy)(this, &That); return *this; }
    virtual ~Token();


#   line 14 "quex/code_base/token/CppDefault.qx"
    std::basic_string<QUEX_TYPE_CHARACTER> text;

#   line 68 "CLILine_Lexer-token.hpp"

#   line 15 "quex/code_base/token/CppDefault.qx"
    size_t                                 number;

#   line 73 "CLILine_Lexer-token.hpp"


public:

#   line 14 "quex/code_base/token/CppDefault.qx"
    std::basic_string<QUEX_TYPE_CHARACTER> get_text() const                                         { return text; }
#   line 14 "quex/code_base/token/CppDefault.qx"
    void                                   set_text(std::basic_string<QUEX_TYPE_CHARACTER>& Value)  { text = Value; }
#   line 15 "quex/code_base/token/CppDefault.qx"
    size_t                                 get_number() const                                       { return number; }
#   line 15 "quex/code_base/token/CppDefault.qx"
    void                                   set_number(size_t Value)                                 { number = Value; }

#   line 87 "CLILine_Lexer-token.hpp"

    void set(const QUEX_TYPE_TOKEN_ID ID) { _id = ID; }
    void set(const QUEX_TYPE_TOKEN_ID ID, const std::basic_string<QUEX_TYPE_CHARACTER>& Value0)
    { _id = ID; text = Value0; }
    void set(const QUEX_TYPE_TOKEN_ID ID, const size_t& Value0)
    { _id = ID; number = Value0; }
    void set(const QUEX_TYPE_TOKEN_ID ID, const std::basic_string<QUEX_TYPE_CHARACTER>& Value0, const size_t& Value1)
    { _id = ID; text = Value0; number = Value1; }


    public:
        /* _line_n, _column_n, and _id are public, so that the token policy 
         * can access it. making it private would imply complications for 
         * user customized token classes. The token policies should be the
         * same for C and Cpp.                                               */
        QUEX_TYPE_TOKEN_ID    _id;
        QUEX_TYPE_TOKEN_ID    type_id() const                             { return _id; }
        const char*           map_id_to_name(QUEX_TYPE_TOKEN_ID ID) const { return QUEX_NAME_TOKEN(map_id_to_name)(ID); }
        const std::string     type_id_name() const                        { return map_id_to_name(_id); }

#   ifdef     QUEX_OPTION_TOKEN_STAMPING_WITH_LINE_AND_COLUMN
#       ifdef QUEX_OPTION_LINE_NUMBER_COUNTING
    public:
        QUEX_TYPE_TOKEN_LINE_N    _line_n;
        QUEX_TYPE_TOKEN_LINE_N    line_number() const                                 { return _line_n; }
        void                      set_line_number(const QUEX_TYPE_TOKEN_LINE_N Value) { _line_n = Value; }
#       endif
#       ifdef  QUEX_OPTION_COLUMN_NUMBER_COUNTING
    public:
        QUEX_TYPE_TOKEN_COLUMN_N  _column_n;
        QUEX_TYPE_TOKEN_COLUMN_N  column_number() const                                   { return _column_n; }
        void                      set_column_number(const QUEX_TYPE_TOKEN_COLUMN_N Value) { _column_n = Value; }
#       endif
#   endif
    public:
#   define self (*this)

#   line 77 "quex/code_base/token/CppDefault.qx"

        typedef std::basic_string<QUEX_TYPE_CHARACTER> __string;

        operator 
        const std::string() const
        { return get_string(); }

        const std::string
        get_string() const 
        {
            std::string             tmp;

            tmp = map_id_to_name(self._id);

            tmp += " '" + pretty_char_text() + "' ";

            return tmp;
        }
    
        const std::string 
        pretty_char_text() const
        /* Provides a somehow pretty-print of the text in the token. */
        {
            std::string             tmp = QUEX_CONVERTER_STRING(identical,char)(self.text);
            std::string::size_type  pos = 0;

            while( (pos = tmp.find("\n") ) != __string::npos ) tmp.replace(pos, (size_t)1, "\\n");
            while( (pos = tmp.find("\t") ) != __string::npos ) tmp.replace(pos, (size_t)1, "\\t");
            while( (pos = tmp.find("\r") ) != __string::npos ) tmp.replace(pos, (size_t)1, "\\r");

            return tmp;
        }

#       if ! defined(__QUEX_OPTION_WCHAR_T_DISABLED)
        const std::wstring 
        pretty_wchar_text() const
        {
            std::wstring             tmp = QUEX_CONVERTER_STRING(identical,wchar)(self.text);
            std::wstring::size_type  pos = 0;

            while( (pos = tmp.find(L"\n") ) != __string::npos ) tmp.replace(pos, (size_t)1, L"\\n");
            while( (pos = tmp.find(L"\t") ) != __string::npos ) tmp.replace(pos, (size_t)1, L"\\t");
            while( (pos = tmp.find(L"\r") ) != __string::npos ) tmp.replace(pos, (size_t)1, L"\\r");

            return tmp;
        }
#       endif
   

#   line 175 "CLILine_Lexer-token.hpp"

#   undef  self

    private:
        friend void QUEX_NAME_TOKEN(copy)(Token* me, const Token* That);
        friend bool QUEX_NAME_TOKEN(take_text)(Token*               __this, 
                                               QUEX_TYPE_ANALYZER*         analyzer, 
                                               const QUEX_TYPE_CHARACTER*  Begin, 
                                               const QUEX_TYPE_CHARACTER*  End);
};

} /* Closing Namespace 'CLILine' */



#   line 129 "quex/code_base/token/CppDefault.qx"

        namespace CLILine {
        inline std::ostream&
        operator<<(std::ostream& ostr, const Token& Tok)
        { ostr << Tok.get_string(); return ostr; }
        } /* Closing Namespace 'CLILine' */

#include <quex/code_base/converter_helper/identity.i>
   

#   line 202 "CLILine_Lexer-token.hpp"




#endif /* __QUEX_INCLUDE_GUARD__TOKEN__GENERATED____CLILINE____TOKEN */

/* -*- C++ -*-   vim: set syntax=cpp: 
 * (C) 2004-2009 Frank-Rene Schaefer
 * ABSOLUTELY NO WARRANTY
 */
#ifndef __QUEX_INCLUDE_GUARD__TOKEN__GENERATED____CLILINE____TOKEN_I
#define __QUEX_INCLUDE_GUARD__TOKEN__GENERATED____CLILINE____TOKEN_I

#include <quex/code_base/definitions>

QUEX_NAMESPACE_LEXEME_NULL_OPEN
extern QUEX_TYPE_CHARACTER   QUEX_LEXEME_NULL_IN_ITS_NAMESPACE;
QUEX_NAMESPACE_LEXEME_NULL_CLOSE


namespace CLILine {


QUEX_INLINE
Token::Token()
{
#   define self (*this)

#   undef  self
}

QUEX_INLINE
Token::Token(const Token& Other)
{
   QUEX_NAME_TOKEN(copy)(this, &Other);
#   define self (*this)

#   undef  self
}

QUEX_INLINE
Token::~Token()
{
#   define self (*this)

#   undef  self
}

QUEX_INLINE void
QUEX_NAME_TOKEN(construct)(Token* __this)
{
    /* Explicit constructor call by 'placement new' */
    new ((void*)__this) Token;
}

QUEX_INLINE void
QUEX_NAME_TOKEN(destruct)(Token* __this)
{
    __this->Token::~Token();  
}

QUEX_INLINE void
QUEX_NAME_TOKEN(copy)(Token* __this, const Token* __That)
{
#   define self  (*__this)
#   define Other (*__That)
    (void)__this;
    (void)__That;

#   line 28 "quex/code_base/token/CppDefault.qx"

        self._id    = Other._id;
        self.text   = Other.text;
        self.number = Other.number;
#       ifdef QUEX_OPTION_TOKEN_STAMPING_WITH_LINE_AND_COLUMN
        __QUEX_IF_COUNT_LINES(self._line_n     = Other._line_n);
        __QUEX_IF_COUNT_COLUMNS(self._column_n = Other._column_n);
#       endif
   

#   line 283 "CLILine_Lexer-token.hpp"

#   undef Other
#   undef self
   /* If the user even misses to copy the token id, then there's
    * something seriously wrong.                                 */
   __quex_assert(__this->_id == __That->_id);
#ifdef     QUEX_OPTION_TOKEN_STAMPING_WITH_LINE_AND_COLUMN
#   ifdef QUEX_OPTION_TOKEN_STAMPING_WITH_LINE_AND_COLUMN
    __QUEX_IF_COUNT_LINES(__quex_assert(__this->_line_n == __That->_line_n));
    __QUEX_IF_COUNT_COLUMNS(__quex_assert(__this->_column_n == __That->_column_n));
#   endif
#endif
}

QUEX_INLINE bool 
QUEX_NAME_TOKEN(take_text)(Token*              __this, 
                           QUEX_TYPE_ANALYZER*        __analyzer, 
                           const QUEX_TYPE_CHARACTER* Begin, 
                           const QUEX_TYPE_CHARACTER* End)
/* RETURNS: true -- if the token claims ownership over the given memory.
 *          false -- if no ownership is claimed.                             */
{
#   define self      (*__this)
#   define analyzer  (*__analyzer)
    (void)__this;
    (void)__analyzer;

#   line 111 "quex/code_base/token/CppDefault.qx"

#       if 0
        {
            /* Hint for debug: To check take_text change "#if 0" to "#if 1". */
            QUEX_TYPE_CHARACTER* it = 0x0;
            printf("%lX ", (long)__this);
            printf("previous:  '");
            for(it = (QUEX_TYPE_CHARACTER*)self.text.c_str(); *it ; ++it) printf("%04X.", (int)*it);
            printf("'\n");
            printf("take_text: '");
            for(it = (QUEX_TYPE_CHARACTER*)Begin; it != End; ++it) printf("%04X.", (int)*it);
            printf("'\n");
        }
#       endif

        /* It cannot be assumed that the lexeme is zero terminated. */
        __quex_assert(End >= Begin);
        self.text.assign(Begin, (size_t)(End-Begin));

#       if 0
        {
            /* Hint for debug: To check take_text change "#if 0" to "#if 1". */
            QUEX_TYPE_CHARACTER* it = 0x0;
            printf("after:     '");
            for(it = (QUEX_TYPE_CHARACTER*)self.text.c_str(); *it ; ++it) printf("%04X.", (int)*it);
            printf("'\n");
        }
#       endif

        /* This token copied the text from the chunk into the string, 
         * so we do not claim owneship over it.                       */
        return false;      
   

#   line 346 "CLILine_Lexer-token.hpp"

#   undef analyzer
#   undef self
    /* Default: no ownership.                                                */
    return false;
}

#ifdef QUEX_OPTION_TOKEN_REPETITION_SUPPORT
QUEX_INLINE size_t 
QUEX_NAME_TOKEN(repetition_n_get)(Token* __this)
{
#   define self (*__this)
    (void)__this;
    
#   line 119 "quex/code_base/token/CppDefault.qx"

       return self.number;
   

#   line 366 "CLILine_Lexer-token.hpp"

#   undef self
}

QUEX_INLINE void 
QUEX_NAME_TOKEN(repetition_n_set)(Token* __this, size_t N)
{
#   define self (*__this)
    (void)__this;
    (void)N;
    
#   line 115 "quex/code_base/token/CppDefault.qx"

       self.number = N;
   

#   line 383 "CLILine_Lexer-token.hpp"

#   undef  self
}
#endif /* QUEX_OPTION_TOKEN_REPETITION_SUPPORT */

} /* Closing Namespace 'CLILine' */




#endif /* __QUEX_INCLUDE_GUARD__TOKEN__GENERATED____CLILINE____TOKEN_I */
