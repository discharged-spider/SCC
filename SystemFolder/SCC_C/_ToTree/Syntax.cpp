/*
GetMain i < size GetC;

GetAA GetAB '||' GetAB
GetAB GetAC  &&  GetAC
GetAC ! GetAA || GetAD || GetAA == GetAA || GetAA != GetAA || GetAA < GetAA || GetAA > GetAA || GetAA <= GetAA || GetAA >= GetAA
GetAD GetAE + GetAE || GetAE - GetAE
GetAE GetAF * GetAF || GetAF / GetAF || GetAF % GetAF
GetAF [0-9] || GetCF || GetAF || (GetAA) || - GetAF || + GetAF || true || false || GetAG
GetAG Name || Name[GetAA]

GetC empty || {GetC, ...} || GetBA || GetCA || GetCB || GetCC || GetCD || GetCE || GetCF || GetCG || GetCH

GetBA new GetBB || new GetBB, ...
GetBB Name || Name = GetAA || Name [GetAA] || Name [GetAA] = GetArr || &Name || Name (new GetBB, ...) {GetC; || return GetAA; ...}

GetCA del Name || del Name[]

GetCB GetAG = GetAA

GetCC if (GetAA) GetC || if (GetAA) GetC else GetC

GetCD while (GetAA) {GetC; || break;, ...}

GetCE for (GetBA; GetAA; GetC) {GetC; || break;, ...}

GetCF Name (GetAA || Name [] || &Name || &Name [GetAA], ...)

GetCG echo GetAA || get GetAG

GetCH : Name || goto Name

GetCJ native Name (new GetBB, ...);

*/
