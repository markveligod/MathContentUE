@echo off
set dirsToRemove=Intermediate Saved DerivedDataCache Binaries Build .vs .idea Script
set filesToRemove=*.sln

for %%a in (%dirsToRemove%) do (
    rmdir /S /Q %%a
)

for %%a in (%filesToRemove%) do (
   del %%a
)

for /D %%a in ("Plugins\*") do (
   rmdir /S /Q %%a\Intermediate
   rmdir /S /Q %%a\Binaries
)