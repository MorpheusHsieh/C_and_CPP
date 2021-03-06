;  旼컴컴컴컴컴컴컴컴컴컴컴컴컴커
;  � Program : LEDUP.ASM        �
;  � Aother  : Avatar           �
;  � Date    : 94/05/24         �
;  � Function: LED Demo up Move �
;  읕컴컴컴컴컴컴컴컴컴컴컴컴컴켸


port_a   EQU   0200h       ;define 8255
port_c   EQU   0202h
ct8255   EQU   0243h
md8255   EQU   80h
timer0   EQU   0248h         ;define 8253
ct8253   EQU   024bh
md8253   EQU   36h
ocw1     EQU   21h         ;define 8259
ocw2     EQU   20h
EOI      EQU   20h
;
;
stck     segment stack
         dw 100 dup(0)
stck     ends
;
;
code     segment
         assume cs:code, ds:data, ss:stck

begin:   cli               ;disable interrupt of CPU

         mov    dx,ct8255   ;initialize 8255
         mov    al,md8255
         out    dx,al

         mov    ax,code     ;set interrupt vector
         mov    ds,ax
         mov    dx,offset myint0d
         mov    ax,250bh
         int    21h

         mov    ax,data
         mov    ds,ax
         mov    es,ax
         mov    dx,ct8253   ;set 8253 mode
         mov    al,md8253
         out    dx,al

         mov    dx,timer0   ;set timer0
         mov    al,0ffh
         out    dx,al
         mov    al,01h
         out    dx,al
         mov    dx,ocw1
         in     al,dx
         and    al,0f7h
         out    dx,al
         sti                ;enable interrupt of CPU
;
         call   show
         jmp    exit
;
;
myint0d: push   ax        ;Interrupt service routine
         push   bx        ;save registers
         push   dx
         push   si


         mov    dx,port_a   ;clean screen
         mov    ax,0
         out    dx,ax

         mov    dx,port_c   ;move to next column
         inc    column
         cmp    column,40h
         jne    nextcl
         mov    column,0
nextcl:  mov    ax,column
         out    dx,al

         mov    dx,port_a   ;display one column
         mov    si,column
         shl    si,1
         mov    bx,OFFSET buffer
         mov    ax,[bx+si]
         out    dx,ax

         mov    al,EOI      ;send EOI to 8259
         out    ocw2,al

         pop    si
         pop    dx
         pop    bx
         pop    ax
         iret               ;end of interrupt service routine
;
delay    proc               ;delay part
         push   cx
         mov    cx,0afffh
         nop
         nop
         loop   $
         pop    cx
         ret
delay    endp
;
show     proc
         mov    bx,offset buffer
         mov    dx,16
loop1:   mov    si,0
         mov    cx,64
loop2:   mov    ax,[bx+si]
         shr    ax,1
         mov    [bx+si],ax
         inc    si
         inc    si
         loop   loop2
         call   delay
         call   delay
         call   delay
         dec    dx
         cmp    dx,0
         jne    loop1
show     endp
;
exit:    mov    dx,ocw1
         in     al,dx
         or     al,08h
         out    dx,al

         mov    dx,ct8255
         mov    al,md8255
         out    dx,al

         mov    ah,4ch
         int    21h
code     ends
;
;
data     segment
column   dw 0ffffh

