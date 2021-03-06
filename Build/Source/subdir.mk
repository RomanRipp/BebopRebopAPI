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
../Source/CVideoInterface.cpp \
../Source/CBebopCallbacks.cpp \
../Source/CBebopState.cpp \
../Source/CVideoDecoder.cpp \
../Source/Utility.cpp \
../Source/Tests/main.cpp \

OBJS += \
./Source/CBebopInterface.o \
./Source/CCommandPacket.o \
./Source/CNetworkInterface.o \
./Source/CNetworkSettings.o \
./Source/CVehicleInterface.o \
./Source/CVideoInterface.o \
./Source/CBebopCallbacks.o \
./Source/CVideoDecoder.o \
./Source/CBebopState.o \
./Source/Utility.o \
./Source/Tests/main.o \

CPP_DEPS += \
./Source/CBebopInterface.d \
./Source/CCommandPacket.d \
./Source/CNetworkInterface.d \
./Source/CNetworkSettings.d \
./Source/CVehicleInterface.d \
./Source/CVideoInterface.d \
./Source/CBebopCallbacks.d \
./Source/CVideoDecoder.d \
./Source/CBebopState.d \
./Source/Utility.d \
./Source/Tests/main.d \

# Each subdirectory must supply rules for building sources it contributes
Source/%.o: ../Source/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I /usr/local/include -I../ThirdPartyLibs/ArDroneSDK3/include -I../ThirdPartyLibs/rapidjson/include -I../ThirdPartyLibs/easyloggingpp/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


