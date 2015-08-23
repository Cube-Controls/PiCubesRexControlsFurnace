/************************************************************
*
* REXLANG - Using the PiCube UIs via I2C
*
*************************************************************/

#define I2CDEV_FNAME 67 // I2C device is defined by the fname parameter of the REXLANG block (e.g. set it to /dev/i2c-1 on the Raspberry Pi minicomputer)

long input(0) module_address;  // Address of Module
long input(1) input_number;    // Universal Input Number
long input(2) input_type;      // Universal Input Type

long output(0)  input_value;   // Universal Input Value 

long i2c_bufTx[5]; //buffer for transmitting data
long i2c_bufRx[5]; //buffer for receiving data
long i2c_bus_handle;
long i2c_chip_address;
long i2c_register_address;
long i2c_write_count;
long i2c_read_count;
long i2c_ret_fun;

long init(void)
{
    i2c_bus_handle = Open(I2CDEV_FNAME); // open I2C device
    i2c_chip_address = 0x1C; // 7-bit address of the I2C device

    return 0;
}

long getSignedNumber(long number,long bitlength)
{
  long mask;
  
  mask  = (long)pow(2,bitlength) - 1;
  
  if (number & (1 << (bitlength -1)))
  {
     return(number | (~mask));
  } else
  {
     return(number & mask);
  }	 
	 
}

long main(void)
{
    // Sending I2C
    i2c_bufTx[0] = (module_address-1)*8+(input_number-1)*2;
    i2c_bufTx[1] = input_type;
    i2c_write_count = 2;
    i2c_read_count = 0;
    i2c_ret_fun = I2C(i2c_bus_handle, i2c_chip_address, i2c_bufTx, i2c_write_count, i2c_bufRx, i2c_read_count);

	// Sending and Reading I2C
    i2c_bufTx[0] = 48+(module_address-1)*16+(input_number-1)*4;
    i2c_write_count = 1;
    i2c_read_count = 4;
    i2c_ret_fun = I2C(i2c_bus_handle, i2c_chip_address, i2c_bufTx, i2c_write_count, i2c_bufRx, i2c_read_count);
	
	if (input_type == 4)
	{
	   input_value = (long)(i2c_bufRx[0] | i2c_bufRx[1]<<8 | i2c_bufRx[2]<<16 | i2c_bufRx[3]<<24);
	} else
    {
	   input_value = getSignedNumber((i2c_bufRx[0] | i2c_bufRx[1]<<8 | i2c_bufRx[2]<<16 | i2c_bufRx[3]<<24),32);
    } 	
	
    return 0;
}