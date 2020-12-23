#ifndef VL6180_REGISTERS
#define VL6180_REGISTERS

#include "UserSpaceI2C_driver.h"


#define WriteByte(addr, data)   i2c->WriteWordData(addr>>8, addr&0x00FF, data)


// https://www.st.com/resource/en/datasheet/vl6180x.pdf
// https://www.st.com/resource/en/application_note/dm00122600-vl6180x-basic-ranging-application-note-stmicroelectronics.pdf

// The VL6180 is a short range distance measuring sensor.
// The sensor connects to the host using I2C.

#define IDENTIFICATION__MODEL_ID        0xB4    // 8 bits
#define RESULT__RANGE_STATUS            0x4d    // 8 bits
#define RESULT__RANGE_VAL               0x62    // 8 bits
#define RESULT__INTERRUPT_STATUS_GPIO   0x4f    // 8 bits
#define SYSTEM__INTERRUPT_CLEAR         0x15    // 8 bits
#define SYSRANGE__START                 0x18    // 8 bits

// Init Sequence
// Check bit 0 of RESULT__RANGE_STATUS {0x4d} is set.
// Write 0x01 to SYSRANGE__START {0x18}.
// Poll RESULT__INTERRUPT_STATUS_GPIO {0x4f} register till bit 2 is set to 1. 
// Read RESULT__RANGE_VAL {0x62}.
// Write 0x07 to SYSTEM__INTERRUPT_CLEAR {0x15}.

class VL6180driver
{
    private:
        UserSpaceI2Cdriver *i2c;
        int status;
        uint8_t range;

    public:
        VL6180driver(UserSpaceI2Cdriver *i2c)
        {
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
        int GetStatus() { return status; }

        uint8_t GetRange(void)
        {
            uint8_t check = 0;
            while( (check&0x01) == 0 )
            {
                i2c->WriteByteData(0x00, RESULT__RANGE_STATUS);
                check = i2c->ReadByte();
            }

            if( status = WriteByte(SYSRANGE__START, 0x01))
            {
                return status;
            }

            for(check=0; ((check&0x04) == 0); )
            {
                i2c->WriteByteData(0x00, RESULT__INTERRUPT_STATUS_GPIO);
                check = i2c->ReadByte();
            }

            i2c->WriteByteData(0x00, RESULT__RANGE_VAL);
            range = i2c->ReadByte();

            if( status = WriteByte(SYSTEM__INTERRUPT_CLEAR, 0x07))
            {
                return status;
            }
            return range;
        }
};


#endif
