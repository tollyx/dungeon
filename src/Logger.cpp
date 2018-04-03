#include "Logger.h"
#include <SDL2/SDL.h>
#include <io.h>

Log::LogLevel log_level = Log::LogLevel::Warning;

void Log::set_log_level(LogLevel level) {
  log_level = level;
}

void Log::error(string tag, string message) {
  if (log_level <= LogLevel::Error) {
    printf("[ERR] <%s> %s\n", tag, message);
  }
}

void Log::warning(string tag, string message) {
  if (log_level <= LogLevel::Warning) {
    printf("[WRN] <%s> %s\n", tag, message);
  }
}

void Log::info(string tag, string message) {
  if (log_level <= LogLevel::Info) {
    printf("[INF] <%s> %s\n", tag, message);
  }
}

void Log::debug(string tag, string message) {
  if (log_level <= LogLevel::Debug) {
    printf("[DBG] <%s> %s\n", tag, message);
  }
}

void Log::trace(string tag, string message) {
  if (log_level <= LogLevel::Trace) {
    printf("[TRC] <%s> %s\n", tag, message);
  }
}
