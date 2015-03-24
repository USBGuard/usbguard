/* -*- C++ -*- vim:set syntax=cpp:
 * (C) 2005-2009 Frank-Rene Schaefer
 * ABSOLUTELY NO WARRANTY                   */
#ifndef __QUEX_INCLUDE_GUARD__ANALYZER__MEMBER__CONSTRUCTOR_I
#define __QUEX_INCLUDE_GUARD__ANALYZER__MEMBER__CONSTRUCTOR_I

#include <quex/code_base/buffer/Buffer.i>

#include <quex/code_base/temporary_macros_on>

QUEX_NAMESPACE_MAIN_OPEN

QUEX_INLINE void
QUEX_NAME(construct_memory)(QUEX_TYPE_ANALYZER*  me,
                            QUEX_TYPE_CHARACTER* BufferMemoryBegin, 
                            size_t               BufferMemorySize,
                            QUEX_TYPE_CHARACTER* BufferEndOfContentP,  /* = 0x0   */
                            const char*          CharacterEncodingName /* = 0x0   */,
                            bool                 ByteOrderReversionF   /* = false */)
{
    size_t  memory_size = BufferMemoryBegin != 0 ? BufferMemorySize 
                          :                        QUEX_SETTING_BUFFER_SIZE;
#   ifdef QUEX_OPTION_ASSERTS
    QUEX_TYPE_CHARACTER*   iterator = 0x0;

    __quex_assert(memory_size == 0 || memory_size > 2);
    if( BufferMemoryBegin != 0x0 ) {
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
    if( BufferEndOfContentP != 0x0 ) {
        __quex_assert(BufferEndOfContentP >  BufferMemoryBegin);
        __quex_assert(BufferEndOfContentP <= BufferMemoryBegin + memory_size - 1);

        /* The memory provided must be initialized. If it is not, then that's wrong.
         * Try to detect this by searching for BLC and PTC.                         */
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

    QUEX_NAME(constructor_core)(me, 
#   if defined(__QUEX_OPTION_PLAIN_C)
                                (FILE*)0x0,
#   else
                                (void*)0x0, 
#   endif
                                CharacterEncodingName, ByteOrderReversionF,
                                BufferMemoryBegin, memory_size, BufferEndOfContentP);
}

QUEX_INLINE void
QUEX_NAME(construct_file_name)(QUEX_TYPE_ANALYZER* me,
                               const char*         Filename, 
                               const char*         CharacterEncodingName /* = 0x0   */,
                               bool                ByteOrderReversionF   /* = false */)
{
    /* Buffer: Size = (see macro def.), Fallback = 10 Characters
     * prefer FILE* based buffers, because we can turn low-level buffering off.
     * ownership of FILE* id passed to the input strategy of the buffer.         */
    __QUEX_STD_FILE*   fh = __QUEX_STD_fopen(Filename, "rb");

    QUEX_NAME(construct_FILE)(me, fh, CharacterEncodingName, ByteOrderReversionF);

    /* Recall, that this thing as to be deleted/closed */
    me->__file_handle_allocated_by_constructor = fh;
}

QUEX_INLINE void
QUEX_NAME(construct_FILE)(QUEX_TYPE_ANALYZER* me,
                          __QUEX_STD_FILE*    fh, 
                          const char*         CharacterEncodingName /* = 0x0   */,
                          bool                ByteOrderReversionF   /* = false */)
{
    if( fh == NULL ) QUEX_ERROR_EXIT("Error: received NULL as a file handle.");

    /* At the time of this writing 'stdin' as located in the C++ global namespace. 
     * This seemed suspicous to the author. To avoid compilation errors in the future
     * the test for the standard input is only active in 'C'. It is only about
     * user information anyway. So better no risks taken.      <fschaef 2010y02m06d> */
#   ifdef __QUEX_OPTION_PLAIN_C
    if( fh == __QUEX_STD_STDIN ) QUEX_ERROR_EXIT(__QUEX_MESSAGE_STDIN_IN_CONSTRUCTOR);
#   endif
    setbuf(fh, 0);   /* turn off system based buffering! 
    **               ** this is essential to profit from the quex buffer! */
    QUEX_NAME(constructor_core)(me, fh, 
                                CharacterEncodingName, ByteOrderReversionF, 
                                0x0, QUEX_SETTING_BUFFER_SIZE, 0x0);
}

#ifndef __QUEX_OPTION_PLAIN_C
QUEX_INLINE void
QUEX_NAME(construct_istream)(QUEX_TYPE_ANALYZER* me,
                             std::istream*       p_input_stream, 
                             const char*         CharacterEncodingName /* = 0x0   */,
                             bool                ByteOrderReversionF   /* = false */)
{
    if( p_input_stream == NULL )      QUEX_ERROR_EXIT("Error: received NULL as pointer to input stream.");
    if( p_input_stream == &std::cin ) QUEX_ERROR_EXIT(__QUEX_MESSAGE_STDIN_IN_CONSTRUCTOR);
    QUEX_NAME(constructor_core)(me, p_input_stream, 
                                CharacterEncodingName, ByteOrderReversionF, 
                                0x0,  QUEX_SETTING_BUFFER_SIZE, 0x0);
}
#endif


#if defined(__QUEX_OPTION_WCHAR_T) && ! defined(__QUEX_OPTION_PLAIN_C)
QUEX_INLINE void
QUEX_NAME(construct_wistream)(QUEX_TYPE_ANALYZER* me,
                              std::wistream*      p_input_stream, 
                              const char*         CharacterEncodingName /* = 0x0   */,
                              bool                ByteOrderReversionF   /* = false */)
{
    if( p_input_stream == NULL ) 
        QUEX_ERROR_EXIT("Error: received NULL as pointer to input stream.\n");
    QUEX_NAME(constructor_core)(me, p_input_stream, 
                                CharacterEncodingName, ByteOrderReversionF, 
                                0x0, QUEX_SETTING_BUFFER_SIZE, 0x0);
}
#endif

QUEX_INLINE void
QUEX_NAME(destruct)(QUEX_TYPE_ANALYZER* me) 
{
    QUEX_NAME(destruct_basic)(me);
}

TEMPLATE_IN(InputHandleT) void
QUEX_NAME(reset)(QUEX_TYPE_ANALYZER*  me,
                 InputHandleT*        input_handle, 
                 const char*          CharacterEncodingName /* = 0x0 */) 
{
    QUEX_NAME(reset_basic)(me, input_handle, CharacterEncodingName, 
                           QUEX_SETTING_TRANSLATION_BUFFER_SIZE);
    me->__current_mode_p = 0x0; /* REQUIRED, for mode transition check */
    QUEX_NAME(set_mode_brutally_by_id)(me, __QUEX_SETTING_INITIAL_LEXER_MODE_ID);
}

QUEX_INLINE QUEX_TYPE_CHARACTER*
QUEX_NAME(reset_buffer)(QUEX_TYPE_ANALYZER*   me,
                        QUEX_TYPE_CHARACTER*  BufferMemoryBegin, 
                        size_t                BufferMemorySize,
                        QUEX_TYPE_CHARACTER*  BufferEndOfContentP,  /* = 0x0 */
                        const char*           CharacterEncodingName /* = 0x0 */) 
{
    QUEX_TYPE_CHARACTER* old_memory = 0x0;
    /* End of Content **must** lie inside the provided buffer */
#   if defined(QUEX_OPTION_ASSERTS)
    if(    BufferEndOfContentP < BufferMemoryBegin 
        || BufferEndOfContentP > (BufferMemoryBegin + BufferMemorySize - 1)) {
        QUEX_ERROR_EXIT("\nreset_buffer: Argument 'BufferEndOfContentP' must be inside the provided memory\n"
                        "reset_buffer: buffer (speficied by 'BufferMemoryBegin' and 'BufferMemorySize').\n"
                        "reset_buffer: Note, that the last element of the buffer is to be filled with\n"
                        "reset_buffer: the buffer limit code character.\n");
    }
#   endif

    /* reset_basic(...) does not reset the buffer memory itself. This must
     * be done by hand.                                                    */
    old_memory = QUEX_NAME(BufferMemory_reset)(&me->buffer._memory,
                                               BufferMemoryBegin, BufferMemorySize, 
                                               BufferEndOfContentP);

    if( BufferMemoryBegin == 0x0 ) return old_memory;

    QUEX_NAME(reset_basic)(me, /* InputHandle */ (FILE*)0x0, CharacterEncodingName, 
                           QUEX_SETTING_TRANSLATION_BUFFER_SIZE);

    me->__current_mode_p = 0x0; /* REQUIRED, for mode transition check */
    QUEX_NAME(set_mode_brutally_by_id)(me, __QUEX_SETTING_INITIAL_LEXER_MODE_ID);

    return old_memory;
}

QUEX_INLINE void 
QUEX_NAME(reset_plain)(QUEX_TYPE_ANALYZER*  me,
                       const char*          CharacterEncodingName /* = 0x0 */)
{ QUEX_NAME(reset)(me, (FILE*)0x0, CharacterEncodingName); }

#if ! defined(__QUEX_OPTION_PLAIN_C)
QUEX_INLINE
QUEX_MEMBER(QUEX_TYPE0_ANALYZER)(QUEX_TYPE_CHARACTER* BufferMemoryBegin, 
                                 size_t               BufferMemorySize,
                                 QUEX_TYPE_CHARACTER* BufferEndOfFileP,     /* = 0x0   */
                                 const char*          CharacterEncodingName /* = 0x0   */,
                                 bool                 ByteOrderReversionF   /* = false */)
{ QUEX_NAME(construct_memory)(this, BufferMemoryBegin, BufferMemorySize, BufferEndOfFileP,
                              CharacterEncodingName, ByteOrderReversionF); }

QUEX_INLINE
QUEX_MEMBER(QUEX_TYPE0_ANALYZER)(const std::string&  Filename, 
                                 const char*         CharacterEncodingName /* = 0x0   */,
                                 bool                ByteOrderReversionF   /* = false */)
{ QUEX_NAME(construct_file_name)(this, Filename.c_str(), CharacterEncodingName, ByteOrderReversionF); }

QUEX_INLINE
QUEX_MEMBER(QUEX_TYPE0_ANALYZER)(std::FILE*   fh, 
                                 const char*  CharacterEncodingName /* = 0x0   */,
                                 bool         ByteOrderReversionF   /* = false */)
{ QUEX_NAME(construct_FILE)(this, fh, CharacterEncodingName, ByteOrderReversionF); }

QUEX_INLINE
QUEX_MEMBER(QUEX_TYPE0_ANALYZER)(std::istream*   p_input_stream, 
                                 const char*     CharacterEncodingName /* = 0x0   */,
                                 bool            ByteOrderReversionF   /* = false */)
{ QUEX_NAME(construct_istream)(this, p_input_stream, CharacterEncodingName, ByteOrderReversionF); }

#if defined(__QUEX_OPTION_WCHAR_T)
QUEX_INLINE
QUEX_MEMBER(QUEX_TYPE0_ANALYZER)(std::wistream*  p_input_stream, 
                                 const char*     CharacterEncodingName /* = 0x0   */,
                                 bool            ByteOrderReversionF   /* = false */)
{ QUEX_NAME(construct_wistream)(this, p_input_stream, CharacterEncodingName, ByteOrderReversionF); }
#endif

#if defined(__QUEX_OPTION_UNIT_TEST) && ! defined (__QUEX_OPTION_PLAIN_C)
/* StrangeStreams are not for C-language stuff */
template<class UnderlyingStreamT> QUEX_INLINE
QUEX_MEMBER(QUEX_TYPE0_ANALYZER)(quex::StrangeStream<UnderlyingStreamT>*  p_input_stream, 
                                const char*      CharacterEncodingName /* = 0x0   */,
                                bool             ByteOrderReversionF   /* = false */)
{
    if( p_input_stream == NULL ) QUEX_ERROR_EXIT("Error: received NULL as pointer to input stream.");
    QUEX_NAME(constructor_core)(this, p_input_stream, 
                                CharacterEncodingName, ByteOrderReversionF, 
                                0x0, QUEX_SETTING_BUFFER_SIZE, 0x0);
}
#endif

QUEX_INLINE
QUEX_MEMBER(~QUEX_TYPE0_ANALYZER)() 
{ QUEX_NAME(destruct)(this); }

template<class InputHandleT> void
QUEX_MEMBER(reset)(InputHandleT* input_handle, const char* CharacterEncodingName /* = 0x0 */) 
{ QUEX_NAME(reset)(this, input_handle, CharacterEncodingName); }

QUEX_INLINE QUEX_TYPE_CHARACTER*
QUEX_MEMBER(reset_buffer)(QUEX_TYPE_CHARACTER* BufferMemoryBegin, 
                          size_t               BufferMemorySize,
                          QUEX_TYPE_CHARACTER* BufferEndOfContentP,  
                          const char*          CharacterEncodingName /* = 0x0 */) 
{ return QUEX_NAME(reset_buffer)(this, BufferMemoryBegin, BufferMemorySize, BufferEndOfContentP, CharacterEncodingName); }
#endif

QUEX_NAMESPACE_MAIN_CLOSE

#include <quex/code_base/temporary_macros_off>

#endif /* __QUEX_INCLUDE_GUARD__ANALYZER__MEMBER__CONSTRUCTOR_I */
