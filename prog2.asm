SET R1, $"foobar\n"
SET R2, $'0'
IFE R1, R2
HLT
OUT R1
INC R1
SET IP, $2
