/* -*- C++ -*- vim:set syntax=cpp:
 * (C)  Frank-Rene Schaefer
 * ABSOLUTELY NO WARRANTY                                                    */
#ifndef __QUEX_INCLUDE_GUARD__ANALYZER__STRUCT__RESET_I
#define __QUEX_INCLUDE_GUARD__ANALYZER__STRUCT__RESET_I

#include <quex/code_base/buffer/Buffer.i>
#include <quex/code_base/analyzer/struct/reset>

QUEX_NAMESPACE_MAIN_OPEN

/* Level (0) __________________________________________________________________
 *                                                                           */
QUEX_INLINE void
QUEX_MEMBER_FUNCTIONO(reset)  
{
    QUEX_MAP_THIS_TO_ME(QUEX_TYPE_ANALYZER)
    QUEX_NAME(BufferFiller)* filler = me->buffer.filler;

    if( filler ) {
        QUEX_NAME(BufferFiller_reset)(filler, filler->byte_loader);
    }

    QUEX_MEMBER_FUNCTION_CALL1(reset, BufferFiller, filler);
}

/* Level (1) __________________________________________________________________
 *                                                                           */
QUEX_INLINE void
QUEX_MEMBER_FUNCTION2(reset, file_name, 
                      const char* FileName, 
                      const char* CodecName /* = 0x0 */) 
{
    QUEX_MAP_THIS_TO_ME(QUEX_TYPE_ANALYZER)
    QUEX_NAME(ByteLoader)*   byte_loader;

    byte_loader = QUEX_NAME(ByteLoader_FILE_new_from_file_name)(FileName);
    /* NOT: Abort/return if byte_loader == 0 !!
     *      Incomplete construction => propper destruction IMPOSSIBLE!       */
    if( byte_loader ) {
        byte_loader->ownership = E_Ownership_LEXICAL_ANALYZER;
    }
    QUEX_MEMBER_FUNCTION_CALL2(reset, ByteLoader, byte_loader, CodecName); 
}

/* Level (2) __________________________________________________________________
 *                                                                           */
QUEX_INLINE void
QUEX_MEMBER_FUNCTION3(reset, FILE,
                      __QUEX_STD_FILE* fh, 
                      const char*      CodecName /* = 0x0   */,
                      bool             BinaryModeF)
{
    QUEX_MAP_THIS_TO_ME(QUEX_TYPE_ANALYZER)
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
    QUEX_MEMBER_FUNCTION_CALL2(reset, ByteLoader, byte_loader, CodecName); 
}

#ifndef __QUEX_OPTION_PLAIN_C
QUEX_INLINE void
QUEX_MEMBER_FUNCTION2(reset, istream,
                      std::istream*   istream_p, 
                      const char*     CodecName /* = 0x0   */)
{
    QUEX_MAP_THIS_TO_ME(QUEX_TYPE_ANALYZER)
    QUEX_NAME(ByteLoader)*   byte_loader;
    __quex_assert( istream_p );

    byte_loader = QUEX_NAME(ByteLoader_stream_new)(istream_p);
    /* NOT: Abort/return if byte_loader == 0 !!
     *      Incomplete construction => propper destruction IMPOSSIBLE!       */
    if( byte_loader ) {
        byte_loader->ownership = E_Ownership_LEXICAL_ANALYZER;
    }
    QUEX_MEMBER_FUNCTION_CALL2(reset, ByteLoader, byte_loader, CodecName); 
}
#endif


#if defined(__QUEX_OPTION_WCHAR_T) && ! defined(__QUEX_OPTION_PLAIN_C)
QUEX_INLINE void
QUEX_MEMBER_FUNCTION2(reset, wistream,
                      std::wistream*  istream_p, 
                      const char*     CodecName /* = 0x0   */)
{
    QUEX_MAP_THIS_TO_ME(QUEX_TYPE_ANALYZER)
    QUEX_NAME(ByteLoader)*   byte_loader;
    __quex_assert( istream_p );
    byte_loader = QUEX_NAME(ByteLoader_stream_new)(istream_p);
    /* NOT: Abort/return if byte_loader == 0 !!
     *      Incomplete construction => propper destruction IMPOSSIBLE!       */
    if( byte_loader ) {
        byte_loader->ownership = E_Ownership_LEXICAL_ANALYZER;
    }
    QUEX_MEMBER_FUNCTION_CALL2(reset, ByteLoader, byte_loader, CodecName); 
}
#endif

#if defined(__QUEX_OPTION_UNIT_TEST) && ! defined (__QUEX_OPTION_PLAIN_C)
/* StrangeStreams are not for C-language stuff */
template<class UnderlyingStreamT> QUEX_INLINE void
QUEX_MEMBER_FUNCTION2(reset_StrangeStream, strange_stream, 
                      quex::StrangeStream<UnderlyingStreamT>*  istream_p, 
                      const char*                              CodecName /* = 0x0   */)
{
    QUEX_MAP_THIS_TO_ME(QUEX_TYPE_ANALYZER)
    QUEX_NAME(ByteLoader)*   byte_loader;
    __quex_assert( istream_p );
    byte_loader = QUEX_NAME(ByteLoader_stream_new)(istream_p);
    /* NOT: Abort/return if byte_loader == 0 !!
     *      Incomplete construction => propper destruction IMPOSSIBLE!       */
    if( byte_loader ) {
        byte_loader->ownership = E_Ownership_LEXICAL_ANALYZER;
    }
    QUEX_MEMBER_FUNCTION_CALL2(reset, ByteLoader, byte_loader, CodecName); 
}
#endif


/* Level (3) __________________________________________________________________
 *                                                                           */
QUEX_INLINE void
QUEX_MEMBER_FUNCTION2(reset, ByteLoader,
                      QUEX_NAME(ByteLoader)*  byte_loader,
                      const char*             CodecName) 
{
    QUEX_MAP_THIS_TO_ME(QUEX_TYPE_ANALYZER)
    QUEX_NAME(BufferFiller)* filler = me->buffer.filler;
    QUEX_NAME(Asserts_construct)(CodecName);
    
    if( filler ) {
        QUEX_NAME(BufferFiller_reset)(filler, byte_loader);
    }
    else {
        filler = QUEX_NAME(BufferFiller_new_DEFAULT)(byte_loader, CodecName);
        /* NOT: Abort/return if filler == 0 !!
         *      Incomplete construction => propper destruction IMPOSSIBLE!   */
        if( filler ) {
            filler->ownership = E_Ownership_LEXICAL_ANALYZER;
        }
    }

    QUEX_MEMBER_FUNCTION_CALL1(reset, BufferFiller, filler);
}

/* Level (4) __________________________________________________________________
 *                                                                           */
QUEX_INLINE void
QUEX_MEMBER_FUNCTION1(reset, BufferFiller,
                      QUEX_NAME(BufferFiller)* filler)
{
    QUEX_MAP_THIS_TO_ME(QUEX_TYPE_ANALYZER)
    if( filler != me->buffer.filler ) {
        if( me->buffer.filler && me->buffer.filler->ownership == E_Ownership_LEXICAL_ANALYZER ) {
            me->buffer.filler->delete_self(me->buffer.filler); 
        }
        me->buffer.filler = filler;
    }
    else {
        /* Assume, that buffer filler has been reset.                        */
    }
    QUEX_NAME(Buffer_init_content)(&me->buffer, (QUEX_TYPE_CHARACTER*)0);
    QUEX_NAME(Buffer_init_analyzis)(&me->buffer); 
    QUEX_MEMBER_FUNCTION_CALLO(basic_reset);
}

/* Level (5) __________________________________________________________________
 *                                                                           */
QUEX_INLINE QUEX_TYPE_CHARACTER*
QUEX_MEMBER_FUNCTION3(reset, memory,
                      QUEX_TYPE_CHARACTER*    Memory,
                      const size_t            MemorySize,
                      QUEX_TYPE_CHARACTER*    EndOfFileP)
/* When memory is provided from extern, the 'external entity' is
 * responsible for filling it. There is no 'file/stream handle', no 'byte
 * loader', and 'no buffer filler'.                                          
 *
 * RETURN: != 0, previous buffer memory, in case that the user is responsible
 *               for deleting it.
 *         == 0  if the previous memory was owned by the lexical analyzer and
 *               accordingly, it deleted it itself.                          */
{
    QUEX_MAP_THIS_TO_ME(QUEX_TYPE_ANALYZER)
    QUEX_TYPE_CHARACTER*       previous_buffer_memory;
    __quex_assert((! Memory) || (EndOfFileP > Memory && EndOfFileP <= &Memory[MemorySize]));
    __quex_assert((  Memory) || (MemorySize == 0     && EndOfFileP == (QUEX_TYPE_CHARACTER*)0)); 

    QUEX_NAME(Buffer_destruct)(&me->buffer); 
    /* In case, that the memory was owned by the analyzer, the destructor did
     * not delete it and did not set 'me->buffer._memory._front' to zero.    */
    previous_buffer_memory = me->buffer._memory._front;
    QUEX_NAME(Buffer_construct)(&me->buffer, 
                                (QUEX_NAME(BufferFiller)*)0,
                                Memory, MemorySize, EndOfFileP,
                                E_Ownership_EXTERNAL);
    QUEX_MEMBER_FUNCTION_CALLO(basic_reset);

    return previous_buffer_memory;
}

QUEX_INLINE void
QUEX_MEMBER_FUNCTIONO(basic_reset)
{
    QUEX_MAP_THIS_TO_ME(QUEX_TYPE_ANALYZER)
    bool  byte_order_reversion_f = me->buffer.filler ? 
                                     me->buffer.filler->_byte_order_reversion_active_f
                                   : false;
    QUEX_NAME(Tokens_destruct)(me);
    QUEX_NAME(Tokens_construct)(me);

    QUEX_NAME(ModeStack_construct)(me);

    __QUEX_IF_INCLUDE_STACK(     QUEX_MEMBER_FUNCTION_CALLO(include_stack_delete));

    __QUEX_IF_STRING_ACCUMULATOR(QUEX_NAME(Accumulator_destruct)(&me->accumulator));
    __QUEX_IF_STRING_ACCUMULATOR(QUEX_NAME(Accumulator_construct)(&me->accumulator, me));

    __QUEX_IF_POST_CATEGORIZER(  QUEX_NAME(PostCategorizer_destruct)(&me->post_categorizer));
    __QUEX_IF_POST_CATEGORIZER(  QUEX_NAME(PostCategorizer_construct)(&me->post_categorizer));

    __QUEX_IF_COUNT(             QUEX_NAME(Counter_construct)(&me->counter); )

    QUEX_NAME(set_mode_brutally_by_id)(me, __QUEX_SETTING_INITIAL_LEXER_MODE_ID);

    if( me->buffer.filler && byte_order_reversion_f )
    {
        me->buffer.filler->_byte_order_reversion_active_f = true;
    }
    QUEX_MEMBER_FUNCTION_CALLO(user_reset);
}


QUEX_NAMESPACE_MAIN_CLOSE

#endif /* __QUEX_INCLUDE_GUARD__ANALYZER__STRUCT__RESET_I */
