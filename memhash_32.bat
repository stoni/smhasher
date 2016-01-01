REM  Assemble  asm source file for 32-bit environment
ml /c /Fomemhash_32.obj msvc\i686\memhash.asm
lib /NODEFAULTLIB /out:memhash.lib memhash_32.obj
