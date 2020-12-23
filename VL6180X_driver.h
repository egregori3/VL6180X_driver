#ifndef VL6180_REGISTERS
#define VL6180_REGISTERS
// https://www.st.com/resource/en/datasheet/vl6180x.pdf
// https://www.st.com/resource/en/application_note/dm00122600-vl6180x-basic-ranging-application-note-stmicroelectronics.pdf

// The VL6180 is a short range distance measuring sensor.
// The sensor connects to the host using I2C.


#include "UserSpaceI2C_driver.h"

#if 1
#define DEBUGP(x) printf(x);
#else
#define DEBUGP(x) 
#endif

#define WriteByte(addr, data)   i2c->WriteWordData(addr>>8, addr&0x00FF, data)

class VL6180driver
{
    private:
        UserSpaceI2Cdriver *i2c;

    public:
        VL6180driver(UserSpaceI2Cdriver *pi2c)
        {
            i2c = pi2c;
            // This init sequence is from the datasheet
            WriteByte(0x0207, 0x01);
            WriteByte(0x0208, 0x01);
            WriteByte(0x0096, 0x00);
            WriteByte(0x0097, 0xfd);
            WriteByte(0x00e3, 0x01);
            WriteByte(0x00e4, 0x03);
            WriteByte(0x00e5, 0x02);
            WriteByte(0x00e6, 0x01);
            WriteByte(0x00e7, 0x03);
            WriteByte(0x00f5, 0x02);
            WriteByte(0x00d9, 0x05);
            WriteByte(0x00db, 0xce);
            WriteByte(0x00dc, 0x03);
            WriteByte(0x00dd, 0xf8);
            WriteByte(0x009f, 0x00);
            WriteByte(0x00a3, 0x3c);
            WriteByte(0x00b7, 0x00);
            WriteByte(0x00bb, 0x3c);
            WriteByte(0x00b2, 0x09);
            WriteByte(0x00ca, 0x09);
            WriteByte(0x0198, 0x01);
            WriteByte(0x01b0, 0x17);
            WriteByte(0x01ad, 0x00);
            WriteByte(0x00ff, 0x05);
            WriteByte(0x0100, 0x05);
            WriteByte(0x0199, 0x05);
            WriteByte(0x01a6, 0x1b);
            WriteByte(0x01ac, 0x3e);
            WriteByte(0x01a7, 0x1f);
            WriteByte(0x0030, 0x00);

            // Recommended : Public registers - See data sheet for more detail$
            WriteByte(0x0011, 0x10); // Enables polling for ‘New Sample ready’$
                                     // when measurement completes$
            WriteByte(0x010a, 0x30); // Set the averaging sample period$
                                     // (compromise between lower noise and$
                                     // increased execution time)$
            WriteByte(0x003f, 0x46); // Sets the light and dark gain (upper$
                                     // nibble). Dark gain should not be$
                                     // changed.$
            WriteByte(0x0031, 0xFF); // sets the # of range measurements after$
                                     // which auto calibration of system is$
                                     // performed$
            WriteByte(0x0041, 0x63); // Set ALS integration time to 100ms$
 
            WriteByte(0x002e, 0x01); // perform a single temperature calibration$
                                     // of the ranging sensor$
 
            WriteByte(0x016, 0x00);  //change fresh out of set status to 0$

        }

        uint8_t GetVersion()
        {
            // Read VL6180 ID register (0x00, 0x00)
            // |m|m addr(0x29) |m|s| master      |s| master     |s|m|
            // |S|0|1|0|1|0|0|1|R|A| index(15:8) |A| index(7:0) |A|P|
            i2c->WriteByteData(0x00, 0x00);
 
            // |m|m addr(0x29) |m|s| slave     |m|m|$
            // |S|0|1|0|1|0|0|1|R|A| data(7:0) |A|P|$
            return(i2c->ReadByte());
        }

        int GetRange(void)
        {
            int     range = -1;

            // start single range measurement
            usleep(50000);
            VL6180_Start_Range();
            usleep(50000);

            // poll the VL6180 till new sample ready
            //VL6180_Poll_Range();

            // read range result
            range = VL6180_Read_Range();

            // clear the interrupt on VL6180
            VL6180_Clear_Interrupts();

            return range;
        }

    private:
        uint8_t ReadByte(uint16_t addr)
        {
            i2c->WriteByteData(addr>>8, addr&0x00ff);
            return(i2c->ReadByte());
        }
        // These functions are out of the ST appnote

        ///////////////////////////////////////////////////////////////////
        // Start a range measurement in single shot mode
        ///////////////////////////////////////////////////////////////////
        int VL6180_Start_Range() 
        {
            DEBUGP("START\n");
            WriteByte(0x018,0x01);
            return 0;
        }

        ///////////////////////////////////////////////////////////////////
        // poll for new sample ready ready
        ///////////////////////////////////////////////////////////////////
        int VL6180_Poll_Range() 
        {
            uint8_t status;
            uint8_t range_status;

            DEBUGP("POLL\n");
            // check the status
            status = ReadByte(0x04f);
            range_status = status & 0x07;

            // wait for new measurement ready status
            while (range_status != 0x04) 
            {
                status = ReadByte(0x04f);
                printf("Status: %02x", status);
                range_status = status & 0x07;
            }
            return 0;
        }

        ///////////////////////////////////////////////////////////////////
        // Read range result (mm)
        ///////////////////////////////////////////////////////////////////
        int VL6180_Read_Range() 
        {
            int range;
            DEBUGP("READ\n");
            range = ReadByte(0x062);
            return range;
        }

        ///////////////////////////////////////////////////////////////////
        // clear interrupts
        ///////////////////////////////////////////////////////////////////
        int VL6180_Clear_Interrupts() 
        {
            DEBUGP("CLEAR\n");
            WriteByte(0x015,0x07);
            return 0;
        }

};


#endif
