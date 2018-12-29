(*
 * SNU 4190.310 Programming Languages 
 * Homework "Continuation Passing Style" Skeleton
 *)

open M0

let count = ref 0

let new_name () = 
  let _ = count := !count + 1 in
  "x_" ^ (string_of_int !count)
  
  exception RunError of string

let rec alpha_conv exp subst = 
  match exp with
  | Num n -> Num n
  | Var x -> (try Var (List.assoc x subst) with Not_found -> Var x)
  | Fn (x, e) ->
    let x' = new_name () in
    let subst' = (x, x') :: subst in
    Fn (x', alpha_conv e subst')
  | App (e1, e2) -> App (alpha_conv e1 subst, alpha_conv e2 subst)
  | Rec (f, x, e) -> 
    let x' = new_name () in
    let f' = new_name () in
    let subst' = (f, f') :: (x, x') :: subst in
    Rec (f', x', alpha_conv e subst')
  | Ifz (e1, e2, e3) -> 
    Ifz (alpha_conv e1 subst, alpha_conv e2 subst, alpha_conv e3 subst)
  | Add (e1, e2) -> Add (alpha_conv e1 subst, alpha_conv e2 subst)
  | Pair (e1, e2) -> Pair (alpha_conv e1 subst, alpha_conv e2 subst)
  | Fst e -> Fst (alpha_conv e subst)
  | Snd e -> Snd (alpha_conv e subst)

(* TODO : Complete this function *)
let rec cps' exp = 
  let k = new_name () in
  match exp with
  (* Constant expressions *)
  | Num n -> Fn (k,App (Var k,Num n))
  | Var x -> Fn (k,App (Var k,Var x))
  | Fn (x, e) -> Fn (k, App (Var k,Fn (x, cps' e)))
  | Rec (f, x, e) -> 
	Fn (k, App (Var k,Rec (f,x,cps' e)))
	(* ???? *)
  (* Non constant expressions *)
  | App (e1, e2) -> 
    let v1 = new_name () in
	let v2 = new_name () in
	Fn (k, 
		App (cps' e1,
			Fn (v1,
				App (cps' e2,
					Fn (v2,
						App (App (Var v1, Var v2),Var k)
						)
					)
				)
			)
		)		
			
  | Ifz (e1, e2, e3) -> 
    let v1 = new_name() in
	let v2 = new_name() in
	let v3 = new_name() in
	let e2' = App (cps' e2,
				Fn (v2,
					App (Var k,Var v2)
					)
				  ) in
	let e3' = App (cps' e3,
			    Fn (v3,
				    App (Var k,Var v3)
					)
				  ) in
	Fn (k,
		App (cps' e1,
			Fn (v1,
				Ifz(Var v1,e2',e3')
				)
			)
		)
			
		(*
			(if ((Var v1)==(Num 0)) then App (cps' e2, Fn (v2, App (Var k, Var v2)))
					else App (cps' e3, Fn (v3, App (Var k, Var v3))))
				)
			)
		 *)
	(* if만 고치면 돼 *)
  | Add (e1, e2) ->
    let v1 = new_name () in
    let v2 = new_name () in
    Fn (k, 
        App (cps' e1, 
            Fn (v1, 
                App (cps' e2, 
                    Fn (v2, 
                        App (Var k, Add (Var v1, Var v2))
                        )
                    )
                )
            )
        )
  | Pair (e1, e2) -> 
	let v1 = new_name() in 
	let v2 = new_name() in
	Fn (k,
		App (cps' e1,
			Fn (v1,
				App (cps' e2,
					Fn (v2,
						App (Var k, Pair (Var v1, Var v2))
						)
					)
				)
			)
		)
  | Fst e ->  
	let v = new_name() in
	Fn (k, App (cps' e, Fn (v, App (Var k,(Fst (Var v))))))
		(*
		(match e with 
		     | Pair (e1,e2) -> App (cps' e1, Fn (v, App (Var k,Var v)))
			 | _ -> raise (RunError "Error")
		 )
		 *)
  | Snd e ->
	let v = new_name() in
	Fn (k, App (cps' e, Fn (v, App (Var k,(Snd (Var v))))))
		(*
		(match e with 
		     | Pair (e1,e2) -> App (cps' e2, Fn (v, App (Var k,Var v)))
			 | _ -> raise (RunError "Error")
		 )
		 *)

let cps exp = cps' (alpha_conv exp [])

