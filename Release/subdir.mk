################################################################################
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../GPIO.c \
../Led.c \
../OSC.c \
../SoftPwm.c \
../SoftSpi.c \
../Work.c \
../config_set.c \
../main.c \
../timer.c 

OBJS += \
./GPIO.o \
./Led.o \
./OSC.o \
./SoftPwm.o \
./SoftSpi.o \
./Work.o \
./config_set.o \
./main.o \
./timer.o 

C_DEPS += \
./GPIO.dep \
./Led.dep \
./OSC.dep \
./SoftPwm.dep \
./SoftSpi.dep \
./Work.dep \
./config_set.dep \
./main.dep \
./timer.dep 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo '���ڹ����ļ��� $<'
	@echo '���ڵ��ã� C Compiler'
	kf8cc -c -pKF8A200GQS -I"C:\ChipONCC\include" -I"C:\Users\Rick\Desktop\EF1E-B" -o"$@" "$<"
	@echo '�ѽ��������� $<'
	@echo ' '


