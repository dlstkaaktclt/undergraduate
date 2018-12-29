(*
 * SNU 4190.310 Programming Languages
 * Type Checker Skeleton Code
 *)

open M
open Pp

type var = string

let count = ref 0 

let new_var () = 
  let _ = count := !count +1 in
  "x_" ^ (string_of_int !count)

type typ = 
  | TInt
  | TBool
  | TString
  | TPair of typ * typ
  | TLoc of typ
  | TFun of typ * typ
  | TVar of var
  (*
  | TClosure of tclosure
  and tclosure = fexpr * env
  and fexpr = Fun of id * exp
            | RecFun of id * id * exp
			*)
  and Tenv = var -> typ
  (* Modify, or add more if needed *)
  let (@+) f (x, v) = (fun y -> if y = x then v else f y)
  (* (env' @+ (x, v2)) *)
  let emptyTenv = (fun x -> raise (TypeError ("unbound type: " ^ x)))
(*
  (* types in M  *)
  type types = TyInt                     (* integer type *)
             | TyBool                    (* boolean type *)
             | TyString                  (* string type *)
             | TyPair of types * types   (* pair type *)
             | TyLoc of types            (* location type *)
             | TyArrow of types * types  (* function type *)
*)
let rec hasSameTypeMver (a:M.types) (b:M.types) : M.types = 
	match (a,b) with
	| (TyInt,TyInt) -> true
	| (TyBool,TyBool) -> true
	| (TyString,TyString) -> true
	| (TyLoc ta,TyLoc tb) -> (hasSameTypeMver ta tb)
	| (TyPair (a1,a2),TyPair (b1,b2)) -> ((hasSameTypeMver (a1,b1)) && hasSameTypeMver (a2,b2))
	| (TyArrow (a1,a2),TyArrow (b1,b2)) -> ((hasSameTypeMver (a1,b1)) && hasSameTypeMver (a2,b2))
	| _ -> false 

let rec hasSameType (a:typ) (b:typ) : typ = 
	match (a,b) with
	| (TInt,TInt) -> true
	| (TBool,TBool) -> true
	| (TString,TString) -> true
	| (TLoc ta,TLoc tb) -> (hasSameType ta tb)
	| (TPair (a1,a2),TPair (b1,b2)) -> ((hasSameType (a1,b1)) && hasSameType(a2,b2))
	| (TFun (a1,a2),TFun (b1,b2)) -> ((hasSameType (a1,b1)) && hasSameType(a2,b2))
	| _ -> false 	

let rec alpha_conv exp subst = 
  match exp with
  | CONST (S s) -> CONST (S s)
  | CONST (N n) -> CONST (N n)
  | CONST (B b) -> CONST (B b)
  | VAR x -> (try VAR (List.assoc x subst) with Not_found -> VAR x)
  | Fn (x, e) ->
    let x' = new_var () in
    let subst' = (x, x') :: subst in
    Fn (x', alpha_conv e subst')
  | APP (e1, e2) -> APP (alpha_conv e1 subst, alpha_conv e2 subst)
  | LET (decl, e) ->
	(match decl with
	  | VAL (x_id,x_exp) -> 
			let x' = new_var () in
			let subst' = (x_id, x') :: subst in
			LET (VAL (x', alpha_conv x_exp subst'), (alpha_conv e subst')) in
	  | REC (f_id,a_id,f_exp) -> 
			let f' = new_var () in
			let a' = new_var () in
			let subst' = (a_id a') :: (f_id, f') :: subst in
			LET (REC (f',a',alpha_conv f_exp subst'), (alpha_conv e subst')) in 
	  )
  | Rec (f, x, e) -> 
    let x' = new_var () in
    let f' = new_var () in
    let subst' = (f, f') :: (x, x') :: subst in
    Rec (f', x', alpha_conv e subst')
  | IF (e1, e2, e3) -> 
    IF (alpha_conv e1 subst, alpha_conv e2 subst, alpha_conv e3 subst)
  | READ -> READ
  | WRITE e -> WRITE (alpha_conv e subst) 
  | MALLOC e -> MALLOC (alpha_conv e subst)
  | ASSIGN (e1,e2) -> ASSIGN (alpha_conv e1 subst,alpha_conv e2 subst)
  | BANG e -> BANG (alpha_conv e subst)
  | SEQ (e1,e2) -> SEQ (alpha_conv e1 subst,alpha_conv e2 subst)
  | BOP (op, e1, e2) -> BOP (op, alpha_conv e1 subst, alpha_conv e2 subst)
  | PAIR (e1, e2) -> PAIR (alpha_conv e1 subst, alpha_conv e2 subst)
  | FST e -> FST (alpha_conv e subst)
  | SND e -> SND (alpha_conv e subst)

let rec typechecker (exp:M.exp) (env : Tenv) : typ = 
	match exp with
	| CONST (S s) -> TString
    | CONST (N n) -> TInt
    | CONST (B b) -> TBool
    | VAR x -> (env x)
    | FN (x, e) -> 
		let tyx = (Tvar x) in
		let env' = (env @+ (x,tyx)) in
		(TFun (tyx,(typechecker e env')))
    | APP (e1, e2) ->
	  match e2 with
	  | FN (x,e) -> 
      (typechecker e2 (env @+ (x, TVar new_var)))
	| LET (decl, e) ->
	  (match decl with
	  | VAL (x_id,x_exp) -> 
			let tyx = (typechecker x_exp env) in
			let tye = (typechecker e (env @+ (x_id, tyx))) in
			tye
	  | REC (f_id,a_id,f_exp) -> 
			let tyf = (typechecker f_exp env) in
			let tye = (typechecker e (env @+ (f_id,tyf))) in
			tye
	  )
    | IF (e1, e2, e3) ->
		(if (hasSameType (typechecker e1 env) TBool), then
			let ty2 = (typechecker e2 env) in
			let ty3 = (typechecker e3 env) in
			(if (hasSameType ty2 ty3) then ty2
			else raise (TypeError "IF then&else should have same type")
		else raise (TypeError "IF should get boolean type")
		)
    | BOP (op, e1, e2) -> 
	  let ty1 = (typechecker e1 env) in
	  let ty2 = (typechecker e2 env) in
	  match op with
	  | ADD -> (if ((hasSameType ty1,TInt) && (hasSameType ty2,TInt)) then TInt
				else raise (TypeError "Add should get integer"))
	  | SUB -> (if ((hasSameType ty1,TInt) && (hasSameType ty2,TInt)) then TInt
				else raise (TypeError "Sub should get integer"))
	  | AND -> (if ((hasSameType ty1,TBool) && (hasSameType ty2,TBool)) then TBool
				else raise (TypeError "And should get Boolean"))
	  | OR -> (if ((hasSameType ty1,TBool) && (hasSameType ty2,TBool)) then TBool
				else raise (TypeError "OR should get Boolean"))
	  | EQ -> (if (hasSameType ty1,ty2) then 
					(match ty1 with 
					| TInt -> TBool
					| TBool -> TBool
					| TString -> TBool
					| TLoc t -> TBool
					| _ -> raise (TypeError "Eq should get int,bool,string,loc Type")
					)
				else raise (TypeError "Eq should get SameType"))
    | READ -> TInt

    | WRITE e -> 
		let ty = (typechecker e env) in
		match ty with
		| TString -> TString
		| TInt -> TInt
		| TBool -> TBool
		| _ -> raise (TypeError "Write should get String,bool,int type")
	| MALLOC e -> (TLoc (typechecker e env))
	| ASSIGN (e1,e2) ->
		let ty1 = (typechecker e1 env) in
		let ty2 = (typechecker e2 env) in
		match ty1 with
		| TLoc ty -> (if (hasSameType ty ty2) then ty2
					   else raise (TypeError "Assign should get same type"))
		| _ -> raise (TypeError "Assign's first should get Loc type")
	| BANG e ->
		let ty = (typechecker e env) in
		match ty with
		| TLoc ti -> ti
		| _ -> raise (TypeError "Bang should get Loc type")
	| SEQ (e1,e2) ->
		let ty1 = (typechecker e1 env) in
		let ty2 = (typechecker e2 env) in
		ty2
    | PAIR (e1, e2) -> 
		let ty1 = (typechecker e1 env) in
		let ty2 = (typechecker e2 env) in
		TPair (ty1,ty2)
    | FST e -> 
		let ty = (typechecker e env) in
		match ty with
		| TPair (e1,e2) -> e1
		| _ -> raise (TypeError "Fst should get Pair type")
    | SND e -> 
		let ty = (typechecker e env) in
		match ty with
		| TPair (e1,e2) -> e2
		| _ -> raise (TypeError "Snd should get Pair type")
let rec typeconvert : typ -> M.types =
	

  
(* TODO : Implement this function *)
let check : M.exp -> M.types = fun exp ->
	typechecker (alpha_conv exp []) emptyTenv 
(*
    | CONST (S s) -> TyString
    | CONST (N n) -> TyInt
    | CONST (B b) -> TyBool
    | VAR x -> (env x, mem)
    | FN (x, e) -> 
	  let tx = check x in
	  let te = check e in
	  (match tx with
		| TFun (t1,t2) -> (match te with 
						   | T
    | APP (e1, e2) ->
      let (v1, m') = eval env mem e1 in
      let (v2, m'') = eval env m' e2 in
      let (c, env') = getClosure v1 in
	 (* fexpr * env, fexpr = fun id * exp or recfun id * id * exp *)
      (match c with 
      | Fun (x, e) -> eval (env' @+ (x, v2)) m'' e
      | RecFun (f, x, e) -> eval ((env' @+ (x,v2)) @+ (f,v1)) m'' e 
	  )
	| LET (decl, e) ->
	  (match decl with
	  | VAL (x_id,x_exp) ->
	  | REC (f_id,a_id,f_exp) -> 

	  )
    | IF (e1, e2, e3) ->
		(if (hasSameType (check e1) TyBool), then
			let ty2 = (check e2) in
			let ty3 = (check e3) in
			(if (hasSameType ty2 ty3) then ty2
			else raise (TypeError "IF then&else should have same type")
		else raise (TypeError "IF should get boolean type")
		)
 
    | BOP (op, e1, e2) ->

    | READ -> TyInt

    | WRITE e -> 
		let ty = (check e) in
		match ty with
		| TyString -> TyString
		| TyInt -> TyInt
		| TyBool -> TyBool
		| _ -> raise (TypeError "Write should get String,bool,int type")
	| MALLOC e -> (TyLoc (check e))
  
	| ASSIGN (e1,e2) ->
		let ty1 = (check e1) in
		let ty2 = (check e2) in
		match ty1 with
		| TyLoc ty -> (if (hasSameType ty ty2) then ty2
					   else raise (TypeError "Assign should get same type"))
		| _ -> raise (TypeError "Assign's first should get Loc type")
	| BANG e ->
	

	| SEQ (e1,e2) ->

    | PAIR (e1, e2) -> 

    | FST e -> 

    | SND e -> 

  raise (M.TypeError "Type Checker Unimplemented")
*)