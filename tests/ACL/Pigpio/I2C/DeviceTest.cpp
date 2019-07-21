#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err61-cpp"

#include "../../../../../catch2/catch.hpp"
#include "../../../../../fake_it/single_header/catch/fakeit.hpp"
#include "../../../../src/ACL/Pigpio/I2C/Device.hpp"
#include "../../../../src/ACL/Pigpio/PigpioException.hpp"
#include "../../../../src/ACL/Pigpio/LogicException.hpp"

extern  "C"
{
#include <pigpio.h>
}

using namespace fakeit;
using namespace GForce::ACL::Pigpio;
using namespace GForce::ACL::Pigpio::I2C;

TEST_CASE( "Opening I2C device", "[I2C_DEVICE]" ) {
    SECTION( "Correct handle" ) {
        Mock<ProxyInterface> proxyMock;
        When(Method(proxyMock,i2cOpen)).Return(12);
        ProxyInterface &proxy = proxyMock.get();

        auto *device = new Device(1, 0x63, 0, &proxy);

        device->open();
        CHECK(device->isOpen());
        Verify(Method(proxyMock,i2cOpen)).Once();
        Verify(Method(proxyMock,i2cOpen).Using(1, 0x63, 0));
    }

    SECTION( "Idempotent on double opening" ) {
        Mock<ProxyInterface> proxyMock;
        When(Method(proxyMock,i2cOpen)).Return(12);
        ProxyInterface &proxy = proxyMock.get();

        auto *device = new Device(1, 0x63, 0, &proxy);

        device->open();
        device->open();
        CHECK(device->isOpen());
        Verify(Method(proxyMock,i2cOpen)).Once();
    }

    SECTION( "Throw an exception on invalid bus" ) {
        Mock<ProxyInterface> proxyMock;
        When(Method(proxyMock,i2cOpen)).Return(PI_BAD_I2C_BUS);
        ProxyInterface &proxy = proxyMock.get();

        auto *device = new Device(1, 0x63, 0, &proxy);

        try {
            device->open();
            FAIL("No exception was thrown");
        } catch (PigpioException& e) {
            REQUIRE(e.getMessage() == "Opening i2c device failed => invalid bus given");
        }

        REQUIRE_FALSE(device->isOpen());
    }

    SECTION( "Throw an exception on invalid address" ) {
        Mock<ProxyInterface> proxyMock;
        When(Method(proxyMock,i2cOpen)).Return(PI_BAD_I2C_ADDR);
        ProxyInterface &proxy = proxyMock.get();

        auto *device = new Device(1, 0x63, 0, &proxy);

        try {
            device->open();
            FAIL("No exception was thrown");
        } catch (PigpioException& e) {
            REQUIRE(e.getMessage() == "Opening i2c device failed => invalid address given");
        }

        REQUIRE_FALSE(device->isOpen());
    }

    SECTION( "Throw an exception on invalid flags" ) {
        Mock<ProxyInterface> proxyMock;
        When(Method(proxyMock,i2cOpen)).Return(PI_BAD_FLAGS);
        ProxyInterface &proxy = proxyMock.get();

        auto *device = new Device(1, 0x63, 0, &proxy);

        try {
            device->open();
            FAIL("No exception was thrown");
        } catch (PigpioException& e) {
            REQUIRE(e.getMessage() == "Opening i2c device failed => invalid flags given");
        }

        REQUIRE_FALSE(device->isOpen());
    }

    SECTION( "Throw an exception if no handle is available" ) {
        Mock<ProxyInterface> proxyMock;
        When(Method(proxyMock,i2cOpen)).Return(PI_NO_HANDLE);
        ProxyInterface &proxy = proxyMock.get();

        auto *device = new Device(1, 0x63, 0, &proxy);

        try {
            device->open();
            FAIL("No exception was thrown");
        } catch (PigpioException& e) {
            REQUIRE(e.getMessage() == "Opening i2c device failed => no handle available");
        }

        REQUIRE_FALSE(device->isOpen());
    }

    SECTION( "Throw an exception on internal device error" ) {
        Mock<ProxyInterface> proxyMock;
        When(Method(proxyMock,i2cOpen)).Return(PI_I2C_OPEN_FAILED);
        ProxyInterface &proxy = proxyMock.get();

        auto *device = new Device(1, 0x63, 0, &proxy);

        try {
            device->open();
            FAIL("No exception was thrown");
        } catch (PigpioException& e) {
            REQUIRE(e.getMessage() == "Opening i2c device failed => internal device error");
        }

        REQUIRE_FALSE(device->isOpen());
    }

    SECTION( "Throw an exception on unknown error" ) {
        Mock<ProxyInterface> proxyMock;
        When(Method(proxyMock,i2cOpen)).Return(-99);
        ProxyInterface &proxy = proxyMock.get();

        auto *device = new Device(1, 0x63, 0, &proxy);

        try {
            device->open();
            FAIL("No exception was thrown");
        } catch (PigpioException& e) {
            REQUIRE(e.getMessage() == "Opening i2c device failed => unknown error (RC:-99)");
        }

        REQUIRE_FALSE(device->isOpen());
    }
}

TEST_CASE( "Closing I2C device", "[I2C_DEVICE]" ) {
    SECTION( "Correct closing" ) {
        Mock<ProxyInterface> proxyMock;
        When(Method(proxyMock,i2cOpen)).Return(12);
        When(Method(proxyMock,i2cClose)).Return(0);
        ProxyInterface &proxy = proxyMock.get();

        auto *device = new Device(1, 0x63, 0, &proxy);

        device->open();
        device->close();
        CHECK_FALSE(device->isOpen());
        Verify(Method(proxyMock,i2cClose)).Once();
        Verify(Method(proxyMock,i2cClose).Using(12));
    }

    SECTION( "No error if device is not open" ) {
        Mock<ProxyInterface> proxyMock;
        When(Method(proxyMock,i2cClose)).Throw(PigpioException("Method i2cClose() of proxy was not expected to be called never"));
        ProxyInterface &proxy = proxyMock.get();

        auto *device = new Device(1, 0x63, 0, &proxy);

        device->close();
        CHECK_FALSE(device->isOpen());
    }

    SECTION( "Throw an exception on unknown error" ) {
        Mock<ProxyInterface> proxyMock;
        When(Method(proxyMock,i2cOpen)).Return(1);
        When(Method(proxyMock,i2cClose)).Return(-98);
        ProxyInterface &proxy = proxyMock.get();

        auto *device = new Device(1, 0x63, 0, &proxy);
        device->open();

        try {
            device->close();
            FAIL("No exception was thrown");
        } catch (PigpioException& e) {
            REQUIRE(e.getMessage() == "Closing i2c device failed => unknown error (RC:-98)");
        }

        REQUIRE(device->isOpen());
    }
}

TEST_CASE( "Read byte data", "[I2C_DEVICE]" ) {
    SECTION( "Correct reading" ) {
        Mock<ProxyInterface> proxyMock;
        When(Method(proxyMock,i2cOpen)).Return(14);
        When(Method(proxyMock,i2cReadByteData)).Return(0x79);
        ProxyInterface &proxy = proxyMock.get();

        auto *device = new Device(1, 0x63, 0, &proxy);
        device->open();
        unsigned result = device->readByteData(0x12);

        REQUIRE(result == 0x79);
        Verify(Method(proxyMock,i2cReadByteData)).Once();
        Verify(Method(proxyMock,i2cReadByteData).Using(14, 0x12));
    }

    SECTION( "Throw logic exception if device is not open" ) {
        Mock<ProxyInterface> proxyMock;
        ProxyInterface &proxy = proxyMock.get();

        auto *device = new Device(1, 0x63, 0, &proxy);
        try {
            device->readByteData(0x12);
            FAIL("No exception was thrown");
        } catch (LogicException& e) {
            REQUIRE(e.getMessage() == "Unable to interact with non-open i2c device");
        }
    }

    SECTION( "Throw exception on invalid handle" ) {
        Mock<ProxyInterface> proxyMock;
        When(Method(proxyMock,i2cOpen)).Return(14);
        When(Method(proxyMock,i2cReadByteData)).Return(PI_BAD_HANDLE);
        ProxyInterface &proxy = proxyMock.get();

        auto *device = new Device(1, 0x63, 0, &proxy);
        device->open();

        try {
            device->readByteData(0x12);
            FAIL("No exception was thrown");
        } catch (PigpioException& e) {
            REQUIRE(e.getMessage() == "Data transfer (byte read) with i2c device failed => invalid handle used");
        }
    }

    SECTION( "Throw exception on register parameter" ) {
        Mock<ProxyInterface> proxyMock;
        When(Method(proxyMock,i2cOpen)).Return(14);
        When(Method(proxyMock,i2cReadByteData)).Return(PI_BAD_PARAM);
        ProxyInterface &proxy = proxyMock.get();

        auto *device = new Device(1, 0x63, 0, &proxy);
        device->open();

        try {
            device->readByteData(0x12);
            FAIL("No exception was thrown");
        } catch (PigpioException& e) {
            REQUIRE(e.getMessage() == "Data transfer (byte read) with i2c device failed => invalid parameter (register)");
        }
    }

    SECTION( "Throw exception on internal device error" ) {
        Mock<ProxyInterface> proxyMock;
        When(Method(proxyMock,i2cOpen)).Return(14);
        When(Method(proxyMock,i2cReadByteData)).Return(PI_I2C_READ_FAILED);
        ProxyInterface &proxy = proxyMock.get();

        auto *device = new Device(1, 0x63, 0, &proxy);
        device->open();

        try {
            device->readByteData(0x12);
            FAIL("No exception was thrown");
        } catch (PigpioException& e) {
            REQUIRE(e.getMessage() == "Data transfer (byte read) with i2c device failed => internal device error");
        }
    }

    SECTION( "Throw exception on unknown error" ) {
        Mock<ProxyInterface> proxyMock;
        When(Method(proxyMock,i2cOpen)).Return(14);
        When(Method(proxyMock,i2cReadByteData)).Return(-99);
        ProxyInterface &proxy = proxyMock.get();

        auto *device = new Device(1, 0x63, 0, &proxy);
        device->open();

        try {
            device->readByteData(0x12);
            FAIL("No exception was thrown");
        } catch (PigpioException& e) {
            REQUIRE(e.getMessage() == "Data transfer (byte read) with i2c device failed => unknown error (RC:-99)");
        }
    }
}


TEST_CASE( "Write byte data", "[I2C_DEVICE]" ) {
    SECTION( "Correct writing" ) {
        Mock<ProxyInterface> proxyMock;
        When(Method(proxyMock,i2cOpen)).Return(14);
        When(Method(proxyMock,i2cWriteByteData)).Return(0);
        ProxyInterface &proxy = proxyMock.get();

        auto *device = new Device(1, 0x63, 0, &proxy);
        device->open();
        device->writeByteData(0x12, 0x10);
        
        Verify(Method(proxyMock,i2cWriteByteData)).Once();
        Verify(Method(proxyMock,i2cWriteByteData).Using(14, 0x12, 0x10));
    }

    SECTION( "Throw logic exception if device is not open" ) {
        Mock<ProxyInterface> proxyMock;
        ProxyInterface &proxy = proxyMock.get();

        auto *device = new Device(1, 0x63, 0, &proxy);
        try {
            device->writeByteData(0x12, 0x10);
            FAIL("No exception was thrown");
        } catch (LogicException& e) {
            REQUIRE(e.getMessage() == "Unable to interact with non-open i2c device");
        }
    }

    SECTION( "Throw exception on invalid handle" ) {
        Mock<ProxyInterface> proxyMock;
        When(Method(proxyMock,i2cOpen)).Return(14);
        When(Method(proxyMock,i2cWriteByteData)).Return(PI_BAD_HANDLE);
        ProxyInterface &proxy = proxyMock.get();

        auto *device = new Device(1, 0x63, 0, &proxy);
        device->open();

        try {
            device->writeByteData(0x12, 0x10);
            FAIL("No exception was thrown");
        } catch (PigpioException& e) {
            REQUIRE(e.getMessage() == "Data transfer (byte write) with i2c device failed => invalid handle used");
        }
    }

    SECTION( "Throw exception on register parameter" ) {
        Mock<ProxyInterface> proxyMock;
        When(Method(proxyMock,i2cOpen)).Return(14);
        When(Method(proxyMock,i2cWriteByteData)).Return(PI_BAD_PARAM);
        ProxyInterface &proxy = proxyMock.get();

        auto *device = new Device(1, 0x63, 0, &proxy);
        device->open();

        try {
            device->writeByteData(0x12, 0x10);
            FAIL("No exception was thrown");
        } catch (PigpioException& e) {
            REQUIRE(e.getMessage() == "Data transfer (byte write) with i2c device failed => invalid parameter (register)");
        }
    }

    SECTION( "Throw exception on internal device error" ) {
        Mock<ProxyInterface> proxyMock;
        When(Method(proxyMock,i2cOpen)).Return(14);
        When(Method(proxyMock,i2cWriteByteData)).Return(PI_I2C_WRITE_FAILED);
        ProxyInterface &proxy = proxyMock.get();

        auto *device = new Device(1, 0x63, 0, &proxy);
        device->open();

        try {
            device->writeByteData(0x12, 0x10);
            FAIL("No exception was thrown");
        } catch (PigpioException& e) {
            REQUIRE(e.getMessage() == "Data transfer (byte write) with i2c device failed => internal device error");
        }
    }

    SECTION( "Throw exception on unknown error" ) {
        Mock<ProxyInterface> proxyMock;
        When(Method(proxyMock,i2cOpen)).Return(14);
        When(Method(proxyMock,i2cWriteByteData)).Return(-99);
        ProxyInterface &proxy = proxyMock.get();

        auto *device = new Device(1, 0x63, 0, &proxy);
        device->open();

        try {
            device->writeByteData(0x12, 0x10);
            FAIL("No exception was thrown");
        } catch (PigpioException& e) {
            REQUIRE(e.getMessage() == "Data transfer (byte write) with i2c device failed => unknown error (RC:-99)");
        }
    }
}

#pragma clang diagnostic push