@echo %cd%
RD /S /Q "Binaries/"
#RD /S /Q "Build/"
RD /S /Q ".vs/"
RD /S /Q "Intermediate/"
RD /S /Q "Saved/"
RD /S /Q "Plugins/AnkrSDK/Binaries/"
RD /S /Q "Plugins/AnkrSDK/Intermediate/"
Del symbols.zip
Del AnkrUnrealSDK.sln
exit