################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/scene/lightning/DirectionalLight.cpp \
../src/scene/lightning/Light.cpp \
../src/scene/lightning/PositionalLight.cpp \
../src/scene/lightning/SpotLight.cpp 

OBJS += \
./src/scene/lightning/DirectionalLight.o \
./src/scene/lightning/Light.o \
./src/scene/lightning/PositionalLight.o \
./src/scene/lightning/SpotLight.o 

CPP_DEPS += \
./src/scene/lightning/DirectionalLight.d \
./src/scene/lightning/Light.d \
./src/scene/lightning/PositionalLight.d \
./src/scene/lightning/SpotLight.d 


# Each subdirectory must supply rules for building sources it contributes
src/scene/lightning/%.o: ../src/scene/lightning/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


