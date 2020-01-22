#ifndef GFORCE_TRAINER_CONTROLLER_STANDARDLOGGER_HPP
#define GFORCE_TRAINER_CONTROLLER_STANDARDLOGGER_HPP

#include <string>
#include <spdlog/logger.h>
#include "LoggerInterface.hpp"

namespace GForce::Utils::Logging {

static const std::string LOG_CHANNEL_UNKNOWN        = "unknown";
static const std::string LOG_CHANNEL_MAIN           = "main";
static const std::string LOG_CHANNEL_PROCESSING     = "processing";
static const std::string LOG_CHANNEL_BRAKE_INPUT_RX = "brakeInputRX";
static const std::string LOG_CHANNEL_BRAKE_INPUT_TX = "brakeInputTX";
static const std::string LOG_CHANNEL_WEBSOCKET      = "websocket";
static const std::string LOG_CHANNEL_MOVI_DRIVE     = "moviDrive";

class StandardLogger : public LoggerInterface
{
    private:
        /**
         * Writes rotating files to /var/log/gforce
         */
        std::shared_ptr<spdlog::logger> fileLogger;

        /**
         * Stores the global context. Get merged with context of every log message
         */
        std::map<std::string, std::string> globalContext;

        /**
         * Caches the latest printed timestamp
         */
        std::string timestamp;

        /**
         * JSON encodes a message
         */
        std::string encode(const std::string &level, const std::string &channel, const std::string &message, nlohmann::json context);

        void printMetadata(const std::string& channel, const std::string& level);
        void printTimestamp();
        static void printTopic(const std::string& channel, const std::string& level);

    public:
        StandardLogger();

        void setGlobalContext(std::string key, std::string value) override;

        void info(const std::string &channel, const std::string &message, const nlohmann::json &context) override;
        void error(const std::string &channel, const std::string &message, const nlohmann::json &context) override;
        void warning(const std::string &channel, const std::string &message, const nlohmann::json &context) override;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_STANDARDLOGGER_HPP
