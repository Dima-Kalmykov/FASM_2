; Вариант 10.
; Разработать программу, которая вводит одномерный массив A[N]
; и формирует из элементов массива А новый массив B по правилам,
; указанным в таблице, и выводит его.
; Таблица:
; 1) Ввод и вывод массивов оформить как подпрограммы.
; 2) Выполнение задания по варианту оформить как процедуру.
; 3) Организовать вывод как исходного, так и сформированного массивов.
; Массив B состоит из элементов А в обратном порядке.
                                                               format PE console
entry start

include 'win32a.inc'

section '.data' data readable writable

        strVecSize   db 'size of vector? ', 0
        strIncorSize db 'Incorrect size of vector = %d', 10, 0
        strVecElemI  db '[%d]? ', 0
        strScanInt   db '%d', 0
        strEmpty db '', 10, 0
        strA db 'Straight array:', 10,  0
        strB db 'Reversed array:', 10, 0
        strSumValue  db 'Summa = %d', 10, 0
        strCur db 'Current value = %d', 10, 0
        strVecElemOut  db '[%d] = %d', 10, 0

        vec_size     dd 0
        sum          dd 0
        i            dd ?
        tmpStack     dd ?
        tmp          dd ?
        A          rd 100
        B          rd 100


section '.code' code readable executable

start:

; Input vector.
call VectorInput

; Create new vector and reverse it.
call Reverse

; Print empty str and array A.
push strEmpty
call [printf]
push strA
call[printf]

call PrintStraightVector

; Print empty str and array B.
push strEmpty
call [printf]
push strB
call [printf]

call PrintReversedVector




finish:
        call [getch]

        push 0
        call [ExitProcess]

;--------------------------------------------- Input Vector -------------------------------------
VectorInput:
        push strVecSize
        call [printf]
        add esp, 4

        push vec_size
        push strScanInt
        call [scanf]
        add esp, 8

        mov eax, [vec_size]
        cmp eax, 0
        jg  getVector
; fail size
        push vec_size
        push strIncorSize
        call [printf]
        push 0
        call [ExitProcess]
; else continue...
getVector:
        xor ecx, ecx            ; ecx = 0
        mov ebx, A
getVecLoop:
        mov [tmp], ebx
        cmp ecx, [vec_size]
        jge endInputVector       ; to end of loop

        ; input element
        mov [i], ecx
        push ecx
        push strVecElemI
        call [printf]
        add esp, 8

        push ebx
        push strScanInt
        call [scanf]
        add esp, 8

        mov ecx, [i]
        inc ecx
        mov ebx, [tmp]
        add ebx, 4
        jmp getVecLoop
endInputVector:
        ret
; --------------------------------------------------------------------------------------------------------------------------


; ------------------------------------------------------------------- Reverse -----------------------------------------------
Reverse:
  reverse:
        mov ecx, [vec_size]
        mov ebx, A                        ; ecx = 0
        mov eax, B
reverseLoop:
        mov [tmp], eax

        cmp ecx, 0
        je endReverseLoop     ; to end of loop
        mov [i], ecx

        mov edx, [vec_size]

        mov esi, [ebx + edx * 4 - 4]
        mov [eax], esi

        mov ecx, [i]
        dec ecx
        mov eax, [tmp]
        add eax, 4
        sub ebx, 4

        jmp reverseLoop
endReverseLoop:
ret
; -----------------------------------------------------------------------------------------------------

; --------------------------------------------------------------- Print reverse vector ---------------------------------------------
PrintReversedVector:


        putVector2:
        mov [tmpStack], esp
        xor ecx, ecx            ; ecx = 0
        mov ebx, B ; ebx = &vec
putVecLoop2:
        mov [tmp], ebx
        cmp ecx, [vec_size]
        je endOutputVector2      ; to end of loop
        mov [i], ecx

        ; output element
        push dword [ebx]
        push ecx
        push strVecElemOut
        call [printf]

        mov ecx, [i]
        inc ecx
        mov ebx, [tmp]
        add ebx, 4
        jmp putVecLoop2
endOutputVector2:
        mov esp, [tmpStack]
        ret
;-------------------------------------------------------------------------------------------------------------------------


; ------------------------------------------------------------ Print straight vector ---------------------------------------
PrintStraightVector:

        putVector:
        mov [tmpStack], esp
        xor ecx, ecx            ; ecx = 0
        mov ebx, A        ; ebx = &vec
putVecLoop:
        mov [tmp], ebx
        cmp ecx, [vec_size]
        je endOutputVector      ; to end of loop
        mov [i], ecx

        ; output element
        push dword [ebx]
        push ecx
        push strVecElemOut
        call [printf]

        mov ecx, [i]
        inc ecx
        mov ebx, [tmp]
        add ebx, 4
        jmp putVecLoop
endOutputVector:
        mov esp, [tmpStack]
        ret

;-------------------------------third act - including HeapApi--------------------------
                                                 
section '.idata' import data readable
    library kernel, 'kernel32.dll',\
            msvcrt, 'msvcrt.dll',\
            user32,'USER32.DLL'

include 'api\user32.inc'
include 'api\kernel32.inc'
    import kernel,\
           ExitProcess, 'ExitProcess',\
           HeapCreate,'HeapCreate',\
           HeapAlloc,'HeapAlloc'
  include 'api\kernel32.inc'
    import msvcrt,\
           printf, 'printf',\
           scanf, 'scanf',\
           getch, '_getch'