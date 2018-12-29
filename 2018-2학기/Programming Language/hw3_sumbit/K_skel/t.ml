[Header]
open K
open Unix

let test_run pgm_str = 
  let pgm = Parser.program Lexer.start (Lexing.from_string pgm_str) in
  K.run (K.emptyMemory, K.emptyEnv, pgm)

let test_error_str pgm_str = 
  let pgm = Parser.program Lexer.start (Lexing.from_string pgm_str) in
  try 
    (try let _ =  K.run (K.emptyMemory, K.emptyEnv, pgm) in "" with
    K.Error s -> String.sub s 0 7)
  with Invalid_argument _ -> ""

let redirect_f = "stdout_redirect.txt"

let test_output pgm_str = 
  (* Stdout Redirection *)
  let new_stdout = open_out redirect_f in
  let _ = Unix.dup2 (Unix.descr_of_out_channel new_stdout) Unix.stdout in
  let pgm = Parser.program Lexer.start (Lexing.from_string pgm_str) in
  let _ = K.run (K.emptyMemory, K.emptyEnv, pgm) in
  let _ = flush_all() in (* Current stdout channel must be flushed *)
  let _ = close_out new_stdout in 

  let result_chan = open_in redirect_f in
  really_input_string result_chan (in_channel_length result_chan)


[Test]
(* 1. let, arithmetic *)
test_output
"
let x := 5 in
let y := 10 in
write ((x - 2) * 4 + y / 5)
"

[Value]
"14\n"


[Test]
(* 2. branch, sequence, write *)
test_output
"
if (3 < 5) then (
  write 1;
  if (not false) then (
    write 11;
    if (20 < 10) then 
      write 111
    else 
      write 222
  )
  else 
    write 22
)
else 
  unit
"

[Value]
"1\n11\n222\n"


[Test]
(* 3. assignment, type error *)
test_error_str
"
let x := 1 in 
let y := 2 in
x:= true;
x + y
"

[Value]
"TypeErr"


[Test]
(* 4. Unbound proecdure *)
test_error_str
"
let proc f (x) = x + 1 in
g(1)
"

[Value]
"Unbound"

[Test]
(* 5. While + LetV *)
test_output
"let x := 1 in
let sum := 0 in
while (x < 11) do (
   sum := sum + x;
   x := x + 1
); write sum"

[Value]
"55\n"


[Test]
(* 6. Record *)
test_output 
"write (( {fst := true, snd := 5 } ).snd)"

[Value]
"5\n"

[Test]
(* 7. LetV + AssignF *)
test_output
"
let x := {fst := true, snd := 3 } in
x.fst := 7;
write x.fst
"

[Value]
"7\n"


[Test]
(* 8. LetF + CallV *)
test_output
"
let proc f (x, y, z) = (x + y) * z in
write (f (2, 3, 5))
"

[Value]
"25\n"


[Test]
(* 9. Recursive call *)
test_output
"
let proc f (x) = 
  if (x < 1) then 1 else (x * f (x-1)) 
in
write (f (6))
"

[Value]
"720\n"

[Test]
(* 10. LetF + CallV + CallR *)
test_output
"
let proc f (x) = (x := 3) in
let proc g (x) = (x := 3) in
let x := 1 in
let y := 2 in
f <x>; g (y); 
write (x + y)
"

[Value]
"5\n"


[Test]
(* 11. Exception in false branch *)
test_output
"
if 3 < 2 then
  unit
else (
	if (unit = 3) then
	  x
	else 
    write 25
)
"

[Value]
"25\n"


[Test]
(* 12. Empty record *)
test_output "if {} = unit then write 11 else write 22"

[Value]
"11\n"

[Test]
(* 13. Result of Assign *)
test_output
"
let x := () in
let y := () in
let z := () in
if ((x := 1) + (y := 2) + (z := 3) = 6) then write (x + y + z) else unit
"

[Value]
"6\n"

[Test]
(* 14. Scope *)
test_output
"
let x := 0 in
let z :=
  (let x := 1 in (x := 5); ((x := x + 1) * (let x := 2 in (x := x + 1)))) +
  (let y := 2 in 2 * x)
in 
write z
"

[Value]
"18\n"

[Test]
(* 15. Complex record *)
test_output
"
let x := {i1 := 1, i2 := 2, i3 := {ii1 := true, ii2 := unit} } in
if x.i3.ii2 = unit then write 100 else write 200
"

[Value]
"100\n"


[Test]
(* 16. Arguments in CallF *)
test_output
"
let x := 1 in
let proc f (x, y, z) = x + y + z in
write (f (x := x + 1, x := x + 1, x := x + 1))
"

[Value]
"9\n"


[Test]
(* 17. Arguments in CallR *)
test_output
"let x := 1 in
let proc f (x, y, z) = 
  (x := x + (y := y + 1) + (z := z + 1)) 
in
f <x, x, x>; 
write x
"

[Value]
"6\n"


[Test]
(* 18. Scope case II *)
test_output
"
let proc f (x) = x + 1 in
let f := 2 in
write (f + f)
"

[Value]
"4\n"


[Test]
(* 19. Exception case - invalid function call *)
test_error_str
"
let proc f (x, y, z) = x + y + z in
(f (1, 2, 3) +
	let proc f (x, y) = x + y in
	f (1, 2, 3)
)
"

[Value]
"Invalid"


[Test]
(* 20. Complex scope *)
test_output
"let x := 1 in
let y := 5 in
let proc f (z) =
  if z then (x := x + 1) else (y := y - 2)
in
let x := 10 in
let y := 50 in
let proc g (z) =
  if z then (x := x + 5) else (y := y - 10)
in
write (g (true) + g (false) + x + y + f (true) + f (false) + x + y)
"

[Value]
"170\n"