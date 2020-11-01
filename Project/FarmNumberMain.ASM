;--------------------------------------------------------------------------
format PE console

entry start

include 'win32a.inc'
include 'FarmNumberMethods.inc'

;--------------------------------------------------------------------------
section '.data' data readable writable

        strDefinition db 'Farm number: F[n] = 2 ^ (2 ^ n) + 1', 10, 10, 0
        strEmpty db '', 10, 0
        strOutOfRange  db 'Out of range of dword for n = %d', 10, 10, 0
        strExit db 'Press any key to exit...', 0
        strFarmNumber  db 'F[%d] = %d', 10, 0
        strTotalCount db 'Total count: %d', 10, 10, 0


        ; Power = n: F[n] = 2 ^ (2 ^ n) + 1.
        power              dd ?

        ; Temp power for calculating new power of 2.
        cur_power          dd ?

        ; Count amount of Farm numbers.
        counter            dd ?

        ; Temp value for calculate new power of 2.
        temp               dd ?

        ; F[n] = 2 ^ (2 ^ n) + 1
        ; Top expression = (2 ^ n)
        topExpressionValue dd ?

        ; AllExpression = F[n]
        allExpressionValue dd ?

;--------------------------------------------------------------------------
section '.code' code readable executable

        start:

                ; Current index of Farm number.
                mov [counter], 0

                ; Print definition of Farm number.
                PrintFarmNumberDefinition

                ; Initialize fields with start values.
                SetInitialValues power, topExpressionValue, allEaxpressionValue

        getNextValueLoop:

                ; Print current Farm number.
                PrintCurrentFarmNumber allExpressionValue, power

                ; Get new Farm number.
                GetNextFarmNumber [allExpressionValue], topExpressionValue

                inc [power]
                inc [counter]

                ; Check for overflow.
                ; Compare with 1, because of 2 ^ (2 ^ n) cause overflow,
                ; then 2 ^ (2 ^ n) = 0,
                ; but F[n] = 2 ^ (2 ^ n) + 1 = 0 + 1 = 1
                cmp [allExpressionValue], 1

                jne getNextValueLoop


        finish:

                ; Print info about overflow and total count.
                PrintResult power, counter

                call [getch]

                push 0
                call [ExitProcess]

;---------------------------------------------------------------------------
                                                 
section '.idata' import data readable
library kernel, 'kernel32.dll',\
                msvcrt, 'msvcrt.dll'

        import kernel,\
               ExitProcess, 'ExitProcess'

        import msvcrt,\
               printf, 'printf',\
               getch, '_getch',\
               scanf, 'scanf'
;--------------------------------------------------------------------------