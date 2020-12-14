#include "keyboard.h"
#include "misc.h"
#include "isr.h"

//scan code set 2
//source: https://wiki.osdev.org/PS/2_Keyboard#Scan_Code_Set_2
uint8_t kbd_layout[128] =
{
    0,              0/*F9*/,        0,              0/*F5*/,
    0/*F3*/,        0/*F1*/,        0/*F2*/,        0/*F12*/,
    0,              0/*F10*/,       0/*F8*/,        0/*F6*/,
    0/*F4*/,        0/*tab*/,       '`',            0,
    0,              0/*ltf alt*/,   0/*lft shft*/,  0,
    0/*lft ctrl*/,  'q',            '1',            0,
    0,              0,              'z',            's',
    'a',            'w',            '2',            0,
    0,              'c',            'x',            'd',
    'e',            '4',            '3',            0,
    0,              ' ',            'v',            'f',
    't',            'r',            '5',            0,
    0,              'n',            'b',            'h',
    'g',            'y',            '6',            0,
    0,              0,              'm',            'j',
    'u',            '7',            '8',            0,
    0,              ',',            'k',            'i',
    'o',            '0',            '9',            0,
    0,              '.',            '/',            'l',
    ';',            'p',            '-',            0,
    0,              0,              '\'',           0,
    '[',            '=',            0,              0,
    0/*caps lock*/, 0/*rght shft*/, '\n',           ']',
    0,              '\\',           0,              0,
    0,              0,              0,              0,
    0,              0,              129/*bckspc*/,  0,
    0,              '1',            0,              '4',
    '7',            0,              0,              0,
    '0',            '.',            '2',            '5',
    '6',            '8',            0/*escape*/,    0/*numlock*/
};

//-----------------------scancodes stack part------------------------------------------------------

kbd_stack_t kbd_stack ={.array = {0}, .is_full = 0, .begin = 0, .end = 0, .size = KBD_QUEUE_SIZE};   //stack init

void
kbd_stack_push(kbd_stack_t *stack, uint8_t value)
{
    stack->array[stack->end] = value;
    stack->end = (++stack->end) % stack->size;
    if(stack->begin == stack->end) stack->is_full = TRUE;
    if(stack->is_full) stack->begin = stack->end;
}

uint8_t
kbd_stack_pop(kbd_stack_t *stack, uint8_t *err)
{
    uint8_t ret;
    if(stack->begin != stack->end || stack->is_full)
    {
        ret = stack->array[stack->begin];
        stack->begin = (++stack->begin) % stack->size;
        if(stack->is_full) stack->is_full = FALSE;
        if(err != NULL) *err = 0;
    }
    else { if(err != NULL) *err = 1;}
    return ret;
}

//---------------------------------------------primitive getchar------------------------------------

uint8_t kbd_getchar(uint8_t *flags)
{
    uint8_t err;
    uint8_t scancode;
    uint8_t pressed = TRUE;

    //if stack is empty we waiting to get something in
    do{ scancode = kbd_stack_pop(&kbd_stack, &err); }while(err);
    if(GET_BYTE(scancode, 7))           //if last bit is set then we have release key
    {
        scancode = kbd_stack_pop(&kbd_stack, &err);     //next byte will be scan code of currently realesed key
        pressed = FALSE;
    }
    switch(scancode)
    {
    //left shift
        case 0x12:  (SET_BYTE(*flags, 0, pressed)); break;
    //left alt
        case 0x11:  (SET_BYTE(*flags, 1, pressed)); break;
    //left control
        case 0x14:  (SET_BYTE(*flags, 2, pressed)); break;
    //normal char 
        default:
        if(pressed == TRUE)    
            return kbd_layout[scancode];
    }

    return kbd_getchar(flags);
}

//----------------------------------------------------low level part-------------------------------------


/*this function creates proper configurtion byte for 8042 but it's little
bit overkill for now, I let her here for maybe futher development of os */
uint8_t PS2_ctrl_config_byte(   uint8_t intr_1,         //first port interrupts
                                uint8_t intr_2,         //second port interrupts
                                uint8_t sys_flag,       //system flag
                                uint8_t clk_1,          //first port clock
                                uint8_t clk_2,          //second port clock
                                uint8_t translation)    //translation on first port
{
    uint8_t control_byte = intr_1;
    SET_BYTE(control_byte, 1, (intr_2) ? 1 : 0);
    SET_BYTE(control_byte, 2, (sys_flag) ? 1 : 0);
    SET_BYTE(control_byte, 3, 0);
    SET_BYTE(control_byte, 4, (clk_1) ? 0 : 1);
    SET_BYTE(control_byte, 5, (clk_2) ? 0 : 1);
    SET_BYTE(control_byte, 6, (translation) ? 1 : 0);
    SET_BYTE(control_byte, 7, 0);
    return control_byte;
}


/*  func in case of reading data (used with 'r' flag ) from 8042 check
    if buffor is full. In case of writing data ('w' flag) checks if RAM
    inside 8042 is empty */
static
void PS2_ctrl_status_wait(char mode)
{
    for(uint16_t i = 0; i < 65535; ++i)
    {
        if(mode == 'r') { if(GET_BYTE(inb(PS2_STATS_CMD), 0)) return; }
        else            { if(!GET_BYTE(inb(PS2_STATS_CMD),1)) return; }
    }
}

/* this func write command to command port of 8042 */
static
void PS2_ctrl_write_cmd(uint8_t cmd)
{
    PS2_ctrl_status_wait('w');
    outb(PS2_STATS_CMD, cmd);
}

/* this one is for reading data port */
static 
uint8_t PS2_ctrl_read_data(void)
{
    PS2_ctrl_status_wait('r');
    return inb(PS2_DATA);
}

//============IRQ HANDLER=====================
static 
void PS2_handler(registers_t regs)
{kbd_stack_push(&kbd_stack, PS2_ctrl_read_data());}

uint8_t PS2_init(void)
{    
    //turning off devices
    PS2_ctrl_write_cmd(0xAD);   //0xAD - disable first PS2 device 
    PS2_ctrl_write_cmd(0xA7);   //0xA7 - disable second PS2 device
    
    //flushing buffer
    PS2_ctrl_read_data();

    //setting controller config byte
    PS2_ctrl_write_cmd(0x60);                   //0x60 - write byte from data port to internal ram        
    PS2_ctrl_status_wait('w');
    outb(PS2_DATA, PS2_ctrl_config_byte(TRUE    ,FALSE  ,TRUE    ,TRUE   ,FALSE  ,FALSE));
   
    //controller selftest
    PS2_ctrl_write_cmd(0xAA);                  //0xAA - test PS2 controller
    if(PS2_ctrl_read_data() != 0x55) return 1; //if everything is OK we should get 0x55

    //first port test
    PS2_ctrl_write_cmd(0xAB);                   //0xAB - test first PS/2 port
    if(PS2_ctrl_read_data() != 0x00) return 1;  //if everything is OK we should get 0x00

    //enable first device
    PS2_ctrl_write_cmd(0xAE);                   //0xAE - enable first PS/2 port

    //add interrupt handler for keyboard
    register_interrupt_handlers(IRQ1, &PS2_handler);

    return 0;
}