/* -*- C++ -*- vim:set syntax=cpp:
 * (C) Frankt-Rene Schaefer
 * ABSOLUTELY NO WARRANTY               */
#ifndef __QUEX_INCLUDE_GUARD__ANALYZER__MEMBER__MODE_HANDLING_I
#define __QUEX_INCLUDE_GUARD__ANALYZER__MEMBER__MODE_HANDLING_I

#include <quex/code_base/definitions>
#include <quex/code_base/analyzer/Mode>

QUEX_NAMESPACE_MAIN_OPEN

    QUEX_INLINE QUEX_NAME(Mode)*
    QUEX_NAME(get_mode)(QUEX_TYPE_ANALYZER* me) 
    { return me->__current_mode_p; }

    QUEX_INLINE int
    QUEX_NAME(get_mode_id)(const QUEX_TYPE_ANALYZER* me)
    { return me->__current_mode_p->id; }

    QUEX_INLINE const char*
    QUEX_NAME(get_mode_name)(const QUEX_TYPE_ANALYZER* me)
    { return me->__current_mode_p->name; }

    QUEX_INLINE void 
    QUEX_NAME(set_mode_brutally)(QUEX_TYPE_ANALYZER* me, QUEX_NAME(Mode)* ModeP) 
    { 
#       ifdef     QUEX_OPTION_DEBUG_SHOW_MODES
        if( me->__current_mode_p != 0x0 ) {
            __QUEX_STD_fprintf(stderr, "| Mode change from %s\n", me->__current_mode_p->name);
            __QUEX_STD_fprintf(stderr, "|             to   %s\n", ModeP->name);
        } else {
            __QUEX_STD_fprintf(stderr, "| Mode change to %s\n", ModeP->name);
        }
#       endif

        me->__current_mode_p          = ModeP;
        me->current_analyzer_function = ModeP->analyzer_function; 
    }

    QUEX_INLINE void
    QUEX_NAME(set_mode_brutally_by_id)(QUEX_TYPE_ANALYZER* me, const int ModeID)
    { QUEX_NAME(set_mode_brutally)(me, me->mode_db[ModeID]); }

    QUEX_INLINE void    
    QUEX_NAME(enter_mode)(QUEX_TYPE_ANALYZER* me, /* NOT const*/ QUEX_NAME(Mode)* TargetMode) 
    {
#       ifdef __QUEX_OPTION_ON_ENTRY_HANDLER_PRESENT
        QUEX_NAME(Mode)* SourceMode = me->__current_mode_p;
#       endif

#       ifdef __QUEX_OPTION_ON_EXIT_HANDLER_PRESENT
        me->__current_mode_p->on_exit(me, TargetMode);
#       endif

        QUEX_NAME(set_mode_brutally)(me, TargetMode);

#       ifdef __QUEX_OPTION_ON_ENTRY_HANDLER_PRESENT
        TargetMode->on_entry(me, SourceMode);         
#       endif
    }

    QUEX_INLINE QUEX_NAME(Mode)*
    QUEX_NAME(map_mode_id_to_mode)(QUEX_TYPE_ANALYZER* me, const int ModeID)
    { 
        __quex_assert(ModeID >= 0);
        __quex_assert(ModeID < __QUEX_SETTING_MAX_MODE_CLASS_N); 
        return me->mode_db[ModeID]; 
    }

    QUEX_INLINE int  
    QUEX_NAME(map_mode_to_mode_id)(const QUEX_TYPE_ANALYZER* me, const QUEX_NAME(Mode)* Mode)
    { (void)me; return Mode->id; }

    QUEX_INLINE void 
    QUEX_NAME(pop_mode)(QUEX_TYPE_ANALYZER* me) 
    { 
        __quex_assert(me->_mode_stack.end != me->_mode_stack.begin);
        --(me->_mode_stack.end);
        QUEX_NAME(enter_mode)(me, *me->_mode_stack.end); 
    }

    QUEX_INLINE void
    QUEX_NAME(pop_drop_mode)(QUEX_TYPE_ANALYZER* me) 
    { 
        __quex_assert(me->_mode_stack.end != me->_mode_stack.begin);
        --(me->_mode_stack.end);
        /* do not care about what was popped */
    }
        
    QUEX_INLINE void       
    QUEX_NAME(push_mode)(QUEX_TYPE_ANALYZER* me, QUEX_NAME(Mode)* new_mode) 
    { 
#       ifdef QUEX_OPTION_ASSERTS
        if( me->_mode_stack.end == me->_mode_stack.memory_end ) 
            QUEX_ERROR_EXIT(__QUEX_MESSAGE_MODE_STACK_OVERFLOW);
#       endif
        *me->_mode_stack.end = me->__current_mode_p;
        ++(me->_mode_stack.end);
        QUEX_NAME(enter_mode)(me, new_mode); 
    }

#   ifndef __QUEX_OPTION_PLAIN_C
    QUEX_INLINE QUEX_NAME(Mode)&
    QUEX_MEMBER(mode)()
    { return *QUEX_NAME(get_mode)(this); }
    
    QUEX_INLINE int
    QUEX_MEMBER(mode_id)() const
    { return QUEX_NAME(get_mode_id)(this); }
    
    QUEX_INLINE const char*
    QUEX_MEMBER(mode_name)() const
    { return QUEX_NAME(get_mode_name)(this); }

    QUEX_INLINE void
    QUEX_MEMBER(set_mode_brutally)(const int ModeID)
    { QUEX_NAME(set_mode_brutally_by_id)(this, ModeID); }

    QUEX_INLINE void 
    QUEX_MEMBER(set_mode_brutally)(const QUEX_NAME(Mode)& TheMode) 
    { QUEX_NAME(set_mode_brutally)(this, ((QUEX_NAME(Mode)*)&TheMode)); }

    QUEX_INLINE void    
    QUEX_MEMBER(enter_mode)(/* NOT const*/ QUEX_NAME(Mode)& TargetMode) 
    { QUEX_NAME(enter_mode)(this, &TargetMode); }

    QUEX_INLINE QUEX_NAME(Mode)&
    QUEX_MEMBER(map_mode_id_to_mode)(const int ModeID)
    { return *(QUEX_NAME(map_mode_id_to_mode)(this, ModeID)); }

    QUEX_INLINE int  
    QUEX_MEMBER(map_mode_to_mode_id)(const QUEX_NAME(Mode)& TheMode) const
    { return QUEX_NAME(map_mode_to_mode_id)(this, &TheMode); }

    QUEX_INLINE void 
    QUEX_MEMBER(operator)<<(const int ModeID) 
    { enter_mode(*(QUEX_NAME(map_mode_id_to_mode)(this, ModeID))); }

    QUEX_INLINE void 
    QUEX_MEMBER(operator)<<(/* NOT const*/ QUEX_NAME(Mode)& TheMode) 
    { enter_mode(TheMode); }

    QUEX_INLINE void 
    QUEX_MEMBER(pop_mode)() 
    { QUEX_NAME(pop_mode)(this); }

    QUEX_INLINE void
    QUEX_MEMBER(pop_drop_mode)() 
    { QUEX_NAME(pop_drop_mode)(this); }

    QUEX_INLINE void       
    QUEX_MEMBER(push_mode)(QUEX_NAME(Mode)& new_mode) 
    { QUEX_NAME(push_mode)(this, &new_mode); }

#   endif

QUEX_NAMESPACE_MAIN_CLOSE

#endif /* __QUEX_INCLUDE_GUARD__ANALYZER__MEMBER__MODE_HANDLING_I */
