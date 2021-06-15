#ifndef __MEMMAP_H__
#define __MEMMAP_H__

#define __ALIGNMENT_BYTES__     (1)
#define __ALIGNMENT__           (1 << (__ALIGNMENT_BYTES__ - 1))
#define __ALIGN__(address)      (address & ~(__ALIGNMENT__ - 1))
#define __MEMORY__(address)      (*((volatile unsigned char *) __ALIGN__(address)))

#define MCUCR       __MEMORY__(0x55)

#define PORTB       __MEMORY__(0x25)
#define DDRB        __MEMORY__(0x24)
#define PINB        __MEMORY__(0x23)

#define PORTC       __MEMORY__(0x28)
#define DDRC        __MEMORY__(0x27)
#define PINC        __MEMORY__(0x26)

#define PORTD       __MEMORY__(0x2B)
#define DDRD        __MEMORY__(0x2A)
#define PIND        __MEMORY__(0x29)

#define UDR0        __MEMORY__(0xC6)
#define UBRR0H        __MEMORY__(0xC5)
#define UBRR0L        __MEMORY__(0xC4)
#define UCSR0C        __MEMORY__(0xC2)
#define UCSR0B        __MEMORY__(0xC1)
#define UCSR0A        __MEMORY__(0xC0)

#endif //__MEMMAP_H__