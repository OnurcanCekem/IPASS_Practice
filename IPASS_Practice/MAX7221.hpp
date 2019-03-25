#ifndef MAX7221_HPP
#define MAX7221_HPP

#include "hwlib.hpp"

class MAX7221 {
private:
    hwlib::pin_out & Chip_Select;
    hwlib::spi_bus_bit_banged_sclk_mosi_miso & SPI_Bus;
    int Number_Of_Matrixes;
    int Array_Size = Number_Of_Matrixes*2;
    const uint8_t No_Op = 0x00;
    const uint8_t Digit0 = 0x01;
    const uint8_t Digit1 = 0x02;
    const uint8_t Digit2 = 0x03;
    const uint8_t Digit3 = 0x04;
    const uint8_t Digit4 = 0x05;
    const uint8_t Digit5 = 0x06;
    const uint8_t Digit6 = 0x07;
    const uint8_t Digit7 = 0x08;
    const uint8_t Decode_Mode = 0x09;
    const uint8_t Intensity = 0x0A;
    const uint8_t Scan_Limit = 0x0B;
    const uint8_t Shutdown = 0x0C;
    const uint8_t Display_Test = 0x0F;
    
public:
    MAX7221(hwlib::pin_out & Chip_Select, hwlib::spi_bus_bit_banged_sclk_mosi_miso & SPI_Bus, int Number_Of_Matrixes = 1):
    Chip_Select(Chip_Select),
    SPI_Bus(SPI_Bus),
    Number_Of_Matrixes(Number_Of_Matrixes)
    {}
    
//    int GetNumber_Of_Matrixes() const{
//        return Number_Of_Matrixes;
//    }
    
    void Setup();
    void Test( const uint8_t Data[][8] );
    void Test2( const uint8_t Data[], int Selected_Matrix );
    void Translate( const uint32_t Map[] );
    void WriteTest( const uint8_t & x, const uint8_t & y );
    //void WriteTest(  uint16_t Coordinates[] );
    void Clear();
    void Fill();
};


#endif // MAX7221_HPP