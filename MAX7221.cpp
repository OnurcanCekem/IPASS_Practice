#include "MAX7221.hpp"



// This function sets the MAX7221 ready for data transfer.
void MAX7221::Setup(){
    
    // Create the arrays.
    uint8_t ShutdownData[Array_Size];
    uint8_t Decode_ModeData[Array_Size];
    uint8_t Scan_LimitData[Array_Size];
    uint8_t BootupData[Array_Size];
    uint8_t IntensityData[Array_Size];
    
    for( int i = 0; i < Number_Of_Matrixes; i++){
        
        // Put the correct values in the arrays. First 8-bits is the register, second 8-bits is the value.
        ShutdownData[i*2] = Shutdown;
        ShutdownData[i*2+1] = 0;
        
        Decode_ModeData[i*2] = Decode_Mode;
        Decode_ModeData[i*2+1] = 0;
        
        Scan_LimitData[i*2] = Scan_Limit;
        Scan_LimitData[i*2+1] = 7;
        
        BootupData[i*2] = Shutdown;
        BootupData[i*2+1] = 1;
        
        IntensityData[i*2] = Intensity;
        IntensityData[i*2+1] = 5;
        
    }
    
    // Write to the registers.
    SPI_Bus.write_and_read(Chip_Select, Array_Size, ShutdownData, 0);
    SPI_Bus.write_and_read(Chip_Select, Array_Size, Decode_ModeData, 0);
    SPI_Bus.write_and_read(Chip_Select, Array_Size, Scan_LimitData, 0);
    SPI_Bus.write_and_read(Chip_Select, Array_Size, BootupData, 0);
    SPI_Bus.write_and_read(Chip_Select, Array_Size, IntensityData, 0);
    
}

// This function sets the intensity of every matrix.
void MAX7221::SetIntensity( const uint8_t IntensityValue ){
    
    // Create the array.
    uint8_t IntensityData[Array_Size];
    
    for( int i = 0; i < Number_Of_Matrixes; i++ ){
        
        // Put the correct values inside the array.
        IntensityData[i*2] = Intensity;
        IntensityData[i*2+1] = IntensityValue;
    
    }
    
    // Write the values.
    SPI_Bus.write_and_read(Chip_Select, Array_Size, IntensityData, 0);

}

// This function converts char strings to uint8_t data.
void MAX7221::Translate( const char Text[], uint8_t Storage[][8] ){
    
    // Create ASCII variables to correctly point the index of the Font_Map.
    int ASCII;
    int ASCII_Offset = 0;
    
    for( int i = 0;; i++ ){
        
        // To check if the string is empty.
        if( Text[i] == 0 ){
            break;
        }
        
        // Give the ASCII variables values.
        ASCII = Text[i];
        ASCII_Offset = ASCII;
        
        // Upper case alphabet.
        if( ASCII >= 65 && ASCII <= 90 ){
            ASCII_Offset -= 65; // Location 0-25
        }
        
        // Lower case alphabet
        else if( ASCII >= 97 && ASCII <= 122 ){
            ASCII_Offset -= 97; // Location 0-25
        }
        
        // Numbers.
        else if( ASCII >= 48 && ASCII <= 57 ){
            ASCII_Offset -= 19; // Location 29-38
            
        }
        
        // "."
        else if( ASCII == 46 ){
            ASCII_Offset -= 20; // Location 26
            
        }
        
        // ","
        else if( ASCII == 44){
            ASCII_Offset -= 17; // Location 27
        }
        
        // " "
        else if( ASCII == 32){
            ASCII_Offset -= 4; // Location 28
        }
        
        // Put the data of the character in the Storage array.
        for( int j = 0; j < 8; j++){
            Storage[i][j] = Font_Map[ASCII_Offset][j];
        }
    }
}

// This function shifts data to the left.
void MAX7221::ShiftLeft( uint8_t Data[][8], const int Size){
        
    for( int i = 0; i < Size; i++ ){
        for( int j = 0; j < 8; j++ ){
            
            // To check if we're in the furthest left matrix.
            if( i != 0 ){
                
                // Shift the carry data over to the next matrix.
                Data[i-1][j] |= (Data[i][j] << 7);
                
            }
            
            // Shift the data within the matrix.
            Data[i][j] = Data[i][j] >> 1;
        
        }
    }
}

// This function shifts data to the left, but also keeps the shifted data.
void MAX7221::ShiftLeftCircular( uint8_t Data[][8], const int Size){
    
    for( int i = 0; i < Size; i++ ){
        for( int j = 0; j < 8; j++ ){
            
            // To check if we're in the furthest left matrix.
            if( i != 0 ){
                
                // Shift the carry data over to the next matrix.
                Data[i-1][j] |= (Data[i][j] << 7);
                
            }
            else{
                
                // Shift the carry data over back to the furthest Data index to the right.
                Data[Size-1][j] |= (Data[i][j] << 7);
            

            }
            // Shift the data within the matrix.
            Data[i][j] = Data[i][j] >> 1;
        
        }
    }
}

// This function shifts data to the right.
void MAX7221::ShiftRight( uint8_t Data[][8], const int Size ){

    for(int i = Size-1; i >= 0; i-- ){
        for( int j = 0; j < 8; j++ ){
            
            // To check if we're in the furthest right matrix.
            if(i != Size-1){

                // Shift the carry data over to the next matrix.
                Data[i+1][j] |= (Data[i][j] >> 7);
            
            }
            
            // Shift the data within the matrix.
            Data[i][j] = Data[i][j] << 1;
        
        }
    }
}

// This function shifts data to the right, but also keeps the shifted data.
void MAX7221::ShiftRightCircular( uint8_t Data[][8], const int Size ){

    for(int i = Size-1; i >= 0; i-- ){
        for( int j = 0; j < 8; j++ ){
            
            // To check if we're in the furthest right matrix.
            if(i != Size-1){

                // Shift the carry data over to the next matrix.
                Data[i+1][j] |= (Data[i][j] >> 7);
            
            }
            else{
                
                // Shift the carry data over back to the furthest Data index to the left.
                Data[0][j] |= (Data[i][j] >> 7);
            
            }
            
            // Shift the data within the matrix.
            Data[i][j] = Data[i][j] << 1;
        
        }
    }
}


// This function writes data to all the Matrixes.
void MAX7221::WriteAll( const uint8_t Data[][8] ){
    
    // Create the array.
    uint8_t Writable[8][Array_Size];
    
    for( int i = 0; i < 8; i++ ){
        for( int j = 0; j < Number_Of_Matrixes; j++ ){
            
            // Put the data from Data to the correct index in Writable 
            Writable[i][Array_Size - 2 - j * 2] = i+1;
            Writable[i][Array_Size - 1 - j * 2] = Data[j][i];
        
        }
        
        // Write Writable.
        SPI_Bus.write_and_read(Chip_Select, Array_Size, Writable[i], 0);
    
    }
}


// This function writes data to a single matrix.
// Selected Matrix has to be 1 to select the first matrix.
void MAX7221::WriteSingle( const uint8_t Data[], const int Selected_Matrix){
    
    // Check to see if Selected_Matrix is within range, 1 up until Number_Of_Matrixes.
    if(!(Selected_Matrix > Number_Of_Matrixes || Selected_Matrix < 1)){
        
        uint8_t Writable[Array_Size];
        
        // Loop in the Array Writable filling it with 0's where we don't want any lights to turn on.
        for( int i = 0; i < Array_Size; i++ ){
            
            // To check if the Selected_Matrix is within range of Number_Of_Matrix.
            if( i != Array_Size - Selected_Matrix * 2 || Array_Size - Selected_Matrix * 2 + 1){
                Writable[i] = 0;
            
            }
        }
        
        // Loop in Columns
        for( int i = 0; i < 8; i++ ){
            
            // Put the data from Data in the correct indexes in Writable.
            Writable[Array_Size - Selected_Matrix * 2] = i+1;
            Writable[Array_Size - Selected_Matrix * 2 + 1] = Data[i];
            
            // Write Writable.
            SPI_Bus.write_and_read(Chip_Select, Array_Size, Writable, 0);
        
        }
    }
}

// This function puts the value 0 in every register, filling them with 0's.
void MAX7221::Clear(){
    
    // Create Data array.
    uint8_t Data[8];
    
    for( int i = 1; i < (Number_Of_Matrixes * 8 + 1); i++ ){
        
        // Put the correct values in Data.
        Data[0] = i;
        Data[1] = 0;
        
        // Write Data.
        SPI_Bus.write_and_read(Chip_Select, 2, Data, 0);
    
    }
}

// This function puts the value 255 in every register, filling them with 1's.
void MAX7221::Fill(){
    
    // Create Data array.
    uint8_t Data[8];
    
    for( int i = 1; i < (Number_Of_Matrixes * 8 + 1); i++ ){
        
        // Put the correct values in Data.
        Data[0] = i;
        Data[1] = 255;
        
        // Write Data.
        SPI_Bus.write_and_read(Chip_Select, 2, Data, 0);
    
    }
}

