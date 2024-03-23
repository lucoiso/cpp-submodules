// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/cpp-submodules

#ifndef CONFIGURATIONMODULE_H
#define CONFIGURATIONMODULE_H

#ifdef BUILD_DLL
#define CONFIGURATIONMODULE_API _declspec(dllexport)
#else
#define CONFIGURATIONMODULE_API _declspec(dllimport)
#endif
#endif
