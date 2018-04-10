;  ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
;  ³ This program shows welcome words!!                                  ³
;  ³        Wang Yuan-Yuan     C.C.I.T.   Dep. of electrical engineering ³
;  ³  Program's Name:car1.asm                                            ³
;  ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ


port_a   EQU   03e0h       ;define 8255
port_c   EQU   03e2h
ct8255   EQU   03e3h
md8255   EQU   80h
timer0	 EQU   03e8h	     ;define 8253
ct8253	 EQU   03ebh
md8253   EQU   36h
ocw1     EQU   21h         ;define 8259
ocw2     EQU   20h
EOI      EQU   20h

         .MODEL   SMALL
         .CODE

begin:   cli               ;disable interrupt of CPU

         mov   dx,ct8255   ;initialize 8255
         mov   al,md8255
         out   dx,al

         mov   ax,@code    ;set interrupt vector
         mov   ds,ax
         mov   dx, OFFSET myint0d
	 mov   ax,250bh
         int   21h

         mov   ax,@data
         mov   ds,ax
         mov   es,ax
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


again:   mov   bx, OFFSET buffer     ;the start of data move
         mov   dx,64
next1:   mov   si,0
         mov   cx,192
next2:   mov   ax,[bx+si+2]          ;move two dytes
         mov   [bx+si],ax
         inc   si
         inc   si
         loop  next2
         call  delay
         dec   dx
         cmp   dx,0
         jne   next1


         cld
         mov   cx,64
         mov   si,OFFSET buffer
         mov   di,OFFSET tmpbuf

next3:   movsw
         loop  next3
         mov   ah,01h      ;check any key was hit!!
         int   16h
	 je    again

	 mov   dx,ocw1
	 in    al,dx
	 or    al,08h
	 out   dx,al

	 mov   dx,ct8255
	 mov   al,md8255
	 out   dx,al

         mov   ah,4ch      ;end the program
         int   21h


delay    PROC              ;delay part
         push  cx
         mov   cx,0afffh
         nop
         nop
         loop  $
         pop   cx
         ret
delay    ENDP


;Interrupt service routine
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

         .DATA

column   DW    0ffffh
buffer   db    128 DUP (0)
         DB    000h,000h,03AH,001H,0AAH,03FH,0EFH,02AH
         DB    0BAH,02AH,082H,02AH,0BAH,03FH,0AFH,02AH
         DB    0AAH,02AH,0BAH,02AH,008H,000H,007H,020H
         DB    004H,010H,0F4H,00FH,004H,010H,01CH,020H
         DB    000H,000H,091H,020H,0D2H,010H,0B4H,00FH
         DB    010H,010H,000H,020H,0FEH,027H,002H,022H
         DB    002H,021H,081H,020H,000H,020H,002H,020H
         DB    0FEH,02FH,002H,022H,002H,022H,0FEH,021H
         DB    000H,000H,020H,020H,022H,010H,024H,008H
         DB    028H,004H,0F0H,003H,020H,000H,020H,000H
         DB    03FH,000H,020H,000H,0E0H,03FH,030H,020H
         DB    028H,020H,024H,020H,022H,020H,020H,038H
         DB    000H,000H,0FFH,03FH,011H,021H,01FH,03FH
         DB    011H,021H,0F1H,021H,000H,000H,008H,03EH
         DB    004H,022H,0F7H,022H,094H,03EH,094H,000H
         DB    094H,03EH,0F4H,022H,004H,022H,004H,03EH
tmpbuf   DB    000h,000h,03AH,001H,0AAH,03FH,0EFH,02AH
         DB    0BAH,02AH,082H,02AH,0BAH,03FH,0AFH,02AH
         DB    0AAH,02AH,0BAH,02AH,008H,000H,007H,020H
         DB    004H,010H,0F4H,00FH,004H,010H,01CH,020H
         DB    000H,000H,091H,020H,0D2H,010H,0B4H,00FH
         DB    010H,010H,000H,020H,0FEH,027H,002H,022H
         DB    002H,021H,081H,020H,000H,020H,002H,020H
         DB    0FEH,02FH,002H,022H,002H,022H,0FEH,021H
         DB    000H,000H,020H,020H,022H,010H,024H,008H
         DB    028H,004H,0F0H,003H,020H,000H,020H,000H
         DB    03FH,000H,020H,000H,0E0H,03FH,030H,020H
         DB    028H,020H,024H,020H,022H,020H,020H,038H
         DB    000H,000H,0FFH,03FH,011H,021H,01FH,03FH
         DB    011H,021H,0F1H,021H,000H,000H,008H,03EH
         DB    004H,022H,0F7H,022H,094H,03EH,094H,000H
         DB    094H,03EH,0F4H,022H,004H,022H,004H,03EH
         db    64 DUP (0)
         .STACK

         END   begin
