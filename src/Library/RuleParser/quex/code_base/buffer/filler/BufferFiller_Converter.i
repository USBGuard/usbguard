/* -*- C++ -*-  vim: set syntax=cpp:
 * (C) 2007-2015 Frank-Rene Schaefer  */
#ifndef  __QUEX_INCLUDE_GUARD__BUFFER__FILLER__BUFFER_FILLER_CONVERTER_I
#define  __QUEX_INCLUDE_GUARD__BUFFER__FILLER__BUFFER_FILLER_CONVERTER_I

#include <quex/code_base/MemoryManager>
#include <quex/code_base/buffer/filler/BufferFiller>
#include <quex/code_base/buffer/filler/BufferFiller_Converter>
#include <quex/code_base/compatibility/iconv-argument-types.h>


QUEX_NAMESPACE_MAIN_OPEN

QUEX_INLINE void
QUEX_NAME(BufferFiller_Converter_construct)(QUEX_NAME(BufferFiller_Converter)* me, 
                                            QUEX_NAME(ByteLoader)*            byte_loader,
                                            QUEX_NAME(Converter)*  converter,
                                            size_t                 RawMemorySize);

QUEX_INLINE void   
QUEX_NAME(BufferFiller_Converter_stomach_clear)(QUEX_NAME(BufferFiller)* alter_ego);

QUEX_INLINE void   
QUEX_NAME(BufferFiller_Converter_destruct_self)(QUEX_NAME(BufferFiller)* alter_ego);

QUEX_INLINE ptrdiff_t   
QUEX_NAME(BufferFiller_Converter_stomach_byte_n)(QUEX_NAME(BufferFiller)* alter_ego);

QUEX_INLINE size_t 
QUEX_NAME(BufferFiller_Converter_load_characters)(QUEX_NAME(BufferFiller)*  alter_ego,
                                                       QUEX_TYPE_CHARACTER* RegionBeginP, 
                                                       const size_t         N,
                                                       bool*                end_of_stream_f);
QUEX_INLINE void 
QUEX_NAME(BufferFiller_Converter_fill_prepare)(QUEX_NAME(BufferFiller)*   alter_ego,
                                               QUEX_TYPE_CHARACTER*       RegionBeginP,
                                               QUEX_TYPE_CHARACTER*       RegionEndP,
                                               void**                     begin_p,
                                               const void**               end_p);

QUEX_INLINE ptrdiff_t 
QUEX_NAME(BufferFiller_Converter_fill_finish)(QUEX_NAME(BufferFiller)*   alter_ego,
                                              QUEX_TYPE_CHARACTER*       insertion_p,
                                              const QUEX_TYPE_CHARACTER* BufferEnd,
                                              const void*                ContentEnd);

QUEX_INLINE void   
QUEX_NAME(RawBuffer_init)(QUEX_NAME(RawBuffer)* me, 
                          uint8_t* Begin, size_t SizeInBytes);
QUEX_INLINE void 
QUEX_NAME(RawBuffer_move_away_passed_content)(QUEX_NAME(RawBuffer)*  me);

QUEX_INLINE bool 
QUEX_NAME(RawBuffer_load)(QUEX_NAME(RawBuffer)*  me,
                          QUEX_NAME(ByteLoader)*            byte_loader, 
                          bool*                  end_of_stream_f);

QUEX_INLINE void
QUEX_NAME(BufferFiller_remove_spurious_BOM)(QUEX_NAME(BufferFiller_Converter)* me,
                                            QUEX_TYPE_CHARACTER**              buffer_insertion_p,
                                            QUEX_TYPE_CHARACTER*               RegionBeginP);

QUEX_INLINE QUEX_NAME(BufferFiller)*
QUEX_NAME(BufferFiller_Converter_new)(QUEX_NAME(ByteLoader)* byte_loader,
                                      QUEX_NAME(Converter)*  converter,
                                      size_t                 RawMemorySize)
{ 
    QUEX_NAME(BufferFiller_Converter)*  me;
    __quex_assert(RawMemorySize >= 6);  /* UTF-8 char can be 6 bytes long    */

    if( ! converter ) {
        return (QUEX_NAME(BufferFiller)*)0;
    }
    else if( byte_loader && ! byte_loader->binary_mode_f ) {
        /* Binary mode is ABSOLUTELY REQUIRED for converters, otherwise the 
         * positioning with respect to the raw buffer becomes unreliable.    */
        __QUEX_STD_printf("! BufferFiller_Converter_new: QUEX_NAME(ByteLoader )is not in binary mode. !\n");
        __QUEX_STD_printf("! Has file been opened in binary mode?                          !\n");
        return (QUEX_NAME(BufferFiller)*)0;
    }

    /* The 'BufferFiller_Converter' is the same host for all converters.
     * Converters are pointed to by 'converter',                             */
    me = (QUEX_NAME(BufferFiller_Converter)*) \
          QUEXED(MemoryManager_allocate)(sizeof(QUEX_NAME(BufferFiller_Converter)),
                                         E_MemoryObjectType_BUFFER_FILLER);
    if( ! me) return (QUEX_NAME(BufferFiller)*)0;

    QUEX_NAME(BufferFiller_Converter_construct)(me, byte_loader, converter, RawMemorySize);

    return &me->base;

}

QUEX_INLINE void
QUEX_NAME(BufferFiller_Converter_construct)(QUEX_NAME(BufferFiller_Converter)* me, 
                                            QUEX_NAME(ByteLoader)*             byte_loader,
                                            QUEX_NAME(Converter)*              converter,
                                            size_t                             RawMemorySize)
{
    /* A linear relationship between stream position and character index 
     * requires that: (1) The input stream is in 'binary mode'. That is, the 
     * stream position is proportional to the number of bytes that lie 
     * behind. (2) The input codec is of fixed size, i.e. 
     * converter->byte_n_per_character != -1.                                */ 
    ptrdiff_t   byte_n_per_character = byte_loader && byte_loader->binary_mode_f ? 
                                       converter->byte_n_per_character : -1;
    uint8_t*    raw_memory;

    /* Binary mode is absolutely required for converters, otherwise the 
     * positioning with respect to the raw buffer becomes unreliable.        */
    __quex_assert( ! byte_loader || byte_loader->binary_mode_f );

    QUEX_NAME(BufferFiller_setup)(&me->base,
                                  QUEX_NAME(BufferFiller_Converter_load_characters),
                                  QUEX_NAME(BufferFiller_Converter_stomach_byte_n),
                                  QUEX_NAME(BufferFiller_Converter_stomach_clear),
                                  QUEX_NAME(BufferFiller_Converter_destruct_self),
                                  QUEX_NAME(BufferFiller_Converter_fill_prepare),
                                  QUEX_NAME(BufferFiller_Converter_fill_finish),
                                  byte_loader,
                                  byte_n_per_character);

    /* Initialize the conversion operations                                  */
    me->converter = converter;
    me->converter->virginity_f = true;

    /* Initialize the raw buffer that holds the plain bytes of the input file
     * (setup to trigger initial reload)                                     */
    raw_memory = QUEXED(MemoryManager_allocate)(RawMemorySize, 
                                                E_MemoryObjectType_BUFFER_RAW);
    QUEX_NAME(RawBuffer_init)(&me->raw_buffer, raw_memory, RawMemorySize);

    QUEX_ASSERT_RAW_BUFFER(&me->raw_buffer);
}

QUEX_INLINE ptrdiff_t   
QUEX_NAME(BufferFiller_Converter_stomach_byte_n)(QUEX_NAME(BufferFiller)*  alter_ego)
/* RETURNS: Number of bytes that were read from the input stream, but remained
 *                 inside the 'stomach' without being filled into the drain.
 *          '-1'   reports that the BufferFiller cannot tell how many bytes are
 *                 in the stomach.                                           */
{
    QUEX_NAME(BufferFiller_Converter)* me = (QUEX_NAME(BufferFiller_Converter)*)alter_ego;
    ptrdiff_t  byte_n;
    ptrdiff_t  converter_byte_n;

    byte_n = me->raw_buffer.fill_end_p - me->raw_buffer.next_to_convert_p;

    /* me->converter->stomach_byte_n   == 0   => converter does NOT keep any 
     *                                           bytes in stomach.
     * me->converter->stomach_byte_n() == -1  => converter CANNOT tell how 
     *                                           many bytes in stomach.      */
    if( me->converter->stomach_byte_n ) {
        converter_byte_n = me->converter->stomach_byte_n(me->converter);
        if( converter_byte_n == - 1) return (ptrdiff_t)-1;
        byte_n += converter_byte_n;
    }

    return byte_n;
}

QUEX_INLINE void   
QUEX_NAME(BufferFiller_Converter_stomach_clear)(QUEX_NAME(BufferFiller)* alter_ego)
{
    QUEX_NAME(BufferFiller_Converter)* me = (QUEX_NAME(BufferFiller_Converter)*)alter_ego;
    QUEX_NAME(RawBuffer_init)(&me->raw_buffer, 0, 0);
    if( me->converter->stomach_clear ) me->converter->stomach_clear(me->converter);
}

QUEX_INLINE void   
QUEX_NAME(BufferFiller_Converter_destruct_self)(QUEX_NAME(BufferFiller)* alter_ego)
/* destruct_self: Free resources occupied by 'me' BUT NOT 'myself'.
 * delete_self:   Free resources occupied by 'me' AND 'myself'.              */
{ 
    QUEX_NAME(BufferFiller_Converter)* me = (QUEX_NAME(BufferFiller_Converter)*)alter_ego;

    if( ! me ) return;

    QUEX_ASSERT_RAW_BUFFER(&me->raw_buffer);

    if( me->converter && me->converter->ownership == E_Ownership_LEXICAL_ANALYZER ) {
        me->converter->delete_self(me->converter); 
    }

    QUEXED(MemoryManager_free)((void*)me->raw_buffer.begin,
                               E_MemoryObjectType_BUFFER_RAW); 
}

QUEX_INLINE size_t 
QUEX_NAME(BufferFiller_Converter_load_characters)(QUEX_NAME(BufferFiller)*  alter_ego,
                                                  QUEX_TYPE_CHARACTER*      RegionBeginP, 
                                                  const size_t              N,
                                                  bool*                     end_of_stream_f)
/* Loads content into the raw buffer, convert it and write it to the engine's
 * buffer. The region where to write into the engine's buffer expands from
 * 'RegionBeginP' to 'N' characters after it.                                
 *
 * RETURNS: Number of loaded characters into the given region.               */
{
    QUEX_NAME(BufferFiller_Converter)* me = (QUEX_NAME(BufferFiller_Converter)*)alter_ego;
    QUEX_NAME(RawBuffer)*              raw = &me->raw_buffer;
    QUEX_TYPE_CHARACTER*               buffer_insertion_p = RegionBeginP;
    const QUEX_TYPE_CHARACTER*         BufferRegionEnd    = &RegionBeginP[N];
    ptrdiff_t                          converted_character_n;
    bool                               drain_filled_f;
    bool                               load_complete_f;
    bool                               raw_end_of_stream_f;
#   if 0
    int                                i;
    QUEX_TYPE_CHARACTER*               buffer_insertion_begin_p;
#   endif

    __quex_assert(me->converter);
    __quex_assert(alter_ego); 
    __quex_assert(RegionBeginP); 
    QUEX_ASSERT_RAW_BUFFER(raw);
    /* NOT: QUEX_IF_ASSERTS_poison(RegionBeginP, &RegionBeginP[N]);
     * The buffer must remain intact, in case that not all is loaded.        */

    /* Some converters keep some content internally. So, it is a more general
     * solution to convert first and reload new bytes upon need.             */
    drain_filled_f = me->converter->convert(me->converter, 
                                            &raw->next_to_convert_p, raw->fill_end_p,
                                            &buffer_insertion_p,     BufferRegionEnd);

    QUEX_NAME(BufferFiller_remove_spurious_BOM)(me, &buffer_insertion_p, RegionBeginP);

    /* Convert, as long as the following two hold:
     *  (i)  Drain is not totally filled.
     *  (ii) Loading of 'to-be-converted bytes' received enough to fill the
     *       raw buffer to its limits. The contrary indicates that there is 
     *       an transmission interuption, or even and end-of-stream. In both
     *       cases, the analyser may continue, before the next try.          */
    raw_end_of_stream_f = false;
    load_complete_f     = true;
    while( (! drain_filled_f) && load_complete_f ) {
        __quex_assert(buffer_insertion_p < BufferRegionEnd);  /* '==' break  */

        if( ! raw_end_of_stream_f ) {
            load_complete_f = QUEX_NAME(RawBuffer_load)(&me->raw_buffer, me->base.byte_loader,
                                                        &raw_end_of_stream_f);
        } else  {
            load_complete_f = false;
        }

        /*  next_to_convert_p == raw->fill_end_p => nothing happens.         */
        drain_filled_f = me->converter->convert(me->converter, 
                                                &raw->next_to_convert_p, raw->fill_end_p,
                                                &buffer_insertion_p,     BufferRegionEnd);

        QUEX_NAME(BufferFiller_remove_spurious_BOM)(me, &buffer_insertion_p, RegionBeginP);
    }

    me->converter->virginity_f = false;

    if( (! drain_filled_f) && raw_end_of_stream_f ) {
       if( raw->next_to_convert_p == raw->fill_end_p ) {
           *end_of_stream_f = true;
       } else {
           /* There are still bytes, but they were not converted.               
            * ('drain_filled_f' => remainder is converted next time)         */
           __QUEX_STD_printf("Error. At end of file, byte sequence not interpreted as character.");
       }
    }
    /* NOT: QUEX_IF_ASSERTS_poison(buffer_insertion_p, BufferRegionEnd);
     *      Buffer MUST be left as is, in case of ERROR!                     */
    __quex_assert(BufferRegionEnd >= buffer_insertion_p);

    /* 'buffer_insertion_p' was updated by 'convert' and points behind the 
     * last byte that was converted.                                         */ 
    converted_character_n = buffer_insertion_p - RegionBeginP;

    return (size_t)converted_character_n;
}

QUEX_INLINE void 
QUEX_NAME(BufferFiller_Converter_fill_prepare)(QUEX_NAME(BufferFiller)*   alter_ego,
                                               QUEX_TYPE_CHARACTER*       RegionBeginP,
                                               QUEX_TYPE_CHARACTER*       RegionEndP,
                                               void**                     begin_p,
                                               const void**               end_p)
{
    QUEX_NAME(BufferFiller_Converter)* me = (QUEX_NAME(BufferFiller_Converter)*)alter_ego;
    QUEX_NAME(RawBuffer)*              raw = &me->raw_buffer;
    (void)RegionBeginP; (void)RegionEndP;

    QUEX_NAME(RawBuffer_move_away_passed_content)(&me->raw_buffer);

    *begin_p = (void*)raw->fill_end_p; 
    *end_p   = (void*)raw->memory_end;
}

QUEX_INLINE ptrdiff_t 
QUEX_NAME(BufferFiller_Converter_fill_finish)(QUEX_NAME(BufferFiller)*   alter_ego,
                                              QUEX_TYPE_CHARACTER*       RegionBeginP,
                                              const QUEX_TYPE_CHARACTER* RegionEndP,
                                              const void*                FilledEndP_raw)
/* Converts what has been filled into the 'raw_buffer' until 'FilledEndP
 * and stores it into the buffer.                                            */
{
    QUEX_NAME(BufferFiller_Converter)*  me  = (QUEX_NAME(BufferFiller_Converter)*)alter_ego;
    QUEX_NAME(RawBuffer)*               raw = &me->raw_buffer;
    QUEX_TYPE_CHARACTER*                insertion_p = RegionBeginP;
    uint8_t*                            FilledEndP = (uint8_t*)FilledEndP_raw;

    __quex_assert(FilledEndP >= raw->next_to_convert_p);
    __quex_assert(FilledEndP <= raw->memory_end);

    /* Assert triggers => FilledEndP points WRONGLY BEHIND terminating zero. 
     * (FilledEndP, may point to it, at max.)                                */
    __quex_assert(   FilledEndP     <= raw->next_to_convert_p 
                  || FilledEndP[-1] != QUEX_SETTING_BUFFER_LIMIT_CODE);

    raw->fill_end_p = FilledEndP;   
    QUEX_ASSERT_RAW_BUFFER(raw);

    me->converter->convert(me->converter, 
                           &raw->next_to_convert_p, raw->fill_end_p,
                           &insertion_p,            RegionEndP);

    QUEX_NAME(BufferFiller_remove_spurious_BOM)(me, &insertion_p, RegionBeginP);
    me->converter->virginity_f = false;
    
    QUEX_ASSERT_RAW_BUFFER(raw);
    return insertion_p - RegionBeginP;
}


QUEX_INLINE void
QUEX_NAME(BufferFiller_remove_spurious_BOM)(QUEX_NAME(BufferFiller_Converter)* me,
                                            QUEX_TYPE_CHARACTER**              buffer_insertion_p,
                                            QUEX_TYPE_CHARACTER*               RegionBeginP)
{
    uint32_t  first_character;

    if( *buffer_insertion_p == RegionBeginP ) return;

    first_character = (uint32_t)RegionBeginP[0];    /* avoid warning */
    if( first_character != 0xFEFF ) return;

    if( ! me->converter->virginity_f ) {
        QUEX_ERROR_EXIT("Converter produced BOM upon not-first call to 'convert'\n"
                        "Better make sure that converter NEVER produces BOM.\n"
                        "(May be, by specifiying the endianness of 'FromCoding' or 'ToCoding')\n");
    }

    __QUEX_STD_memmove(RegionBeginP, &RegionBeginP[1], 
                       (size_t)(*buffer_insertion_p - &RegionBeginP[1]) * sizeof(QUEX_TYPE_CHARACTER)); 

    *buffer_insertion_p = &(*buffer_insertion_p)[-1];
}

QUEX_NAMESPACE_MAIN_CLOSE

#include <quex/code_base/buffer/filler/BufferFiller.i>
#include <quex/code_base/buffer/filler/BufferFiller_Converter_RawBuffer.i>
#include <quex/code_base/buffer/filler/converter/Converter.i>

#ifdef QUEX_OPTION_CONVERTER_ICONV
#   include <quex/code_base/buffer/filler/converter/iconv/Converter_IConv.i>
#endif
#ifdef QUEX_OPTION_CONVERTER_ICU
#   include <quex/code_base/buffer/filler/converter/icu/Converter_ICU.i>
#endif


#endif /*  __QUEX_INCLUDE_GUARD__BUFFER__FILLER__BUFFER_FILLER_CONVERTER_I */
