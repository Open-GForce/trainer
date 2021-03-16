#ifndef GFORCE_TRAINER_CONTROLLER_THREADSYNCHRONIZER_HPP
#define GFORCE_TRAINER_CONTROLLER_THREADSYNCHRONIZER_HPP

#include <condition_variable>
#include <functional>

namespace GForce::Utils {

class ThreadSynchronizer
{
    private:
        std::function<void()> wakeupTriggerFunction{};

    public:
        ThreadSynchronizer()
        {
            this->wakeupTriggerFunction = [] () {};
        }

        /**
         * Sleeps until another threads wakes us up
         */
        void sleep()
        {
            std::mutex mutex;
            std::condition_variable conditionVariable;

            bool eventTriggered = false;

            this->wakeupTriggerFunction = [&conditionVariable, &mutex, &eventTriggered] () {
                std::lock_guard<std::mutex> lck(mutex);
                eventTriggered = true;
                conditionVariable.notify_one();
            };

            std::unique_lock<std::mutex> lock(mutex);
            conditionVariable.wait(lock, [&eventTriggered]{ return eventTriggered;});
        }

        void wakeup()
        {
            this->wakeupTriggerFunction();
        }
};

}

#endif //GFORCE_TRAINER_CONTROLLER_THREADSYNCHRONIZER_HPP
