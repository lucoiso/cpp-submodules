// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

#ifndef RUNTIMEINFOMODULE_H
#define RUNTIMEINFOMODULE_H

#ifdef BUILD_DLL
#define RUNTIMEINFOMODULE_API _declspec(dllexport)
#else
#define RUNTIMEINFOMODULE_API _declspec(dllimport)
#endif
#endif
