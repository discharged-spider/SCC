//COMAND_DEF(NUM, NAME, DESCRIPTOR, PARAMS, CODE)

COMAND_DEF(0 , "push",   0,  1, ({Stack_.Push (Program_ [i + 1]);})                                                                                 )
COMAND_DEF(1 , "pop",    1,  0, ({Stack_.Pop  ();})                                                                                                 )
COMAND_DEF(2 , "dup",    2,  0, ({double DupCell = Stack_.Pop (); Stack_.Push (DupCell); Stack_.Push (DupCell);})                                   )
COMAND_DEF(3 , "swap",   3,  0, ({double SwapCell1 = Stack_.Pop (), SwapCell2 = Stack_.Pop (); Stack_.Push (SwapCell1); Stack_.Push (SwapCell2); }) )
COMAND_DEF(4 , "down",   4,  1, ({Stack_.Down ((int)Program_ [i + 1]);})                                                                            )

COMAND_DEF(5 , "sum",    10, 0, ({Stack_.Push (Stack_.Pop () + Stack_.Pop ()); })           )
COMAND_DEF(6 , "sub",    11, 0, ({Stack_.Push (Stack_.Pop () - Stack_.Pop ()); })           )
COMAND_DEF(7 , "mul",    12, 0, ({Stack_.Push (Stack_.Pop () * Stack_.Pop ()); })           )
COMAND_DEF(8 , "div",    13, 0, ({Stack_.Push (Stack_.Pop () / Stack_.Pop ()); })           )
COMAND_DEF(9 , "mod",    14, 0, ({Stack_.Push ((int)Stack_.Pop () % (int)Stack_.Pop ()); }) )

COMAND_DEF(10, ":",      20, 1, ({ })                                                      )
COMAND_DEF(11, "goto",   21, 1, ({i = (int)(Program_ [i + 1]); })                          )
COMAND_DEF(12, "call",   22, 1, ({Stack_.Push (i + 1); i = (int)(Program_ [i + 1]); })     )
COMAND_DEF(13, "end",    23, 0, ({i = (int)Stack_.Pop (); })                                )

COMAND_DEF(14, "equal",  30, 0, ({Stack_.Push ( Stack_.Pop () == Stack_.Pop ()); })        )
COMAND_DEF(15, "more",   31, 0, ({Stack_.Push ( Stack_.Pop () >  Stack_.Pop ()); })        )
COMAND_DEF(16, "less",   32, 0, ({Stack_.Push ( Stack_.Pop () <  Stack_.Pop ()); })        )
COMAND_DEF(17, "not",    33, 0, ({Stack_.Push ((Stack_.Pop () == 0)? 1 : 0); })            )
COMAND_DEF(18, "ifnot",  34, 1, ({if (Stack_.Pop () == 0) i = (int)(Program_ [i + 1]); }) )

COMAND_DEF(19, "pushm",  40, 1, ({Stack_.Push (Memory_ [(int)(Program_ [i + 1])]); })           )
COMAND_DEF(20, "popm",   41, 1, ({Memory_ [(int)(Program_ [i + 1])] = Stack_.Pop (); })         )
COMAND_DEF(21, "pushms", 42, 0, ({Stack_.Push (Memory_ [(int)(Stack_.Pop ())]); })              )
COMAND_DEF(22, "popms",  43, 0, ({int N = (int)(Stack_.Pop ()); Memory_ [N] = Stack_.Pop (); }) )
COMAND_DEF(23, "add",    44, 0, ({Add (); })                                                     )
COMAND_DEF(24, "rem",    45, 0, ({Rem (); })                                                     )

COMAND_DEF(25, "@",      50, 0, ({char c = (char)Stack_.Pop (); if (c == -1) system ("cls"); else printf ("%c", c);}) )
COMAND_DEF(26, "&",      51, 0, ({char c = getchar (); Stack_.Push ((double)c); })                                         )
