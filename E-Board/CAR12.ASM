;  ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
;  ³ This program shows welcome words!!                                  ³
;  ³        Wang Yuan-Yuan     C.C.I.T.   Dep. of electrical engineering ³
;  ³  Program's Name:car1.asm                                            ³
;  ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

         .MODEL   SMALL
         .CODE
begin:   sti               ;enable interrupt of CPU
         MOV   AH,15       ;Set BH active display page
         INT   10H
         MOV   AH,0        ;Select 80 x 25 B/W text mode
         INT   10H

         mov   ax,@data
         mov   ds,ax
         mov   es,ax

;        CALL  ISRMO

again:   mov   bx, OFFSET buffer     ;the start of data move
         mov   dx,64
next1:   mov   si,0
         mov   cx,192
next2:   mov   ax,[bx+si+2]          ;move two dytes
         mov   [bx+si],ax
         inc   si
         inc   si
         loop  next2
         call  ISRMO
         dec   dx
         cmp   dx,0
         jne   next1


         cld
         mov   cx,64
         mov   si,OFFSET buffer
         mov   di,OFFSET tmpbuf
next3:   movsw
         loop  next3

;===================================================================
         mov   bx,OFFSET buffer
         mov   dx,64
B1:      mov   si,0
         mov   cx,192
B2:      mov   ax,[bx+si+2]
         mov   [bx+si],ax
         inc   si
         inc   si
         loop  B2
         call  ISRMO
         dec   dx
         cmp   dx,0
         jne   B1

;==================================================================
         cld
         mov   cx,64
         mov   si,OFFSET buffer
         mov   di,OFFSET tmpbuf
B3:      movsw
         loop   B3

;==================================================================

         mov   bx,OFFSET buffer
         mov   dx,16
C1:      mov   si,0
         mov   cx,64
C2:      mov   ax,[bx+si]
         ror   ax,1
         mov   [bx+si],ax
         inc   si
         inc   si
         loop  C2
         call  ISRMO
         dec   dx
         cmp   dx,0
         jne   C1

         mov   bx,OFFSET buffer
         mov   dx,16
R1:      mov   si,0
         mov   cx,64
R2:      mov   ax,[bx+si]
         rol   ax,1
         mov   [bx+si],ax
         inc   si
         inc   si
         loop  R2
         call  ISRMO
         dec   dx
         cmp   dx,0
         jne   R1
;==================================================================

         cld
         mov   cx,64
         mov   si,OFFSET buffer
         mov   di,OFFSET tmpbuf
C3:      movsw
         loop   C3

         mov   dx,16
         mov   databuf,1
next5a1: mov   cx,64
         mov   si,0
next5a2: push  cx
         mov   bx,offset tmpbuf
         mov   ax,[bx+si]
         mov   cl,databuf
         rcl   ax,cl
         mov   bx,offset buffer
         mov   ax,[bx+si]
         rcl   ax,1
         mov   [bx+si],ax
         inc   si
         inc   si
         pop   cx
         loop  next5a2
         call  ISRMO
         inc   databuf
         dec   dx
         cmp   dx,0
         jne   next5a1
;================================
         mov   dx,8
         mov   databuf,1
next5a3: mov   cx,64
         mov   si,0
next5a4: push  cx
         mov   bx,offset buffer1
         mov   ax,[bx+si]
         mov   cl,databuf
         rcr   ax,cl
         mov   bx,offset buffer
         mov   ax,[bx+si]
         rcr   ax,1
         mov   [bx+si],ax
         inc   si
         inc   si
         pop   cx
         loop  next5a4
         call  ISRMO
         inc   databuf
         dec   dx
         cmp   dx,0
         jne   next5a3
;================================
         mov   dx,8
         mov   databuf,9
next5a5: mov   cx,64
         mov   si,0
next5a6: push  cx
         mov   bx,offset tmpbuf
         mov   ax,[bx+si]
         mov   cl,databuf
         rcl   ax,cl
         mov   bx,offset buffer
         mov   ax,[bx+si]
         rcl   ax,1
         mov   [bx+si],ax
         inc   si
         inc   si
         pop   cx
         loop  next5a6
         call  ISRMO
         inc   databuf
         dec   dx
         cmp   dx,0
         jne   next5a5
;=================================
         mov   dx,4
         mov   databuf,1
next5a7: mov   cx,64
         mov   si,0
next5a8: push  cx
         mov   bx,offset buffer1
         mov   ax,[bx+si]
         mov   cl,databuf
         rcr   ax,cl
         mov   bx,offset buffer
         mov   ax,[bx+si]
         rcr   ax,1
         mov   [bx+si],ax
         inc   si
         inc   si
         pop   cx
         loop  next5a8
         call  ISRMO
         inc   databuf
         dec   dx
         cmp   dx,0
         jne   next5a7
;=============================
         mov   dx,4
         mov   databuf,13
next5a9: mov   cx,64
         mov   si,0
next5a10:push  cx
         mov   bx,offset tmpbuf
         mov   ax,[bx+si]
         mov   cl,databuf
         rcl   ax,cl
         mov   bx,offset buffer
         mov   ax,[bx+si]
         rcl   ax,1
         mov   [bx+si],ax
         inc   si
         inc   si
         pop   cx
         loop  next5a10
         call  ISRMO
         inc   databuf
         dec   dx
         cmp   dx,0
         jne   next5a9
;=============================
         mov   dx,2
         mov   databuf,1
next5a11:mov   cx,64
         mov   si,0
next5a12:push  cx
         mov   bx,offset buffer1
         mov   ax,[bx+si]
         mov   cl,databuf
         rcr   ax,cl
         mov   bx,offset buffer
         mov   ax,[bx+si]
         rcr   ax,1
         mov   [bx+si],ax
         inc   si
         inc   si
         pop   cx
         loop  next5a12
         call  ISRMO
         inc   databuf
         dec   dx
         cmp   dx,0
         jne   next5a11
;================================
         mov   dx,2
         mov   databuf,15
next5a13:mov   cx,64
         mov   si,0
next5a14:push  cx
         mov   bx,offset tmpbuf
         mov   ax,[bx+si]
         mov   cl,databuf
         rcl   ax,cl
         mov   bx,offset buffer
         mov   ax,[bx+si]
         rcl   ax,1
         mov   [bx+si],ax
         inc   si
         inc   si
         pop   cx
         loop  next5a14
         call  ISRMO
         inc   databuf
         dec   dx
         cmp   dx,0
         jne   next5a13
;==================================================================
;        mov   ah,01h      ;check any key was hit!!
;        int   16h
         mov   ah,4ch      ;end the program
         int   21h

;====================================================================
ISRMO PROC
         push  ax             ;save registers
         push  bx
         push  cx
         push  dx
         push  si

SET_A:   cmp   column,0348h
         jne   nextcl
         mov   column,0308h
         jmp   exit
nextcl:  MOV   dx,column      ; Strat at row 3, column 8
;================================================================
CLEAN:
         MOV   AH,2
         INT   10H
         MOV   CX,1            ; Character count=1
         MOV   AX,0A00h
         INT   10H

         INC   DH
         CMP   DH,19
         JNE   CLEAN

;================================================================

         inc   column
         mov   ax,column
         sub   ax,0309h
         mov   si,ax
         shl   si,1
         mov   bx,OFFSET buffer
         mov   cx,[bx+si]
         mov   bh,0

;========================================================

         mov   dh,3        ;Display one column
 cdis1:  shr   cx,1
         PUSH  CX
         MOV   CX,1
         jc    bri2

         MOV   AH,2
         INT   10H
         MOV   AX,0A00h
         INT   10H
         INC   DH

         jmp   ctinu
 bri2:
         MOV   AH,2
         INT   10H
         MOV   AX,0ADBh
         INT   10H
         INC   DH

ctinu:   cmp   dh,19
         POP   CX
         jne   cdis1
;=======================================================
         cmp   dl,72                   ; Margin of screen?
         JNE   SET_a
;=======================================================
exit:
         pop   si
         pop   dx
         pop   cx
         pop   bx
         pop   ax
         ret
ISRMO ENDP
;=======================================================
         .DATA

         .DATA

column   DW    00308h
databuf  db    ?
buffer   db    128 DUP (0)
         DB    000H,000H,000H,000H,000H,000H,000H,010H
         DB    0FEH,00FH,002H,000H,042H,000H,04AH,007H
         DB    04AH,005H,04AH,005H,07EH,005H,04AH,005H
         DB    04AH,005H,04AH,007H,042H,010H,002H,020H
         DB    0FEH,01FH,000H,000H,000H,000H,000H,000H
         DB    000H,000H,000H,000H,000H,000H,000H,000H
         DB    008H,020H,086H,010H,082H,007H,0AAH,03DH
         DB    0AAH,025H,0AAH,027H,0AAH,025H,0FFH,025H
         DB    0AAH,02DH,0AAH,027H,0AAH,025H,0AAH,035H
         DB    082H,007H,08AH,008H,006H,010H,000H,000H
         DB    000H,000H,000H,000H,000H,000H,000H,000H
         DB    000H,000H,002H,004H,0E2H,007H,002H,004H
         DB    0FEH,007H,022H,002H,022H,002H,022H,012H
         DB    010H,008H,06CH,004H,087H,002H,004H,001H
         DB    084H,006H,07CH,018H,004H,010H,004H,010H
         DB    000H,000H,000H,000H,000H,000H,000H,000H
tmpbuf   DB    000H,000H,000H,000H,000H,000H,000H,010H
         DB    0FEH,00FH,002H,000H,042H,000H,04AH,007H
         DB    04AH,005H,04AH,005H,07EH,005H,04AH,005H
         DB    04AH,005H,04AH,007H,042H,010H,002H,020H
         DB    0FEH,01FH,000H,000H,000H,000H,000H,000H
         DB    000H,000H,000H,000H,000H,000H,000H,000H
         DB    008H,020H,086H,010H,082H,007H,0AAH,03DH
         DB    0AAH,025H,0AAH,027H,0AAH,025H,0FFH,025H
         DB    0AAH,02DH,0AAH,027H,0AAH,025H,0AAH,035H
         DB    082H,007H,08AH,008H,006H,010H,000H,000H
         DB    000H,000H,000H,000H,000H,000H,000H,000H
         DB    000H,000H,002H,004H,0E2H,007H,002H,004H
         DB    0FEH,007H,022H,002H,022H,002H,022H,012H
         DB    010H,008H,06CH,004H,087H,002H,004H,001H
         DB    084H,006H,07CH,018H,004H,010H,004H,010H
         DB    000H,000H,000H,000H,000H,000H,000H,000H
buffer1  db    128 dup(0)
         db    64 DUP (0)
         .STACK

         END   begin
