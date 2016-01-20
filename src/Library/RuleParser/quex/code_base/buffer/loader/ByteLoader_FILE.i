/* vim: set ft=c:
 * (C) Frank-Rene Schaefer */
#ifndef  __QUEX_INCLUDE_GUARD__BUFFER__LOADER__BYTE_LOADER_FILE_I
#define  __QUEX_INCLUDE_GUARD__BUFFER__LOADER__BYTE_LOADER_FILE_I

#include <quex/code_base/MemoryManager>

QUEX_NAMESPACE_MAIN_OPEN

QUEX_INLINE void                       QUEX_NAME(ByteLoader_FILE_construct)(QUEX_NAME(ByteLoader_FILE)* me, __QUEX_STD_FILE* fh);
QUEX_INLINE QUEX_TYPE_STREAM_POSITION  QUEX_NAME(ByteLoader_FILE_tell)(QUEX_NAME(ByteLoader)* me);
QUEX_INLINE void                       QUEX_NAME(ByteLoader_FILE_seek)(QUEX_NAME(ByteLoader)* me, QUEX_TYPE_STREAM_POSITION Pos);
QUEX_INLINE size_t                     QUEX_NAME(ByteLoader_FILE_load)(QUEX_NAME(ByteLoader)* me, void* buffer, const size_t ByteN, bool*);
QUEX_INLINE void                       QUEX_NAME(ByteLoader_FILE_delete_self)(QUEX_NAME(ByteLoader)* me);
QUEX_INLINE bool                       QUEX_NAME(ByteLoader_FILE_compare_handle)(const QUEX_NAME(ByteLoader)* alter_ego_A, 
                                                                                 const QUEX_NAME(ByteLoader)* alter_ego_B);

QUEX_INLINE QUEX_NAME(ByteLoader)*    
QUEX_NAME(ByteLoader_FILE_new)(FILE* fh, bool BinaryModeF)
{
    QUEX_NAME(ByteLoader_FILE)* me;
   
    if( ! fh ) return (QUEX_NAME(ByteLoader)*)0;

    me = (QUEX_NAME(ByteLoader_FILE)*)QUEXED(MemoryManager_allocate)(sizeof(QUEX_NAME(ByteLoader_FILE)),
                                                          E_MemoryObjectType_BYTE_LOADER);
    if( ! me ) return (QUEX_NAME(ByteLoader)*)0;

    QUEX_NAME(ByteLoader_FILE_construct)(me, fh);
    me->base.binary_mode_f = BinaryModeF;

    return &me->base;
}

QUEX_INLINE QUEX_NAME(ByteLoader)*    
QUEX_NAME(ByteLoader_FILE_new_from_file_name)(const char* FileName)
{
    __QUEX_STD_FILE*        fh = fopen(FileName, "rb");
    QUEX_NAME(ByteLoader)*  alter_ego;

    if( ! fh ) {
        return (QUEX_NAME(ByteLoader)*)0;
    }
    alter_ego = QUEX_NAME(ByteLoader_FILE_new)(fh, true);
    if( ! alter_ego ) {
        return (QUEX_NAME(ByteLoader)*)0;
    }
    alter_ego->handle_ownership = E_Ownership_LEXICAL_ANALYZER;
    return alter_ego;
}

QUEX_INLINE void
QUEX_NAME(ByteLoader_FILE_construct)(QUEX_NAME(ByteLoader_FILE)* me, __QUEX_STD_FILE* fh)
{
    /* IMPORTANT: input_handle must be set BEFORE call to constructor!
     *            Constructor does call 'tell()'                             */
    me->input_handle = fh;

    QUEX_NAME(ByteLoader_construct)(&me->base,
                         QUEX_NAME(ByteLoader_FILE_tell),
                         QUEX_NAME(ByteLoader_FILE_seek),
                         QUEX_NAME(ByteLoader_FILE_load),
                         QUEX_NAME(ByteLoader_FILE_delete_self),
                         QUEX_NAME(ByteLoader_FILE_compare_handle));
}

QUEX_INLINE void    
QUEX_NAME(ByteLoader_FILE_delete_self)(QUEX_NAME(ByteLoader)* alter_ego)
{
    QUEX_NAME(ByteLoader_FILE)* me = (QUEX_NAME(ByteLoader_FILE)*)(alter_ego);

    if( me->input_handle && me->base.handle_ownership == E_Ownership_LEXICAL_ANALYZER ) {
        fclose(me->input_handle);
    }
    QUEXED(MemoryManager_free)(me, E_MemoryObjectType_BYTE_LOADER);
}

QUEX_INLINE QUEX_TYPE_STREAM_POSITION    
QUEX_NAME(ByteLoader_FILE_tell)(QUEX_NAME(ByteLoader)* alter_ego)            
{ 
    QUEX_NAME(ByteLoader_FILE)* me = (QUEX_NAME(ByteLoader_FILE)*)(alter_ego);

    return (QUEX_TYPE_STREAM_POSITION)ftell(me->input_handle); 
}

QUEX_INLINE void    
QUEX_NAME(ByteLoader_FILE_seek)(QUEX_NAME(ByteLoader)* alter_ego, QUEX_TYPE_STREAM_POSITION Pos) 
{ 
    QUEX_NAME(ByteLoader_FILE)* me = (QUEX_NAME(ByteLoader_FILE)*)(alter_ego);
    fseek(me->input_handle, (long)Pos, SEEK_SET); 
}

QUEX_INLINE size_t  
QUEX_NAME(ByteLoader_FILE_load)(QUEX_NAME(ByteLoader)* alter_ego, 
                                void*                  buffer, 
                                const size_t           ByteN, 
                                bool*                  end_of_stream_f) 
{ 
    QUEX_NAME(ByteLoader_FILE)* me = (QUEX_NAME(ByteLoader_FILE)*)(alter_ego);
    size_t                      loaded_byte_n = fread(buffer, 1, ByteN, me->input_handle); 
#   if 0
    int    i;
    printf("#load @%i:         [", (int)ftell(((QUEX_NAME(ByteLoader_FILE)*)me)->input_handle));
    for(i=0; i<loaded_byte_n; ++i) printf("%02X.", ((uint8_t*)buffer)[i]);
    printf("]\n");
#   endif
    *end_of_stream_f = feof(me->input_handle) ? true : false;
    return loaded_byte_n;
}

QUEX_INLINE bool  
QUEX_NAME(ByteLoader_FILE_compare_handle)(const QUEX_NAME(ByteLoader)* alter_ego_A, 
                                          const QUEX_NAME(ByteLoader)* alter_ego_B) 
/* RETURNS: true  -- if A and B point to the same FILE object.
 *          false -- else.                                                   */
{ 
    const QUEX_NAME(ByteLoader_FILE)* A = (QUEX_NAME(ByteLoader_FILE)*)(alter_ego_A);
    const QUEX_NAME(ByteLoader_FILE)* B = (QUEX_NAME(ByteLoader_FILE)*)(alter_ego_B);

    return A->input_handle == B->input_handle;
}

QUEX_NAMESPACE_MAIN_CLOSE

#endif /*  __QUEX_INCLUDE_GUARD__BUFFER__LOADER__BYTE_LOADER_FILE_I */
