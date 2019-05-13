\ Model ILP1
\ LP format - for model browsing. Use MPS format to capture full model detail.
Minimize
  C0 + 6 C1 + 4 C2 + C3 + 2 C4 + 3 C5 + 3 C13 + 2 C14 + 2 C15 + 2 C16 + C17
   + 4 C18 + 3 C19
Subject To
 R0: C0 + C1 <= 1
 R1: C0 + C2 <= 1
 R2: C1 + C3 <= 1
 R3: C2 + C3 <= 1
 R4: C2 + C4 <= 1
 R5: C4 + C5 <= 1
 R6: C3 + C5 <= 1
 R7: C0 + C1 - C6 = 0
 R8: C0 + C2 - C7 = 0
 R9: C1 + C3 - C8 = 0
 R10: C2 + C3 - C9 = 0
 R11: C2 + C4 - C10 = 0
 R12: C4 + C5 - C11 = 0
 R13: C3 + C5 - C12 = 0
 R14: - C6 + C13 <= 0
 R15: - C7 + C14 <= 0
 R16: - C8 + C15 <= 0
 R17: - C9 + C16 <= 0
 R18: - C10 + C17 <= 0
 R19: - C11 + C18 <= 0
 R20: - C12 + C19 <= 0
 R21: C0 + C1 + C2 >= 1
 R22: C1 + C3 >= 1
 R23: C2 + C3 + C4 >= 1
 R24: C3 >= 1
 R25: C4 + C5 >= 1
 R26: C3 + C5 >= 1
 R27: C0 + C13 + C14 >= 1
 R28: C1 + C15 >= 1
 R29: C2 + C16 + C17 >= 1
 R30: C3 >= 1
 R31: C4 + C18 >= 1
 R32: C5 + C19 >= 1
Bounds
Binaries
 C0 C1 C2 C3 C4 C5 C6 C7 C8 C9 C10 C11 C12 C13 C14 C15 C16 C17 C18 C19
End
