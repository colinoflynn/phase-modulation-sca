#include "sam4s_hal.h"
#include "genclk.h"
#include "osc.h"
#include "pio.h"
#include "uart.h"
#include "sysclk.h"
#include "gpio.h"

/*
define options:

PLL_120MHZ
PLL_15MHZ
USE_PLL
USE_EXTERNAL_CLOCK
USE_EMBEDDED_CLOCK
PCLK_ENABLED
*/

//12 MHz clock input
//#define USE_EMBEDDED_CLOCK

//External clock should be 12 MHz in
//#define USE_EXTERNAL_CLOCK

//Turn on the PLL
//#define USE_PLL

//Set PLL frequency 
//#define PLL_120MHZ
//#define PLL_15MHZ

//Turn on the PCLK output
//#defined PCLK_ENABLED


void platform_init(void)
{
	WDT->WDT_MR = WDT_MR_WDDIS;
    sysclk_enable_peripheral_clock(ID_SUPC);
    sysclk_enable_peripheral_clock(ID_RSTC);
    sysclk_enable_peripheral_clock(ID_PMC);
    pmc_set_writeprotect(0);
    system_init_flash(CHIP_FREQ_CPU_MAX);
    pmc_switch_mck_to_sclk(SYSCLK_PRES_1);

    //We have started boot - LED on
    gpio_configure_pin(PIO_PA16_IDX, PIO_OUTPUT_1 | PIO_DEFAULT);

#ifdef USE_PLL
    // // ===== PLL0 (A) Options   (Fpll = (Fclk * PLL_mul) / PLL_div)
    // // Use mul and div effective values here.
    // #define CONFIG_PLL0_SOURCE          PLL_SRC_MAINCK_XTAL
    // #define CONFIG_PLL0_MUL             20
    // #define CONFIG_PLL0_DIV             1

    // // ===== Target frequency (System clock)
    // // - XTAL frequency: 12MHz
    // // - System clock source: PLLA
    // // - System clock prescaler: 2 (divided by 2)
    // // - PLLA source: XTAL
    // // - PLLA output: XTAL * 20 / 1
    // // - System clock: 12 * 20 / 1 / 2 = 120MHz


    /* Set 6 FWS for Embedded Flash Access according to 120MHz configuration */
    EFC0->EEFC_FMR = EEFC_FMR_FWS(5)|EEFC_FMR_CLOE;
    #if defined(EFC1) // Only valid for products with two flash banks
    EFC1->EEFC_FMR = EEFC_FMR_FWS(5)|EEFC_FMR_CLOE;
    #endif // EFC1

    /*
    * We are coming from a Hard Reset or Backup mode.
    * The core is clocked by Internal Fast RC @ 4MHz.
    * We intend to use the device @120MHz from external Oscillator.
    * Steps are (cf datasheet chapter '29.14 Programming Sequence'):
    * 1- Activation of external oscillator
    * 2- Switch the MAINCK to the main crystal oscillator
    * 3- Wait for the MOSCSELS to be set
    * 4- Check the main clock frequency
    * 5- Set PLLx and Divider
    * 6- Select the master clock and processor clock
    * 7- Select the programmable clocks (optional)
    */

#ifdef USE_EXTERNAL_CLOCK
    /* Step 1 - Activation of external oscillator
    * As we are clocking the core from internal Fast RC, we keep the bit CKGR_MOR_MOSCRCEN.
    * Main Crystal Oscillator Start-up Time (CKGR_MOR_MOSCXTST) is set to maximum value.
    * Then, we wait the startup time to be finished by checking PMC_SR_MOSCXTS in PMC_SR.
    */
    PMC->CKGR_MOR = CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCXTST(0xfful) | CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN;
    for ( ; (PMC->PMC_SR & PMC_SR_MOSCXTS) != PMC_SR_MOSCXTS ; );

    /* Step 2 - Switch the MAINCK to the main crystal oscillator
    * We add the CKGR_MOR_MOSCSEL bit.
    * Then we wait for the selection to be done by checking PMC_SR_MOSCSELS in PMC_SR.
    */
    //#ifdef USE_EMBEDDED_CLOCK
    //PMC->CKGR_MOR = CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCXTST(0xfful) | CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN;
    //#else
    PMC->CKGR_MOR = CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCXTST(0xfful) | CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN | CKGR_MOR_MOSCSEL;
    //#endif /* USE_EMBEEDED_CLOCK */
    
    /* Step 3 - Wait for the MOSCSELS to be set */
    //#ifndef USE_EMBEDDED_CLOCK
    for ( ; (PMC->PMC_SR & PMC_SR_MOSCSELS) != PMC_SR_MOSCSELS ; );
    //#endif
#endif
    /* Step 4 - Check the main clock frequency */
    /* As written in the DS, we could check the MAINF value here (0x18a2) */

#ifdef USE_EMBEDDED_CLOCK
    pmc_switch_mainck_to_fastrc(CKGR_MOR_MOSCRCF_12_MHz);
#endif

    /* Step 5 - Set PLLx and Divider
    * The external oscillator is 12MHz. As we intend to clock the system @120MHz,
    * we need to multiply the oscillator frequency by 10.
    * This can be done by setting MULx to value 9 and DIV to 1.
    * We set the maximum PLL Lock time to maximum in CKGR_PLLAR_PLLACOUNT.
    */
    //PMC->CKGR_PLLAR = CKGR_PLLAR_ONE | (CKGR_PLLAR_MULA(0x1dul) | CKGR_PLLAR_DIVA(3ul) | CKGR_PLLAR_PLLACOUNT(0x1ul));
    //PMC->CKGR_PLLAR = CKGR_PLLAR_ONE | (CKGR_PLLAR_MULA(0x9ul) | CKGR_PLLAR_DIVA(1ul) | CKGR_PLLAR_PLLACOUNT(0x3ful)); //120 MHz
    //PMC->CKGR_PLLAR = CKGR_PLLAR_ONE | (CKGR_PLLAR_MULA(0x9ul) | CKGR_PLLAR_DIVA(16ul) | CKGR_PLLAR_PLLACOUNT(0x3ful)); //7.5 MHz

#if defined(PLL_120MHZ)
    PMC->CKGR_PLLAR = CKGR_PLLAR_ONE | (CKGR_PLLAR_MULA(0x9ul) | CKGR_PLLAR_DIVA(1ul) | CKGR_PLLAR_PLLACOUNT(0x3ful)); //120 MHZ
#elif defined(PLL_15MHZ)
    PMC->CKGR_PLLAR = CKGR_PLLAR_ONE | (CKGR_PLLAR_MULA(0x9ul) | CKGR_PLLAR_DIVA(8ul) | CKGR_PLLAR_PLLACOUNT(0x3ful)); //15 MHz
#elif defined(PLL_5MHZ)
    PMC->CKGR_PLLAR = CKGR_PLLAR_ONE | (CKGR_PLLAR_MULA(0x9ul) | CKGR_PLLAR_DIVA(24ul) | CKGR_PLLAR_PLLACOUNT(0x3ful)); //5 MHz
#elif defined(PLL_2MHZ)
    PMC->CKGR_PLLAR = CKGR_PLLAR_ONE | (CKGR_PLLAR_MULA(0x9ul) | CKGR_PLLAR_DIVA(60ul) | CKGR_PLLAR_PLLACOUNT(0x3ful)); //2 MHz
#else
#error "PLL UNDEFINED FREQUENCY"
#endif

    for ( ; (PMC->PMC_SR & PMC_SR_LOCKA) != PMC_SR_LOCKA ; );

    /* Step 6 - Select the master clock and processor clock
    * Source for MasterClock will be PLLA output (PMC_MCKR_CSS_PLLA_CLK), without frequency division.
    */
    PMC->PMC_MCKR = PMC_MCKR_PRES_CLK_2 | PMC_MCKR_CSS_PLLA_CLK;
    for ( ; (PMC->PMC_SR & PMC_SR_MCKRDY) != PMC_SR_MCKRDY ; );

    //PMC->PMC_SCER = PMC_SCER_PCK0 ;
    //PMC->PMC_PCK[0] = 2;

    SystemCoreClockUpdate();
    
    //#ifdef USE_EMBEDDED_CLOCK
    // BP: Switch to use Fast RC clock instead of the external clock source
    //pmc_switch_mainck_to_fastrc(CKGR_MOR_MOSCRCF_12_MHz);
    //pmc_enable_pck(0);
    //pmc_switch_pck_to_mainck(0, PMC_MCKR_PRES_CLK_1);
    //gpio_configure_pin(PIO_PA6_IDX, PIO_PERIPH_B | PIO_DEFAULT);
    //#else

    //PCLLK0 on
#ifdef PCLK_ENABLED
    pmc_enable_pck(0);
    pmc_switch_pck_to_pllack(0, PMC_MCKR_PRES_CLK_2);
    gpio_configure_pin(PIO_PA6_IDX, PIO_PERIPH_B | PIO_DEFAULT);
#else
    gpio_configure_pin(PIO_PA6_IDX, PIO_OUTPUT_1 | PIO_DEFAULT);
#endif

    //Another LED on to indicate booting off PLL is done
    gpio_configure_pin(PIO_PA14_IDX, PIO_OUTPUT_1 | PIO_DEFAULT);

#else
    osc_enable(OSC_MAINCK_BYPASS);
    osc_wait_ready(OSC_MAINCK_BYPASS);
    pmc_switch_mck_to_mainck(SYSCLK_PRES_1);
    
    // BP: ENABLE CLOCK ON OUTPUT PIN
#ifdef PCLK_ENABLED
    pmc_enable_pck(0);
    pmc_switch_pck_to_mainck(0, SYSCLK_PRES_1);
    gpio_configure_pin(PIO_PA6_IDX, PIO_PERIPH_B | PIO_DEFAULT);
#else

#ifdef PCLK_PIN_LOW
    gpio_configure_pin(PIO_PA6_IDX, PIO_OUTPUT_0 | PIO_DEFAULT);
#else
    gpio_configure_pin(PIO_PA6_IDX, PIO_OUTPUT_1 | PIO_DEFAULT);
#endif
#endif
    
    SystemCoreClockUpdate();
    system_init_flash(7.37E6);
#endif

    EFC0->EEFC_FMR &= ~(0b100 << 24); // disable code loop optimizations (causes trace desyncs)
    // EFC0->EEFC_FMR &= ~(0b001 << 16); // disable code loop optimizations (causes trace desyncs)

    sysclk_enable_peripheral_clock(ID_PIOA);
    gpio_configure_pin(PIO_PA15_IDX, PIO_OUTPUT_1 | PIO_DEFAULT);
}

void init_uart(void)
{
    // todo: just set these registers manually, doing so is simple and avoids asf rounding errors
    sam_uart_opt_t uopts;
    //#if SS_VER == SS_VER_2_1
    //    uopts.ul_baudrate = 230400;
    //#else
    uopts.ul_baudrate = 38400;
    //#endif

#ifdef USE_PLL
    //CO: Clock is 120 MHz / 2 for peripherals.
    //    This hack works at 120 MHz now.
    #if defined(PLL_120MHZ)
        uopts.ul_mck = 60E6;
    #elif defined(PLL_15MHZ)
        uopts.ul_mck = 7.5E6;
    #elif defined(PLL_5MHZ)
        uopts.ul_mck = 2.5E6;
    #elif defined(PLL_2MHZ)
        uopts.ul_mck = 1E6;
    #else
        #error "UNDEFINED PLL FREQ"
    #endif


    
#else
    uopts.ul_mck = 7.45E6; //overestimate clock a little to avoid large baud error from calc rounding down, 38400/7.37E6 results in baud of 41k, not 38400
#endif

    uopts.ul_mode = (0x00 << 14) | (0x04 << 9);

    sysclk_enable_peripheral_clock(ID_UART0);


    uart_init(UART0, &uopts);
    uart_enable_rx(UART0);
    uart_enable_tx(UART0);
    gpio_configure_pin(PIO_PA9_IDX, PIO_PERIPH_A | PIO_DEFAULT | PIO_PULLUP);
    gpio_configure_pin(PIO_PA10_IDX, PIO_PERIPH_A | PIO_DEFAULT);
}

void putch(char c)
{
    // while (!uart_is_tx_buf_end(UART0));
    while(uart_write(UART0, c));
}

char getch(void)
{
    uint8_t c;
    while(uart_read(UART0, &c));
    return c;
}

void trigger_setup(void)
{
    sysclk_enable_peripheral_clock(ID_PIOA);
    gpio_configure_pin(PIO_PA7_IDX, PIO_OUTPUT_0 | PIO_DEFAULT);
}

void trigger_low(void)
{
    gpio_set_pin_low(PIO_PA7_IDX);
    gpio_configure_pin(PIO_PA7_IDX, PIO_OUTPUT_0 | PIO_DEFAULT);
}

void trigger_high(void)
{
    gpio_set_pin_high(PIO_PA7_IDX);
    gpio_configure_pin(PIO_PA7_IDX, PIO_OUTPUT_1 | PIO_DEFAULT);
}
