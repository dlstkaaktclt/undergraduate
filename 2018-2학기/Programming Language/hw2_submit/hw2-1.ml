exception FreeVariable

type exp = X
		| INT of int
		| REAL of float
		| ADD of exp * exp
		| SUB of exp * exp
		| MUL of exp * exp
		| DIV of exp * exp
		| SIGMA of exp * exp * exp
		| INTEGRAL of exp * exp * exp

		
let rec solveExp (x : float) (e : exp) : exp =
	match e with
	| X -> REAL x
	| INT i -> INT i
	| REAL f -> REAL f
	| ADD (a,b) -> ADD ((solveExp x a), (solveExp x b))
	| SUB (a,b) -> SUB ((solveExp x a), (solveExp x b))
	| MUL (a,b) -> MUL ((solveExp x a), (solveExp x b))
	| DIV (a,b) -> DIV ((solveExp x a), (solveExp x b))
	| SIGMA (a,b,k) -> SIGMA ((solveExp x a),(solveExp x b),k)
	| INTEGRAL (a,b,k) -> INTEGRAL ((solveExp x a),(solveExp x b),k)
	

let rec calculate (e : exp) : float = 
match e with
	| X -> raise FreeVariable
	| INT i -> float_of_int i
	| REAL f -> f
	| ADD (a,b) -> (calculate a) +. (calculate b)
	| SUB (a,b) -> (calculate a) -. (calculate b)
	| MUL (a,b) -> (calculate a) *. (calculate b)
	| DIV (a,b) -> (calculate a) /. (calculate b)
	| SIGMA (a,b,k) -> let ai = int_of_float (calculate a) in let bi = int_of_float (calculate b) in
						if (ai > bi) then 0.0
					    else ((calculate (solveExp (float_of_int ai) k)) +. (calculate (SIGMA ( REAL ((float_of_int ai) +. 1.0),(REAL (float_of_int bi)),k))))
	| INTEGRAL (a,b,k) -> let af = (calculate a) in let bf = (calculate b) in
						  if (af > bf) then (calculate (MUL ((REAL ~-.1.0), (INTEGRAL ((REAL bf),(REAL af),k)))))
						  else if (bf -. af < 0.1) then 0.0
						  else ( (calculate (solveExp af k)) *. 0.1) +. (calculate (INTEGRAL ( REAL (af +. 0.1),(REAL bf),k))) 

(*
let a = (SIGMA ((INT 1),(SUB ((INT 7),(INT 3))),(DIV ((MUL (X,X)),(SUB ((REAL 2.0),X))))))	    (* infinity *)
let b = (SIGMA ((INT 1),(SUB ((INT 7),(INT 3))),(DIV ((MUL (X,X)),(SUB ((REAL 5.0),X))))))		(* 22.083333333..*)
let c = (INTEGRAL ((REAL 2.0), (REAL 5.0), (REAL 3.0))) (* 9.0 *)
let c1 = (INTEGRAL ((REAL ~-.5.0), (REAL ~-.2.0), (REAL 3.0)))  (* 9.0 *)
let c2 = (INTEGRAL ((REAL 2.0), (REAL 2.09), (REAL 3.0))) (* 0.0 *)
let c3 = (INTEGRAL ((REAL 5.0), (REAL 2.0), (REAL 3.0))) (* -9.0 *)
let c4 = (INTEGRAL ((REAL 5.0), (REAL 2.0), (X))) (* -10.350000...03 *)
let c5 = (INTEGRAL ((REAL 2.0), (REAL 5.0), (X))) (* 10.350000...03 *)

let d = (INTEGRAL ((REAL 0.0), (REAL 3.0), (MUL (X,X)))) (* 7.71400...04 *)
let d1 = (SIGMA ((INTEGRAL ((REAL 2.0), (REAL 5.0), (REAL 3.0))) , (INT 11) , (SUB (X,REAL 7.0)))) (* 9.0 *)
let d2 = (INTEGRAL ((SIGMA ((INTEGRAL ((REAL 2.0), (REAL 5.0), (REAL 3.0))) , (INT 11) , (SUB (X,REAL 7.0)))) , (REAL 85.55), (DIV (X,X)))) (* 76.49999...9929 *)

let e = (INTEGRAL ((INTEGRAL ((SIGMA ((INTEGRAL ((REAL 2.0), (REAL 5.0), (REAL 3.0))) , (INT 11) , (SUB (X,REAL 7.0)))) , (REAL 85.55), (DIV (X,X)))), (REAL ~-.7.55), (MUL ((MUL ((REAL ~-.1.0),X)),(MUL (X,X))))))
(* 8516684.2320001535 *)

let f = (INTEGRAL (X,(REAL 3.5), MUL (X,X)))  (* Exception *)

let f1 = (INTEGRAL ((MUL (X,X)),(REAL 3.5), MUL (X,X))) (* Exception *)

let f2 = (INTEGRAL (INTEGRAL ((INT 0),(INT 1),(MUL (X,X))),(REAL 3.5), MUL (X,X)))  (* 13.5031200..015 *)

let f3 = (SIGMA ((REAL 1.9),(REAL 1.1),X)) (* 1 or 0 *)

*)

