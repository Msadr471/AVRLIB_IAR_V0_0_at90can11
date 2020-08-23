/**
   @file spi_lib.c   

   @brief This file provides a minimal funtion set for the SPI
   
   The function return value on int (for char value wanted) :
   -# 0=< xx <0x100 : good value
   -#     xx <0     : error/warning :
         - odd  : error like colision or probably never ansver corectilly : -1 -3 ...
         - even : warning like no data, data lost : -2 -...
         .
   .
  
   @par Copyright (c) 2004 Atmel. & zoubata
  
   Please read file license.txt for copyright notice.

   @version $Revision: 1.1 $ $Name:  $
  
   @todo
   @bug

   @par CVS Info : 
   
   $Header: Y:\cvs_public/AVR/Lib/spi/spi_lib.c,v 1.1 2005/04/23 21:12:08 zoubata Exp $
   $Date: 2005/04/23 21:12:08 $
   $Revision: 1.1 $

   $History: spi_lib.c $
 
   $Log: spi_lib.c,v $
   Revision 1.1  2005/04/23 21:12:08  zoubata
   1st commit

   Revision 1.5  2004/11/13 23:19:08  zoubata
   *** empty log message ***


*/
/*_____ I N C L U D E S ____________________________________________________*/
#include <config.h>
#include "lib_mcu\spi\spi_lib.h"


/*_____ G L O B A L    D E F I N I T I O N _________________________________*/

/*_____ D E F I N I T I O N ________________________________________________*/

/*_____ M A C R O S ________________________________________________________*/

/** This function configures the SPI controller:
   -# MASTER or SLAVE
   -# bit timing
   -# enable the controller 
   .
   @param config : configuration of the node (MASTER or SLAVE).
  
   @return status of the init:
   -# TRUE
   -# FALSE 
   .
   @pre before calling this function some declaration must be define in config.h:\n
          ONFIG select the prescaler, CPHA leading, CPOL LOW, LSB first.
 */
bit spi_init (spi_cf_t config)
{
  if(config == SPI_MASTER)
  {
    Spi_select_master_mode();
    SPI_SSDDR|=(1<<SPI_SSPIN_PIN);
    SPI_SSPORT&=~(1<<SPI_SSPIN_PIN);// devise selected on SS pin 
  }
  else                    
  {
    Spi_select_slave_mode();
    SPI_SSPORT|=(1<<SPI_SSPIN_PIN);// pull up-> unselect devise without any collision on SS pin
    SPI_SSDDR&=~(1<<SPI_SSPIN_PIN);
  }

  Spi_hw_init(SPI_CONFIG);
  Spi_enable();
  
  return TRUE;
}

/**
   @brief This function checks if a bytes has been received on the SPI.
  
   @return TRUE(say only on time) if byte received.
 */
bit spi_test_hit (void)
{
return Spi_rx_ready();
}

/**
   @brief This function sends a byte on the SPI.
  
   @param ch : character to send on the SPI.
   @return status of transmition : - 0 : ok, 
                                   - (U16)-1 if fail : colision status(say only on time)
                                   - (U16)-2 to -15 : reserved for futur error.
                                   .
 */
unsigned int spi_putchar (unsigned char ch)
{
    Spi_send_byte(ch);
    return (Spi_get_colision_status()?(unsigned int)-1:0);
}

/**
 * @brief This function sends & reads a byte on the SPI.

    /!\ : becareful : spi_Transmitchar & spi_putchar+spi_getchar aren't the same effet :
    if on line there is :
    MISO :   _C1________C2_______C3___...
    MOSI :   _c4________c5_______c6___...
                   ^-call one case the functions :
                 
    -# c_in_a=spi_Transmitchar(c_out_a);          =>  c_out_a=c5 ;    c_in_a==c2; lost C1 if not read before
    -# c_in_b=spi_getchar();spi_putchar(c_out_b); =>  c_out_b=c5 ;    c_in_b==c2
    -# spi_putchar(c_out_b);c_in_b=spi_getchar(); =>  c_out_b=c5 ;    c_in_b==c3;  C2 lost if not read before
    .
    
   @param  cData : character to send on the SPI.
   @return cData : - character read(<0x100) or
                   - (U16)-1        : if the SPI Data Register is written during a data transfer : colision status (say only on time).
                   - (U16)-2        : if a data is present on SPI Data Register and not read (say only on time).
                   - (U16)-3 to -15 : reserved for futur error.
                   .
 */
unsigned int spi_Transmitchar (unsigned char cData)
{
    if (!Spi_tx_ready())// test if a data is recieved and not read
       return (unsigned int)-2;
    Spi_send_byte(cData);
    while(!Spi_tx_ready());
    
    if (!Spi_get_colision_status()) 
        cData = Spi_get_byte();
    else 
        return (unsigned int)-1;
        
    return cData;
}


/**
   @brief This function reads a byte on the SPI
  
   @return cData : - character read(<0x100) or
                   - (U16)-1 if the SPI Data Register written during a data transfer.
                   - (U16)-3 can return any value : devise set in master andf no data present.
                   - (U16)-2 and 4 to -15 : reserved for futur error.
*/
unsigned int spi_getchar (void)
{

    register unsigned int c;
    
    if (Spi_get_colision_status()) 
        return (unsigned int)-1;
    if (Spi_is_master_mode() && (!Spi_rx_ready()) )
        return (unsigned int)-3;
        
    while(!Spi_rx_ready());
        c = Spi_get_byte();

    
    return c; 
}


