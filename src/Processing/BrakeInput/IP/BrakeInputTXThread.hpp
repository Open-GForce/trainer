#ifndef GFORCE_TRAINER_CONTROLLER_BRAKEINPUTTXTHREAD_HPP
#define GFORCE_TRAINER_CONTROLLER_BRAKEINPUTTXTHREAD_HPP

#include <vector>
#include "../../../Sensors/ADCSensorInterface.hpp"
#include "../../../Utils/Logging/LoggerInterface.hpp"
#include "../../../ACL/TCP/TCPConnectionInterface.hpp"

using namespace GForce::Sensors;
using namespace GForce::Utils::Logging;
using namespace GForce::ACL::TCP;

namespace GForce::Processing::BrakeInput::IP {

class BrakeInputTXThread
{
    private:
        LoggerInterface* logger;
        ADCSensorInterface* sensor;

        TCPConnectionInterface* socket;
        TCPConnectionFactory* factory;

        /**
         * IP address of main controller
         */
        std::string mainControllerAddress;

        /**
         * Is thread stopped?
         */
        bool stopped;

        /**
         * Lists with input values
         */
        std::vector<int> firstBrakeHistory;
        std::vector<int> secondBrakeHistory;

        /**
         * Length of inputs used for normalizing/averaging
         */
        int normalizeLength;

        void connect();
        void loop();

        /**
         * Will try to close the socket if exists and performs reconnect
         */
        void reconnect();

        /**
         * Returns general state data which may be used for enhancing exceptions
         */
        nlohmann::json getErrorContext(std::string errorMessage);

        static int scaleSignedInput(int value);

        /**
         * @return Average of the given list
         */
        static int calcAverage(const std::vector<int>& values);

    public:
        BrakeInputTXThread(LoggerInterface *logger, ADCSensorInterface *sensor);

        void start();
        void stop();

        void setSocket(TCPConnectionInterface *value);

        void setSocketFactory(TCPConnectionFactory *socketFactory);
};

}


#endif //GFORCE_TRAINER_CONTROLLER_BRAKEINPUTTXTHREAD_HPP
