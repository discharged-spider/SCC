//COMMAND_DEF(NUM, NAME, STR, DESCRIPTOR, PARAMS, CODE)

COMMAND_DEF(0 , PUSH, "push", 0,  1, ({Stack_.Push (Program_ [i + 1]);})                                                                                 )
COMMAND_DEF(1 , POP,  "pop",  1,  0, ({Stack_.Pop  ();})                                                                                                 )
COMMAND_DEF(2 , DUP,  "dup",  2,  0, ({double DupCell = Stack_.Pop (); Stack_.Push (DupCell); Stack_.Push (DupCell);})                                   )
COMMAND_DEF(3 , SWAP, "swap", 3,  0, ({double SwapCell1 = Stack_.Pop (), SwapCell2 = Stack_.Pop (); Stack_.Push (SwapCell1); Stack_.Push (SwapCell2); }) )
COMMAND_DEF(4 , DOWN, "down", 4,  1, ({Stack_.Down ((int)Program_ [i + 1]);})                                                                            )

COMMAND_DEF(5 , SUM, "sum", 10, 0, ({Stack_.Push (Stack_.Pop () + Stack_.Pop ()); })           )
COMMAND_DEF(6 , SUB, "sub", 11, 0, ({Stack_.Push (Stack_.Pop () - Stack_.Pop ()); })           )
COMMAND_DEF(7 , MUL, "mul", 12, 0, ({Stack_.Push (Stack_.Pop () * Stack_.Pop ()); })           )
COMMAND_DEF(8 , DIV, "div", 13, 0, ({Stack_.Push (Stack_.Pop () / Stack_.Pop ()); })           )
COMMAND_DEF(9 , MOD, "mod", 14, 0, ({Stack_.Push ((int)Stack_.Pop () % (int)Stack_.Pop ()); }) )

COMMAND_DEF(10, MARK, ":",    20, 1, ({ })                                                      )
COMMAND_DEF(11, GOTO, "goto", 21, 1, ({i = (int)(Program_ [i + 1]); })                          )
COMMAND_DEF(12, CALL, "call", 22, 1, ({Stack_.Push (i + 1); i = (int)(Program_ [i + 1]); })     )
COMMAND_DEF(13, END,  "end",  23, 0, ({i = (int)Stack_.Pop (); })                                )

COMMAND_DEF(14, EQUAL, "equal", 30, 0, ({Stack_.Push ( Stack_.Pop () == Stack_.Pop ()); })        )
COMMAND_DEF(15, MORE,  "more",  31, 0, ({Stack_.Push ( Stack_.Pop () >  Stack_.Pop ()); })        )
COMMAND_DEF(16, LESS,  "less",  32, 0, ({Stack_.Push ( Stack_.Pop () <  Stack_.Pop ()); })        )
COMMAND_DEF(17, NOT,   "not",   33, 0, ({Stack_.Push ((Stack_.Pop () == 0)? 1 : 0); })            )
COMMAND_DEF(18, IFNOT, "ifnot", 34, 1, ({if (Stack_.Pop () == 0) i = (int)(Program_ [i + 1]); }) )

COMMAND_DEF(19, PUSHM,  "pushm",  40, 1, ({Stack_.Push (Memory_ [(int)(Program_ [i + 1])]); })           )
COMMAND_DEF(20, POPM,   "popm",   41, 1, ({Memory_ [(int)(Program_ [i + 1])] = Stack_.Pop (); })         )
COMMAND_DEF(21, PUSHMS, "pushms", 42, 0, ({Stack_.Push (Memory_ [(int)(Stack_.Pop ())]); })              )
COMMAND_DEF(22, POPMS,  "popms",  43, 0, ({int N = (int)(Stack_.Pop ()); Memory_ [N] = Stack_.Pop (); }) )
COMMAND_DEF(23, ADD,    "add",    44, 0, ({Add (); })                                                     )
COMMAND_DEF(24, REM,    "rem",    45, 0, ({Rem (); })                                                     )

COMMAND_DEF(25, ECHO, "@", 50, 0, ({char c = (char)Stack_.Pop (); if (output != NULL) fprintf (output, "%c", c); else if (c == -1) system ("cls"); else printf ("%c", c);}) )
COMMAND_DEF(26, GET,  "&", 51, 0, ({char c; if (input == NULL) scanf ("%c", &c); else fscanf (input, "%c", &c); Stack_.Push ((double)c); })                                           )

COMMAND_DEF(27, LOG_GET_VAR, "log_get_var", 60, 1, ({if (UseInspector_) Inspector_.get_var ((int)(Program_ [i + 1]));}) )
COMMAND_DEF(28, LOG_GET_ARR, "log_get_arr", 71, 1, ({if (UseInspector_) Inspector_.get_arr ((int)(Program_ [i + 1]));}) )
COMMAND_DEF(29, LOG_SET_VAR, "log_set_var", 72, 1, ({if (UseInspector_) Inspector_.set_var ((int)(Program_ [i + 1]));}) )
COMMAND_DEF(30, LOG_SET_ARR, "log_set_arr", 73, 1, ({if (UseInspector_) Inspector_.set_arr ((int)(Program_ [i + 1]));}) )
