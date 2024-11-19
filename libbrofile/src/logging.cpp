#include "brofile/logging.hpp"

#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/spdlog.h>

#include <filesystem>

std::shared_ptr<spdlog::sinks::rotating_file_sink_mt> create_sink() {
  std::filesystem::path log_dir;

  // according to https://specifications.freedesktop.org/basedir-spec/latest/
  // $XDG_STATE_HOME is a good place to store logs
  auto xdg_state_home_env = std::getenv("XDG_STATE_HOME");
  if (xdg_state_home_env != nullptr) {
    log_dir = std::filesystem::path(xdg_state_home_env);
  } else {
    // according to https://wiki.archlinux.org/title/XDG_Base_Directory
    // $XDG_STATE_HOME should default to $HOME/.local/state should
    auto home = std::getenv("HOME");
    if (home != nullptr) {
      log_dir = std::filesystem::path(home) / ".local" / "state";
    } else {
      log_dir = std::filesystem::path("~") / ".local" / "state";
    }
  }
  log_dir = log_dir / "brofile";

  if (!std::filesystem::exists(log_dir)) {
    std::filesystem::create_directories(log_dir);
  }

  return std::make_shared<spdlog::sinks::rotating_file_sink_mt>(log_dir / "brofile.log", 1048576 * 5, 3);
}

std::shared_ptr<spdlog::logger> bf::setup_logger(const std::string &name, const std::string &log_level) {
  static auto sink = create_sink();

  auto logger = std::make_shared<spdlog::logger>(name, sink);
  spdlog::register_logger(logger);

  if (log_level == "debug") {
    logger->set_level(spdlog::level::debug);
  } else if (log_level == "info") {
    logger->set_level(spdlog::level::info);
  } else if (log_level == "warn") {
    logger->set_level(spdlog::level::warn);
  } else if (log_level == "error") {
    logger->set_level(spdlog::level::err);
  } else if (log_level == "critical") {
    logger->set_level(spdlog::level::critical);
  }

  return logger;
}