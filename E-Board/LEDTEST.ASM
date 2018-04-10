;  ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
;  ³ Program : LEDshow.asm    ³
;  ³ Aother  : Avatar         ³
;  ³ Date    : 94/05/24       ³
;  ³ Function: LED Demo shows ³
;  ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ


port_a   EQU   0200h       ;define 8255
port_c   EQU   0202h
ct8255   EQU   0243h
md8255   EQU   80h
timer0   EQU   0248h       ;define 8253
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

         mov   dx,ct8255   ;initialize 8255
         mov   al,md8255
         out   dx,al

         mov   ax,data
         mov   ds,ax
         mov   es,ax
         mov   dx, offset myint0d
         mov   ax,250bh
         int   21h

         mov   dx,ct8253   ;set 8253 mode
         mov   al,md8253
         out   dx,al

         mov   dx,timer0   ;set timer0
         mov   al,0ffh
         out   dx,al
         mov   al,01h
         out   dx,al
         mov   dx,ocw1
         in    al,dx
         and   al,0f7h
         out   dx,al
         sti               ;enable interrupt of CPU
;
         jmp main          ;jump main program for show
;
;
myint0d: push  ax          ;save registers
         push  bx
         push  dx
         push  si


         mov   dx,port_a   ;clean screen
         mov   ax,0
         out   dx,ax

         mov   dx,port_c   ;move to next column
         inc   column
         cmp   column,40h
         jne   nextcl
         mov   column,0
nextcl:  mov   ax,column
         out   dx,al

         mov   dx,port_a   ;display one column
         mov   si,column
         shl   si,1
         mov   bx,OFFSET buffer
         mov   ax,[bx+si]
         out   dx,ax

         mov   al,EOI      ;send EOI to 8259
         out   ocw2,al

         pop   si
         pop   dx
         pop   bx
         pop   ax
         iret
;
;
delay    proc              ;delay part
         push  cx
         mov   cx,0afffh
         nop
         nop
         loop  $
         pop   cx
         ret
delay    endp
;
show     proc
again:   mov     bx, offset buffer
         mov     dx,64
next1:   mov     si,0
         mov     cx,128
next2:   mov     ax,[bx+si+2]
         mov     [bx+si],ax
         inc     si
         inc     si
         loop    next2
         call    delay
         dec     dx
         cmp     dx,0
         jne     next1
         ret
show     endp
;
main:    call  show
         mov   ah,4ch      ;end the program
         int   21h
code     ends
;
;
data     segment
column   dw      0ffffh
buffer   db      000H,008H,002H,008H,0FAH,004H,0AAH,004H
         db      0AAH,002H,0AAH,001H,0AAH,000H,0FFH,03FH
         db      0AAH,000H,0AAH,001H,0AAH,002H,0AAH,004H
         db      0FAH,004H,002H,008H,000H,008H,000H,000H
         db      008H,020H,008H,010H,008H,008H,008H,004H
         db      008H,002H,0F9H,001H,04AH,000H,04EH,010H
         db      048H,020H,048H,020H,048H,010H,0E8H,00FH
         db      048H,000H,00CH,000H,008H,000H,000H,000H
         db      002H,000H,002H,004H,002H,002H,002H,001H
         db      082H,000H,042H,000H,022H,000H,0FAH,03FH
         db      006H,000H,022H,000H,042H,000H,082H,000H
         db      082H,001H,003H,003H,002H,000H,000H,000H
         db      000H,020H,0FFH,017H,049H,00AH,049H,006H
         db      049H,00AH,0FFH,017H,000H,020H,010H,010H
         db      06CH,008H,087H,005H,004H,006H,084H,009H
         db      07CH,010H,006H,030H,004H,010H,000H,000H
         db 100 dup(0)
data     ends
         end
