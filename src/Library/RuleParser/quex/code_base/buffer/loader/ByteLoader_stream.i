/* vim: set ft=c:
 * (C) Frank-Rene Schaefer */
#ifndef  __QUEX_INCLUDE_GUARD__BUFFER__LOADER__BYTE_LOADER_STREAM_I
#define  __QUEX_INCLUDE_GUARD__BUFFER__LOADER__BYTE_LOADER_STREAM_I

#if ! defined(__QUEX_OPTION_PLAIN_C)

#include <fstream>
#include <sstream>

QUEX_NAMESPACE_MAIN_OPEN

template <class StreamType> QUEX_INLINE QUEX_TYPE_STREAM_POSITION  QUEX_NAME(ByteLoader_stream_tell)(QUEX_NAME(ByteLoader)* me);
template <class StreamType> QUEX_INLINE void                       QUEX_NAME(ByteLoader_stream_seek)(QUEX_NAME(ByteLoader)* me, 
                                                                                          QUEX_TYPE_STREAM_POSITION Pos);
template <class StreamType> QUEX_INLINE size_t                     QUEX_NAME(ByteLoader_stream_load)(QUEX_NAME(ByteLoader)* me, void* buffer, const size_t ByteN, bool*);
template <class StreamType> QUEX_INLINE void                       QUEX_NAME(ByteLoader_stream_delete_self)(QUEX_NAME(ByteLoader)* me);
template <class StreamType> QUEX_INLINE bool                       QUEX_NAME(ByteLoader_stream_compare_handle)(const QUEX_NAME(ByteLoader)* alter_ego_A, 
                                                                                                               const QUEX_NAME(ByteLoader)* alter_ego_B);

template <class StreamType> QUEX_INLINE QUEX_NAME(ByteLoader)*
QUEX_NAME(ByteLoader_stream_new)(StreamType* sh)
{
    QUEX_NAME(ByteLoader_stream)<StreamType>* me;

    if( ! sh ) return (QUEX_NAME(ByteLoader)*)0;

    me = (QUEX_NAME(ByteLoader_stream)<StreamType>*)QUEXED(MemoryManager_allocate)(sizeof(QUEX_NAME(ByteLoader_stream)<StreamType>),
                                                                        E_MemoryObjectType_BYTE_LOADER);

    if( ! me ) return (QUEX_NAME(ByteLoader)*)0;

    QUEX_NAME(ByteLoader_stream_construct)(me, sh);
    return &me->base;
}

QUEX_INLINE QUEX_NAME(ByteLoader)*    
QUEX_NAME(ByteLoader_stream_new_from_file_name)(const char* FileName)
{
    std::ifstream*  sh = new std::ifstream(FileName, std::ios_base::binary | std::ios::in);
    QUEX_NAME(ByteLoader)*     alter_ego;
    if( ! sh || ! *sh ) {
        return (QUEX_NAME(ByteLoader)*)0;
    }
    alter_ego = QUEX_NAME(ByteLoader_stream_new)(sh);
    if( ! alter_ego ) {
        return (QUEX_NAME(ByteLoader)*)0;
    }
    alter_ego->handle_ownership = E_Ownership_LEXICAL_ANALYZER;
    return alter_ego;
}

template <class StreamType> QUEX_INLINE void
QUEX_NAME(ByteLoader_stream_construct)(QUEX_NAME(ByteLoader_stream)<StreamType>* me, 
                                       StreamType*                               sh)
{
    /* IMPORTANT: input_handle must be set BEFORE call to constructor!
     *            Constructor does call 'tell()'                             */
    me->input_handle = sh;

    QUEX_NAME(ByteLoader_construct)(&me->base,
                         QUEX_NAME(ByteLoader_stream_tell)<StreamType>,
                         QUEX_NAME(ByteLoader_stream_seek)<StreamType>,
                         QUEX_NAME(ByteLoader_stream_load)<StreamType>,
                         QUEX_NAME(ByteLoader_stream_delete_self)<StreamType>,
                         QUEX_NAME(ByteLoader_stream_compare_handle)<StreamType>);
}

template <class StreamType> QUEX_INLINE void
QUEX_NAME(ByteLoader_stream_delete_self)(QUEX_NAME(ByteLoader)* alter_ego)
{
    QUEX_NAME(ByteLoader_stream)<StreamType>* me = (QUEX_NAME(ByteLoader_stream)<StreamType>*)alter_ego;

    if( me->input_handle && me->base.handle_ownership == E_Ownership_LEXICAL_ANALYZER ) {
        delete me->input_handle;
    }
    QUEXED(MemoryManager_free)(me, E_MemoryObjectType_BYTE_LOADER);
}

/* The 'char_type' of a stream determines the atomic size of elements which are
 * read from the stream. It is unrelated to QUEX_TYPE_CHARACTER which
 * determines the size of a buffer element.                                  */
template <class StreamType> QUEX_INLINE QUEX_TYPE_STREAM_POSITION    
QUEX_NAME(ByteLoader_stream_tell)(QUEX_NAME(ByteLoader)* alter_ego)            
{ 
    QUEX_NAME(ByteLoader_stream)<StreamType>*       me = (QUEX_NAME(ByteLoader_stream)<StreamType>*)alter_ego;
    const QUEX_TYPE_STREAM_POSITION      CharSize = \
         (QUEX_TYPE_STREAM_POSITION)sizeof(typename StreamType::char_type);
    std::streampos                       Position = me->input_handle->tellg();

    return (QUEX_TYPE_STREAM_POSITION)(Position * CharSize); 
}

template <class StreamType> QUEX_INLINE void    
QUEX_NAME(ByteLoader_stream_seek)(QUEX_NAME(ByteLoader)*    alter_ego, 
                                  QUEX_TYPE_STREAM_POSITION Pos) 
{ 
    QUEX_NAME(ByteLoader_stream)<StreamType>*       me = (QUEX_NAME(ByteLoader_stream)<StreamType>*)alter_ego;
    const QUEX_TYPE_STREAM_POSITION      CharSize = \
         (QUEX_TYPE_STREAM_POSITION)sizeof(typename StreamType::char_type);
    std::streampos                       Target   = (std::streampos)(Pos / CharSize);

    me->input_handle->clear();                    /* Clear any iostate flag. */
    me->input_handle->seekg(Target); 
}

template <class StreamType> QUEX_INLINE size_t  
QUEX_NAME(ByteLoader_stream_load)(QUEX_NAME(ByteLoader)*  alter_ego, 
                                  void*        buffer, 
                                  const size_t ByteN, 
                                  bool*        end_of_stream_f) 
{ 
    QUEX_NAME(ByteLoader_stream)<StreamType>*    me = (QUEX_NAME(ByteLoader_stream)<StreamType>*)alter_ego;
    const QUEX_TYPE_STREAM_POSITION   CharSize = \
         (QUEX_TYPE_STREAM_POSITION)sizeof(typename StreamType::char_type);

    if( ! ByteN ) return (size_t)0;

    me->input_handle->read((typename StreamType::char_type*)buffer, 
                           (std::streamsize)(ByteN / CharSize)); 

    const size_t loaded_char_n = (size_t)(me->input_handle->gcount());

    *end_of_stream_f = me->input_handle->eof();

    if( (! *end_of_stream_f) && me->input_handle->fail() ) {
        throw std::runtime_error("Fatal error during stream reading.");
    }

    /* std::fprintf(stdout, "tell 1 = %i, loaded_char_n = %i\n", (long)(me->input_handle->tellg()), loaded_char_n);*/
    return (size_t)(loaded_char_n * CharSize);
}

template <class StreamType> QUEX_INLINE bool  
QUEX_NAME(ByteLoader_stream_compare_handle)(const QUEX_NAME(ByteLoader)* alter_ego_A, 
                                            const QUEX_NAME(ByteLoader)* alter_ego_B) 
/* RETURNS: true  -- if A and B point to the same StreamType object.
 *          false -- else.                                                   */
{ 
    const QUEX_NAME(ByteLoader_stream)<StreamType>* A = (QUEX_NAME(ByteLoader_stream)<StreamType>*)(alter_ego_A);
    const QUEX_NAME(ByteLoader_stream)<StreamType>* B = (QUEX_NAME(ByteLoader_stream)<StreamType>*)(alter_ego_B);

    return A->input_handle == B->input_handle;
}

QUEX_NAMESPACE_MAIN_CLOSE

#endif /* __QUEX_OPTION_PLAIN_C                 */
#endif /*  __QUEX_INCLUDE_GUARD__BUFFER__LOADER__BYTE_LOADER_STREAM_I */

