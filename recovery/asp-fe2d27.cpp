#include "asp.h"

void asp::datapath()
{
    if(result_rst){
        result = 0;
    }
    if(ave_window_size==0){
        L = 4;
    }else{
        L = 8;
    }
    if(store_cmd){
        R_CMD = data_in;
    }
    // TODO FIX THIS: result output
    /*//TODO MOVE TO ANI
    if(res_ready && ((opcode.read()==0)||(opcode.read()==1)){
        result_packet.set_bit(16,1); // bit 16 = RECEIVED bit
    }
    */
    if(res_ready && ((opcode.read()==0)||(opcode.read()==1)){
        store_received = 1;
    }else{
        store_received = 0;
    }
    if(res_ready){
        result_packet = result;
    }
}

void asp::alu()
{
    if(reset_AB){ // Reset
        A[counter_q.read()] = 0;
        B[counter_q.read()] = 0;
    } else if(reset_mem_sel){ // STORE 0000
        if(cmd_mem_sel.read() == 0){
            A[counter_q.read()] = 0;
        }else{
            B[counter_q.read()] = 0;
        }
    } else if(read_data_mem_sel){ // STORE 0001
        if(cmd_mem_sel.read() == 0){
            A[data_in.read().range(16,24).to_uint()] = data_in.read().range(0,15);
        }else{
            B[data_in.read().range(16,24).to_uint()] = data_in.read().range(0,15);
        }
    } else if(operation_ctrl.read() == op_xor){ // INVOKE_ASP 001X
        if(opcode.read().get_bit(0)==0){
            if(counter_q.read()==(n.read().to_uint() - m.read().to_uint() - 1)){
                result = A[m.read().to_uint() + counter_q.read().to_uint() + 1] ^ A[m.read().to_uint() + counter_q.read().to_uint()];
            }else{
                result = result ^ A[m.read().to_uint() + counter_q.read().to_uint()];
            }
        }else{
            if(counter_q.read()==(n.read().to_uint() - m.read().to_uint() - 1)){
                result = B[m.read().to_uint() + counter_q.read().to_uint() + 1] ^ B[m.read().to_uint() + counter_q.read().to_uint()];
            }else{
                result = result ^ B[m.read().to_uint() + counter_q.read().to_uint()];
            }
        }
    } else if(operation_ctrl.read() == op_mac){ // INVOKE_ASP 0100
        result_temp += A[counter_q.read()].to_uint() * B[counter_q.read()].to_uint();
        result = result_temp;
    } else if(operation_ctrl.read() == op_ave){ // INVOKE_ASP 0101, 0110
        uint32_t temp = 0;
        uint16_t compensator;
        if(counter_q.read()<L){
            compensator = L - 1 - counter_q.read();
        }else{
            compensator = 0;
        }
        if(opcode.read().get_bit(1)==0){
            for(int i=0; i<(L - compensator); i++){
                temp += A[511 - counter_q.read() + i].to_uint();
            }
            A[511 - counter_q.read()] = temp/L;
        } else{
            for(int i=0; i<(L - compensator); i++){
                temp += B[511 - counter_q.read() + i].to_uint();
            }
            B[511 - counter_q.read()] = temp/L;
        }
    }
}

void asp::reset_proc()
{
    if(reset) {
        next_state = s_start;
    }
}
void asp::next_state_logic()
{
    switch(curr_state.read())
    {
        case s_start:
            next_state = s_reset;
        break;

        case s_reset:
            if(counter_comp_zero){
                next_state = s_read_cmd;
            } else {
                next_state = curr_state;
            }
        break;

        case s_read_cmd:
            if(valid){
                next_state = s_dec_cmd;
            } else {
                next_state = curr_state;
            }
        break;

        case s_dec_cmd:
            if(opcode.read()==0){
                next_state = s_sel_rst;
            } else if(opcode.read()==1){
                next_state = s_data_str;
            } else if(opcode.read()==2 || opcode.read()==3 ){
                next_state = s_xor;
            } else if(opcode.read()==4){
                next_state = s_mac;
            } else if(opcode.read()==5 || opcode.read()==6){
                next_state = s_ave;
            } else {
                next_state = s_read_cmd;
            }
        break;

        case s_sel_rst:
            next_state = s_sel_rst2;
        break;

        case s_sel_rst2:
            if(counter_comp_zero){
                next_state = s_read_cmd;
            } else{
                next_state = curr_state;
            }
        break;

        case s_data_str:
            next_state = s_data_str2;
        break;

        case s_data_str2:
            if(counter_comp_zero){ // TODO check last bit bit
                next_state = s_read_cmd;
            } else if(valid){
                next_state = s_data_str3;
            } else{
                next_state = curr_state;
            }
        break;

        case s_data_str3:
            next_state = s_data_str2;
        break;

        case s_xor:
            next_state = s_xor2;
        break;

        case s_xor2:
            if(counter_comp_zero){
                next_state = s_read_cmd;
            } else{
                next_state = curr_state;
            }
        break;

        case s_mac:
            next_state = s_mac2;
        break;

        case s_mac2:
            if(counter_comp_zero){
                next_state = s_read_cmd;
            } else{
                next_state = curr_state;
            }
        break;

        case s_ave:
            next_state = s_ave2;
        break;

        case s_ave2:
            if(counter_comp_zero){
                next_state = s_read_cmd;
            } else{
                next_state = curr_state;
            }
        break;
    }
}
void asp::output_decode()
{
    while (true) {
        busy = 0;
        res_ready = 0;
        counter_reset = 0;
        counter_load = 0;
        counter_enable = 0;
        reset_AB = 0;
        reset_mem_sel = 0;
        read_data_mem_sel = 0;
        store_cmd = 0;
        result_rst = 0;
        operation_ctrl = op_noop;

        switch(curr_state.read())
        {
            case s_start:
                busy = 1;
                counter_reset = 1;
            break;

            case s_reset:
                busy = 1;
                reset_AB = 1;
            break;

            case s_read_cmd: // S2
                res_ready = 1;
                store_cmd = 1;
            break;

            case s_dec_cmd: // S3
                busy = 1;
            break;

            case s_sel_rst:
                busy = 1;
                counter_reset = 1;
            break;

            case s_sel_rst2:
                busy = 1;
                counter_enable = 1;
                reset_mem_sel = 1;
            break;

            case s_data_str: // S6
                busy = 1;
                counter_load = 1;
                counter_load_sel = ld_cmd;
            break;

            case s_data_str2: // S7
                busy = 0;
                counter_enable = 0;
                read_data_mem_sel = 1;
            break;

            case s_data_str3:
                busy = 1;
                counter_enable = 1;
                read_data_mem_sel = 0;
            break;

            case s_xor:
                busy = 1;
                counter_load = 1;
                counter_load_sel = ld_nm1; // counter_load (load(n-m-1))
            break;

            case s_xor2:
                busy = 1;
                counter_enable = 1;
                operation_ctrl = op_xor; // (m+counter + 1) xor (m + counter)
            break;

            case s_mac:
                busy = 1;
                counter_load = 1;
                counter_load_sel = ld_nm1; // counter_load (load(n-m-1))
                result_rst = 1; // reset result buffer

            break;

            case s_mac2:
                busy = 1;
                counter_enable = 1;
                operation_ctrl = op_mac; // MAC(counter_val)
            break;

            case s_ave:
                busy = 1;
                // vect_sel = OPCODE(1)
                counter_reset = 1;
            break;

            case s_ave2:
                busy = 1;
                counter_enable = 1;
                operation_ctrl = op_ave; // AVE(511 - counter_val)
            break;
        }
    }
};
