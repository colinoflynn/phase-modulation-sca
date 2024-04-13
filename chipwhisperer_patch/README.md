# ChipWhisperer-Husky JTAG Attack Mode

This patch adds the option to route a clock out on TCK/TDO. It is enabled by writing to one of the registers via the USB command, see notebooks for demos.

We will see if ChipWhisperer would be interested in having this patch, but as seen it is a simple Verilog patch you can apply locally.

### Pre-built bitstreams

There is no need to rebuild this if you want to test. We have included bitstreams in the `notebooks_capture\firmwares` folder. They are used by the
capture notebooks by default.

### Adjusting CLocks

This outputs a clock on TCK and TDO which is based on the `CLKGEN` clock (e.g., you'd set with `clkgen_freq`).

The *ratio* between these different clocks can be adjusted by changing the Verilog source & resynthesizing.

Note the PLL block VCO needs to be in the range 800-1600MHz. When Vivado runs synthesis it will pull the input clock from the constraints, which is set to 200 MHz.
This means the "multiply" is set to `8`. I found in practice this works even if you are running the CLKGEN much slower (down to ~10MHz or so), which means your VCO
is only going to be running at ~80-100 MHz! Unfortunutly Vivado gives an error if you try setting the multiply higher because it thinks you will be exceeding the maximum
VCO frequency.

Here is the important settings, where `CLKFBOUT_MULT` is the multiply I mean.

```
   PLLE2_BASE #(
       .BANDWIDTH("OPTIMIZED"),  // OPTIMIZED, HIGH, LOW
       .CLKFBOUT_MULT(8),        // Multiply value for all CLKOUT, (2-64)
       .CLKFBOUT_PHASE(0.0),     // Phase offset in degrees of CLKFB, (-360.000-360.000).
       .CLKIN1_PERIOD(0.0),      // Input clock period in ns to ps resolution (i.e. 33.333 is 30 MHz) (IGNORED)
       // CLKOUT0_DIVIDE - CLKOUT5_DIVIDE: Divide amount for each CLKOUT (1-128)
       .CLKOUT0_DIVIDE(8), //8
       .CLKOUT1_DIVIDE(16), //16
    ...
```

The TCK divide is set by `CLKOUT0_DIVIDE`, here it is set to 8 to match `CLKFBOUT_MULT`. This means TCK will follow CLKGEN.

The TDI divide is set by `CLKOUT1_DIVIDE`. here it is set to 16 so you have `TDI` running at half the frequency of TCK.

You can adjust these ratios as you wish.

There is also some phase offsets you can experiment with, see the Xilinx `PLLE2` documentation. Note there is also the "advanced"
version you can replace this with for even more settings.