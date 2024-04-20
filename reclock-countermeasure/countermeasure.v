/* Countermeasure.v

   Simple example countermeasure using TinyFPGA BX.

*/

module top (
   input  CLK,   // 16 MHz on-board clock
   output LED,   // on-board LED
   output USBPU,  // USB pull-up enable, set low to disable

   input PIN_21, //TDO input (from target)
   output PIN_8, //TDO output (to attacker)
   output PIN_3 //debug


   );

    /**
    * PLL configuration
    *
    * This Verilog module was generated automatically
    * using the icepll tool from the IceStorm project.
    * Use at your own risk.
    *
    * Given input frequency:        16.000 MHz
    * Requested output frequency:  150.000 MHz
    * Achieved output frequency:   148.000 MHz
    */
    wire clock_in;
    wire clock_out;
    wire locked;
    SB_PLL40_CORE #(
                    .FEEDBACK_PATH("SIMPLE"),
/*
                     //Settings for 148 MHz
                    .DIVR(4'b0000),         // DIVR =  0
                    .DIVF(7'b0100100),      // DIVF = 36
                    .DIVQ(3'b010),          // DIVQ =  2
*/


                     //Settings for 50 MHz
                    .DIVR(4'b0000),         // DIVR =  0
                    .DIVF(7'b0110001),      // DIVF = 49
                    .DIVQ(3'b100),          // DIVQ =  4


                    .FILTER_RANGE(3'b001)   // FILTER_RANGE = 1
            ) uut (
                    .LOCK(locked),
                    .RESETB(1'b1),
                    .BYPASS(1'b0),
                    .REFERENCECLK(clock_in),
                    .PLLOUTCORE(clock_out)
                    );


  reg relatched;
  wire clock;

  //PLL connection
  assign clock_in = CLK;
  assign clock = clock_out;

  //Countermeasure OFF - uncomment this block
  assign relatched = PIN_21;

  //Countermeasure ON - uncomment this block
  //always @(posedge clock) relatched <= PIN_21;

   assign PIN_8 = relatched;
   assign PIN_3 = relatched;


   assign LED = 1'b1;  // Set LED on
   assign USBPU = 1'b0;   // disable USB
endmodule  // top
