//
// memoria_ext_logger.hpp
//
// Lightweight system for event registration to log application behavior.
//
// The system is designed for registering custom handlers via `RegisterLogger`,
// but also provides a quick way to deploy familiar logging systems,
// such as the system console (`InitializeConsoleLogging`).
//

#pragma once

#include "memoria_common.hpp"

MEMORIA_BEGIN

using LoggerCallback_t = void(*)(const char *text);

extern bool RegisterLogger(LoggerCallback_t cb);
extern bool UnregisterLogger(LoggerCallback_t cb);

extern size_t GetLoggerCount();

extern bool InitializeConsoleLogging();
extern bool FinalizeConsoleLogging();

extern void DispatchLog(const char *fmt, ...);

MEMORIA_END

#ifdef _DEBUG

#define LOG_DBG(fmt, ...) Memoria::DispatchLog(fmt, __VA_ARGS__)
#define LOG(fmt, ...)	  Memoria::DispatchLog(fmt, __VA_ARGS__)

#else

#define LOG_DBG(fmt, ...) ((void)0)
#define LOG(fmt, ...)	  Memoria::DispatchLog(fmt, __VA_ARGS__)

#endif