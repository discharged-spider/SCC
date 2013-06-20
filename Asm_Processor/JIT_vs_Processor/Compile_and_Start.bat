"..\Compiler\Compiler.exe" -i %1 -o Program.scc_exe -p
"..\Compiler\Compiler.exe" -i %1 -o Program.scc_bin -j

del Error.txt

start StartProcessor.bat Program.scc_exe
start StartJIT.bat       Program.scc_bin
