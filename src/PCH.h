#pragma once

#pragma warning(push)
#include <F4SE/F4SE.h>
#include <RE/Fallout.h>
#include <REL/Relocation.h>

#include <SimpleIni.h>

#ifdef NDEBUG
#	include <spdlog/sinks/basic_file_sink.h>
#else
#	include <spdlog/sinks/msvc_sink.h>
#endif
#pragma warning(pop)

using namespace std::literals;

namespace logger = F4SE::log;

namespace util
{
	using F4SE::stl::report_and_fail;
}

#define DLLEXPORT __declspec(dllexport)

#include "Version.h"
