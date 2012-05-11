################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/scene/entities/Bullet.cpp \
../src/scene/entities/SmallTank.cpp \
../src/scene/entities/Tank.cpp \
../src/scene/entities/Turret.cpp 

OBJS += \
./src/scene/entities/Bullet.o \
./src/scene/entities/SmallTank.o \
./src/scene/entities/Tank.o \
./src/scene/entities/Turret.o 

CPP_DEPS += \
./src/scene/entities/Bullet.d \
./src/scene/entities/SmallTank.d \
./src/scene/entities/Tank.d \
./src/scene/entities/Turret.d 


# Each subdirectory must supply rules for building sources it contributes
src/scene/entities/%.o: ../src/scene/entities/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


