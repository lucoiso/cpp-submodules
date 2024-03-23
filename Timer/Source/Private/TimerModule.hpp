// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/cpp-submodules

#ifndef TIMERMODULE_H
#define TIMERMODULE_H

#ifdef BUILD_DLL
#define TIMERMODULE_API _declspec(dllexport)
#else
#define TIMERMODULE_API _declspec(dllimport)
#endif
#endif
