;**** **** **** **** ****
;
; Bluejay digital ESC firmware for controlling brushless motors in multirotors
;
; Copyright 2020 Mathias Rasmussen
; Copyright 2011, 2012 Steffen Skaug
;
; This file is part of Bluejay.
;
; Bluejay is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.
;
; Bluejay is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with Bluejay.  If not, see <http://www.gnu.org/licenses/>.
;
;**** **** **** **** ****
;
; Hardware definition file "C"
; Ac Ap MC MB MA CC X  RC    X  X  X  X  Cc Cp Bc Bp
;
;**** **** **** **** ****

PWM_ACTIVE_HIGH	EQU	1		; Pwm non-inverted
COMPWM_ACTIVE_HIGH	EQU	1		; Damping inverted

; Note: currently all comparator pins (Comp_ and Mux_) must be on the same port
COMPARATOR_PORT	EQU	0

IF FETON_DELAY == 0
	PCA0CPM_POWER	EQU	PCA0CPM0
	PCA0CPL_POWER	EQU	PCA0CPL0
	PCA0CPH_POWER	EQU	PCA0CPH0

	PCA0CPM_DAMP	EQU	PCA0CPM1
	PCA0CPL_DAMP	EQU	PCA0CPL1
	PCA0CPH_DAMP	EQU	PCA0CPH1
ELSE
	PCA0CPM_POWER	EQU	PCA0CPM0
	PCA0CPL_POWER	EQU	PCA0CPL0
	PCA0CPH_POWER	EQU	PCA0CPH0

	PCA0CPM_DAMP	EQU	PCA0CPM1
	PCA0CPL_DAMP	EQU	PCA0CPL1
	PCA0CPH_DAMP	EQU	PCA0CPH1
ENDIF

;*********************
; PORT 0 definitions *
;*********************
A_com		EQU	7	;o
A_pwm		EQU	6	;o
C_Mux		EQU	5	;i
B_Mux		EQU	4	;i
A_Mux		EQU	3	;i
V_Mux		EQU	2	;i
;			EQU	1	;i
Rcp_In		EQU	0	;i


P0_DIGITAL	EQU	NOT((1 SHL A_Mux) + (1 SHL B_Mux) + (1 SHL C_Mux) + (1 SHL V_Mux))
P0_INIT		EQU	NOT((1 SHL A_pwm) + (1 SHL A_com))
P0_PUSHPULL	EQU	(1 SHL A_pwm) + (1 SHL A_com)
P0_SKIP		EQU	0FFh


;*********************
; PORT 1 definitions *
;*********************
;			EQU	7	;i
;			EQU	6	;i
;			EQU	5	;i
;			EQU	4	;i
C_com		EQU	3	;o
C_pwm		EQU	2	;o
B_com		EQU	1	;o
B_pwm		EQU	0	;o


P_ApwmFET		EQU	P0.A_pwm
P_AcomFET		EQU	P0.A_com
P_BpwmFET		EQU	P1.B_pwm
P_BcomFET		EQU	P1.B_com
P_CpwmFET		EQU	P1.C_pwm
P_CcomFET		EQU	P1.C_com


P1_DIGITAL	EQU	(1 SHL B_pwm) + (1 SHL C_pwm) + (1 SHL B_com) + (1 SHL C_com)
P1_INIT		EQU	00h
P1_PUSHPULL	EQU	(1 SHL B_pwm) + (1 SHL C_pwm) + (1 SHL B_com) + (1 SHL C_com)
P1_SKIP		EQU	0FFh


Set_Pwm_A MACRO
IF FETON_DELAY == 0
	cON	P_AcomFET
	mov	P0SKIP, #(NOT (1 SHL A_pwm))
	mov	P1SKIP, #0FFh
ELSE
	mov	P0SKIP, #(NOT ((1 SHL A_pwm) + (1 SHL A_com)))
	mov	P1SKIP, #0FFh
ENDIF
ENDM

Set_Pwm_B MACRO
IF FETON_DELAY == 0
	cON	P_BcomFET
	mov	P0SKIP, #0FFh
	mov	P1SKIP, #(NOT (1 SHL B_pwm))
ELSE
	mov	P0SKIP, #0FFh
	mov	P1SKIP, #(NOT ((1 SHL B_pwm) + (1 SHL B_com)))
ENDIF
ENDM

Set_Pwm_C MACRO
IF FETON_DELAY == 0
	cON	P_CcomFET
	mov	P0SKIP, #0FFh
	mov	P1SKIP, #(NOT (1 SHL C_pwm))
ELSE
	mov	P0SKIP, #0FFh
	mov	P1SKIP, #(NOT ((1 SHL C_pwm) + (1 SHL C_com)))
ENDIF
ENDM

Set_Pwms_Off MACRO
	mov	P0SKIP, #0FFh
	mov	P1SKIP, #0FFh
ENDM


;*********************
; PORT 2 definitions *
;*********************
DebugPin		EQU	0	;o

P2_DIGITAL	EQU	(1 SHL DebugPin)
P2_PUSHPULL	EQU	(1 SHL DebugPin)
P2_SKIP		EQU	0FFh


;**** **** **** **** ****
; Inherit base layout
;**** **** **** **** ****
$set(CUSTOM_PWM_PHASE)
$include (Base.inc)