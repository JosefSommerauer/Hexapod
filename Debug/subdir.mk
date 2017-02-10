################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Hexapod.cpp \
../InverseKinematic.cpp \
../Leg.cpp \
../LegCtrl.cpp \
../Movement.cpp \
../Object.cpp \
../Parser.cpp \
../RippleGait.cpp \
../Servo.cpp \
../SocketServer.cpp \
../TripodGait.cpp \
../WaveGait.cpp \
../main.cpp 

OBJS += \
./Hexapod.o \
./InverseKinematic.o \
./Leg.o \
./LegCtrl.o \
./Movement.o \
./Object.o \
./Parser.o \
./RippleGait.o \
./Servo.o \
./SocketServer.o \
./TripodGait.o \
./WaveGait.o \
./main.o 

CPP_DEPS += \
./Hexapod.d \
./InverseKinematic.d \
./Leg.d \
./LegCtrl.d \
./Movement.d \
./Object.d \
./Parser.d \
./RippleGait.d \
./Servo.d \
./SocketServer.d \
./TripodGait.d \
./WaveGait.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo ' '


