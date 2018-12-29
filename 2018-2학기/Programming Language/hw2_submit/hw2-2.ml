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

let r = let exp =SUM ([TIMES [CONST 5; TIMES([VAR "x";VAR "x"])]; CONST 1]) in diff(exp, "x")

*)
