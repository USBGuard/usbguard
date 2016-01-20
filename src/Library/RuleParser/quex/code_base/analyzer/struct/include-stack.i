/* -*- C++ -*- vim:set syntax=cpp:
 *
 * (C) 2004-2009 Frank-Rene Schaefer
 *
 *  __QUEX_INCLUDE_GUARD__ANALYZER__STRUCT__INCLUDE_STACK_I may be undefined in case
 *    that multiple lexical analyzers are used. Then, the name of the
 *    QUEX_NAME(Accumulator) must be different.                             */
#ifndef  __QUEX_INCLUDE_GUARD__ANALYZER__STRUCT__INCLUDE_STACK_I
#define  __QUEX_INCLUDE_GUARD__ANALYZER__STRUCT__INCLUDE_STACK_I

#ifndef   QUEX_TYPE_ANALYZER
#   error "Macro QUEX_TYPE_ANALYZER must be defined before inclusion of this file."
#endif


QUEX_NAMESPACE_MAIN_OPEN

/* Level (1) __________________________________________________________________
 *                                                                           */
QUEX_INLINE bool
QUEX_MEMBER_FUNCTION2(include_push, file_name, 
                      const char* FileName, 
                      const char* CodecName /* = 0x0 */) 
{
    QUEX_MAP_THIS_TO_ME(QUEX_TYPE_ANALYZER)
    bool          verdict_f;
    QUEX_NAME(ByteLoader)*   byte_loader;

    byte_loader = QUEX_NAME(ByteLoader_FILE_new_from_file_name)(FileName);
    if( ! byte_loader ) {
        return false;
    }
    byte_loader->ownership = E_Ownership_LEXICAL_ANALYZER;
    verdict_f = QUEX_MEMBER_FUNCTION_CALL3(include_push, ByteLoader, FileName, byte_loader, CodecName); 
    if( ! verdict_f ) {
        QUEX_NAME(ByteLoader_delete)(&byte_loader);
    }
    return verdict_f;
}

/* Level (2) __________________________________________________________________
 *                                                                           */
QUEX_INLINE bool
QUEX_MEMBER_FUNCTION4(include_push, FILE,
                      const char*       InputName,
                      __QUEX_STD_FILE*  fh, 
                      const char*       CodecName /* = 0x0   */,
                      bool              BinaryModeF)
{
    QUEX_MAP_THIS_TO_ME(QUEX_TYPE_ANALYZER)
    bool          verdict_f;
    QUEX_NAME(ByteLoader)*   byte_loader;
    __quex_assert( fh );

    /* At the time of this writing 'stdin' as located in the C++ global namespace. 
     * This seemed suspicous to the author. To avoid compilation errors in the future
     * the test for the standard input is only active in 'C'. It is only about
     * user information anyway. So better no risks taken.      <fschaef 2010y02m06d> */
    setbuf(fh, 0);   /* turn off system based buffering! 
    **               ** this is essential to profit from the quex buffer! */
    byte_loader = QUEX_NAME(ByteLoader_FILE_new)(fh, BinaryModeF);
    /* NOT: Abort/return if byte_loader == 0 !!
     *      Incomplete construction => propper destruction IMPOSSIBLE!       */
    if( ! byte_loader ) {
        return false;
    }
    byte_loader->ownership = E_Ownership_LEXICAL_ANALYZER;
    verdict_f = QUEX_MEMBER_FUNCTION_CALL3(include_push, ByteLoader, InputName, byte_loader, CodecName); 
    if( ! verdict_f ) {
        QUEX_NAME(ByteLoader_delete)(&byte_loader);
    }
    return verdict_f;
}

#ifndef __QUEX_OPTION_PLAIN_C
QUEX_INLINE bool
QUEX_MEMBER_FUNCTION3(include_push, istream,
                      const char*    InputName,
                      std::istream*  istream_p, 
                      const char*    CodecName /* = 0x0   */)
{
    QUEX_MAP_THIS_TO_ME(QUEX_TYPE_ANALYZER)
    bool          verdict_f;
    QUEX_NAME(ByteLoader)*   byte_loader;
    __quex_assert( istream_p );

    byte_loader = QUEX_NAME(ByteLoader_stream_new)(istream_p);
    /* NOT: Abort/return if byte_loader == 0 !!
     *      Incomplete construction => propper destruction IMPOSSIBLE!       */
    if( ! byte_loader ) {
        return false;
    }
    byte_loader->ownership = E_Ownership_LEXICAL_ANALYZER;
    verdict_f = QUEX_MEMBER_FUNCTION_CALL3(include_push, ByteLoader, InputName, byte_loader, CodecName); 
    if( ! verdict_f ) {
        QUEX_NAME(ByteLoader_delete)(&byte_loader);
    }
    return verdict_f;
}
#endif


#if defined(__QUEX_OPTION_WCHAR_T) && ! defined(__QUEX_OPTION_PLAIN_C)
QUEX_INLINE bool
QUEX_MEMBER_FUNCTION3(include_push, wistream,
                      const char*     InputName,
                      std::wistream*  istream_p, 
                      const char*     CodecName /* = 0x0   */)
{
    QUEX_MAP_THIS_TO_ME(QUEX_TYPE_ANALYZER)
    bool          verdict_f;
    QUEX_NAME(ByteLoader)*   byte_loader;
    __quex_assert( istream_p );

    byte_loader = QUEX_NAME(ByteLoader_stream_new)(istream_p);
    /* NOT: Abort/return if byte_loader == 0 !!
     *      Incomplete construction => propper destruction IMPOSSIBLE!       */
    if( ! byte_loader ) {
        return false;
    }
    byte_loader->ownership = E_Ownership_LEXICAL_ANALYZER;
    verdict_f = QUEX_MEMBER_FUNCTION_CALL3(include_push, ByteLoader, InputName, byte_loader, CodecName); 
    if( ! verdict_f ) {
        QUEX_NAME(ByteLoader_delete)(&byte_loader);
    }
    return verdict_f;
}
#endif

#if defined(__QUEX_OPTION_UNIT_TEST) && ! defined (__QUEX_OPTION_PLAIN_C)
/* StrangeStreams are not for C-language stuff */
template<class UnderlyingStreamT> QUEX_INLINE bool
QUEX_MEMBER_FUNCTION3(include_push, strange_stream, 
                      const char*                              InputName,
                      quex::StrangeStream<UnderlyingStreamT>*  istream_p, 
                      const char*                              CodecName /* = 0x0   */)
{
    QUEX_MAP_THIS_TO_ME(QUEX_TYPE_ANALYZER)
    bool          verdict_f;
    QUEX_NAME(ByteLoader)*   byte_loader;
    __quex_assert( istream_p );

    byte_loader = QUEX_NAME(ByteLoader_stream_new)(istream_p);
    /* NOT: Abort/return if byte_loader == 0 !!
     *      Incomplete construction => propper destruction IMPOSSIBLE!       */
    if( ! byte_loader ) {
        return false;
    }
    byte_loader->ownership = E_Ownership_LEXICAL_ANALYZER;
    verdict_f = QUEX_MEMBER_FUNCTION_CALL3(include_push, ByteLoader, InputName, byte_loader, CodecName); 
    if( ! verdict_f ) {
        QUEX_NAME(ByteLoader_delete)(&byte_loader);
    }
    return verdict_f;
}
#endif


/* Level (3) __________________________________________________________________
 *                                                                           */
QUEX_INLINE bool
QUEX_MEMBER_FUNCTION3(include_push, ByteLoader,
                      const char*   InputName,
                      QUEX_NAME(ByteLoader)*   byte_loader,
                      const char*   CodecName) 
{
    QUEX_MAP_THIS_TO_ME(QUEX_TYPE_ANALYZER)
    bool                     verdict_f;
    QUEX_NAME(BufferFiller)* filler;
    QUEX_NAME(Asserts_construct)(CodecName);

    filler = QUEX_NAME(BufferFiller_new_DEFAULT)(byte_loader, CodecName);
    /* NOT: Abort/return if filler == 0 !!
     *      Incomplete construction => propper destruction IMPOSSIBLE!       */
    if( filler ) {
        filler->ownership = E_Ownership_LEXICAL_ANALYZER;
    }
    verdict_f = QUEX_MEMBER_FUNCTION_CALL2(include_push, BufferFiller, InputName, filler);
    if( ! verdict_f && filler->ownership == E_Ownership_LEXICAL_ANALYZER ) {
        me->buffer.filler->delete_self(me->buffer.filler); 
        me->buffer.filler = (QUEX_NAME(BufferFiller)*)0;
    }
    return verdict_f;
}

/* Level (4) __________________________________________________________________
 *                                                                           */
QUEX_INLINE bool
QUEX_MEMBER_FUNCTION2(include_push, BufferFiller,
                      const char*              InputName,
                      QUEX_NAME(BufferFiller)* filler)
{
    QUEX_MAP_THIS_TO_ME(QUEX_TYPE_ANALYZER)
    bool                 verdict_f;
    QUEX_TYPE_CHARACTER* memory;
    QUEX_NAME(Buffer)    new_buffer_setup;

    memory = (QUEX_TYPE_CHARACTER*)QUEXED(MemoryManager_allocate)(
                       QUEX_SETTING_BUFFER_SIZE * sizeof(QUEX_TYPE_CHARACTER), 
                       E_MemoryObjectType_BUFFER_MEMORY);
    /* NOT: Abort/return if memory == 0 !!
     *      Incomplete construction => propper destruction IMPOSSIBLE!       */

    QUEX_NAME(Buffer_construct)(&new_buffer_setup, filler,
                                memory, QUEX_SETTING_BUFFER_SIZE, 
                                (QUEX_TYPE_CHARACTER*)0,
                                E_Ownership_LEXICAL_ANALYZER);

    /* The 'new_buffer_setup' is only copied including the reference to the
     * new memory. However, the box object 'new_buffer_setup' is left alone. */
    verdict_f = QUEX_MEMBER_FUNCTION_CALLO2(basic_include_push, InputName, &new_buffer_setup);
    if( ! verdict_f ) {
        QUEXED(MemoryManager_free)(memory, E_MemoryObjectType_BUFFER_MEMORY);
    }
    return verdict_f;
}

/* Level (5) __________________________________________________________________
 *                                                                           */
QUEX_INLINE bool
QUEX_MEMBER_FUNCTION4(include_push, memory,
                      const char*             InputName,
                      QUEX_TYPE_CHARACTER*    Memory,
                      const size_t            MemorySize,
                      QUEX_TYPE_CHARACTER*    EndOfFileP)
/* When memory is provided from extern, the 'external entity' is
 * responsible for filling it. There is no 'file/stream handle', no 'byte
 * loader', and 'no buffer filler'.                                          */
{
    QUEX_MAP_THIS_TO_ME(QUEX_TYPE_ANALYZER)
    QUEX_NAME(Buffer) new_buffer_setup;
    __quex_assert(EndOfFileP > Memory && EndOfFileP <= &Memory[MemorySize]);

    QUEX_NAME(Buffer_construct)(&new_buffer_setup, 
                                (QUEX_NAME(BufferFiller)*)0,
                                Memory, MemorySize, EndOfFileP,
                                E_Ownership_EXTERNAL);

    /* The 'new_buffer_setup' is only copied including the reference to the
     * new memory. However, the box object 'new_buffer_setup' is left alone. */
    return QUEX_MEMBER_FUNCTION_CALLO2(basic_include_push, InputName, &new_buffer_setup);
}

QUEX_INLINE bool
QUEX_MEMBER_FUNCTIONO2(basic_include_push, 
                       const char*        InputName,
                       QUEX_NAME(Buffer)* new_buffer_setup)
{
    QUEX_MAP_THIS_TO_ME(QUEX_TYPE_ANALYZER)
    QUEX_NAME(Memento)* memento = (QUEX_NAME(Memento)*)QUEXED(MemoryManager_allocate)(
                                     sizeof(QUEX_NAME(Memento)), E_MemoryObjectType_MEMENTO);
    if( ! memento ) {
        return false;
    }
#   ifndef __QUEX_OPTION_PLAIN_C
    /* Use placement 'new' for explicit call of constructor. 
     * Necessary in C++: Call to constructor for user defined members.       */
    new ((void*)memento) QUEX_NAME(Memento);
#   endif
    if( me->buffer.filler )
    {
        /* By default overtake the byte order reversion behavior of the 
         * including buffer.                                                 */
        new_buffer_setup->filler->_byte_order_reversion_active_f = \
                          me->buffer.filler->_byte_order_reversion_active_f;
    }

    memento->input_name                       = me->input_name;
    memento->_parent_memento                  = me->_parent_memento;
    memento->buffer                           = me->buffer;
    memento->__current_mode_p                 = me->__current_mode_p; 
    memento->current_analyzer_function        = me->current_analyzer_function;
#   if    defined(QUEX_OPTION_AUTOMATIC_ANALYSIS_CONTINUATION_ON_MODE_CHANGE) \
       || defined(QUEX_OPTION_ASSERTS)
    memento->DEBUG_analyzer_function_at_entry = me->DEBUG_analyzer_function_at_entry;
#   endif
    __QUEX_IF_COUNT(memento->counter          = me->counter);

    me->buffer                                = *new_buffer_setup;
    __QUEX_IF_COUNT(QUEX_NAME(Counter_construct)(&me->counter); )

    /* Deriberately not subject to include handling:
     *    -- Mode stack.
     *    -- Token and token queues.
     *    -- Post categorizer.                                               */
    if( ! QUEX_MEMBER_FUNCTION_CALLO2(user_memento_pack, InputName, memento) ) {
        QUEXED(MemoryManager_free)(memento, E_MemoryObjectType_MEMENTO);
        return false;
    }

    me->input_name      = InputName;

    /* Put memento on stack AFTER user has done to it its duties.            */
    me->_parent_memento = memento;

    return true;
}   

QUEX_INLINE bool
QUEX_MEMBER_FUNCTIONO(include_pop) 
/* RETURNS: true, if there was a memento that has been restored. 
 *          false, else.                                                     */
{
    QUEX_MAP_THIS_TO_ME(QUEX_TYPE_ANALYZER)
    QUEX_NAME(Memento)* memento;
    /* Not included? return 'false' to indicate we're on the top level       */
    if( ! me->_parent_memento ) return false;                             
                                                                            
    /* Buffer_destruct() takes care of propper destructor calls for byte-
     * loaders, buffer fillers, and converters.                              */
    QUEX_NAME(Buffer_destruct)(&me->buffer);                              

    memento             = me->_parent_memento;
    me->_parent_memento = memento->_parent_memento;
    /* memento_unpack():                                                    
     *    => Current mode                                                   
     *           => __current_mode_p                                        
     *              current_analyzer_function                                           
     *              DEBUG_analyzer_function_at_entry                                       
     *    => Line/Column Counters                                           
     *                                                                      
     * Unchanged by memento_unpack():                                       
     *    -- Mode stack                                                     
     *    -- Tokens and token queues.                                       
     *    -- Accumulator.                                                   
     *    -- Post categorizer.                                              
     *    -- File handle by constructor                                      */
                                                                            
    /* Copy Back of content that was stored upon inclusion.                  */

    me->input_name                       = memento->input_name;
    me->buffer                           = memento->buffer;
    me->__current_mode_p                 = memento->__current_mode_p; 
    me->current_analyzer_function        = memento->current_analyzer_function;
#   if    defined(QUEX_OPTION_AUTOMATIC_ANALYSIS_CONTINUATION_ON_MODE_CHANGE) \
       || defined(QUEX_OPTION_ASSERTS)
    me->DEBUG_analyzer_function_at_entry = memento->DEBUG_analyzer_function_at_entry;
#   endif
    __QUEX_IF_COUNT(me->counter          = memento->counter);

    QUEX_MEMBER_FUNCTION_CALLO1(user_memento_unpack,memento);

#   ifndef __QUEX_OPTION_PLAIN_C
    /* Counterpart to placement new: Explicit destructor call.
     * Necessary in C++: Trigger call to destructor for user defined members.*/
    memento->~QUEX_NAME(Memento_tag)();
#   endif

    QUEXED(MemoryManager_free)((void*)memento, E_MemoryObjectType_MEMENTO); 

    /* Return to including file succesful                                    */
    return true;
}
     
QUEX_INLINE void
QUEX_MEMBER_FUNCTIONO(include_stack_delete)
{
    QUEX_MAP_THIS_TO_ME(QUEX_TYPE_ANALYZER)
    while( QUEX_MEMBER_FUNCTION_CALLO(include_pop) );
}

QUEX_INLINE bool
QUEX_MEMBER_FUNCTIONO1(include_detect_recursion,
                       const char* InputName)
{
    QUEX_MAP_THIS_TO_ME(QUEX_TYPE_ANALYZER)
    QUEX_NAME(Memento)* iterator;
    for(iterator = me->_parent_memento; iterator ; 
        iterator = iterator->_parent_memento ) {
        if( __QUEX_STD_strcmp(iterator->input_name, InputName) == 0 ) {
            return true;
        }
    }
    return false;
}

QUEX_NAMESPACE_MAIN_CLOSE

#endif /*  __QUEX_INCLUDE_GUARD__ANALYZER__STRUCT__INCLUDE_STACK_I */
