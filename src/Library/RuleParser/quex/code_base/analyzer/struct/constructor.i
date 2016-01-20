/* -*- C++ -*- vim:set syntax=cpp:
 * (C) 2005-2015 Frank-Rene Schaefer
 * ABSOLUTELY NO WARRANTY                                                    */
#ifndef  __QUEX_INCLUDE_GUARD__ANALYZER__STRUCT__CONSTRUCTOR_I
#define  __QUEX_INCLUDE_GUARD__ANALYZER__STRUCT__CONSTRUCTOR_I

#include <quex/code_base/buffer/Buffer.i>
#include <quex/code_base/buffer/filler/BufferFiller.i>
#include <quex/code_base/analyzer/struct/include-stack>

QUEX_NAMESPACE_MAIN_OPEN
                    
QUEX_INLINE void   QUEX_NAME(Asserts_user_memory)(QUEX_TYPE_ANALYZER*  me,
                             QUEX_TYPE_CHARACTER* BufferMemoryBegin, 
                             size_t               BufferMemorySize,
                             QUEX_TYPE_CHARACTER* BufferEndOfContentP  /* = 0x0   */);
QUEX_INLINE void   QUEX_NAME(Asserts_construct)(const char* CodecName);
QUEX_INLINE void   QUEX_NAME(Tokens_construct)(QUEX_TYPE_ANALYZER* me);
QUEX_INLINE void   QUEX_NAME(Tokens_reset)(QUEX_TYPE_ANALYZER* me);
QUEX_INLINE void   QUEX_NAME(Tokens_destruct)(QUEX_TYPE_ANALYZER* me);
QUEX_INLINE void   QUEX_NAME(ModeStack_construct)(QUEX_TYPE_ANALYZER* me);


/* Level (1) __________________________________________________________________
 *                                                                           */
QUEX_INLINE void
QUEX_MEMBER_FUNCTION2(from, file_name,
                      const char*  FileName, 
                      const char*  CodecName)
{
    QUEX_NAME(ByteLoader)*   byte_loader;

    byte_loader = QUEX_NAME(ByteLoader_FILE_new_from_file_name)(FileName);
    /* NOT: Abort/return if byte_loader == 0 !!
     *      Incomplete construction => propper destruction IMPOSSIBLE!       */
    if( byte_loader ) {
        byte_loader->ownership = E_Ownership_LEXICAL_ANALYZER;
    }
    QUEX_MEMBER_FUNCTION_CALL2(from, ByteLoader, byte_loader, CodecName); 
}

/* Level (2) __________________________________________________________________
 *                                                                           */
QUEX_INLINE void
QUEX_MEMBER_FUNCTION3(from, FILE,
                      __QUEX_STD_FILE*  fh, 
                      const char*       CodecName /* = 0x0   */,
                      bool              BinaryModeF)
/* 'BinaryModeF' tells whether the file has been opened in 'binary mode'.    */
{
    QUEX_NAME(ByteLoader)*   byte_loader;
    __quex_assert( fh );

    /* At the time of this writing 'stdin' as located in the C++ global
     * namespace.  This seemed suspicous to the author. To avoid compilation
     * errors in the future the test for the standard input is only active in
     * 'C'. It is only about user information anyway. So better no risks taken.
     * <fschaef 2010y02m06d>                                                 */
    setbuf(fh, 0);   /* turn off system based buffering! 
    **               ** this is essential to profit from the quex buffer!    */
    byte_loader = QUEX_NAME(ByteLoader_FILE_new)(fh, BinaryModeF);
    /* NOT: Abort/return if byte_loader == 0 !!
     *      Incomplete construction => propper destruction IMPOSSIBLE!       */
    if( byte_loader ) {
        byte_loader->ownership = E_Ownership_LEXICAL_ANALYZER;
    }
    QUEX_MEMBER_FUNCTION_CALL2(from, ByteLoader, byte_loader, CodecName); 
}

#ifndef __QUEX_OPTION_PLAIN_C
QUEX_INLINE void
QUEX_MEMBER_FUNCTION2(from, istream,
                      std::istream*   istream_p, 
                      const char*     CodecName /* = 0x0   */)
{
    QUEX_NAME(ByteLoader)*   byte_loader;
    __quex_assert( istream_p );

    byte_loader = QUEX_NAME(ByteLoader_stream_new)(istream_p);
    /* NOT: Abort/return if byte_loader == 0 !!
     *      Incomplete construction => propper destruction IMPOSSIBLE!       */
    if( byte_loader ) {
        byte_loader->ownership = E_Ownership_LEXICAL_ANALYZER;
    }
    QUEX_MEMBER_FUNCTION_CALL2(from, ByteLoader, byte_loader, CodecName); 
}
#endif


#if defined(__QUEX_OPTION_WCHAR_T) && ! defined(__QUEX_OPTION_PLAIN_C)
QUEX_INLINE void 
QUEX_MEMBER_FUNCTION2(from, wistream,
                      std::wistream*  istream_p, 
                      const char*     CodecName /* = 0x0   */)
{
    QUEX_NAME(ByteLoader)*   byte_loader;
    __quex_assert( istream_p );

    byte_loader = QUEX_NAME(ByteLoader_stream_new)(istream_p);
    /* NOT: Abort/return if byte_loader == 0 !!
     *      Incomplete construction => propper destruction IMPOSSIBLE!       */
    if( byte_loader ) {
        byte_loader->ownership = E_Ownership_LEXICAL_ANALYZER;
    }
    byte_loader->ownership = E_Ownership_LEXICAL_ANALYZER;
    QUEX_MEMBER_FUNCTION_CALL2(from, ByteLoader, byte_loader, CodecName); 
}
#endif

#if defined(__QUEX_OPTION_UNIT_TEST) && ! defined (__QUEX_OPTION_PLAIN_C)
/* StrangeStreams are not for C-language stuff */
template<class UnderlyingStreamT> QUEX_INLINE void
QUEX_MEMBER_FUNCTION2(from_StrangeStream, strange_stream, 
                      quex::StrangeStream<UnderlyingStreamT>*  istream_p, 
                      const char*                              CodecName /* = 0x0   */)
{
    QUEX_NAME(ByteLoader)*   byte_loader;
    __quex_assert( istream_p );

    byte_loader = QUEX_NAME(ByteLoader_stream_new)(istream_p);
    /* NOT: Abort/return if byte_loader == 0 !!
     *      Incomplete construction => propper destruction IMPOSSIBLE!       */
    if( byte_loader ) {
        byte_loader->ownership = E_Ownership_LEXICAL_ANALYZER;
    }

    byte_loader->ownership = E_Ownership_LEXICAL_ANALYZER;
    QUEX_MEMBER_FUNCTION_CALL2(from, ByteLoader, byte_loader, CodecName); 
}
#endif


/* Level (3) __________________________________________________________________
 *                                                                           */
QUEX_INLINE void
QUEX_MEMBER_FUNCTION2(from, ByteLoader,
                      QUEX_NAME(ByteLoader)*   byte_loader,
                      const char*   CodecName) 
{
    QUEX_NAME(BufferFiller)* filler;
    QUEX_NAME(Asserts_construct)(CodecName);

    filler = QUEX_NAME(BufferFiller_new_DEFAULT)(byte_loader, CodecName);
    /* NOT: Abort/return if filler == 0 !!
     *      Incomplete construction => propper destruction IMPOSSIBLE!       */
    if( filler ) {
        filler->ownership = E_Ownership_LEXICAL_ANALYZER;
    }

    QUEX_MEMBER_FUNCTION_CALL1(from, BufferFiller, filler);
}

/* Level (4) __________________________________________________________________
 *                                                                           */
QUEX_INLINE void
QUEX_MEMBER_FUNCTION1(from, BufferFiller,
                      QUEX_NAME(BufferFiller)* filler)
{
    QUEX_MAP_THIS_TO_ME(QUEX_TYPE_ANALYZER)
    QUEX_TYPE_CHARACTER* memory;

    memory = (QUEX_TYPE_CHARACTER*)QUEXED(MemoryManager_allocate)(
                       QUEX_SETTING_BUFFER_SIZE * sizeof(QUEX_TYPE_CHARACTER), 
                       E_MemoryObjectType_BUFFER_MEMORY);
    /* NOT: Abort/return if memory == 0 !!
     *      Incomplete construction => propper destruction IMPOSSIBLE!       */

    QUEX_NAME(Buffer_construct)(&me->buffer, filler,
                                memory, QUEX_SETTING_BUFFER_SIZE, 
                                (QUEX_TYPE_CHARACTER*)0,
                                E_Ownership_LEXICAL_ANALYZER);
    QUEX_MEMBER_FUNCTION_CALLO(basic_constructor);
}

/* Level (5) __________________________________________________________________
 *                                                                           */
QUEX_INLINE void
QUEX_MEMBER_FUNCTION3(from, memory,
                      QUEX_TYPE_CHARACTER*    Memory,
                      const size_t            MemorySize,
                      QUEX_TYPE_CHARACTER*    EndOfFileP)

/* When memory is provided from extern, the 'external entity' is responsible
 * for filling it. There is no 'file/stream handle', no 'byte loader', and 'no
 * buffer filler'.                                                           */
{
    QUEX_MAP_THIS_TO_ME(QUEX_TYPE_ANALYZER)
    __quex_assert((! Memory) || (EndOfFileP > Memory && EndOfFileP <= &Memory[MemorySize]));
    __quex_assert((  Memory) || (MemorySize == 0     && EndOfFileP == (QUEX_TYPE_CHARACTER*)0)); 

    QUEX_NAME(Buffer_construct)(&me->buffer, 
                                (QUEX_NAME(BufferFiller)*)0,
                                Memory, MemorySize, EndOfFileP,
                                E_Ownership_EXTERNAL);
    QUEX_MEMBER_FUNCTION_CALLO(basic_constructor);
}

QUEX_INLINE void
QUEX_MEMBER_FUNCTIONO(basic_constructor)
{
    QUEX_MAP_THIS_TO_ME(QUEX_TYPE_ANALYZER)
    QUEX_NAME(Tokens_construct)(me);
    QUEX_NAME(ModeStack_construct)(me);
    __QUEX_IF_INCLUDE_STACK(     me->_parent_memento = (QUEX_NAME(Memento)*)0);
    __QUEX_IF_STRING_ACCUMULATOR(QUEX_NAME(Accumulator_construct)(&me->accumulator, me));
    __QUEX_IF_POST_CATEGORIZER(  QUEX_NAME(PostCategorizer_construct)(&me->post_categorizer));
    __QUEX_IF_COUNT(             QUEX_NAME(Counter_construct)(&me->counter); )

    /* A user's mode change callbacks may be called as a consequence of the 
     * call to 'set_mode_brutally_by_id()'. The current mode must be set to '0'
     * so that the user may detect whether this is the first mode transition.*/
    me->__current_mode_p = (QUEX_NAME(Mode)*)0;
    QUEX_NAME(set_mode_brutally_by_id)(me, __QUEX_SETTING_INITIAL_LEXER_MODE_ID);

    QUEX_MEMBER_FUNCTION_CALLO(user_constructor);
}

QUEX_INLINE 
QUEX_DESTRUCTOR() 
{
    QUEX_MAP_THIS_TO_ME(QUEX_TYPE_ANALYZER)
    QUEX_NAME(Tokens_destruct)(me);

    __QUEX_IF_INCLUDE_STACK(QUEX_MEMBER_FUNCTION_CALLO(include_stack_delete));
    /* IMPORTANT: THE ACCUMULATOR CAN ONLY BE DESTRUCTED AFTER THE INCLUDE 
     *            STACK HAS BEEN DELETED. OTHERWISE, THERE MIGHT BE LEAKS. 
     * TODO: Why? I cannot see a reason <fschaef 15y08m03d>                  */
    __QUEX_IF_STRING_ACCUMULATOR( QUEX_NAME(Accumulator_destruct)(&me->accumulator));
    __QUEX_IF_POST_CATEGORIZER(   QUEX_NAME(PostCategorizer_destruct)(&me->post_categorizer));

    QUEX_NAME(Buffer_destruct)(&me->buffer);
}

QUEX_INLINE void
QUEX_NAME(Asserts_user_memory)(QUEX_TYPE_ANALYZER*  me,
                               QUEX_TYPE_CHARACTER* BufferMemoryBegin, 
                               size_t               BufferMemorySize,
                               QUEX_TYPE_CHARACTER* BufferEndOfContentP  /* = 0x0   */)
{
#   ifdef QUEX_OPTION_ASSERTS
    size_t  memory_size = BufferMemoryBegin ? BufferMemorySize 
                          :                   QUEX_SETTING_BUFFER_SIZE;
    QUEX_TYPE_CHARACTER*   iterator = 0x0;

    __quex_assert(memory_size == 0 || memory_size > 2);
    if( BufferMemoryBegin ) {
        /* End of File must be inside the buffer, because we assume that the buffer
         * contains all that is required.                                           */
        if( BufferMemorySize <= QUEX_SETTING_BUFFER_MIN_FALLBACK_N + 2) {
            QUEX_ERROR_EXIT("\nConstructor: Provided memory size must be more than 2 greater than\n"
                            "Constructor: QUEX_SETTING_BUFFER_MIN_FALLBACK_N. If in doubt, specify\n"
                            "Constructor: -DQUEX_SETTING_BUFFER_MIN_FALLBACK_N=0 as compile option.\n");
        }
        if(    BufferEndOfContentP < BufferMemoryBegin 
            || BufferEndOfContentP > (BufferMemoryBegin + BufferMemorySize - 1)) {
            QUEX_ERROR_EXIT("\nConstructor: Argument 'BufferEndOfContentP' must be inside the provided memory\n"
                            "Constructor: buffer (speficied by 'BufferMemoryBegin' and 'BufferMemorySize').\n"
                            "Constructor: Note, that the last element of the buffer is to be filled with\n"
                            "Constructor: the buffer limit code character.\n");
        }
    }
    if( BufferEndOfContentP ) {
        __quex_assert(BufferEndOfContentP >  BufferMemoryBegin);
        __quex_assert(BufferEndOfContentP <= BufferMemoryBegin + memory_size - 1);

        /* The memory provided must be initialized. If it is not, then that's wrong.
         * Try to detect me by searching for BLC and PTC.                         */
        for(iterator = BufferMemoryBegin + 1; iterator != BufferEndOfContentP; ++iterator) {
            if(    *iterator == QUEX_SETTING_BUFFER_LIMIT_CODE 
                || *iterator == QUEX_SETTING_PATH_TERMINATION_CODE ) {
                QUEX_ERROR_EXIT("\nConstructor: Buffer limit code and/or path termination code appeared in buffer\n"
                                "Constructor: when pointed to user memory. Note, that the memory pointed to must\n"
                                "Constructor: be initialized! You might redefine QUEX_SETTING_PATH_TERMINATION_CODE\n"
                                "Constructor: and/or QUEX_SETTING_PATH_TERMINATION_CODE; or use command line arguments\n"
                                "Constructor: '--buffer-limit' and '--path-termination'.");
            }
        }
    }
#   endif

    /* NOT: before ifdef, otherwise c90 issue: mixed declarations and code   */
    (void)me; (void)BufferMemoryBegin; (void)BufferMemorySize; (void)BufferEndOfContentP;
}

/* AUXILIARY FUNCTIONS FOR CONSTRUCTION _______________________________________                                     
 *                                                                           */

QUEX_INLINE void
QUEX_NAME(Asserts_construct)(const char* CodecName)
{
    (void)CodecName;

#   if      defined(QUEX_OPTION_ASSERTS) \
       && ! defined(QUEX_OPTION_ASSERTS_WARNING_MESSAGE_DISABLED)
    __QUEX_STD_printf(__QUEX_MESSAGE_ASSERTS_INFO);
#   endif

#   if defined(QUEX_OPTION_ASSERTS) 
    if( QUEX_SETTING_BUFFER_LIMIT_CODE == QUEX_SETTING_PATH_TERMINATION_CODE ) {
        QUEX_ERROR_EXIT("Path termination code (PTC) and buffer limit code (BLC) must be different.\n");
    }
#   endif

#   if defined(__QUEX_OPTION_ENGINE_RUNNING_ON_CODEC)
    if( CodecName ) {
        __QUEX_STD_printf(__QUEX_MESSAGE_CHARACTER_ENCODING_SPECIFIED_WITHOUT_CONVERTER, CodecName);
    }
#   endif
}

#if ! defined(QUEX_TYPE_TOKEN)
#      error "QUEX_TYPE_TOKEN must be defined before inclusion of this file."
#endif
QUEX_INLINE void
QUEX_NAME(Tokens_construct)(QUEX_TYPE_ANALYZER* me)
{
#if defined(QUEX_OPTION_TOKEN_POLICY_QUEUE)
#   if defined(QUEX_OPTION_USER_MANAGED_TOKEN_MEMORY)
    /* Assume that the user will pass us a constructed token queue */
    QUEX_NAME(TokenQueue_init)(&me->_token_queue, 0, 0x0);
#   else
    QUEX_NAME(TokenQueue_construct)(&me->_token_queue, 
                                    (QUEX_TYPE_TOKEN*)&me->__memory_token_queue,
                                    QUEX_SETTING_TOKEN_QUEUE_SIZE);
#   endif
#elif defined(QUEX_OPTION_USER_MANAGED_TOKEN_MEMORY)
    /* Assume that the user will pass us a constructed token */
    me->token = (QUEX_TYPE_TOKEN*)0x0;     
#   else
    me->token = &me->__memory_token;     
#   ifdef __QUEX_OPTION_PLAIN_C
    QUEX_NAME_TOKEN(construct)(me->token);
#   endif
#endif
}

QUEX_INLINE void
QUEX_NAME(Tokens_destruct)(QUEX_TYPE_ANALYZER* me)
{
    /* Even if the token memory is user managed, the destruction (not the
     * freeing of memory) must happen at this place.                     */
#ifdef QUEX_OPTION_TOKEN_POLICY_QUEUE 
    QUEX_NAME(TokenQueue_destruct)(&me->_token_queue);
#else
#   if      defined(__QUEX_OPTION_PLAIN_C) \
       && ! defined(QUEX_OPTION_USER_MANAGED_TOKEN_MEMORY)
        QUEX_NAME_TOKEN(destruct)(me->token);
#   endif
#endif
}

QUEX_INLINE void 
QUEX_NAME(Tokens_reset)(QUEX_TYPE_ANALYZER* me)
{
#ifdef QUEX_OPTION_TOKEN_POLICY_QUEUE
    QUEX_NAME(TokenQueue_reset)(&me->_token_queue);
#else
    QUEX_NAME(Tokens_destruct(me));
    QUEX_NAME(Tokens_construct(me));
#endif
}

QUEX_INLINE void
QUEX_NAME(ModeStack_construct)(QUEX_TYPE_ANALYZER* me)
{
    me->_mode_stack.end        = me->_mode_stack.begin;
    me->_mode_stack.memory_end = &me->_mode_stack.begin[QUEX_SETTING_MODE_STACK_SIZE];
}


QUEX_NAMESPACE_MAIN_CLOSE

#endif /*  __QUEX_INCLUDE_GUARD__ANALYZER__STRUCT__CONSTRUCTOR_I */
