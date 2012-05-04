################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/scene/particleEngine/Explosion.cpp \
../src/scene/particleEngine/Smoke.cpp \
../src/scene/particleEngine/Sparkle.cpp 

OBJS += \
./src/scene/particleEngine/Explosion.o \
./src/scene/particleEngine/Smoke.o \
./src/scene/particleEngine/Sparkle.o 

CPP_DEPS += \
./src/scene/particleEngine/Explosion.d \
./src/scene/particleEngine/Smoke.d \
./src/scene/particleEngine/Sparkle.d 


# Each subdirectory must supply rules for building sources it contributes
src/scene/particleEngine/%.o: ../src/scene/particleEngine/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


