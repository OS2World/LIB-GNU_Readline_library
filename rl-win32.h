#ifndef __RL_WIN32_H
#define __RL_WIN32_H

#ifdef __CRTRSXNT__
#include <crtrsxnt.h>
#undef timezone

#ifndef READLINE_LIBRARY

/* readline data items */
#define funmap (*funmap)
#define rl_library_version (*rl_library_version)
#define rl_readline_name (*rl_readline_name)
#define rl_prompt (*rl_prompt)
#define rl_line_buffer (*rl_line_buffer)
#define rl_point (*rl_point)
#define rl_end (*rl_end)
#define rl_mark (*rl_mark)
#define rl_done (*rl_done)
#define rl_pending_input (*rl_pending_input)
#define rl_dispatching (*rl_dispatching)
#define rl_terminal_name (*rl_terminal_name)
#define rl_instream (*rl_instream)
#define rl_outstream (*rl_outstream)
#define rl_startup_hook (*rl_startup_hook)
#define rl_event_hook (*rl_event_hook)
#define rl_getc_function (*rl_getc_function)
#define rl_redisplay_function (*rl_redisplay_function)
#define rl_prep_term_function (*rl_prep_term_function)
#define rl_deprep_term_function (*rl_deprep_term_function)
#define rl_executing_keymap (*rl_executing_keymap)
#define rl_binding_keymap (*rl_binding_keymap)
#define rl_completion_entry_function (*rl_completion_entry_function)
#define rl_ignore_some_completions_function (*rl_ignore_some_completions_function)
#define rl_attempted_completion_function (*rl_attempted_completion_function)
#define rl_basic_word_break_characters (*rl_basic_word_break_characters)
#define rl_completer_word_break_characters (*rl_completer_word_break_characters)
#define rl_completer_quote_characters (*rl_completer_quote_characters)
#define rl_basic_quote_characters (*rl_basic_quote_characters)
#define rl_filename_quote_characters (*rl_filename_quote_characters)
#define rl_special_prefixes (*rl_special_prefixes)
#define rl_directory_completion_hook (*rl_directory_completion_hook)
#define rl_filename_completion_desired (*rl_filename_completion_desired)
#define rl_filename_quoting_desired (*rl_filename_quoting_desired)
#define rl_filename_quoting_function (*rl_filename_quoting_function)
#define rl_filename_dequoting_function (*rl_filename_dequoting_function)
#define rl_char_is_quoted_p (*rl_char_is_quoted_p)
#define rl_attempted_completion_over (*rl_attempted_completion_over)
#define rl_completion_type (*rl_completion_type)
#define rl_completion_append_character (*rl_completion_append_character)
#define rl_completion_query_items (*rl_completion_query_items)
#define rl_ignore_completion_duplicates (*rl_ignore_completion_duplicates)
#define rl_inhibit_completion (*rl_inhibit_completion)
#define emacs_standard_keymap (*emacs_standard_keymap)
#define emacs_meta_keymap (*emacs_meta_keymap)
#define emacs_ctlx_keymap (*emacs_ctlx_keymap)
#define vi_insertion_keymap (*vi_insertion_keymap)
#define vi_movement_keymap (*vi_movement_keymap)

/* history data items */
#define history_base (*history_base)
#define history_length (*history_length)
#define max_input_history (*max_input_history)
#define history_expansion_char (*history_expansion_char)
#define history_subst_char (*history_subst_char)
#define history_comment_char (*history_comment_char)
#define history_no_expand_chars (*history_no_expand_chars)
#define history_search_delimiter_chars (*history_search_delimiter_chars)
#define history_quotes_inhibit_expansion (*history_quotes_inhibit_expansion)
#define history_inhibit_expansion_function (*history_inhibit_expansion_function)

/* tilde data items */
#define tilde_expansion_preexpansion_hook (*tilde_expansion_preexpansion_hook)
#define tilde_expansion_failure_hook (*tilde_expansion_failure_hook)
#define tilde_additional_prefixes (*tilde_additional_prefixes)
#define tilde_additional_suffixes (*tilde_additional_suffixes)

#endif /* READLINE_LIBRARY */

#endif /* __CRTRSXNT__ */

#endif /* __RL_WIN32_H */
