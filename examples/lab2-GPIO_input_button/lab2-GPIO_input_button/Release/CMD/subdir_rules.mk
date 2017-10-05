################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
CMD/28335_RAM_lnk.exe: D:/DSP(ccs4.12)/lab2-GPIO_input_button/CMD/28335_RAM_lnk.cmd $(GEN_CMDS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Linker'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.0/bin/cl2000" -v28 -ml -O3 --define="LARGE_MODEL" --diag_warning=225 --display_error_number --diag_wrap=off --obj_directory="D:/DSP(ccs4.12)/lab2-GPIO_input_button/Release" -z -m"D:/DSP(ccs4.12)/lab2-GPIO_input_button/CMD/Release/GPIO_in.map" --warn_sections -i"C:/ti/ccsv5/tools/compiler/c2000_6.1.0/lib" -i"C:/ti/ccsv5/tools/compiler/c2000_6.1.0/include" -i"D:/DSP(ccs4.12)/lab2-GPIO_input_button/lab2-GPIO_input_button" -i"D:/DSP(ccs4.12)/lab2-GPIO_input_button" --reread_libs --display_error_number --diag_wrap=off --rom_model -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

CMD/DSP2833x_Headers_nonBIOS.exe: D:/DSP(ccs4.12)/lab2-GPIO_input_button/CMD/DSP2833x_Headers_nonBIOS.cmd $(GEN_CMDS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Linker'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.0/bin/cl2000" -v28 -ml -O3 --define="LARGE_MODEL" --diag_warning=225 --display_error_number --diag_wrap=off --obj_directory="D:/DSP(ccs4.12)/lab2-GPIO_input_button/Release" -z -m"D:/DSP(ccs4.12)/lab2-GPIO_input_button/CMD/Release/GPIO_in.map" --warn_sections -i"C:/ti/ccsv5/tools/compiler/c2000_6.1.0/lib" -i"C:/ti/ccsv5/tools/compiler/c2000_6.1.0/include" -i"D:/DSP(ccs4.12)/lab2-GPIO_input_button/lab2-GPIO_input_button" -i"D:/DSP(ccs4.12)/lab2-GPIO_input_button" --reread_libs --display_error_number --diag_wrap=off --rom_model -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


