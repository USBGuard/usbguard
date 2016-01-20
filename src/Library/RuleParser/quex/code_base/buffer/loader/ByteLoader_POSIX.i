/* vim: ft=c:
 * (C) Frank-Rene Schaefer */
#ifndef  __QUEX_INCLUDE_GUARD__BUFFER__LOADER__BYTE_LOADER_POSIX_I
#define  __QUEX_INCLUDE_GUARD__BUFFER__LOADER__BYTE_LOADER_POSIX_I

#include <quex/code_base/MemoryManager>
#include <quex/code_base/buffer/loader/ByteLoader_POSIX>

QUEX_NAMESPACE_MAIN_OPEN

QUEX_INLINE void                       QUEX_NAME(ByteLoader_POSIX_construct)(QUEX_NAME(ByteLoader_POSIX)* me, int fd);
QUEX_INLINE QUEX_TYPE_STREAM_POSITION  QUEX_NAME(ByteLoader_POSIX_tell)(QUEX_NAME(ByteLoader)* me);
QUEX_INLINE void                       QUEX_NAME(ByteLoader_POSIX_seek)(QUEX_NAME(ByteLoader)* me, 
                                                                        QUEX_TYPE_STREAM_POSITION Pos);
QUEX_INLINE size_t                     QUEX_NAME(ByteLoader_POSIX_load)(QUEX_NAME(ByteLoader)* me, 
                                                                        void* buffer, const size_t ByteN, 
                                                                        bool*);
QUEX_INLINE void                       QUEX_NAME(ByteLoader_POSIX_delete_self)(QUEX_NAME(ByteLoader)* me);
QUEX_INLINE bool                       QUEX_NAME(ByteLoader_POSIX_compare_handle)(const QUEX_NAME(ByteLoader)* alter_ego_A, 
                                                                       const QUEX_NAME(ByteLoader)* alter_ego_B);

QUEX_INLINE QUEX_NAME(ByteLoader)*    
QUEX_NAME(ByteLoader_POSIX_new)(int fd)
{
    QUEX_NAME(ByteLoader_POSIX)* me;

    if( fd == -1 ) return (QUEX_NAME(ByteLoader)*)0;
    me = (QUEX_NAME(ByteLoader_POSIX)*)QUEXED(MemoryManager_allocate)(sizeof(QUEX_NAME(ByteLoader_POSIX)),
                                                           E_MemoryObjectType_BYTE_LOADER);
    if( ! me ) return (QUEX_NAME(ByteLoader)*)0;
    QUEX_NAME(ByteLoader_POSIX_construct)(me, fd);
    return &me->base;
}

QUEX_INLINE QUEX_NAME(ByteLoader)*    
QUEX_NAME(ByteLoader_POSIX_new_from_file_name)(const char* FileName)
{
    int          fd = open(FileName, O_RDONLY);
    QUEX_NAME(ByteLoader)*  alter_ego;

    if( fd == -1 ) {
        return (QUEX_NAME(ByteLoader)*)0;
    }
    alter_ego = QUEX_NAME(ByteLoader_POSIX_new)(fd);
    if( ! alter_ego ) {
        return (QUEX_NAME(ByteLoader)*)0;
    }
    alter_ego->handle_ownership = E_Ownership_LEXICAL_ANALYZER;
    return alter_ego;
}

QUEX_INLINE void
QUEX_NAME(ByteLoader_POSIX_construct)(QUEX_NAME(ByteLoader_POSIX)* me, int fd)
{
    /* IMPORTANT: fd must be set BEFORE call to constructor!
     *            Constructor does call 'tell()'                             */
    me->fd = fd;

    QUEX_NAME(ByteLoader_construct)(&me->base,
                                    QUEX_NAME(ByteLoader_POSIX_tell),
                                    QUEX_NAME(ByteLoader_POSIX_seek),
                                    QUEX_NAME(ByteLoader_POSIX_load),
                                    QUEX_NAME(ByteLoader_POSIX_delete_self),
                                    QUEX_NAME(ByteLoader_POSIX_compare_handle));

    /* A POSIX file handle is always in binary mode.                         */
    me->base.binary_mode_f = true;
}

QUEX_INLINE void    
QUEX_NAME(ByteLoader_POSIX_delete_self)(QUEX_NAME(ByteLoader)* alter_ego)
{
    QUEX_NAME(ByteLoader_POSIX)* me = (QUEX_NAME(ByteLoader_POSIX)*)(alter_ego);

    if( me->fd && me->base.handle_ownership == E_Ownership_LEXICAL_ANALYZER ) {
        close(me->fd);
    }
    QUEXED(MemoryManager_free)(me, E_MemoryObjectType_BYTE_LOADER);
}

QUEX_INLINE QUEX_TYPE_STREAM_POSITION    
QUEX_NAME(ByteLoader_POSIX_tell)(QUEX_NAME(ByteLoader)* me)            
{ 
    /* Use 'lseek(current position + 0)' to get the current position.        */
    return (QUEX_TYPE_STREAM_POSITION)lseek(((QUEX_NAME(ByteLoader_POSIX)*)me)->fd, 0, SEEK_CUR); 
}

QUEX_INLINE void    
QUEX_NAME(ByteLoader_POSIX_seek)(QUEX_NAME(ByteLoader)*    me, 
                                 QUEX_TYPE_STREAM_POSITION Pos) 
{ lseek(((QUEX_NAME(ByteLoader_POSIX)*)me)->fd, (long)Pos, SEEK_SET); }

QUEX_INLINE size_t  
QUEX_NAME(ByteLoader_POSIX_load)(QUEX_NAME(ByteLoader)* me, 
                                 void*                  buffer, 
                                 const size_t           ByteN, 
                                 bool*                  end_of_stream_f) 
{ 
    /* The POSIX interface does not allow to detect end of file upon reading.
     * The caller will realize end of stream by a return of zero bytes.      */
    int n = read(((QUEX_NAME(ByteLoader_POSIX)*)me)->fd, buffer, ByteN); 
    *end_of_stream_f = false;
    return n;
}

QUEX_INLINE bool  
QUEX_NAME(ByteLoader_POSIX_compare_handle)(const QUEX_NAME(ByteLoader)* alter_ego_A, 
                                           const QUEX_NAME(ByteLoader)* alter_ego_B) 
/* RETURNS: true  -- if A and B point to the same POSIX object.
 *          false -- else.                                                   */
{ 
    const QUEX_NAME(ByteLoader_POSIX)* A = (QUEX_NAME(ByteLoader_POSIX)*)(alter_ego_A);
    const QUEX_NAME(ByteLoader_POSIX)* B = (QUEX_NAME(ByteLoader_POSIX)*)(alter_ego_B);

    return A->fd == B->fd;
}

QUEX_NAMESPACE_MAIN_CLOSE

#endif /*  __QUEX_INCLUDE_GUARD__BUFFER__LOADER__BYTE_LOADER_POSIX_I */

