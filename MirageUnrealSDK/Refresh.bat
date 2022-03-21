@echo %cd%
RD /S /Q "Binaries/"
#RD /S /Q "Build/"
RD /S /Q ".vs/"
RD /S /Q "Intermediate/"
RD /S /Q "Saved/"
RD /S /Q "Plugins/MirageSDK/Binaries/"
RD /S /Q "Plugins/MirageSDK/Intermediate/"
RD /S /Q "Plugins/WebSocket/Binaries/"
RD /S /Q "Plugins/WebSocket/Intermediate/"
RD /S /Q "Plugins/GameLiftServerSDK/Binaries/"
RD /S /Q "Plugins/GameLiftServerSDK/Intermediate/"
Del symbols.zip
Del Whoosh.sln
Del MirageUnrealSDK.sln
exit