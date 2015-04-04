/*
GetMain GetC_REC

%GetAA GetAB '||' GetAB
%GetAB GetAC  &&  GetAC
%GetAC ! GetAA || GetAD || GetAA == GetAA || GetAA != GetAA || GetAA < GetAA || GetAA > GetAA || GetAA <= GetAA || GetAA >= GetAA
%GetAD GetAE + GetAE || GetAE - GetAE
%GetAE GetAF * GetAF || GetAF / GetAF || GetAF % GetAF
%GetAF [0-9] || GetCF || GetAF || (GetAA) || - GetAF || + GetAF || true || false || GetAG
%GetAG Name || Name[GetAA]

GetAA GetAA (SUM, SUB, MUL, DIV, MOD, OR) OF GetAA AN GetAA || BOTH SAEM GetAA AN GetAA || BOTH OF GetAA AN GetAA || N_NONEQ GetAA AN GetAA || THS GetAA (MRE, LSE) THN GetAA || NOT GetAA || GetAB
GetAB [0-9] || TRUE || FALSE || (+, -) GetAB || GetAC || GetCF
GetAC Name

GetC_REC GetC; until END
GetC empty || GetBA || GetCA || GetCB || GetCC || GetCD || GetCF || GetCG || GetCH

GetBA I HAS A Name || I HAS A Name ITZ GetAA || HOW IZ I Name (YR GetAA AN YR GetAA, ...) (NVR MND || (GetC, GetC | GTFO | FOUND YR GetAA) IF U CAN SO)

GetCA del Name

GetCB GetAC = GetAA

GetCC GetAA END O RLY? END YA RLY END GetC (NO WAI END GetC END) OIC

GetCD IM IN YR Name WILE GetAA END (GetC | GTFOHR) IM OUTTA YR Name

GetCF I IZ Name (YR Name AN YR Name, ...) MKAY

GetCG echo GetAA || get GetAC

GetCH : IS Name || goto Name
*/
