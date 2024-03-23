// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/cpp-submodules

#ifndef SOCKETSERVICEMODULE_H
#define SOCKETSERVICEMODULE_H

#ifdef BUILD_DLL
#define SOCKETSERVICEMODULE_API _declspec(dllexport)
#else
#define SOCKETSERVICEMODULE_API _declspec(dllimport)
#endif
#endif
