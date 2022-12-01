@echo %cd%
RD /S /Q "Binaries/"
#RD /S /Q "Build/"
RD /S /Q ".vs/"
RD /S /Q "Intermediate/"
RD /S /Q "Saved/"
RD /S /Q "DerivedDataCache/"
RD /S /Q "Script/"
RD /S /Q "Plugins/MirageSDK/Binaries/"
RD /S /Q "Plugins/MirageSDK/Intermediate/"
Del symbols.zip
Del MirageUnrealSDK.sln
exit