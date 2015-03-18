/* -*- C++ -*- vim: set syntax=cpp: */
#ifndef __QUEX_INCLUDE_GUARD__BUFFER__BUFFER_FILLER_I
#define __QUEX_INCLUDE_GUARD__BUFFER__BUFFER_FILLER_I

#include <quex/code_base/definitions>
#include <quex/code_base/buffer/Buffer>
#include <quex/code_base/buffer/BufferFiller>
#include <quex/code_base/buffer/Buffer_debug>

#include <quex/code_base/temporary_macros_on>

QUEX_NAMESPACE_MAIN_OPEN

    QUEX_INLINE ptrdiff_t  QUEX_NAME(__BufferFiller_forward_compute_fallback_region)(QUEX_NAME(Buffer)*  buffer,
                                                                                     const ptrdiff_t     Distance_LexemeStart_to_InputP);
    QUEX_INLINE ptrdiff_t  QUEX_NAME(__BufferFiller_backward_compute_backward_distance)(QUEX_NAME(Buffer)* buffer);

    QUEX_INLINE void       QUEX_NAME(__BufferFiller_forward_copy_fallback_region)(QUEX_NAME(Buffer)*,
                                                                                  const ptrdiff_t FallBackN);
    QUEX_INLINE void       QUEX_NAME(__BufferFiller_forward_adapt_pointers)(QUEX_NAME(Buffer)*, 
                                                                            const ptrdiff_t DesiredLoadN,
                                                                            const size_t    LoadedN,
                                                                            const ptrdiff_t FallBackN, 
                                                                            const ptrdiff_t Distance_LexemeStart_to_InputP);
    QUEX_INLINE void       QUEX_NAME(__BufferFiller_backward_copy_backup_region)(QUEX_NAME(Buffer)*, 
                                                                                 const ptrdiff_t BackwardDistance);
    QUEX_INLINE void       QUEX_NAME(__BufferFiller_backward_adapt_pointers)(QUEX_NAME(Buffer)*, 
                                                                             const ptrdiff_t BackwardDistance);
    QUEX_INLINE void       QUEX_NAME(__BufferFiller_on_overflow)(QUEX_NAME(Buffer)*, bool ForwardF);
                           
    QUEX_INLINE size_t     QUEX_NAME(__BufferFiller_read_characters)(QUEX_NAME(Buffer)*, QUEX_TYPE_CHARACTER*, 
                                                                     const ptrdiff_t);

#   if defined(__QUEX_OPTION_CONVERTER)

    QUEX_INLINE QUEX_NAME(Converter)*   QUEX_NAME(__Converter_EMPTY_new)() { return 0x0; }

    TEMPLATE_IN(InputHandleT) QUEX_NAME(BufferFiller)*
    QUEX_NAME(BufferFiller_new)(InputHandleT*  input_handle, 
                                const char*    CharacterEncodingName,
                                const size_t   TranslationBufferMemorySize)
        /* CharacterEncoding == 0x0: Impossible; Converter requires codec name --> filler = 0x0
         * input_handle      == 0x0: Possible; Converter might be applied on buffer. 
         *                           (User writes into translation buffer).                     */
    {
        (void)TranslationBufferMemorySize;

        if( CharacterEncodingName == 0x0 ) {
#           ifndef QUEX_OPTION_WARNING_ON_PLAIN_FILLER_DISABLED
            __QUEX_STD_printf("Warning: No character encoding name specified, while this\n" \
                              "Warning: analyzer was generated for use with a converter.\n" \
                              "Warning: Please, consult the documentation about the constructor\n" \
                              "Warning: or the reset function. If it is desired to do a plain\n" \
                              "Warning: buffer filler with this setup, you might want to disable\n" \
                              "Warning: this warning with the macro:\n" \
                              "Warning:     QUEX_OPTION_WARNING_ON_PLAIN_FILLER_DISABLED\n");
#           endif
            return (QUEX_NAME(BufferFiller*))0x0;
        } 

        if( QUEX_SETTING_BUFFER_FILLERS_CONVERTER_NEW == QUEX_NAME(__Converter_EMPTY_new) ) {
            QUEX_ERROR_EXIT("Use of buffer filler type 'CharacterEncodingName' while " \
                            "'QUEX_SETTING_BUFFER_FILLERS_CONVERTER_NEW' has not\n" \
                            "been defined (use --iconv, --icu, --converter-new to specify converter).\n");
        }

        /* The specification of a CharacterEncodingName means that a converter is
         * to be used. This can also happen if the engine is to work on plain memory.
         * In the latter case the input_handle = 0x0 is passed to the 'new' allocator
         * without the slightest harm.                                                 */
        return (QUEX_NAME(BufferFiller)*)QUEX_NAME(BufferFiller_Converter_new)(input_handle, 
                                                                               QUEX_SETTING_BUFFER_FILLERS_CONVERTER_NEW(),
                                                                               CharacterEncodingName, 
                                                                               /* Internal Coding: Default */0x0,
                                                                               TranslationBufferMemorySize);
    }

#   else /* NOT: __QUEX_OPTION_CONVERTER */

    TEMPLATE_IN(InputHandleT) QUEX_NAME(BufferFiller)*
    QUEX_NAME(BufferFiller_new)(InputHandleT*  input_handle, 
                                const char*    CharacterEncodingName,
                                const size_t   TranslationBufferMemorySize)
        /* CharacterEncoding != 0x0: Not possible, converter required   --> filler = 0x0
         * input_handle      == 0x0: Not possible, data source required --> filler = 0x0 */
    {
        (void)TranslationBufferMemorySize;
        if( CharacterEncodingName != 0x0 ) {
            QUEX_ERROR_EXIT("Use of buffer filler type 'CharacterEncodingName' while " \
                            "'QUEX_SETTING_BUFFER_FILLERS_CONVERTER_NEW' has not\n" \
                            "been defined (use --iconv, --icu, --converter-new to specify converter).\n");
            return (QUEX_NAME(BufferFiller*))0x0;
        } 
        /* If no converter is required, it has to be considered whether the buffer needs
         * filling or not. If the input source is not memory, then the 'plain' buffer
         * filling is applied. If the input source is memory, no filler is required.     */
        else if( input_handle == 0x0 ) {
            return (QUEX_NAME(BufferFiller*))0x0;
        }

        return (QUEX_NAME(BufferFiller)*)QUEX_NAME(BufferFiller_Plain_new)(input_handle);
    }
#   endif

    QUEX_INLINE void       
    QUEX_NAME(BufferFiller_delete_self)(QUEX_NAME(BufferFiller)* me)
    { 
        if( me->delete_self == 0x0 ) QUEX_ERROR_EXIT("BufferFiller object did not specify 'delete_self()'\n");
        else                         me->delete_self(me);
    }

    QUEX_INLINE void
    QUEX_NAME(BufferFiller_setup_functions)(QUEX_NAME(BufferFiller)* me,
                                            ptrdiff_t    (*tell_character_index)(QUEX_NAME(BufferFiller)*),
                                            void         (*seek_character_index)(QUEX_NAME(BufferFiller)*, 
                                                                                 const ptrdiff_t),
                                            size_t       (*read_characters)(QUEX_NAME(BufferFiller)*,
                                                                            QUEX_TYPE_CHARACTER*, const size_t),
                                            void         (*delete_self)(QUEX_NAME(BufferFiller)*))
    {
        __quex_assert(me != 0x0);
        __quex_assert(tell_character_index != 0x0);
        __quex_assert(seek_character_index != 0x0);
        __quex_assert(read_characters != 0x0);
        __quex_assert(delete_self != 0x0);


        me->tell_character_index = tell_character_index;
        me->seek_character_index = seek_character_index;
        me->read_characters      = read_characters;
        me->_on_overflow         = 0x0;
        me->delete_self          = delete_self;
    }

    QUEX_INLINE void
    QUEX_NAME(BufferFiller_initial_load)(QUEX_NAME(Buffer)* buffer)
    {
        const ptrdiff_t          ContentSize  = (ptrdiff_t)QUEX_NAME(Buffer_content_size)(buffer);
        QUEX_TYPE_CHARACTER*     ContentFront = QUEX_NAME(Buffer_content_front)(buffer);
        QUEX_NAME(BufferFiller)* me           = buffer->filler;
        size_t                   LoadedN      = 0;

        /* Assume: Buffer initialization happens independently */
        __quex_assert(buffer->_input_p        == ContentFront);   
        __quex_assert(buffer->_lexeme_start_p == ContentFront);

        /* end   != 0, means that the buffer is filled.
         * begin == 0, means that we are standing at the begin.
         * => end != 0 and begin == 0, means that the initial content is loaded already.    */
        /* if( buffer->_content_character_index_begin == 0 ) {
         *     if ( buffer->_content_character_index_end != 0) return;
         *} else {*/
        me->seek_character_index(me, 0);
        /*} */
        LoadedN = QUEX_NAME(__BufferFiller_read_characters)(buffer, ContentFront, ContentSize);

        buffer->_content_character_index_begin = 0; 
        buffer->_content_character_index_end   = me->tell_character_index(buffer->filler);

        if( me->tell_character_index(me) != (ptrdiff_t)LoadedN ) 
            QUEX_ERROR_EXIT(__QUEX_MESSAGE_BUFFER_FILLER_ON_STRANGE_STREAM); 

        /* If end of file has been reached, then the 'end of file' pointer needs to be set. */
        if( LoadedN != (size_t)ContentSize ) QUEX_NAME(Buffer_end_of_file_set)(buffer, ContentFront + LoadedN);
        else                                 QUEX_NAME(Buffer_end_of_file_unset)(buffer);

        QUEX_BUFFER_ASSERT_CONTENT_CONSISTENCY(buffer);
    } 

    QUEX_INLINE size_t
    QUEX_NAME(BufferFiller_load_forward)(QUEX_NAME(Buffer)* buffer)
    /*_________________________________________________________________________
     * This function is to be called as a reaction to a buffer limit code 'BLC'
     * as returned by 'get_forward()'. Its task is to load new content into the
     * buffer such that 'get_forward() can continue iterating. This means that
     * the '_input_p' points to one of the following positions:
     *
     *  (1) Beginning of the Buffer: In this case, no reload needs to take place.
     *      It can basically only appear if 'load_forward()' is called after
     *      'get_backward()'---and this does not make sense. But returning a '0'
     *      (which is >= 0 and indicates that everything is ok) tells the application 
     *      that nothing has been loaded, and the next 'get_forward()' will work 
     *      normally.
     *
     *  (2) End of File Pointer: (which may be equal to the end of the buffer) 
     *      In this case no further content can be loaded. The function returns '0'.
     *
     *  (3) End of Buffer (if it is != End of File Pointer): Here a 'normal' load of
     *      new data into the buffer can happen.
     *
     * RETURNS: '>= 0'   number of characters that were loaded forward in the stream.
     *          '-1'     if forward loading was not possible (end of file)                      
     *_________________________________________________________________________
     *
     * There is a seemingly dangerous case where the loading **just**
     * fills the buffer to the limit. In this case no 'End Of File' is
     * detected, no end of file pointer is set, and as a consequence a new
     * loading will happen later. This new loading, though, will only copy
     * the fallback-region. The 'LoadedN == 0' will cause the
     * _memory._end_of_file_p to be set to the end of the copied
     * fallback-region. And everything is fine.                          
     *_______________________________________________________________________*/
    {
        const ptrdiff_t      ContentSize  = (ptrdiff_t)QUEX_NAME(Buffer_content_size)(buffer);
        QUEX_TYPE_CHARACTER* ContentFront = QUEX_NAME(Buffer_content_front)(buffer);
        ptrdiff_t            Distance_LexemeStart_to_InputP = (ptrdiff_t)-1; 
        ptrdiff_t            FallBackN         = (ptrdiff_t)-1;
        ptrdiff_t            DesiredLoadN      = (ptrdiff_t)-1;
        QUEX_TYPE_CHARACTER* new_content_begin = 0x0;
        size_t               LoadedN           = (size_t)-1;

        QUEX_NAME(BufferFiller)*  me = buffer->filler;
        if( me == 0x0 ) return 0; /* This case it totally rational, if no filler has been specified */

        __quex_assert(buffer != 0x0);
        __quex_assert(me->tell_character_index != 0x0);
        __quex_assert(me->seek_character_index != 0x0);
        __quex_assert(me->read_characters != 0x0);

        /* Catch impossible scenario: If the stretch from _input_p to _lexeme_start_p 
         * spans the whole buffer content, then nothing can be loaded into it.                      */
        Distance_LexemeStart_to_InputP = buffer->_input_p - buffer->_lexeme_start_p;
        if( Distance_LexemeStart_to_InputP >= ContentSize ) { 
            QUEX_NAME(__BufferFiller_on_overflow)(buffer, /* Forward */ true);
            return 0;
        }
        __quex_debug_buffer_load(buffer, "FORWARD(entry)\n");

        /* (*) Check for the three possibilities mentioned above */
        if     ( buffer->_input_p == buffer->_memory._front )         { return 0; }   /* (1)*/
        else if( buffer->_input_p == buffer->_memory._end_of_file_p ) { return 0; }   /* (2)*/
        else if( buffer->_input_p != buffer->_memory._back  ) {                     
            QUEX_ERROR_EXIT("Call to 'load_forward() but '_input_p' not on buffer border.\n" 
                            "(Check character encoding)");  
        }
        else if( buffer->_memory._end_of_file_p != 0x0 ) { 
            /* End of file has been reached before, we cannot load more.                    */
            return 0;                               
        }
        /* HERE: _input_p ---> LAST ELEMENT OF THE BUFFER!                             * (3)*/  


        /*___________________________________________________________________________________*/
        /* (1) Handle fallback region */
        FallBackN    = QUEX_NAME(__BufferFiller_forward_compute_fallback_region)(buffer, 
                                                                                 Distance_LexemeStart_to_InputP); 
        __quex_assert(FallBackN < ContentSize);
        DesiredLoadN = ContentSize - FallBackN;

        QUEX_NAME(__BufferFiller_forward_copy_fallback_region)(buffer, FallBackN);

        /*___________________________________________________________________________________*/
        /* (2) Load new content*/
        /* NOTE: Due to backward loading the character index might not stand on the end of
         *       the buffer. Thus a seek is necessary.                                       */
        me->seek_character_index(me, buffer->_content_character_index_end);
        buffer->_content_character_index_begin = buffer->_content_character_index_end - FallBackN;

        new_content_begin = ContentFront + FallBackN;
        LoadedN           =  QUEX_NAME(__BufferFiller_read_characters)(buffer, 
                                                                       new_content_begin, 
                                                                       DesiredLoadN);
        
        /*___________________________________________________________________________________*/
        /* (3) Adapt the pointers in the buffer*/
        QUEX_NAME(__BufferFiller_forward_adapt_pointers)(buffer, 
                                                         DesiredLoadN, LoadedN, FallBackN, 
                                                         Distance_LexemeStart_to_InputP);

        buffer->_content_character_index_end   =   buffer->_content_character_index_begin
                                                 + (QUEX_NAME(Buffer_text_end)(buffer) - ContentFront);

        /* If the character index in the stream is different from 'old index + LoadedN'
         * then this indicates a 'strange stream' where the stream position increment is
         * not proportional to the number of loaded characters.                              */
        if( (me->tell_character_index(me) - buffer->_content_character_index_begin - FallBackN) != (ptrdiff_t)LoadedN ) 
            QUEX_ERROR_EXIT(__QUEX_MESSAGE_BUFFER_FILLER_ON_STRANGE_STREAM); 

        __quex_debug_buffer_load(buffer, "LOAD FORWARD(exit)\n");
        QUEX_BUFFER_ASSERT_CONSISTENCY(buffer);
        QUEX_BUFFER_ASSERT_CONTENT_CONSISTENCY(buffer);
        /* NOTE: Return value is used for adaptions of memory addresses. It happens that the*/
        /*       address offset is equal to DesiredLoadN; see function __forward_adapt_pointers().*/
        return (size_t)DesiredLoadN; /* THUS NOT: LoadedN*/
    }

    QUEX_INLINE ptrdiff_t
    QUEX_NAME(__BufferFiller_forward_compute_fallback_region)(QUEX_NAME(Buffer)*  buffer,
                                                              const ptrdiff_t     Distance_LexemeStart_to_InputP)
    {
        ptrdiff_t FallBackN = (ptrdiff_t)-1;

        (void)buffer;
        /* (1) Fallback: A certain region of the current buffer is copied in front such that
         *               if necessary the stream can go backwards without a backward load.
         *
         *                            fallback_n
         *                               :
         *                |11111111111111:22222222222222222222222222222222222222|
         *                  copy of      :   new loaded content of buffer
         *                  end of old   
         *                  buffer      
         *
         *     The fallback region is related to the lexeme start pointer. The lexeme start 
         *     pointer always needs to lie inside the buffer, because applications might read
         *     their characters from it. The 'stretch' [lexeme start, current_p] must be 
         *     contained in the new buffer (precisely in the fallback region).                        */
        /* Copying forward shall **only** happen when new content is to be loaded. This is not the case
         * if EOF is reached and the _memory._memory._end_of_file_p lies inside the buffer. Thus the _input_p
         * must have reached the upper border of the buffer.                                          */
        __quex_assert(buffer->_memory._end_of_file_p == 0x0);
        __quex_assert(buffer->_input_p == buffer->_memory._back);
        QUEX_BUFFER_ASSERT_CONSISTENCY(buffer);
        __quex_assert(Distance_LexemeStart_to_InputP == buffer->_input_p - buffer->_lexeme_start_p);
        __quex_assert(Distance_LexemeStart_to_InputP < (ptrdiff_t)QUEX_NAME(Buffer_content_size)(buffer));

        /* (*) Fallback region = max(default size, necessary size)*/
        FallBackN = (ptrdiff_t)QUEX_SETTING_BUFFER_MIN_FALLBACK_N > Distance_LexemeStart_to_InputP 
                    ? (ptrdiff_t)QUEX_SETTING_BUFFER_MIN_FALLBACK_N  
                    : Distance_LexemeStart_to_InputP;
        return FallBackN;
    }

    QUEX_INLINE void
    QUEX_NAME(__BufferFiller_forward_copy_fallback_region)(QUEX_NAME(Buffer)* buffer, const ptrdiff_t FallBackN)
    {
        /* (*) Copy fallback region*/
        /*     If there is no 'overlap' from source and drain than the faster memcpy() can */
        /*     used instead of memmove().*/
        QUEX_TYPE_CHARACTER*  source = QUEX_NAME(Buffer_content_back)(buffer) - FallBackN + 1; /* end of content - fallback*/
        QUEX_TYPE_CHARACTER*  drain  = QUEX_NAME(Buffer_content_front)(buffer);       

        /* Cast to uint8_t to avoid that some smart guy provides a C++ overloading function */
        if( drain + FallBackN >= source  ) {
            __QUEX_STD_memmove((uint8_t*)drain, (uint8_t*)source, (size_t)FallBackN * sizeof(QUEX_TYPE_CHARACTER));
        } else { 
            __QUEX_STD_memcpy((uint8_t*)drain, (uint8_t*)source, (size_t)FallBackN * sizeof(QUEX_TYPE_CHARACTER));
        }

#       ifdef QUEX_OPTION_ASSERTS
        /* Cast to uint8_t to avoid that some smart guy provides a C++ overloading function */
        __QUEX_STD_memset((uint8_t*)(drain + FallBackN), (uint8_t)(0xFF), 
                          (QUEX_NAME(Buffer_content_size)(buffer) - (size_t)FallBackN)*sizeof(QUEX_TYPE_CHARACTER)); 
#       endif

        __quex_assert((size_t)FallBackN < QUEX_NAME(Buffer_content_size)(buffer));
    }

    QUEX_INLINE void
    QUEX_NAME(__BufferFiller_forward_adapt_pointers)(QUEX_NAME(Buffer)*  buffer, 
                                                     const ptrdiff_t DesiredLoadN,
                                                     const size_t    LoadedN,
                                                     const ptrdiff_t FallBackN, 
                                                     const ptrdiff_t Distance_LexemeStart_to_InputP)
    {
        QUEX_TYPE_CHARACTER* ContentFront = QUEX_NAME(Buffer_content_front)(buffer);

        __quex_assert(    buffer->_memory._end_of_file_p == 0x0 
                       || (LoadedN + (size_t)FallBackN) == QUEX_NAME(Buffer_content_size)(buffer) );
        __quex_assert( DesiredLoadN != 0 );

        /* (*) If end of file has been reached, then the 'end of file' pointer needs to be set*/
        if( LoadedN != (size_t)DesiredLoadN ) 
            QUEX_NAME(Buffer_end_of_file_set)(buffer, ContentFront + FallBackN + (ptrdiff_t)LoadedN);
        else
            QUEX_NAME(Buffer_end_of_file_unset)(buffer);

        /*___________________________________________________________________________________*/
        /* (*) Pointer adaption*/
        /*     Next char to be read: '_input_p + 1'*/
        buffer->_input_p        = ContentFront + FallBackN - 1;   
        /*     NOTE: _input_p is set to (_input_p - 1) so that the next *(++_input_p) */
        /*           reads the _input_p.*/
        buffer->_lexeme_start_p = (buffer->_input_p + 1) - Distance_LexemeStart_to_InputP; 

        /* Asserts */
        __quex_assert(   buffer->_memory._end_of_file_p == 0x0 
                      || ((ptrdiff_t)LoadedN + FallBackN) == buffer->_memory._end_of_file_p - buffer->_memory._front - 1);

    }


    QUEX_INLINE size_t   
    QUEX_NAME(BufferFiller_load_backward)(QUEX_NAME(Buffer)* buffer)
    {
        QUEX_NAME(BufferFiller)* me = buffer->filler;
        QUEX_TYPE_CHARACTER*     ContentFront = QUEX_NAME(Buffer_content_front)(buffer);
        QUEX_TYPE_CHARACTER*     ContentBack  = QUEX_NAME(Buffer_content_back)(buffer);
        ptrdiff_t                BackwardDistance              = (ptrdiff_t)-1;
        ptrdiff_t                NewContentCharacterIndexBegin = (ptrdiff_t)-1;
#       if defined(QUEX_OPTION_ASSERTS)
        size_t                   LoadedN                       = (size_t)-1;
#       endif

#       ifdef QUEX_OPTION_STRANGE_ISTREAM_IMPLEMENTATION
        QUEX_ERROR_EXIT(__QUEX_MESSAGE_BUFFER_FILLER_ON_STRANGE_STREAM_IN_BACKWARD_LOAD);
#       endif
        QUEX_BUFFER_ASSERT_CONSISTENCY(buffer);

        if( me == 0x0 ) return 0; /* This case it totally rational, if no filler has been specified */

        /* PURPOSE: This function is to be called as a reaction to a buffer limit code 'BLC'
         *          as returned by 'get_backward()'. Its task is the same as the one of 
         *          'load_forward()'--only in opposite direction. Here only two cases need 
         *          to be distinguished. The current_p points to 
         *
         *          (1) End of Buffer or End of File pointer: No backward load needs to 
         *              happen. This can only occur if a 'get_forward()' was called right
         *              before.
         *
         *          (2) Begin of the buffer and the buffer is the 'start buffer':
         *              in this case no backward load can happen, because we are at the 
         *              beginning. The function returns 0.
         *
         *          (3) Begin of buffer and _begin_of_file_f is not set!: This is the case
         *              where this function, actually, has some work to do. It loads the
         *              buffer with 'earlier' content from the file.
         *
         *
         * RETURNS: Distance that was loaded backwards.
         *          -1 in case of backward loading is not possible (begin of file)
         *     
         * COMMENT: 
         *     
         * For normal cases the fallback region, i.e. the 'FALLBACK_N' buffer bytes 
         * allows to go a certain distance backwards immediately. If still the begin 
         * of the buffer is reached, then this is an indication that something is
         * 'off-the-norm'. Lexical analysis is not supposed to go longtimes
         * backwards. For such cases we step a long stretch backwards: A
         * THIRD of the buffer's size! 
         *
         * A meaningful fallback_n would be 64 Bytes. If the buffer's size
         * is for example 512 kB then the backwards_distance of A THIRD means 170
         * kB. This leaves a  safety region which is about 2730 times
         * greater than normal (64 Bytes). After all, lexical analysis means
         * to go **mainly forward** and not backwards.  */
        __quex_assert(buffer != 0x0);
        __quex_debug_buffer_load(buffer, "BACKWARD(entry)\n");
        QUEX_BUFFER_ASSERT_CONSISTENCY(buffer);

        /* (*) Check for the three possibilities mentioned above*/
        if     ( buffer->_input_p == buffer->_memory._back )          { return 0; } /* (1) */
        else if( buffer->_input_p == buffer->_memory._end_of_file_p ) { return 0; } /* (1) */
        else if( buffer->_input_p != buffer->_memory._front ) {
            QUEX_ERROR_EXIT("Call to 'load_backward() but '_input_p' not on buffer border.\n" 
                            "(Check character encoding)");  
        }
        else if( buffer->_content_character_index_begin == 0 ) { return 0; }  /* (2) */
        /*                                                                     * (3) */
        /* HERE: current_p == FRONT OF THE BUFFER!   
         *_______________________________________________________________________________*/
        /* Catch impossible scenario: If the stretch from _input_p to _lexeme_start_p 
         * spans the whole buffer content, then nothing can be loaded into it.           */
        if( buffer->_lexeme_start_p == ContentBack ) { 
            QUEX_NAME(__BufferFiller_on_overflow)(buffer, /* ForwardF */ false);
            return 0;
        }

        /*_______________________________________________________________________________
         * (1) Compute distance to go backwards */
        BackwardDistance = QUEX_NAME(__BufferFiller_backward_compute_backward_distance)(buffer);
        /*_______________________________________________________________________________
         * (2) Compute the stream position of the 'start to read'   
         *  
         * It is not safe to assume that the character size is fixed. Thus it is up to  
         * the input strategy to determine the input position that belongs to a character  
         * position.                                                                     */
        NewContentCharacterIndexBegin = buffer->_content_character_index_begin - BackwardDistance;
        __quex_assert(BackwardDistance != 0); /* if "_index_begin != 0", then backward load must be. */
        me->seek_character_index(me, NewContentCharacterIndexBegin);

        /*_______________________________________________________________________________
         * (3) Load content*/
        buffer->_content_character_index_begin = NewContentCharacterIndexBegin;
        QUEX_NAME(__BufferFiller_backward_copy_backup_region)(buffer, BackwardDistance);
        
        /* -- If file content < buffer size, then the start position of the stream to which  
         *    the buffer refers is always 0 and no backward loading will ever happen.  
         * -- If the file content >= buffer size, then backward loading must always fill  
         *    the buffer. */
#       if defined(QUEX_OPTION_ASSERTS)
        LoadedN = 
#       else
        (void)
#       endif
        QUEX_NAME(__BufferFiller_read_characters)(buffer, ContentFront, BackwardDistance);
        __quex_assert(LoadedN == (size_t)BackwardDistance);

        /*________________________________________________________________________________
         * (4) Adapt pointers */
        QUEX_NAME(__BufferFiller_backward_adapt_pointers)(buffer, BackwardDistance);

        buffer->_content_character_index_end =   buffer->_content_character_index_begin
                                               + (QUEX_NAME(Buffer_text_end)(buffer) - ContentFront);
        /* If the character index in the stream is different from 'old index + LoadedN'
         * then this indicates a 'strange stream' where the stream position increment is
         * not proportional to the number of loaded characters.                              */
        __quex_assert( (size_t)(me->tell_character_index(me) - buffer->_content_character_index_begin) == LoadedN );

        __quex_debug_buffer_load(buffer, "BACKWARD(exit)\n");
        QUEX_BUFFER_ASSERT_CONSISTENCY(buffer);
        QUEX_BUFFER_ASSERT_CONTENT_CONSISTENCY(buffer);

        return (size_t)BackwardDistance;
    }


    QUEX_INLINE ptrdiff_t
    QUEX_NAME(__BufferFiller_backward_compute_backward_distance)(QUEX_NAME(Buffer)* buffer)
    {
        const ptrdiff_t  ContentSize  = (ptrdiff_t)QUEX_NAME(Buffer_content_size)(buffer);
        ptrdiff_t     IntendedBackwardDistance = (ptrdiff_t)-1;
        ptrdiff_t     Distance_InputP_to_LexemeStart = (ptrdiff_t)-1;
        ptrdiff_t     LimitBackwardDist_1 = -1; 
        ptrdiff_t     LimitBackwardDist_2 = -1;
        ptrdiff_t     Limit_1_and_2 = -1;
        ptrdiff_t     BackwardDistance = -1;

        QUEX_BUFFER_ASSERT_CONSISTENCY(buffer);
        /* Copying backward shall **only** happen when new content is to be loaded. In back
         * ward direction, this makes only sense if the lower border was reached.         */
        __quex_assert(buffer->_input_p == buffer->_memory._front);
        /* Also, if the lower border is the begin of file, then there is no sense in
         * loading more in backward direction.                                            */
        __quex_assert(buffer->_content_character_index_begin != 0);
        /* We need to make sure, that the lexeme start pointer remains inside the
         * buffer, so that we do not loose the reference. From current_p == buffer begin
         * it is safe to say that _lexeme_start_p > _input_p (the lexeme starts
         * on a letter not the buffer limit).                                             */
        __quex_assert(buffer->_lexeme_start_p > buffer->_input_p);
        __quex_assert((size_t)(buffer->_lexeme_start_p - buffer->_input_p) < QUEX_NAME(Buffer_content_size)(buffer));

        IntendedBackwardDistance = ContentSize > (ptrdiff_t)3 ? (ptrdiff_t)(ContentSize / 3) 
                                   :                            (ptrdiff_t)1;   
        /* IntendedBackwardDistance > 0 */

        /* Limit 1:
         *
         *     Before:    |C      L                  |
         *
         *     After:     |       C      L           |
         *                 ------->
         *                 backward distance
         *
         *     Lexeme start pointer L shall lie inside the buffer. Thus, it is required:
         *
         *               LimitBackwardDist_1 + (C - L) < size
         *           =>            LimitBackwardDist_1 < size - (C - L)
         *            
         *           =>  LimitBackwardDist_1 > 0, see __quex_assert(...) above. */
        Distance_InputP_to_LexemeStart = buffer->_lexeme_start_p - buffer->_input_p;
        LimitBackwardDist_1            = ContentSize - Distance_InputP_to_LexemeStart; 

        /* Limit 2:
         *     We cannot go before the first character in the stream. 
         *         LimitBackwardDist_2 > 0, see __quex_assert(...) above. */
        LimitBackwardDist_2 = buffer->_content_character_index_begin;


        /* Taking the minimum of all:*/
        /*           Limit_1_and_2 = min(LimitBackwardDist_1 and _2) > 0 */
        Limit_1_and_2 = LimitBackwardDist_1 < LimitBackwardDist_2 ? LimitBackwardDist_1
                        :                                           LimitBackwardDist_2;

        BackwardDistance = IntendedBackwardDistance < Limit_1_and_2 ? IntendedBackwardDistance 
                           :                                          Limit_1_and_2;

        __quex_assert(BackwardDistance > (ptrdiff_t)0);
        return (ptrdiff_t)BackwardDistance;
    }

    QUEX_INLINE void
    QUEX_NAME(__BufferFiller_backward_copy_backup_region)(QUEX_NAME(Buffer)*  buffer, 
                                                          const ptrdiff_t     BackwardDistance)
    {
        const size_t         ContentSize      = QUEX_NAME(Buffer_content_size)(buffer);
        QUEX_TYPE_CHARACTER* ContentFront     = QUEX_NAME(Buffer_content_front)(buffer);

        __quex_assert(BackwardDistance >= (ptrdiff_t)0);
        __quex_assert((size_t)BackwardDistance < ContentSize);

        /* (*) copy content that is already there to its new position.
         *     (copying is much faster then loading new content from file). */
        __QUEX_STD_memmove(ContentFront + BackwardDistance, ContentFront, 
                           (size_t)(ContentSize - (size_t)BackwardDistance)*sizeof(QUEX_TYPE_CHARACTER));

#       ifdef QUEX_OPTION_ASSERTS
        /* Cast to uint8_t to avoid that some smart guy provides a C++ overloading function */
        __QUEX_STD_memset((uint8_t*)ContentFront, (uint8_t)(0xFF), 
                          (size_t)BackwardDistance * sizeof(QUEX_TYPE_CHARACTER)); 
#       endif
    }

    QUEX_INLINE void
    QUEX_NAME(__BufferFiller_backward_adapt_pointers)(QUEX_NAME(Buffer)*  buffer, 
                                                      const ptrdiff_t     BackwardDistance)
    {
        /* -- end of file / end of buffer:*/
        if( buffer->_memory._end_of_file_p ) {
            QUEX_TYPE_CHARACTER*   NewEndOfFileP = buffer->_memory._end_of_file_p + BackwardDistance;
            if( NewEndOfFileP <= buffer->_memory._back ) 
                QUEX_NAME(Buffer_end_of_file_set)(buffer, NewEndOfFileP);
            else  
                QUEX_NAME(Buffer_end_of_file_unset)(buffer);
        }

        buffer->_input_p        += BackwardDistance + 1; 
        buffer->_lexeme_start_p += BackwardDistance;
    }

    QUEX_INLINE void
    QUEX_NAME(__BufferFiller_on_overflow)(QUEX_NAME(Buffer)* buffer, bool ForwardF)
    {
        QUEX_NAME(BufferFiller)* me = buffer->filler;
#       ifdef QUEX_OPTION_INFORMATIVE_BUFFER_OVERFLOW_MESSAGE
        uint8_t               utf8_encoded_str[512]; 
        char                  message[1024];
        const size_t          MessageSize = (size_t)1024;
        uint8_t*              WEnd        = 0x0;
        uint8_t*              witerator   = 0x0; 
        QUEX_TYPE_CHARACTER*        End         = 0x0; 
        const QUEX_TYPE_CHARACTER*  iterator    = 0x0; 
#       endif

        if(    me->_on_overflow == 0x0
            || me->_on_overflow(buffer, ForwardF) == false ) {

#           ifdef QUEX_OPTION_INFORMATIVE_BUFFER_OVERFLOW_MESSAGE
            /* Print out the lexeme start, so that the user has a hint. */
            WEnd        = utf8_encoded_str + 512 - 7;
            witerator   = utf8_encoded_str; 
            End         = buffer->_memory._back; 
            iterator    = buffer->_lexeme_start_p; 

            QUEX_CONVERTER_STRING(QUEX_SETTING_CHARACTER_CODEC, utf8)(&iterator, End, &witerator, WEnd);

            message[0] = '\0';
            /* No use of 'snprintf()' because not all systems seem to support it propperly. */
            __QUEX_STD_strncat(message, 
                               "Distance between lexeme start and current pointer exceeds buffer size.\n"
                               "(tried to load buffer",
                               MessageSize);
            __QUEX_STD_strncat(message, ForwardF ? "forward)" : "backward)",                   MessageSize);
            __QUEX_STD_strncat(message, "As a hint consider the beginning of the lexeme:\n[[", MessageSize);
            __QUEX_STD_strncat(message, (char*)utf8_encoded_str,                               MessageSize);
            __QUEX_STD_strncat(message, "]]\n",                                                MessageSize);

            QUEX_ERROR_EXIT(message);
#           else
            QUEX_ERROR_EXIT("Distance between lexeme start and current pointer exceeds buffer size.\n"
                            "(tried to load buffer forward). Please, compile with option\n\n"
                            "    QUEX_OPTION_INFORMATIVE_BUFFER_OVERFLOW_MESSAGE\n\n"
                            "in order to get a more informative output. Most likely, one of your patterns\n"
                            "eats longer as you inteded it. Alternatively you might want to set the buffer\n"
                            "size to a greate value or use skippers (<skip: [ \\n\\t]> for example).\n");
#           endif /* QUEX_OPTION_INFORMATIVE_BUFFER_OVERFLOW_MESSAGE */
        }
    }

    QUEX_INLINE void 
    QUEX_NAME(BufferFiller_step_forward_n_characters)(QUEX_NAME(BufferFiller)* me,
                                                      const ptrdiff_t          ForwardN)
    { 
        /* STRATEGY: Starting from a certain point in the file we read characters
         *           Convert one-by-one until we reach the given character index. 
         *           This is, of course, incredibly inefficient but safe to work under
         *           all circumstances. Fillers should only rely on this function
         *           in case of no other alternative. Also, caching some information 
         *           about what character index is located at what position may help
         *           to increase speed.                                                */      
#       ifdef QUEX_OPTION_ASSERTS
        const ptrdiff_t TargetIndex = me->tell_character_index(me) + ForwardN;
#       endif

        /* START: We are now at character index 'CharacterIndex - remaining_character_n'.
         * LOOP:  It remains to interpret 'remaining_character_n' number of characters. Since 
         *        the interpretation is best done using a buffer, we do this in chunks.      */ 
        size_t               remaining_character_n = (size_t)ForwardN;
        const size_t         ChunkSize             = QUEX_SETTING_BUFFER_FILLER_SEEK_TEMP_BUFFER_SIZE;
        QUEX_TYPE_CHARACTER  chunk[QUEX_SETTING_BUFFER_FILLER_SEEK_TEMP_BUFFER_SIZE];

        __quex_assert(QUEX_SETTING_BUFFER_FILLER_SEEK_TEMP_BUFFER_SIZE >= 1);

        /* We CANNOT assume that end the end it will hold: 
         *
         *       __quex_assert(me->tell_character_index(me) == TargetIndex);
         *
         * Because, we do not know wether the stream actually has so many characters.     */
        for(; remaining_character_n > ChunkSize; remaining_character_n -= ChunkSize )  
            if( me->read_characters(me, (QUEX_TYPE_CHARACTER*)chunk, ChunkSize) < ChunkSize ) {
                __quex_assert(me->tell_character_index(me) <= TargetIndex);
                return;
            }
        if( remaining_character_n ) 
            me->read_characters(me, (QUEX_TYPE_CHARACTER*)chunk, remaining_character_n);
       
        __quex_assert(me->tell_character_index(me) <= TargetIndex);
    }

    QUEX_INLINE size_t       
    QUEX_NAME(__BufferFiller_read_characters)(QUEX_NAME(Buffer)*    buffer, 
                                              QUEX_TYPE_CHARACTER*  memory, 
                                              const ptrdiff_t       CharacterNToRead)
    {
        const size_t  LoadedN = buffer->filler->read_characters(buffer->filler, memory, (size_t)CharacterNToRead);

        if( buffer->_byte_order_reversion_active_f ) {
            QUEX_NAME(Buffer_reverse_byte_order)(memory, memory + LoadedN);
        }
        return LoadedN;
    }

QUEX_NAMESPACE_MAIN_CLOSE

#include <quex/code_base/temporary_macros_off>

#include <quex/code_base/buffer/Buffer.i>
#if defined(__QUEX_OPTION_CONVERTER)
#   include <quex/code_base/buffer/converter/BufferFiller_Converter.i>
#else
#   include <quex/code_base/buffer/plain/BufferFiller_Plain.i>
#endif

#endif /* __QUEX_INCLUDE_GUARD__BUFFER__BUFFERFILLER_I */

