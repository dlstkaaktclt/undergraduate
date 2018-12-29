type token =
  | IF
  | THEN
  | ELSE
  | FN
  | RARROW
  | RAISE
  | HANDLE
  | LET
  | IN
  | PLUS
  | MINUS
  | LP
  | RP
  | EOF
  | EQ
  | NUM of (int)
  | ID of (string)

val program :
  (Lexing.lexbuf  -> token) -> Lexing.lexbuf -> Xexp.xexp
