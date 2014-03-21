@echo off & setlocal enabledelayedexpansion

set "protoc=..\..\..\deps\libprotobuf-2.5.0_x64\bin\protoc.exe"

set "in_dir=proto_files\account\"
set "out_dir=generated\account\"
call:generate Login_RQ.proto
call:generate Login_RS.proto

goto:eof

:generate
if not exist %out_dir% mkdir %out_dir%
set "arg=%~1"
set "cur_file=%in_dir%%arg%"
%protoc% --proto_path=%in_dir% --cpp_out=%out_dir% %cur_file%
goto:eof