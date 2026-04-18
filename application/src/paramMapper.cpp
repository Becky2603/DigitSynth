#include "paramMapper.hpp"

uint8_t ParamMapper::getCC(int index, ControlMode current_mode){
    uint8_t cc_num = 0;
    switch(current_mode){
        case EQ:
            switch(index){
                case 0:
                    cc_num = 19;
                    break;
                case 1:
                    cc_num = 20;
                    break;
                case 2:
                    cc_num = 21;
                    break;
                case 3:
                    cc_num = 22;
                    break;
            }
            break;
        case SOURCE_EQ:
            switch(index){
                case 0:
                    cc_num = 23;
                    break;
                case 1:
                    cc_num = 24;
                    break;
                case 2:
                    cc_num = 25;
                    break;
                case 3:
                    cc_num = 26;
                    break;
            }
            break;
        case DETUNE:
            switch(index){
                case 0:
                    cc_num = 27;
                    break;
                case 1:
                    cc_num = 28;
                    break;
                case 2:
                    cc_num = 29;
                    break;
                case 3:
                    cc_num = 30;
                    break;
            }
            break;
	 case CHORD:
	    break;
    }
    return cc_num;
}
