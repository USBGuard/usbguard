#ifndef __QUEX_INCLUDE_GUARD__ANALYZER__C_ADAPTIONS_H
#define __QUEX_INCLUDE_GUARD__ANALYZER__C_ADAPTIONS_H

#ifdef self_accumulator_add
/* Token / Token Policy _____________________________________________________*/
#   undef self_token_take_text

/* Modes ____________________________________________________________________*/
#   undef self_current_mode_p
#   undef self_current_mode_id
#   undef self_current_mode_name
/* Map: mode id to mode and vice versa */
#   undef self_map_mode_id_to_mode_p
#   undef self_map_mode_p_to_mode_id
/* Changing Modes */
#   undef self_set_mode_brutally
#   undef self_enter_mode
/* Changing Modes with stack */ 
#   undef self_pop_mode
#   undef self_pop_drop_mode
#   undef self_push_mode

/* Accumulator ______________________________________________________________*/
#   undef self_accumulator_add
#   undef self_accumulator_add_character
#   undef self_accumulator_clear
#   undef self_accumulator_flush
#   undef self_accumulator_is_empty
/* Indentation/Counter _____________________________________________________*/
#   ifdef  QUEX_OPTION_LINE_NUMBER_COUNTING
#   undef  self_line_number            
#   undef  self_line_number_at_begin 
#   undef  self_line_number_at_begin_set
#   undef  self_line_number_at_end   
#   endif
#   ifdef  QUEX_OPTION_COLUMN_NUMBER_COUNTING
#   undef  self_column_number          
#   undef  self_column_number_at_begin 
#   undef  self_column_number_at_begin_set
#   undef  self_column_number_at_end   
#   endif
#   ifdef QUEX_OPTION_INDENTATION_TRIGGER
#   undef self_indentation                    
#   undef self_disable_next_indentation_event 
#   endif
#endif

/* Token / Token Policy 
 * (NOTE: Macros for token sending are defined separately in file 'member/token-sending'.
 *        Those macros have to perform a little 'type loose').                            */
#define self_token_take_text(Begin, End) \
        QUEX_NAME_TOKEN(take_text)(__QUEX_CURRENT_TOKEN_P, &self, (Begin), (End))

/* Modes */
#define self_current_mode_p()     /* QUEX_NAME(Mode)* */ QUEX_NAME(mode)(&self)
#define self_current_mode_id()    /* int */              QUEX_NAME(mode_id)(&self)
#define self_current_mode_name()  /* const char* */      QUEX_NAME(mode_name)(&self)

/* Map: mode id to mode and vice versa */
#define self_map_mode_id_to_mode_p(ID)    QUEX_NAME(map_mode_id_to_mode)(&self, (ID))
#define self_map_mode_p_to_mode_id(ModeP) QUEX_NAME(map_mode_to_mode_id)(&self, (ModeP))

/* Changing Modes */
#define self_set_mode_brutally(ModeP)     QUEX_NAME(set_mode_brutally)(&self, (ModeP))

#ifdef QUEX_OPTION_TOKEN_POLICY_QUEUE
#define self_enter_mode(ModeP) \
           QUEX_NAME(enter_mode)(&self, (ModeP))
#else
#define self_enter_mode(ModeP) \
        do {                                             \
           QUEX_NAME(enter_mode)(&self, (ModeP));        \
           __self_result_token_id = self_write_token_p()->_id; \
        } while(0)
#endif

/* Changing Modes with stack */ 
#define self_pop_mode()                      QUEX_NAME(pop_mode)(&self)
#define self_pop_drop_mode()                 QUEX_NAME(pop_drop_mode)(&self)
#define self_push_mode(ModeP)                QUEX_NAME(push_mode)(&self, (ModeP))

#ifdef      QUEX_OPTION_LINE_NUMBER_COUNTING
#   define  self_line_number_at_begin()      (self.counter._line_number_at_begin)
#   define  self_line_number_at_begin_set(X) do { self.counter._line_number_at_begin = (X); } while(0)
#   define  self_line_number_at_end()        (self.counter._line_number_at_end)
#   define  self_line_number()               (self_line_number_at_begin())
#endif
#ifdef      QUEX_OPTION_COLUMN_NUMBER_COUNTING
#   define  self_column_number_at_begin()      (self.counter._column_number_at_begin)
#   define  self_column_number_at_begin_set(X) do { self.counter._column_number_at_begin = (X); } while(0)
#   define  self_column_number_at_end()        (self.counter._column_number_at_end)
#   define  self_column_number()               (self_column_number_at_begin())
#endif
#ifdef      QUEX_OPTION_INDENTATION_TRIGGER
#   define  self_indentation()                 (counter._indentation_stack.back - counter._indentation_stack.front + 1)
#endif

/* Accumulator ______________________________________________________________*/
#   define self_accumulator_add(Begin, End)      QUEX_NAME(Accumulator_add)(&self.accumulator, Begin, End)
#   define self_accumulator_add_character(Char)  QUEX_NAME(Accumulator_add_character)(&self.accumulator, Char)
#   define self_accumulator_clear()              QUEX_NAME(Accumulator_clear)(&self.accumulator)
#   define self_accumulator_is_empty()           (self.accumulator.text.begin == self.accumulator.text.end)
#   define self_accumulator_flush(TokenID)                                                \
    do {                                                                                  \
        /* All functions must ensure: there is one cell to store terminating zero. */     \
        __quex_assert(self.accumulator.text.end < self.accumulator.text.memory_end);      \
                                                                                          \
        /* If no text is to be flushed, behave the same as self_send    */                \
        /* That is: self_token_set_id(ID);                              */                \
        /*          QUEX_TOKEN_POLICY_PREPARE_NEXT();                   */                \
        /*          BUT: We clear the text of the otherwise void token. */                \
        self_token_set_id(TokenID) ;                                                      \
        if( self.accumulator.text.begin == self.accumulator.text.end ) {                  \
            QUEX_NAME_TOKEN(take_text)(__QUEX_CURRENT_TOKEN_P, &self,                     \
                                       &QUEX_LEXEME_NULL,                                 \
                                       (&QUEX_LEXEME_NULL) + 1);                          \
        }                                                                                 \
        else {                                                                            \
            *(self.accumulator.text.end) = (QUEX_TYPE_CHARACTER)0; /* see above */        \
                                                                                          \
            if( QUEX_NAME_TOKEN(take_text)(__QUEX_CURRENT_TOKEN_P, &self,                 \
                                           self.accumulator.text.begin,                   \
                                           self.accumulator.text.end) == false ) {        \
                /* The called function does not need the memory chunk, we reuse it. */    \
                QUEX_NAME(Accumulator_clear)(&self.accumulator);                          \
            } else {                                                                      \
                /* The called function wants to use the memory, so we get some new. */    \
                QUEX_NAME(Accumulator_init_memory)(&self.accumulator);                    \
            }                                                                             \
        }                                                                                 \
        QUEX_TOKEN_POLICY_PREPARE_NEXT();                                                 \
    } while(0)


#endif /* __QUEX_INCLUDE_GUARD__ANALYZER__C_ADAPTIONS_H */
