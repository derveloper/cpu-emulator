```bash
~/Projects/qt/cpu➤ cat prog2.asm
SET R1, $"foobar\n"
SET R2, $'0'
IFE R1, R2
HLT
OUT R1
INC R1
SET IP, $2
~/Projects/qt/cpu➤ ./asm prog2.asm prog2
~/Projects/qt/cpu➤ ls -la prog2*
-rw-r--r--  1 vileda  staff    30B 29 Jun 22:05 prog2
-rw-r--r--  1 vileda  staff    73B 29 Jun 22:03 prog2.asm
~/Projects/qt/cpu➤ ./cpu prog2
foobar
~/Projects/qt/cpu➤
