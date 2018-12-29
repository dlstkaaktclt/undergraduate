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
	| SIGMA (a,b,k) -> SIGMA (a,b,k)
	| INTEGRAL (a,b,k) -> INTEGRAL (a,b,k)

(*
let rec solveExpanother (x : exp) (e : exp) : exp =
	match e with
	| X -> x
	| INT i -> INT i
	| REAL f -> REAL f
	| ADD (a,b) -> ADD ((solveExp x a), (solveExp x b))
	| SUB (a,b) -> SUB ((solveExp x a), (solveExp x b))
	| MUL (a,b) -> MUL ((solveExp x a), (solveExp x b))
	| DIV (a,b) -> DIV ((solveExp x a), (solveExp x b))
	| SIGMA (a,b,k) -> SIGMA (a,b,k)
	| INTEGRAL (a,b,k) -> INTEGRAL (a,b,k)	
*)
	
(*
let rec mathsigma (a,b,c) : (int * int * exp) : exp =
	if ((int_of_float a) > (int_of_float b)) 0.0
	else      +. (mathsigma ((a+1),b,c)) 
*)
	

let rec mathemadiga (e : exp) : float = 
match e with
	| X -> raise FreeVariable
	| INT i -> float_of_int i
	| REAL f -> f
	| ADD (a,b) -> (mathemadiga a) +. (mathemadiga b)
	| SUB (a,b) -> (mathemadiga a) -. (mathemadiga b)
	| MUL (a,b) -> (mathemadiga a) *. (mathemadiga b)
	| DIV (a,b) -> (mathemadiga a) /. (mathemadiga b)
	| SIGMA (a,b,k) -> let ai = int_of_float (mathemadiga a) in let bi = int_of_float (mathemadiga b) in
						if (ai > bi) then 0.0
					    else ((mathemadiga (solveExp (float_of_int ai) k)) +. (mathemadiga (SIGMA ( REAL ((float_of_int ai) +. 1.0),(REAL (float_of_int bi)),k))))
	| INTEGRAL (a,b,k) -> let af = (mathemadiga a) in let bf = (mathemadiga b) in
						  if (af > bf) then (mathemadiga (MUL ((REAL ~-.1.0), (INTEGRAL ((REAL bf),(REAL af),k)))))
						  else if (bf -. af < 0.1) then 0.0
						  else ( (mathemadiga (solveExp af k)) *. 0.1) +. (mathemadiga (INTEGRAL ( REAL (af +. 0.1),(REAL bf),k))) 

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

exception InvalidArgument

type ae = CONST of int 
		| VAR of string 
		| POWER of string * int 
		| TIMES of ae list 
		| SUM of ae list
		
let rec diff ((exp,str) : (ae *  string)) : ae = 
	let rec diffcomp (ex : ae) : ae = (diff (ex,str)) in 
	match exp with
				| CONST i -> (CONST 0)
				| VAR x -> if (x = str) then (CONST 1)
							else (CONST 0)
				| POWER (s,i) -> if (s = str) then (TIMES ((CONST i)::(POWER (s,i-1))::[]))
								 else (CONST 0)
				| TIMES al -> (match al with
								|[] -> raise InvalidArgument
								|hd::[] -> (diff (hd,str))
								|hd::tl -> (SUM ( (TIMES ((diff (hd,str))::tl)) ::  (TIMES ( hd :: (diff ((TIMES tl),str)) :: []) )   :: [] ) )
								)
				| SUM al -> (match al with
							|[] -> raise InvalidArgument
							| _ -> (SUM (List.map diffcomp al))
							)
(*
(SUM ( (TIMES ((diff (hd,str))::tl)) ::  (TIMES ( hd :: (diff ((TIMES tl),str)) :: []) )   :: [] ) )
*)

(*
let rec exp_to_str (exp : ae) : string = 
	match exp with
				| CONST i -> (string_of_int i)
				| VAR x -> x
				| POWER (s,i) -> s^"^"^(string_of_int i)
				| TIMES al -> (match al with
								| [] -> ""
								| a::[] -> (exp_to_str a)
								| hd::tl -> (exp_to_str hd)^"*"^"("^(exp_to_str (TIMES tl))^")"
								)
				| SUM al -> (match al with
								| [] -> ""
								| a::[] -> (exp_to_str a)
								| hd::tl -> (exp_to_str hd)^"+"^(exp_to_str (SUM tl))
								)

							


let a = (TIMES ((CONST 3)::POWER(("x",2))::[])) (* 3x^2 *)
let b = (TIMES ((CONST 5)::POWER(("x",1))::[])) (* 5x *)
let c = (CONST (~-7)) (* -7 *)
let d = (TIMES ((CONST 2)::POWER(("x",2))::POWER(("y",3))::[])) (* 2x^2y^3 *)
let e = (SUM (a::b::c::[]))   (* 3x^2 + 5x - 7 *)
let f = (SUM (d::e::[]))    (* 2x^2y^3 + 3x^2 + 5x -7 *)
let q = exp_to_str (diff (f,"z"))
let w = exp_to_str (diff (f,"x"))
let e = exp_to_str (diff (f,"y"))

*)



type heap = EMPTY 
| NODE of rank * value * heap * heap 
and rank = int and value = int

exception EmptyHeap 

let rank (h : heap) : int = match h with 
  | EMPTY -> -1 
  | NODE(r,_,_,_) -> r 

let shake ((x,lh,rh) : (int * heap * heap)) : heap = 
  if (rank lh) >= (rank rh) 
  then NODE((rank rh)+1, x, lh, rh) 
  else NODE((rank lh)+1, x, rh, lh) 

let findMin (h : heap) : int = match h with 
  | EMPTY -> raise EmptyHeap 
  | NODE(_,x,_,_) -> x 

let merge ((lh,rh) : (heap*heap)) : heap =
	
  
  
  
let insert ((x,h) : (int,heap)) : heap = merge(h, NODE(0,x,EMPTY,EMPTY)) 

let deleteMin (h : heap) : heap = match h with 
  | EMPTY -> raise EmptyHeap 
  | NODE(_,x,lh,rh) -> merge (lh,rh) 
  
  

