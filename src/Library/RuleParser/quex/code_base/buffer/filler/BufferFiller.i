/* -*- C++ -*- vim: set syntax=cpp: */
#ifndef  __QUEX_INCLUDE_GUARD__BUFFER__FILLER__BUFFER_FILLER_I
#define  __QUEX_INCLUDE_GUARD__BUFFER__FILLER__BUFFER_FILLER_I

#include <quex/code_base/definitions>
#include <quex/code_base/buffer/Buffer>
#include <quex/code_base/MemoryManager>
#include <quex/code_base/buffer/filler/BufferFiller>
#include <quex/code_base/buffer/Buffer_debug>

QUEX_NAMESPACE_MAIN_OPEN

QUEX_INLINE bool       QUEX_NAME(BufferFiller_character_index_seek)(QUEX_NAME(BufferFiller)*         me, 
                                                                    const QUEX_TYPE_STREAM_POSITION  CharacterIndex);
QUEX_INLINE QUEX_TYPE_STREAM_POSITION 
                       QUEX_NAME(BufferFiller_character_index_tell)(QUEX_NAME(BufferFiller)* me);
QUEX_INLINE bool       QUEX_NAME(BufferFiller_character_index_step_to)(QUEX_NAME(BufferFiller)*        me,
                                                                       const QUEX_TYPE_STREAM_POSITION TargetCI);
QUEX_INLINE void       QUEX_NAME(BufferFiller_character_index_reset_backup)(QUEX_NAME(BufferFiller)* me, 
                                                          QUEX_TYPE_STREAM_POSITION Backup_character_index_next_to_fill, 
                                                          ptrdiff_t                 BackupStomachByteN, 
                                                          QUEX_TYPE_STREAM_POSITION BackupByteLoaderPosition);
QUEX_INLINE void       QUEX_NAME(BufferFiller_reverse_byte_order)(QUEX_TYPE_CHARACTER*       Begin, 
                                                                  const QUEX_TYPE_CHARACTER* End);

QUEX_INLINE void       QUEX_NAME(BufferFiller_delete_self)(QUEX_NAME(BufferFiller)*); 

                       
QUEX_INLINE QUEX_NAME(BufferFiller)*
QUEX_NAME(BufferFiller_new)(QUEX_NAME(ByteLoader)*  byte_loader, 
                            QUEX_NAME(Converter)*   converter,
                            const size_t            TranslationBufferMemorySize)
{
    QUEX_NAME(BufferFiller)* filler;
    (void)TranslationBufferMemorySize;

    /* byte_loader = 0; possible if memory is filled manually.               */
    if( converter ) {
        filler = QUEX_NAME(BufferFiller_Converter_new)(byte_loader, converter, 
                                                       TranslationBufferMemorySize);
    }
    else {
        filler = QUEX_NAME(BufferFiller_Plain_new)(byte_loader); 
    }
    
    return filler;
}

QUEX_INLINE QUEX_NAME(BufferFiller)* 
QUEX_NAME(BufferFiller_new_DEFAULT)(QUEX_NAME(ByteLoader)*   byte_loader, 
                                    const char*              InputCodecName) 
{
#   if   defined(QUEX_OPTION_CONVERTER_ICONV)
    QUEX_NAME(Converter)* converter = QUEX_NAME(Converter_IConv_new)(InputCodecName, 0);
#   elif defined(QUEX_OPTION_CONVERTER_ICU)
    QUEX_NAME(Converter)* converter = QUEX_NAME(Converter_ICU_new)(InputCodecName, 0);
#   else
    QUEX_NAME(Converter)* converter = (QUEX_NAME(Converter)*)0;
#   endif

    if( converter ) {
        converter->ownership = E_Ownership_LEXICAL_ANALYZER;
        if( ! InputCodecName ) {
#           ifndef QUEX_OPTION_WARNING_ON_PLAIN_FILLER_DISABLED
            __QUEX_STD_printf("Warning: No character encoding name specified, while this\n" \
                              "Warning: analyzer was generated for use with a converter.\n" \
                              "Warning: Please, consult the documentation about the constructor\n" \
                              "Warning: or the reset function. If it is desired to do a plain\n" \
                              "Warning: buffer filler with this setup, you might want to disable\n" \
                              "Warning: this warning with the macro:\n" \
                              "Warning:     QUEX_OPTION_WARNING_ON_PLAIN_FILLER_DISABLED\n");
#           endif
            return (QUEX_NAME(BufferFiller)*)0x0;
        }
    } 

    return QUEX_NAME(BufferFiller_new)(byte_loader, converter,
                                       QUEX_SETTING_TRANSLATION_BUFFER_SIZE);
}

QUEX_INLINE void       
QUEX_NAME(BufferFiller_delete_self)(QUEX_NAME(BufferFiller)* me)
{ 
    if( ! me ) return;

    if( me->byte_loader && me->byte_loader->ownership == E_Ownership_LEXICAL_ANALYZER ) {
        QUEX_NAME(ByteLoader_delete)(&me->byte_loader);
    }

    /* destruct_self: Free resources occupied by 'me' BUT NOT 'myself'.
     * delete_self:   Free resources occupied by 'me' AND 'myself'.          */
    if( me->derived.destruct_self ) {
        me->derived.destruct_self(me);
    }

    QUEXED(MemoryManager_free)((void*)me, E_MemoryObjectType_BUFFER_FILLER);
}

QUEX_INLINE void    
QUEX_NAME(BufferFiller_setup)(QUEX_NAME(BufferFiller)*   me,
                              size_t       (*derived_load_characters)(QUEX_NAME(BufferFiller)*,
                                                                           QUEX_TYPE_CHARACTER*, 
                                                                           const size_t, 
                                                                           bool*),
                              ptrdiff_t    (*stomach_byte_n)(QUEX_NAME(BufferFiller)*),
                              void         (*stomach_clear)(QUEX_NAME(BufferFiller)*),
                              void         (*derived_destruct_self)(QUEX_NAME(BufferFiller)*),
                              void         (*derived_fill_prepare)(QUEX_NAME(BufferFiller)*  me,
                                                                   QUEX_TYPE_CHARACTER*      RegionBeginP,
                                                                   QUEX_TYPE_CHARACTER*      RegionEndP,
                                                                   void**                    begin_p,
                                                                   const void**              end_p),
                              ptrdiff_t    (*derived_fill_finish)(QUEX_NAME(BufferFiller)*   me,
                                                                  QUEX_TYPE_CHARACTER*       BeginP,
                                                                  const QUEX_TYPE_CHARACTER* EndP,
                                                                  const void*                FilledEndP),
                              QUEX_NAME(ByteLoader)*  byte_loader,
                              ptrdiff_t    ByteNPerCharacter)
{
    __quex_assert(me);
    __quex_assert(derived_load_characters);
    __quex_assert(derived_destruct_self);

    /* Support for buffer filling without user interaction                   */
    me->stomach_byte_n          = stomach_byte_n;
    me->stomach_clear           = stomach_clear;
    me->input_character_tell    = QUEX_NAME(BufferFiller_character_index_tell);
    me->input_character_seek    = QUEX_NAME(BufferFiller_character_index_seek);
    me->derived.load_characters = derived_load_characters;
    me->derived.destruct_self   = derived_destruct_self;
    me->delete_self             = QUEX_NAME(BufferFiller_delete_self);

    /* Support for manual buffer filling.                                    */
    me->derived.fill_prepare    = derived_fill_prepare;
    me->derived.fill_finish     = derived_fill_finish;

    me->byte_loader                    = byte_loader;

    me->_byte_order_reversion_active_f = false;
    me->character_index_next_to_fill   = 0;
    me->byte_n_per_character           = ByteNPerCharacter;

    /* Default: External ownership                                           */
    me->ownership = E_Ownership_EXTERNAL;
}

QUEX_INLINE void
QUEX_NAME(BufferFiller_reset)(QUEX_NAME(BufferFiller)* me, QUEX_NAME(ByteLoader)* new_byte_loader)
/* Resets the BufferFiller with a new QUEX_NAME(ByteLoader).                            */
{
    __quex_assert(new_byte_loader);

    if( new_byte_loader != me->byte_loader ) {
        if( QUEX_NAME(ByteLoader_is_equivalent)(new_byte_loader, me->byte_loader) ) {
            __QUEX_STD_printf("Upon 'reset': current and new QUEX_NAME(ByteLoader )objects contain same input handle.\n"); 
        }
        QUEX_NAME(ByteLoader_delete)(&me->byte_loader);
        me->byte_loader = new_byte_loader;
    }
    QUEX_NAME(BufferFiller_character_index_reset)(me);
}

QUEX_INLINE ptrdiff_t       
QUEX_NAME(BufferFiller_load)(QUEX_NAME(BufferFiller)*  me, 
                             QUEX_TYPE_CHARACTER*      LoadP, 
                             const ptrdiff_t           LoadN,
                             QUEX_TYPE_STREAM_POSITION StartCharacterIndex,
                             bool*                     end_of_stream_f)
/* Seeks the input position StartCharacterIndex and loads 'LoadN' 
 * characters into the engine's buffer starting from 'LoadP'.
 *
 * RETURNS: Number of loaded characters.                                     */
{
    ptrdiff_t   loaded_n;

    /* (1) Seek to the position where loading shall start.                       
     *                                                                       */
    if( ! me->input_character_seek(me, StartCharacterIndex) ) {
        return 0;
    }
    __quex_assert(me->character_index_next_to_fill == StartCharacterIndex);

    /* (2) Load content into the given region.                                   
     *                                                                       */
    loaded_n = (ptrdiff_t)me->derived.load_characters(me, LoadP, (size_t)LoadN,
                                                      end_of_stream_f);
    __quex_assert(loaded_n <= LoadN);
    me->character_index_next_to_fill += loaded_n;

    /* (3) Optionally reverse the byte order.                                    
     *                                                                       */
    if( me->_byte_order_reversion_active_f ) {
        QUEX_NAME(BufferFiller_reverse_byte_order)(LoadP, &LoadP[loaded_n]);
    }

    return loaded_n;
}


QUEX_INLINE void
QUEX_NAME(BufferFiller_reverse_byte_order)(QUEX_TYPE_CHARACTER*       Begin, 
                                           const QUEX_TYPE_CHARACTER* End)
{
    uint8_t              tmp = 0xFF;
    QUEX_TYPE_CHARACTER* iterator = 0x0;

    switch( sizeof(QUEX_TYPE_CHARACTER) ) {
    default:
        __quex_assert(false);
        break;
    case 1:
        /* Nothing to be done */
        break;
    case 2:
        for(iterator=Begin; iterator != End; ++iterator) {
            tmp = *(((uint8_t*)iterator) + 0);
            *(((uint8_t*)iterator) + 0) = *(((uint8_t*)iterator) + 1);
            *(((uint8_t*)iterator) + 1) = tmp;
        }
        break;
    case 4:
        for(iterator=Begin; iterator != End; ++iterator) {
            tmp = *(((uint8_t*)iterator) + 0);
            *(((uint8_t*)iterator) + 0) = *(((uint8_t*)iterator) + 3);
            *(((uint8_t*)iterator) + 3) = tmp;
            tmp = *(((uint8_t*)iterator) + 1);
            *(((uint8_t*)iterator) + 1) = *(((uint8_t*)iterator) + 2);
            *(((uint8_t*)iterator) + 2) = tmp;
        }
        break;
    }
}

QUEX_NAMESPACE_MAIN_CLOSE

#include <quex/code_base/buffer/Buffer.i>
#include <quex/code_base/buffer/loader/ByteLoader.i>
#include <quex/code_base/buffer/filler/BufferFiller_navigation.i>
#include <quex/code_base/buffer/filler/BufferFiller_Converter.i>
#include <quex/code_base/buffer/filler/BufferFiller_Plain.i>

#endif /* __QUEX_INCLUDE_GUARD__BUFFER__BUFFERFILLER_I */

