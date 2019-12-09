PROGRAM Teste1
  VAR
    LIGA_PID AT %QX0.0 : BOOL;
  END_VAR
  VAR RETAIN
    SP AT %M0 : REAL;
  END_VAR
  VAR
    VP AT %M1 : REAL;
    SC AT %M2 : REAL;
    KP AT %M3 : REAL;
    TI AT %M4 : REAL;
    TD AT %M5 : REAL;
    VP_RAW AT %IW100 : UINT;
    SC_RAW AT %QW100 : UINT;
  END_VAR
  VAR
    UINT_TO_REAL27_OUT : REAL;
    MUL20_OUT : REAL;
    DIV21_OUT : REAL;
    MUL30_OUT : REAL;
    DIV31_OUT : REAL;
    REAL_TO_UINT26_OUT : UINT;
  END_VAR

  UINT_TO_REAL27_OUT := UINT_TO_REAL(VP_RAW);
  MUL20_OUT := MUL(UINT_TO_REAL27_OUT, 100.0);
  DIV21_OUT := DIV(MUL20_OUT, 65536.0);
  VP := DIV21_OUT;
  MUL30_OUT := MUL(SC, 16384.0);
  DIV31_OUT := DIV(MUL30_OUT, 100.0);
  REAL_TO_UINT26_OUT := REAL_TO_UINT(DIV31_OUT);
  SC_RAW := REAL_TO_UINT26_OUT;
END_PROGRAM


CONFIGURATION Config0

  RESOURCE Res0 ON PLC
    TASK task0(INTERVAL := T#20ms,PRIORITY := 0);
    PROGRAM instance0 WITH task0 : Teste1;
  END_RESOURCE
END_CONFIGURATION