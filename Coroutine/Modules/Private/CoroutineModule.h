// Author: Lucas Vilas-Boas
// Year : 2023
// Repo : https://github.com/lucoiso/cpp-submodules

#ifndef COROUTINEMODULE_H
#define COROUTINEMODULE_H

#ifdef BUILD_DLL
#define COROUTINEMODULE_API _declspec(dllexport)
#else
#define COROUTINEMODULE_API _declspec(dllimport)
#endif
#endif
