################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/scene/illumination/DirectionalLight.cpp \
../src/scene/illumination/Light.cpp \
../src/scene/illumination/PositionalLight.cpp \
../src/scene/illumination/SpotLight.cpp 

OBJS += \
./src/scene/illumination/DirectionalLight.o \
./src/scene/illumination/Light.o \
./src/scene/illumination/PositionalLight.o \
./src/scene/illumination/SpotLight.o 

CPP_DEPS += \
./src/scene/illumination/DirectionalLight.d \
./src/scene/illumination/Light.d \
./src/scene/illumination/PositionalLight.d \
./src/scene/illumination/SpotLight.d 


# Each subdirectory must supply rules for building sources it contributes
src/scene/illumination/%.o: ../src/scene/illumination/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


