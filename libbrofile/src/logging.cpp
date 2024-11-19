#include "brofile/logging.hpp"

#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/spdlog.h>
#include <filesystem>

std::shared_ptr<spdlog::sinks::rotating_file_sink_mt> create_sink() {
  std::filesystem::path log_file_path;
  auto home = std::getenv("HOME");
  if (home != nullptr) {
    log_file_path = std::filesystem::path(home) / ".brofile";
  } else {
    log_file_path = std::filesystem::path("~/.brofile");
  }

  if (!std::filesystem::exists(log_file_path)) {
    std::filesystem::create_directory(log_file_path);
  }

  return std::make_shared<spdlog::sinks::rotating_file_sink_mt>(log_file_path / "log.txt", 1048576 * 5, 3);
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