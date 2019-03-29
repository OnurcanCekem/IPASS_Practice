#ifndef MAX7221_HPP
#define MAX7221_HPP

#include "hwlib.hpp"

/// @file

/// \brief
/// MAX7221 library for writing to the chip using SPI.
/// \details
/// The MAX7221 library uses the SPI communication protocol to write data to the MAX7221 LED Module.
/// The MAX7221 does use the SPI communication protocol, but does not contain a master in slave out (MISO).
/// This library contains functions to send data to the corresponding registers. You can also convert char strings to uint8_t data.
/// All registers listed are private variables and can be found in the MAX7219 datasheet - table 2.
class MAX7221 {
private:

    hwlib::pin_out & Chip_Select; // Chip Select.
    hwlib::spi_bus_bit_banged_sclk_mosi_miso & SPI_Bus; // SPI_Bus
    
    const int Number_Of_Matrixes; // This number identicates the Number_Of_Matrixes, which allows the functions to work properly.
    const int Array_Size = Number_Of_Matrixes*2; // Array_Size is used for for the majority of functions to declare array sizes, this allows the user to only give the library 8-bit data instead of 16-bits.
    
    
    // Registers for the MAX7221
    
    // No_Op, to ignore specific matrixes.
    const uint8_t No_Op = 0x00;
    
    // Digit0-Digit7 are the registers to put data in to turn on lights.
    const uint8_t Digit0 = 0x01;
    const uint8_t Digit1 = 0x02;
    const uint8_t Digit2 = 0x03;
    const uint8_t Digit3 = 0x04;
    const uint8_t Digit4 = 0x05;
    const uint8_t Digit5 = 0x06;
    const uint8_t Digit6 = 0x07;
    const uint8_t Digit7 = 0x08;
    
    // Decode_Mode, to select if you want 7-Segment display or not.
    const uint8_t Decode_Mode = 0x09;
    
    // Intensity, to select how bright the matrixes are.
    const uint8_t Intensity = 0x0A;
    
    // Scan limit, to select how many Digit's you want to read from (set to max, all 8).
    const uint8_t Scan_Limit = 0x0B;
    
    // Shutdown, to turn the register on and off.
    const uint8_t Shutdown = 0x0C;
    
    // Display_Test, to test the MAX7221 by turning on all the lights.
    const uint8_t Display_Test = 0x0F;
    
    
    // Font map, to store the presets of characters and numbers (not a register).
    const uint8_t Font_Map[39][8] = 
    {
    {102, 102, 126, 126, 102, 102, 126, 60},   // 0:  A
    {62, 102, 102, 62, 62, 102, 102, 62},      // 1:  B
    {124, 126, 6, 6, 6, 6, 126, 124},          // 2:  C
    {62, 126, 102, 102, 102, 102, 126, 62},    // 3:  D
    {126, 126, 6, 126, 126, 6, 126, 126},      // 4:  E
    {6, 6, 6, 30, 30, 6, 126, 126},            // 5:  F
    {60, 126, 102, 118, 6, 6, 124, 120},       // 6:  G
    {102, 102, 102, 126, 126, 102, 102, 102},  // 7:  H
    {126, 126, 24, 24, 24, 24, 126, 126},      // 8:  I
    {12, 26, 26, 24, 24, 24, 126, 126},        // 9:  J
    {102, 118, 62, 30, 30, 62, 118, 102},      // 10: K
    {126, 126, 6, 6, 6, 6, 6, 6},              // 11: L
    {99, 99, 99, 99, 107, 127, 119, 99},       // 12: M
    {102, 102, 102, 118, 126, 126, 110, 102},  // 13: N
    {60, 126, 102, 102, 102, 102, 126, 60},    // 14: O
    {6, 6, 62, 126, 102, 102, 126, 62},        // 15: P
    {92, 62, 50, 42, 34, 34, 62, 28},          // 16: Q
    {102, 54, 30, 30, 54, 102, 126, 126},      // 17: R
    {126, 126, 96, 126, 126, 6, 126, 126},     // 18: S
    {24, 24, 24, 24, 24, 24, 126, 126},        // 19: T
    {60, 126, 102, 102, 102, 102, 102, 102},   // 20: U
    {24, 60, 126, 126, 102, 102, 66, 66},      // 21: V
    {62, 127, 107, 107, 107, 107, 99, 99},     // 22: W
    {102, 126, 126, 60, 24, 60, 126, 102},     // 23: X
    {24, 24, 24, 24, 60, 126, 102, 66},        // 24: Y
    {126, 126, 12, 24, 48, 96, 126, 126},      // 25: Z
    {24, 24, 0, 0, 0, 0, 0, 0},                // 26: .
    {8, 16, 24, 24, 0, 0, 0, 0},               // 27: ,
    {0, 0, 0, 0, 0, 0, 0, 0},                  // 28:  
    {126, 126, 102, 110, 118, 102, 126, 126},  // 29: 0
    {126, 126, 24, 24, 24, 30, 28, 24},        // 30: 1
    {126, 126, 12, 24, 48, 98, 126, 60},       // 31: 2
    {60, 126, 98, 120, 120, 98, 126, 60},      // 32: 3
    {96, 96, 96, 126, 126, 102, 102, 102},     // 33: 4
    {126, 126, 112, 126, 126, 6, 126, 126},    // 34: 5
    {126, 126, 102, 126, 126, 6, 126, 126},    // 35: 6
    {96, 96, 96, 96, 96, 96, 126, 126},        // 36: 7
    {126, 126, 102, 126, 126, 102, 126, 126},  // 37: 8
    {126, 126, 96, 126, 126, 102, 126, 126}    // 38: 9
    };
public:
    // Constructor
    
    /// \brief
    /// Constructor
    /// \details
    /// Referenced Chip Select, referenced SPI Bus and a number to identicate how many matrixes the programmer is using with default which is set to 1.

    MAX7221(hwlib::pin_out & Chip_Select, hwlib::spi_bus_bit_banged_sclk_mosi_miso & SPI_Bus, const int Number_Of_Matrixes = 1):
    Chip_Select(Chip_Select),
    SPI_Bus(SPI_Bus),
    Number_Of_Matrixes(Number_Of_Matrixes)
    {}
    
    
    // MAX7221 functions.

    /// \brief
    /// Puts the MAX7221 chip ready for writing.
    /// \details
    /// This function writes to the registers: shutdown, decode mode, scan limit, shutdown again and intensity. This allows the chip to function properly
    /// and put it ready for receiving data.
    void Setup();
    
    /// \brief
    /// Sets the intensity for the MAX7221 chip.
    /// \details
    /// This function requires a value, this value is used to set the intensity of all the matrixes by writing to the intensity register with given value.
    void SetIntensity( const uint8_t IntensityValue );
    
    /// \brief
    /// Convert char strings to uint8_t.
    /// \details
    /// This function requires a char string and a place to store the data in. This function looks at the char string and depending on the character it will point
    /// an index in the Font_Map. This index contains the corresponding data to turn on the correct lights to show the chaacter.
    void Translate( const char Text[], uint8_t Converted[][8] );
    
    /// \brief
    /// Shifts the given data to the left.
    /// \details
    /// This function requires the data that you want to shift to the left and the size of the first dimension of the data. This function Shifts all the data one step
    /// to the left and hands it over to the next matrix if another matrix is connected to it's left.
    void ShiftLeft( uint8_t Data[][8], const int Size );
    
    /// \brief
    /// Shifts the given data to the left, but in a circular array.
    /// \details
    /// This function requires the data that you want to shift to the left and the size of the first dimension of the data. This function Shifts all the data one step
    /// to the left and hands it over to the next matrix if another matrix is connected to it's left. The data on the furthest left is put on the data furthest right to make it a
    /// circular array.
    void ShiftLeftCircular( uint8_t Data[][8], const int Size);
    
    /// \brief
    /// Shifts the given data to the right.
    /// \details
    /// This function requires the data that you want to shift to the right and the size of the first dimension of the data. This function shifts all the data one step
    /// to the right and hands it over to the next matrix if another matrix is connected to it's right.
    void ShiftRight( uint8_t Data[][8], const int Size );
    
    /// \brief
    /// Shifts the given data to the right, but in a circular array.
    /// \details
    /// This function requires the data that you want to shift to the right and the size of the first dimension of the data. This function shifts all the data one step
    /// to the right and hands it over to the next matrix if another matrix is connected to it's right. The data on the furthest right is put on the data furthest left to make it a
    /// circular array.
    void ShiftRightCircular( uint8_t Data[][8], const int Size);
    
    
    /// \brief
    /// Turns on lights based on given data.
    /// \details
    /// This function requires data for your entire LED Matrix and this function turns the lights on based on the given data. 
    void WriteAll( const uint8_t Data[][8] );
    
    /// \brief
    /// Turns on lights based on given data for one selected matrix.
    /// \details
    /// This function is similar to the WriteAll function except the only difference is that you can select one single matrix to write to.
    /// This function requires data for a single matrix and a number to select which matrix to select from (the range is 1-Number_Of_Matrixes).
    void WriteSingle( const uint8_t Data[], const int Selected_Matrix ); 

    /// \brief
    /// Clears the MAX7221 chip.
    /// \details
    /// This function fills the MAX7221 chip with 0's, turning the whole thing off. After this function you have to use the Setup function again
    /// in order to write data. It is also recommended to clear before every write.
    void Clear();
    
    /// \brief
    /// Fills the MAX7221 chip.
    /// \details
    /// This function fills the MAX7221 chip with 1's, turning all the lights on. This function is mainly for testing purposes to check if all the pins
    /// are connected properly.
    void Fill();

};


#endif // MAX7221_HPP