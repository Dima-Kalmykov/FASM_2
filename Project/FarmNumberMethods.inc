;---------------------------------------------------------------------------------------------------------------------------
; Summary:
;       Get next Farm number.

; Params:
;       result - variable for recording the result.
;       power - require power of 2 (power = (2 ^ n) for F[n] = 2 ^ (2 ^ n) + 1).

; Return:
;       value equal to F[n] = 2 ^ power + 1 (result).
macro GetNextFarmNumber result, power {

        ; Initial value.
        mov [temp], 1
        mov [cur_power], 0

        shl [power], 1

        ; Loop for calculating 2 ^ power.
        getNextPowerLoop:

                ; temp = temp * 2 (bit shift).
                shl [temp], 1

                ; Increase current power.
                add [cur_power], 1

                ; Write require power to register.
                mov ecx, [power]

                ; Check number of operation (exit condition - require power = current power).
                cmp ecx, [cur_power]
                jne getNextPowerLoop

        ; Write 2 ^ power to ebx and increase it
        mov ebx, [temp]
        inc ebx

        ; Write data from ebx to the result.
        mov result, ebx
}

;----------------------------------------------------------------------------------------------------------------------------
; Summary:
;       Print current Farm number.

; Params:
;       index - number of current Farm number.
;       value - value of current Farm number.

; Return:
;       void.
macro PrintCurrentFarmNumber index, value {
        push [index]
        push [value]
        push strFarmNumber
        call[printf]
}

;-----------------------------------------------------------------------------------------------------------------------------
; Summary:
;       Print definition of Farm number.

; Params:
;       No parameters.

; Return:
;       void.
macro PrintFarmNumberDefinition  {
        push strDefinition
        call[printf]
}

;------------------------------------------------------------------------------------------------------------------------------
; Summary:
;       Set up initial values and calculate initial Farm number.

; Params:
;       power - start power of farm number: F[0] = 2 ^ (2 ^ 0) + 1.
;       topExpressionValue - start value (2 ^ n) of F[n] = 2 ^ (2 ^ n) + 1
;       allExpressionValue - value of F[n].

; Return:
;       void.
macro SetInitialValues power, topExpressionValues, allExpresionValues{
        mov [power], 0
        mov [topExpressionValue], 1
        shl [topExpressionValue], 0

        mov [allExpressionValue], 1
        shl [allExpressionValue], 1
        inc [allExpressionValue]
}

;-------------------------------------------------------------------------------------------------------------------------------
; Summary:
;       Print result.

; Params:
;       power - power that caused the overflow.
;       counter - result amount of Farm numbers.

; Return:
;       void.
macro PrintResult power, counter {

        ; Print empty str just for handsome output.
        push strEmpty
        call[printf]

        ; Print message about overflow.
        push[power]
        push strOutOfRange
        call[printf]

        ; Print total count of Farm numbers.
        push [counter]
        push strTotalCount
        call[printf]

        ; Last phrase: "Press any key to cointinue...".
        push strExit
        call[printf]
}