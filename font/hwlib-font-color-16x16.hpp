#include <cstdlib> 
#include "hwlib-graphics.hpp"

#ifndef HWLIB_FONT_COLOR_16X16_H
#define HWLIB_FONT_COLOR_16X16_H

namespace hwlib {  
    /**
     * @brief 16x16 font with a background and foreground color
     * 
     */
    class color_image_16x16 : public image {
        private:   
            uint8_t *data;

            hwlib::color foreground;
            hwlib::color background;

            color get_implementation( location pos ) const override {
                auto x = 16 - pos.x;
                return
                    (( 
                        data[ ( 2 * pos.y ) + 1 - ( x / 8 ) ]
                        & ( 0x01 << ( x % 8 ))
                    ) == 0 )   
                        ? background
                        : foreground;
            }         
            
        public:   
            /// \brief
            /// the color image class for the 16x16 font                
            color_image_16x16(): 
                image( location( 16, 16 )),
                data( nullptr ) ,
                foreground(hwlib::white),
                background(hwlib::black)
            {}

            /// \brief
            /// the color image class for the 16x16 font         
            color_image_16x16( uint8_t * data, hwlib::color f = hwlib::white, hwlib::color b = hwlib::black ):
                image( location( 16, 16 )),
                data( data ),
                foreground(f),
                background(b)
            {}
    };

    /**
     * @brief Font class with color
     * 
     */
    class font_color_16x16 : public font {
        private:   
            color_image_16x16 images[ 128 ];

        public:   
            /// \brief
            /// the 16x16 font with color    
            font_color_16x16( hwlib::color f = hwlib::white, hwlib::color b = hwlib::black ){
                for( uint_fast8_t i = 0; i < 128; ++i ){
                    uint_fast8_t c = 
                        ( i < 32 ) || ( i == 127 )
                        ? 0
                        : i - 32;
                    images[ i ] = color_image_16x16( font_16x16_data + 4 + ( c * 32 ), f, b);
                }         
            }

            /// \brief
            /// the [] operator for the font
            const image & operator[]( char c ) const override {
                return images[ c & 0x7F ];      
            }      
    };
};
#endif