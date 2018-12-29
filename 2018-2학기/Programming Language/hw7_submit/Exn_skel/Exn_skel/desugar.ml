(*
 * SNU 4190.310 Programming Languages 
 * Homework "Exceptions are sugar" Skeleton
 *)

open Xexp
(*
type xexp = 
  | Num of int
  | Var of string
  | Fn of string * xexp
  | App of xexp * xexp
  | If of xexp * xexp * xexp
  | Equal of xexp * xexp
  | Raise of xexp
  | Handle of xexp * int * xexp

type value = 
  | N of int                (* Integer *)
  | B of bool               (* Boolean *)
  | C of closure            (* Closure *)
and closure = string * xexp * env
and env = string -> value

type result = 
  | Val of value  (* Value *)
  | Exn of int    (* Exception *)
*)
type alphasub = (string * string) list

let count = ref 0

let new_name () = 
  let _ = count := !count + 1 in
  "x_" ^ (string_of_int !count)

  
let rec alpha_conv (exp : xexp) (subst : alphasub) : xexp = 
  match exp with
  | Num n -> Num n
  | Var x -> (try Var (List.assoc x subst) with Not_found -> Var x)
  | Fn (x, e) ->
    let x' = new_name () in
    let subst' = (x, x') :: subst in
    Fn (x', alpha_conv e subst')
  | App (e1, e2) -> App (alpha_conv e1 subst, alpha_conv e2 subst)
  | If (e1, e2, e3) -> 
    If (alpha_conv e1 subst, alpha_conv e2 subst, alpha_conv e3 subst)
  | Equal (e1, e2) -> Equal (alpha_conv e1 subst, alpha_conv e2 subst)
  | Raise e -> Raise (alpha_conv e subst)
  | Handle (t_exp,n,c_exp) -> Handle ((alpha_conv t_exp subst),n,(alpha_conv c_exp subst))

let rec cps' (exp : xexp) : xexp = 
  let k = new_name () in
  let l = new_name () in
  let inner : xexp = match exp with
  | Num n -> App (Var k,Num n)
  | Var x -> App (Var k,Var x)
  | Fn (x, e) -> App (Var k, Fn (x,cps' e)) 
  | App (e1, e2) -> 
    let v1 = new_name () in
	let v2 = new_name () in
	App (App (cps' e1,
			Fn (v1,
				App (App (cps' e2,
						Fn (v2,
							App (App (App (Var v1, Var v2),Var k), Var l)
							)
						)
					, (Var l)
					)
				)
			)
		, (Var l)
		)
  | If (e1, e2, e3) ->  
    let v1 = new_name() in
	let v2 = new_name() in
	let v3 = new_name() in 
	
	let e2' = App (App (cps' e2,
						Fn (v2,
							App (Var k,Var v2)
							)
						)
					, (Var l)
				  ) in
	let e3' = App (App (cps' e3,
						Fn (v3,
							App (Var k,Var v3)
							)
						)
					, (Var l)
				  ) in
	App (App (cps' e1,
			Fn (v1,
				If (Var v1,e2',e3')
				)
			)
		, (Var l)
		)
  | Equal (e1, e2) ->
    let v1 = new_name () in
    let v2 = new_name () in
    App (App (cps' e1, 
            Fn (v1, 
                App (App (cps' e2, 
						Fn (v2, 
							App (Var k, Equal (Var v1, Var v2))
							)
						)
					, (Var l)
					)
				)
            )
		, (Var l)
        )
  | Raise e -> App (App (cps' e,
						(Var l)
						)
					, (Var l)
					)
  | Handle (t_exp,n,c_exp) -> 
	let v1 = new_name () in
	App (App (cps' t_exp,
			  (Var k)
			  )
		,(Fn (v1,
			If (Equal (Var v1, Num n),
				App (App (cps' c_exp,
						  (Var k)
						  )
					,(Var l)
					)
				, App (Var l, Var v1)
			    )
			 )
		 )
		)
	in 
	Fn (k, Fn (l, inner))
(* TODO : Implement this function *)
let removeExn : xexp -> xexp = fun e ->
    let e' = (alpha_conv e []) in 
	let v1 = new_name() in 
	let v2 = new_name() in 
	let norm = Fn (v1, Var v1) in
	let handler = Fn (v2, Num 201812) in
	App (App (cps' e',norm),handler)