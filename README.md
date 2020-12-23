# VL6180X driver

## Datasheet/Appnote
https://www.st.com/resource/en/datasheet/vl6180x.pdf
https://www.st.com/resource/en/datasheet/vl6180.pdf
https://www.st.com/resource/en/application_note/dm00122600-vl6180x-basic-ranging-application-note-stmicroelectronics.pdf

## VL6180 protocol I2C (2 byte address)$

### Write$
|m|m addr(0x29) |m|s| master      |s| master     |s| master    |s|m|$
|S|0|1|0|1|0|0|1|W|A| index(15:8) |A| index(7:0) |A| data(7:0) |A|P|$

i2c_smbus_write_word_data(command=index(15:8), dlow=index(7:0), dhigh=data)$

### Read$
|m|m addr(0x29) |m|s| master      |s| master     |s|m|$
|S|0|1|0|1|0|0|1|R|A| index(15:8) |A| index(7:0) |A|P|$

i2c_smbus_write_byte_data(command=index(15:8), data=index(7:0))$

|m|m addr(0x29) |m|s| slave     |m|m|$
|S|0|1|0|1|0|0|1|R|A| data(7:0) |A|P|$

i2c_smbus_read_byte()$

