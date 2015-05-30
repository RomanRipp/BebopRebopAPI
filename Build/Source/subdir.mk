################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Source/CBebopInterface.cpp \
../Source/CCommandPacket.cpp \
../Source/CNetworkInterface.cpp \
../Source/CNetworkSettings.cpp \
../Source/CVehicleInterface.cpp \
<<<<<<< HEAD
../Source/CVideoInterface.cpp \
../Source/Utility.cpp \
#../Source/main.cpp 
=======
../Source/Utility.cpp \
../Source/main.cpp 
>>>>>>> fc7d4cf3c62ddf5eb895513094771b567f5f56c8

OBJS += \
./Source/CBebopInterface.o \
./Source/CCommandPacket.o \
./Source/CNetworkInterface.o \
./Source/CNetworkSettings.o \
./Source/CVehicleInterface.o \
<<<<<<< HEAD
./Source/CVideoInterface.o \
./Source/Utility.o \
#./Source/main.o
=======
./Source/Utility.o \
./Source/main.o 
>>>>>>> fc7d4cf3c62ddf5eb895513094771b567f5f56c8

CPP_DEPS += \
./Source/CBebopInterface.d \
./Source/CCommandPacket.d \
./Source/CNetworkInterface.d \
./Source/CNetworkSettings.d \
./Source/CVehicleInterface.d \
<<<<<<< HEAD
./Source/CVideoInterface.d \
./Source/Utility.d \
#./Source/main.d
=======
./Source/Utility.d \
./Source/main.d 
>>>>>>> fc7d4cf3c62ddf5eb895513094771b567f5f56c8


# Each subdirectory must supply rules for building sources it contributes
Source/%.o: ../Source/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../ThirdPartyLibs/ArDroneSDK3/include -I../ThirdPartyLibs/rapidjson/include -I../ThirdPartyLibs/easyloggingpp/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


