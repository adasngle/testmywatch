# Define source file lists to SRC_LIST

SRC_LIST = gps\src\gps_init.c \
           gps\src\gps_inject_msg.c \
           gps\src\gps_main.c \
           gps\src\gps_sm.c \
           gps\src\gps_uart.c \
           gps\src\gps_auth.c \
           gps\src\gps_modem.c 

#  Define include path lists to INC_DIR
INC_DIR = gps\inc \
          gps\mnl\include \
          gps\mnl\MNL_OIL \
          plutommi\service\mdi\mdiinc \
          plutommi\Framework\EventHandling\EventsInc \
          plutommi\Framework\Tasks\tasksinc \
          plutommi\Framework\MemManager\MemManagerInc \
          plutommi\Framework\DebugLevels\DebugLevelInc \
          applib\misc\include \
          plutommi\MtkApp\AGPSLog\AGPSLogInc \
          interface\hal\peripheral

# Define the specified compile options to COMP_DEFS
COMP_DEFS = 

