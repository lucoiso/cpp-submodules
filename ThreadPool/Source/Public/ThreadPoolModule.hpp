// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/cpp-submodules

#ifndef THREADPOOLMODULE_H
#define THREADPOOLMODULE_H

#ifdef BUILD_DLL
#define THREADPOOLMODULE_API _declspec(dllexport)
#else
#define THREADPOOLMODULE_API _declspec(dllimport)
#endif
#endif
