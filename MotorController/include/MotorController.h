#ifndef _MOTORCONTROLLER_
#define _MOTORCONTROLLER_
    #include <iostream>
    #include <iostream>
    #include <unistd.h>
    #include <string.h>
    #include "Definitions.h"
#endif

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

#define MAXON_RS232 "MAXON_RS232"
#define MAXON_SERIAL_V2 "MAXON SERIAL V2"
#define CANopen "CANopen"

#define TRUE 1
#define FALSE 0

using std::cout;
using std::cin;
using std::endl;
using std::string;

typedef void* HANDLE;

namespace epos {
    void LogError(string functionName, unsigned int errorCode);
    void LogInfo(string info);

    class MotorController {
        public:
            MotorController();
            ~MotorController();

        private:

    };
};
