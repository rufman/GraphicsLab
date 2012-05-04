################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Tank/bullet.cpp \
../Tank/cannon.cpp \
../Tank/main.cpp \
../Tank/scene.cpp \
../Tank/sea.cpp \
../Tank/target.cpp \
../Tank/textBox.cpp \
../Tank/time.cpp \
../Tank/types.cpp \
../Tank/utils.cpp \
../Tank/window.cpp \
../Tank/windowManager.cpp 

OBJS += \
./Tank/bullet.o \
./Tank/cannon.o \
./Tank/main.o \
./Tank/scene.o \
./Tank/sea.o \
./Tank/target.o \
./Tank/textBox.o \
./Tank/time.o \
./Tank/types.o \
./Tank/utils.o \
./Tank/window.o \
./Tank/windowManager.o 

CPP_DEPS += \
./Tank/bullet.d \
./Tank/cannon.d \
./Tank/main.d \
./Tank/scene.d \
./Tank/sea.d \
./Tank/target.d \
./Tank/textBox.d \
./Tank/time.d \
./Tank/types.d \
./Tank/utils.d \
./Tank/window.d \
./Tank/windowManager.d 


# Each subdirectory must supply rules for building sources it contributes
Tank/%.o: ../Tank/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


