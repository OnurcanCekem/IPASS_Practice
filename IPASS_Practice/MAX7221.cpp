#include "MAX7221.hpp"

void MAX7221::Setup(){
    uint8_t ShutdownData[8]  = {Shutdown, 0, Shutdown, 0, Shutdown, 0, Shutdown, 0};
    uint8_t Decode_ModeData[8] = {Decode_Mode, 0, Decode_Mode, 0, Decode_Mode, 0, Decode_Mode, 0};
    uint8_t Scan_LimitData[8] = {Scan_Limit, 7, Scan_Limit, 7, Scan_Limit, 7, Scan_Limit, 7};
    uint8_t BootupData[8] = {Shutdown, 1, Shutdown, 1, Shutdown, 1, Shutdown, 1};
    uint8_t IntensityData[8] = {Intensity, 5, Intensity, 5, Intensity, 5, Intensity, 5};
    SPI_Bus.write_and_read(Chip_Select, 8, ShutdownData, 0);
    SPI_Bus.write_and_read(Chip_Select, 8, Decode_ModeData, 0);
    SPI_Bus.write_and_read(Chip_Select, 8, Scan_LimitData, 0);
    SPI_Bus.write_and_read(Chip_Select, 8, BootupData, 0);
    SPI_Bus.write_and_read(Chip_Select, 8, IntensityData, 0);
}
// Setup for a single panel.
//void MAX7221::Setup(){
//    uint8_t ShutdownData[2]  = {Shutdown, 0};
//    uint8_t Decode_ModeData[2] = {Decode_Mode, 0};
//    uint8_t Scan_LimitData[2] = {Scan_Limit, 7};
//    uint8_t BootupData[2] = {Shutdown, 1};
//    SPI_Bus.write_and_read(Chip_Select, 2, ShutdownData, 0);
//    SPI_Bus.write_and_read(Chip_Select, 2, Decode_ModeData, 0);
//    SPI_Bus.write_and_read(Chip_Select, 2, Scan_LimitData, 0);
//    SPI_Bus.write_and_read(Chip_Select, 2, BootupData, 0);
//}

// Original Test function V1.0
//void MAX7221::Test( const uint8_t Data[]){
//    uint8_t Writable[8];
//    for( int i = 0; i < 8; i++ ){
//        Writable[0] = i+1;
//        Writable[1] = Data[i];
//        SPI_Bus.write_and_read(Chip_Select, 2, Writable, 0);
//    }
//}

// Test function V1.1, this function writes only to the 4th matrix.
//void MAX7221::Test( const uint8_t Data[]){
//    uint8_t Writable[Array_Size];
//    for(int i = 0; i < Array_Size; i++){
//        Writable[i] = 0;
//    }
//    for( int j = 0; j < 8; j++ ){
//        Writable[0] = j+1;
//        Writable[1] = Data[j];
//        
//        SPI_Bus.write_and_read(Chip_Select, Array_Size, Writable, 0);
//    }
//}

// This function sends data to every matrix.
//void MAX7221::Test( const uint8_t Data[][8] ){
//    uint8_t Writable[8][Array_Size];
//    
//    for( int i = 0; i < 8; i++ ){
//        for( int j = 0; j < Number_Of_Matrixes; j++ ){
//            Writable[i][j*2] = i+1;
//            Writable[i][j*2+1] = Data[j][i];
//        }
//        SPI_Bus.write_and_read(Chip_Select, Array_Size, Writable[i], 0);
//    }
//}

// This function writes data to all the Matrixes.
void MAX7221::Test( const uint8_t Data[][8] ){
    uint8_t Writable[8][Array_Size];
    
    for( int i = 0; i < 8; i++ ){
        for( int j = 0; j < Number_Of_Matrixes; j++ ){
            Writable[i][Array_Size - 2 - j * 2] = i+1;
            Writable[i][Array_Size - 1 - j * 2] = Data[j][i];
        }
        SPI_Bus.write_and_read(Chip_Select, Array_Size, Writable[i], 0);
    }
}


// This function writes data to a single matrix.
// Selected Matrix has to be 1 to select the first matrix.
void MAX7221::Test2( const uint8_t Data[], int Selected_Matrix){
    
    // Check to see if Selected_Matrix is within range, 1 up until Number_Of_Matrixes.
    if(!(Selected_Matrix > Number_Of_Matrixes || Selected_Matrix < 1)){
        uint8_t Writable[Array_Size];
        
        // Loop in the Array Writable filling it with 0's where we don't want any lights to turn on.
        for( int i = 0; i < Array_Size; i++ ){
            if( i != Array_Size - Selected_Matrix * 2 || Array_Size - Selected_Matrix * 2 + 1){
                Writable[i] = 0;
            }
        }
        
        // Loop in Columns
        for( int i = 0; i < 8; i++ ){
            Writable[Array_Size - Selected_Matrix * 2] = i+1;
            Writable[Array_Size - Selected_Matrix * 2 + 1] = Data[i];
            SPI_Bus.write_and_read(Chip_Select, Array_Size, Writable, 0);
        }
    }
    
    // If the Selected_Matrix is not within range, tell the user to correct this by giving feedback.
    else{
        hwlib::cout << "Selected Matrix is not within range, the minimum number is 1 and the maximum number is the number of matrixes." << "\n";
    }
}

void MAX7221::Translate( const uint32_t Map[] ){
    uint8_t TrMap[8][8];
    for( int i = 0; i < 8; i++ ){
        for( int j = 0; j < 4; j++ ){
            int index = 2*j;
            TrMap[i][index] = i+1;
            TrMap[i][index + 1] = (Map[i] >> (j*8));
        }
        SPI_Bus.write_and_read(Chip_Select, 8, TrMap[i], 0);
    }
}

void MAX7221::WriteTest( const uint8_t & x, const uint8_t & y ){
    if(x < 33 || x > 0){
        uint8_t Value = 0x01;
        uint8_t Shifted = (Value << ((x % 8) -1));
//        for( int i = 15; i >= 0; i-- ) {
//            hwlib::cout << ((Shifted >> i) & 1);
//        }
//        hwlib::cout << "\n";
        uint8_t Data[8] = {No_Op, No_Op, No_Op, No_Op, No_Op, No_Op, No_Op, No_Op};
        
        if(x > 24){
            Data[0] = {y};
            Data[1] = {Shifted};
        }
        else if(x > 16){
            Data[2] = {y};
            Data[3] = {Shifted};
        }
        else if(x > 8){
            Data[4] = {y};
            Data[5] = {Shifted};
        }
        else if(x > 0){
            Data[6] = {y};
            Data[7] = {Shifted};
        }
        SPI_Bus.write_and_read(Chip_Select, 8, Data, 0);
    }
    else{};
}



// This function puts the value 0 in every register, filling them with 0's.
void MAX7221::Clear(){
    uint8_t Data[8] = {};
    for( int i = 1; i < Number_Of_Matrixes * 8 + 1; i++ ){
        Data[0] = i;
        Data[1] = 0;
        SPI_Bus.write_and_read(Chip_Select, 2, Data, 0);
    }
}

// This function puts the value 255 in every register, filling them with 1's.
void MAX7221::Fill(){
    uint8_t Data[8] = {};
    for( int i = 1; i < Number_Of_Matrixes * 8 + 1; i++ ){
        Data[0] = i;
        Data[1] = 255;
        SPI_Bus.write_and_read(Chip_Select, 2, Data, 0);
    }
}

// Initial power, decode-mode, shutdown, scan-limit


// Initial power-up: Chip enters shutdown mode, Program the display driver prior to display use or it will scan one digit.

// Decode mode register: Selects which digits use code B decoding mode. Code B decoding let's you write the given characters with those given values.

// Shutdown: Blanks the display, Data in the digit and control registers remains unaltered, power saving mode.

// Scan-Limit: Sets how many digits are displayed, from 1 to 8. The value of Rset resistor must be adjusted according to the number of digits displayed (Table 9). 


// Make text font, let it shift through the LED Module.
// Then perhabs make a black-and-white image reader, white = lights on, black = lights off.
// Make a dummy, re-do everything.
// Shutdown, decoder, scan limit, shutdown.