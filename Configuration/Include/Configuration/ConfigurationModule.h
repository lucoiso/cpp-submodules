// Copyright Notice: [...]

#ifndef CONFIGURATIONMODULE_H
#define CONFIGURATIONMODULE_H

#ifdef BUILD_DLL
#define CONFIGURATIONMODULE_API _declspec(dllexport)
#else
#define CONFIGURATIONMODULE_API _declspec(dllimport)
#endif
#endif
