onbreak {quit -f}
onerror {quit -f}

vsim -voptargs="+acc" -t 1ps -L xbip_utils_v3_0_9 -L xbip_pipe_v3_0_5 -L xbip_bram18k_v3_0_5 -L mult_gen_v12_0_14 -L xbip_dsp48_wrapper_v3_0_4 -L xbip_dsp48_addsub_v3_0_5 -L xbip_dsp48_multadd_v3_0_5 -L xbip_multadd_v3_0_13 -L xil_defaultlib -L secureip -lib xil_defaultlib xil_defaultlib.xbip_multadd_0

do {wave.do}

view wave
view structure
view signals

do {xbip_multadd_0.udo}

run -all

quit -force
