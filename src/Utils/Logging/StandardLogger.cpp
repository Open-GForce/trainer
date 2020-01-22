#include <iostream>
#include <rang.hpp>
#include "StandardLogger.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"

using namespace GForce::Utils::Logging;
using namespace rang;

StandardLogger::StandardLogger()
{
    this->globalContext = {};
    this->timestamp = "";

    // 5 generations of max. 50 MiB size each
    this->fileLogger = spdlog::rotating_logger_mt("fileLogger", "/var/log/gforce/controller.log", 1048576 * 20, 5);
    this->fileLogger->set_level(spdlog::level::debug);

    spdlog::flush_every(std::chrono::seconds(3));
}

void StandardLogger::setGlobalContext(std::string key, std::string value)
{
    this->globalContext[key] = value;
}

void StandardLogger::info(const std::string &channel, const std::string &message, const nlohmann::json &context)
{
    this->printMetadata(channel, "INFO");
    std::cout << message << "\n";

    this->fileLogger->info(this->encode("INFO", channel, message, context));
}

void StandardLogger::error(const std::string &channel, const std::string &message, const nlohmann::json &context)
{
    this->printMetadata(channel, "ERROR");
    std::cout << fgB::gray << bg::red << message << fg::reset << bg::reset << "\n";

    this->fileLogger->error(this->encode("ERROR", channel, message, context));
}

void StandardLogger::warning(const std::string &channel, const std::string &message, const nlohmann::json &context)
{
    this->printMetadata(channel, "WARNING");
    std::cout << fg::yellow << message << fg::reset << "\n";

    this->fileLogger->error(this->encode("WARNING", channel, message, context));
}

std::string StandardLogger::encode(const std::string &level, const std::string &channel, const std::string &message, nlohmann::json context)
{
    try {
        context["timestamp"] = this->timestamp;
        context["level"] = level;
        context["channel"] = channel;
        context["message"] = message;

        for (const auto& [key, value] : this->globalContext) {
            context[key] = value;
        }

        return context.dump();
    } catch (std::exception &e) {
        return "FATAL ERROR WHILE ENCODING LOG => " + std::string(e.what());
    }
}

void StandardLogger::printMetadata(const std::string &channel, const std::string &level)
{
    StandardLogger::printTimestamp();
    StandardLogger::printTopic(channel, level);
}

void StandardLogger::printTopic(const std::string& channel, const std::string& level)
{
    std::cout << fg::blue << "[" << level << "] "
              << style::bold << fg::cyan
              << "[" << channel << "] "
              << style::reset << fg::reset;
}

void StandardLogger::printTimestamp()
{
    using namespace std::chrono;

    auto now = system_clock::now();
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
    auto timer = system_clock::to_time_t(now);

    std::tm bt = *std::localtime(&timer);

    std::ostringstream oss;
    oss << std::put_time(&bt, "%Y-%m-%d %H:%M:%S"); // HH:MM:SS
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();

    this->timestamp = oss.str();
    std::cout << fg::blue << "[" << this->timestamp << "] ";
}