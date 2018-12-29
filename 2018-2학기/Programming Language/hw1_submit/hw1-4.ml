(* exercise 1-4 *)
	
type formula = TRUE
| FALSE
| NOT of formula
| ANDALSO of formula * formula
| ORELSE of formula * formula
| IMPLY of formula * formula
| LESS of expr * expr
and expr = NUM of int
| PLUS of expr * expr
| MINUS of expr * expr

let rec eval (f : formula) : bool = 
	let rec solve_expr (e : expr) : int =
		match e with
		|NUM i -> i
		|PLUS (a,b) -> (solve_expr a) + (solve_expr b)
		|MINUS (a,b) -> (solve_expr a) - (solve_expr b) in
	match f with
	|TRUE -> true
	|FALSE -> false
	|NOT x -> not(eval(x))
	|ANDALSO (a,b) -> (eval a) && (eval b)
	|ORELSE (a,b) -> (eval a) || (eval b)
	|IMPLY (a,b) -> if (eval a) then (eval b)
					else true
	|LESS (a,b) -> if (solve_expr a < solve_expr b) then true
				   else false
