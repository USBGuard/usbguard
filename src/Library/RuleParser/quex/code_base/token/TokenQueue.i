/* -*- C++ -*- vim: set syntax=cpp: 
 * (C) 2004-2009 Frank-Rene Schaefer                               */
#ifndef __QUEX_INCLUDE_GUARD__TOKEN__TOKEN_QUEUE_I
#define __QUEX_INCLUDE_GUARD__TOKEN__TOKEN_QUEUE_I

#include <quex/code_base/definitions>
#include <quex/code_base/asserts>
#include <quex/code_base/MemoryManager>

/* NOTE: QUEX_TYPE_TOKEN must be defined at this place! */


QUEX_NAMESPACE_MAIN_OPEN

    QUEX_INLINE void
    QUEX_NAME(TokenQueue_construct)(QUEX_NAME(TokenQueue)* me, 
                                    QUEX_TYPE_TOKEN*       Memory, 
                                    const size_t           N)
    /* me:     The token queue.
     * Memory: Pointer to memory of token queue, 0x0 --> no initial memory.
     * N:      Number of token objects that the array can carry.               */
    {
#       if ! defined(QUEX_OPTION_USER_MANAGED_TOKEN_MEMORY)
        QUEX_TYPE_TOKEN* iterator   = 0x0;
#       endif
        QUEX_TYPE_TOKEN* memory_end = &Memory[N];

        __quex_assert(Memory != 0x0);
        __quex_assert(N != 0);

#       if ! defined(QUEX_OPTION_USER_MANAGED_TOKEN_MEMORY)
        /* Call placement new (plain constructor) for all tokens in chunk. */
        for(iterator = Memory; iterator != memory_end; ++iterator) {
            QUEX_NAME_TOKEN(construct)(iterator);
        }
#       endif
        QUEX_NAME(TokenQueue_init)(me, Memory, memory_end); 
    }

    QUEX_INLINE void
    QUEX_NAME(TokenQueue_reset)(QUEX_NAME(TokenQueue)* me) 
    {                                                    
        me->read_iterator  = (QUEX_TYPE_TOKEN*)me->begin; 
        me->write_iterator = (QUEX_TYPE_TOKEN*)me->begin; 
    }

    QUEX_INLINE void
    QUEX_NAME(TokenQueue_init)(QUEX_NAME(TokenQueue)* me, 
                               QUEX_TYPE_TOKEN* Memory, 
                               QUEX_TYPE_TOKEN* MemoryEnd) 
    {
        me->begin                   = Memory;                           
        me->end                     = MemoryEnd;                        
        me->end_minus_safety_border = MemoryEnd - QUEX_SETTING_TOKEN_QUEUE_SAFETY_BORDER;         
        QUEX_NAME(TokenQueue_reset)(me);                                
    }


    QUEX_INLINE void
    QUEX_NAME(TokenQueue_destruct)(QUEX_NAME(TokenQueue)* me)
    {
#       if ! defined(QUEX_OPTION_USER_MANAGED_TOKEN_MEMORY)
        QUEX_TYPE_TOKEN* iterator = 0x0;
        /* Call explicit destructors for all tokens in array */
        for(iterator = me->begin; iterator != me->end; ++iterator) {
            QUEX_NAME_TOKEN(destruct)(iterator);
        }
        /* The memory chunk for the token queue itself is located 
         * inside the analyzer object. Thus, no explicit free is
         * necessary. In case of user managed token queue memory
         * the user takes care of the deletion.                   */
#       endif
    }

    QUEX_INLINE void   
    QUEX_NAME(TokenQueue_remainder_get)(QUEX_NAME(TokenQueue)* me,
                                        QUEX_TYPE_TOKEN**      begin,
                                        QUEX_TYPE_TOKEN**      end)
    {
        *begin = me->read_iterator;
        *end   = me->write_iterator;
        QUEX_NAME(TokenQueue_reset)(me);
    }

    QUEX_INLINE void 
    QUEX_NAME(TokenQueue_memory_get)(QUEX_NAME(TokenQueue)* me,
                                     QUEX_TYPE_TOKEN**      memory,
                                     size_t*                n)
    {
        *memory = me->begin;
        *n      = (size_t)(me->end - me->begin);
    }

    QUEX_INLINE bool QUEX_NAME(TokenQueue_is_full)(QUEX_NAME(TokenQueue)* me) 
    { return me->write_iterator >= me->end_minus_safety_border; }
    
    QUEX_INLINE bool QUEX_NAME(TokenQueue_is_empty)(QUEX_NAME(TokenQueue)* me)
    { return me->read_iterator == me->write_iterator; }

    QUEX_INLINE QUEX_TYPE_TOKEN* QUEX_NAME(TokenQueue_pop)(QUEX_NAME(TokenQueue)* me)
    { return me->read_iterator++; }

    QUEX_INLINE QUEX_TYPE_TOKEN* QUEX_NAME(TokenQueue_begin)(QUEX_NAME(TokenQueue)* me)
    { return me->begin; }

    QUEX_INLINE QUEX_TYPE_TOKEN* QUEX_NAME(TokenQueue_back)(QUEX_NAME(TokenQueue)* me)
    { return me->end - 1; }

    QUEX_INLINE size_t QUEX_NAME(TokenQueue_available_n)(QUEX_NAME(TokenQueue)* me) 
    { return (size_t)(me->end - me->write_iterator); }

#ifdef QUEX_OPTION_ASSERTS
    QUEX_INLINE void  
    QUEX_ASSERT_TOKEN_QUEUE_AFTER_WRITE(QUEX_NAME(TokenQueue)* me)
    {
        __quex_assert(me->begin != 0x0);
        __quex_assert(me->read_iterator  >= me->begin);
        __quex_assert(me->write_iterator >= me->read_iterator);
        /* If the following breaks, it means that the given queue size was to small */
        __quex_assert(me->begin == 0x0 || me->end_minus_safety_border >= me->begin + 1);
        if( me->write_iterator > me->end ) { 
            QUEX_ERROR_EXIT("Error: Token queue overflow. This happens if too many tokens are sent\n"
                            "       as a reaction to one single pattern match. Use quex's command line\n"
                            "       option --token-queue-safety-border, or define the macro\n"
                            "       QUEX_SETTING_TOKEN_QUEUE_SAFETY_BORDER with a greater value.\n"); 
        }
    }
    QUEX_INLINE void  
    QUEX_TOKEN_QUEUE_ASSERT(QUEX_NAME(TokenQueue)* me)
    {
        QUEX_ASSERT_TOKEN_QUEUE_AFTER_WRITE(me);
        if( me->write_iterator == me->end ) { 
            QUEX_ERROR_EXIT("Error: Token queue overflow. This happens if too many tokens are sent\n"
                            "       as a reaction to one single pattern match. Use quex's command line\n"
                            "       option --token-queue-safety-border, or define the macro\n"
                            "       QUEX_SETTING_TOKEN_QUEUE_SAFETY_BORDER with a greater value.\n"); 
        }
    }
#else
#   define QUEX_TOKEN_QUEUE_ASSERT(me)             /* empty */
#   define QUEX_ASSERT_TOKEN_QUEUE_AFTER_WRITE(me) /* empty */
#endif

#if 1
QUEX_INLINE void
QUEX_NAME(TokenQueueRemainder_save)(QUEX_NAME(TokenQueueRemainder)* me, QUEX_NAME(TokenQueue)* token_queue)
{
    QUEX_TOKEN_QUEUE_ASSERT(token_queue);
    /* State of the token queue at the entry of this function:
     *
     *                           [A0] [B0]   [A1] [B1]   [A2] [B2]
     *                            |    |      |    |      |   | 
     *    |    |      |    |      |    |      |    |      |   | 
     *  [ token 1  ][ token 2  ][ token 3  ][ token 4  ][ token 5  ][ ....
     *                          |                                   |
     *                          read_iterator                       write_iterator
     *                                          
     *
     * 1. Step: Allocate a plain chunk of memory, to carry the remaining tokens:
     *
     *                           [ store 1  ][ store 2  ][ store 3  ]
     *
     *          The elements of this plain chunk of memory are neither subject
     *          to constructor nor destructor calls.
     *
     * 2. Step: Make a plain copy of the tokens of the remainder (from read_iterator
     *          to write_iterator.
     *
     *                        [A0] [B0]   [A1] [B1]   [A2] [B2]
     *                         |    |      |    |      |   | 
     *                         |    |      |    |      |   | 
     *                       [ store 1  ][ store 2  ][ store 3  ]
     *
     *      As a consequence, the objects to which the original tokens referred
     *      are now referred by the stored tokens. However, at the time of 'restore'
     *      the content is copied at the beginning of the queue. 
     *
     *      !! Thus, the following scenerio is conceivable:
     *      !!
     *      !!    [A0] [B0]   [A1] [B1]   [A2] [B2]   [A1] [B1]   [A2] [B2]
     *      !!     |    |      |    |      |    |      |    |      |   | 
     *      !!     |    |      |    |      |    |      |    |      |   | 
     *      !!   [ token 1  ][ token 2  ][ token 3  ][ token 4  ][ token 5  ][ ....
     *      !!   |                                   |
     *      !!   begin                               |
     *      !!   |<--------- store size ------------>|
     *      !!
     *      !! If this was happening, then the destructor for the objects A1, B1, ... 
     *      !! would be called twice at the destruction time of the token queue!       
     *      !!
     *      !! PREVENTION: See next step.
     *
     * 3. Step: Calling placement new on token objects that are saved:
     *          Resulting original token queue:
     *
     *                           [X1] [X1]   [Y2] [Y2]   [Z0] [Z0]   
     *                            |    |      |    |      |   | 
     *    |    |      |    |      |    |      |    |      |   | 
     *  [ token 1  ][ token 2  ][ token 3  ][ token 4  ][ token 5  ][ ....
     *                          |                                   |
     *                          read_iterator                       write_iterator
     *
     *  Note, that token 3 originally not subject to 'double occurence'. However,
     *  it may be overwritten by filling the queue, and then references to objects
     *  would get lost.                                                         */
    me->size = (size_t)(token_queue->write_iterator - token_queue->read_iterator);
    if( me->size != 0 ) {
        QUEX_TYPE_TOKEN* iterator = 0x0;
        
        /* Step 1: allocate plain chunk of memory.                              */
        me->token_list = (QUEX_TYPE_TOKEN*)QUEXED(MemoryManager_allocate)(sizeof(QUEX_TYPE_TOKEN) * me->size, 
                                                                          E_MemoryObjectType_TOKEN_ARRAY);
        if( me->token_list == 0x0 ) {
            QUEX_ERROR_EXIT("Memory allocation error on request for token array.");
        }

        /* Step 2: copy plain chunk of memory                                   */
        __QUEX_STD_memcpy((void*)me->token_list, (void*)token_queue->read_iterator, sizeof(QUEX_TYPE_TOKEN) * me->size);

        /* Step 3: Call cleaning placement new on objects which are subject to
         *         potential double deletion.                                   */
        for(iterator = token_queue->read_iterator; 
            iterator != token_queue->write_iterator; ++iterator) {
            QUEX_NAME_TOKEN(construct)(iterator);
        }
    }
    QUEX_NAME(TokenQueue_reset)(token_queue);

    QUEX_TOKEN_QUEUE_ASSERT(token_queue);
}

QUEX_INLINE void
QUEX_NAME(TokenQueueRemainder_restore)(QUEX_NAME(TokenQueueRemainder)* me, QUEX_NAME(TokenQueue)* token_queue)
{
    QUEX_TYPE_TOKEN* iterator = 0x0;
    /* NOTE: When a token queue remainder is restored, this happens as a result
     *       of 'return from included file'. The return from an included file
     *       is triggered by a TERMINATION token. By definition, the TERMINATION
     *       is the last token to be sent. When the user detects a TERMINATION
     *       token, the read_iterator == write_iterator, which means that the
     *       token queue is empty. => Thus, the 'refill' can start from the beginning.  
     *       THIS IS TRUE WHEN THE INLCUDE_PUSH, INCLUDE_POP HAPPENS FROM OUTSIDE
     *       THE LEXICAL ANALYZER ENGINE.                                            */
    if( ! QUEX_NAME(TokenQueue_is_empty)(token_queue) ) {
        QUEX_ERROR_EXIT("Token queue not empty on return from included file. This can\n"
                        "only happen if include handling was done from inside analyzer\n"
                        "actions. Please, consider directory demo/005 for an example to\n"
                        "handle file inclusion.\n");
    }
    QUEX_TOKEN_QUEUE_ASSERT(token_queue);

    if( me->size != 0 ) {
        /* Step 1: Call explicit destructors for token objects that are overwritten  */
        for(iterator = token_queue->begin; 
            iterator != token_queue->begin + me->size; ++iterator) {
            QUEX_NAME_TOKEN(destruct)(iterator);
        }
        /* Step 2: Plain copy of objects stored in the 'remainder store'             */
        __QUEX_STD_memcpy((void*)token_queue->begin, (void*)me->token_list, sizeof(QUEX_TYPE_TOKEN) * me->size);

        /* Step 3: De-Allocate the remainder objects                                 
         *         NO explicit destructor calls, since the referred objects are now
         *         referred from inside the 'real' token queue.                      */
        QUEXED(MemoryManager_free)(me->token_list, E_MemoryObjectType_TOKEN_ARRAY);
    }
    /* Reset the read and write iterators */
    token_queue->read_iterator  = token_queue->begin;
    token_queue->write_iterator = token_queue->begin + me->size;

    QUEX_TOKEN_QUEUE_ASSERT(token_queue);
}
#endif

QUEX_NAMESPACE_MAIN_CLOSE

#endif /* __QUEX_INCLUDE_GUARD__TOKEN__TOKEN_QUEUE_I */
